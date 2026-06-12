/*
 * Copyright 2020, 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stddef.h>
#include <string.h>

#include <zephyr/logging/log.h>

#include <zephyr/fs/fs.h>
#include <zephyr/fs/littlefs.h>

#include <zephyr/shell/shell_uart.h>

#include "rtos_apps/shell/shell_storage_app.h"
#include "rtos_apps/storage/storage_app.h"

#include "genavb/helpers.h"

#include "storage.h"
#include "storage_config.h"
#include "shell_config.h"

LOG_MODULE_REGISTER(storage);

#define MAX_FILENAME_LENGTH    32
#define MAX_PWD_LENGTH         128
#define MAX_PATH_LENGTH        (MAX_PWD_LENGTH + MAX_FILENAME_LENGTH)

#if DT_NODE_EXISTS(PARTITION_NODE)
FS_FSTAB_DECLARE_ENTRY(PARTITION_NODE);
#else
#error "partition node does not exist"
#endif

static struct fs_mount_t *mountpoint = &FS_FSTAB_ENTRY(PARTITION_NODE);

/* Storage context structure */
struct storage_ctx {
    struct fs_mount_t *mount;
    void *shell;
};

static struct storage_ctx storage;

/* get nth dir/file from dirname */
static int storage_get(const char *dirname, unsigned int n, struct fs_dirent *info)
{
    void *shell = storage.shell;
    struct fs_dir_t dir;
    unsigned int count = 0;
    int rc = -1;

    fs_dir_t_init(&dir);

    if (fs_opendir(&dir, dirname) < 0) {
        goto err;
    }

    while (fs_readdir(&dir, info) == 0 && info->name[0] != 0) {
        if (count == n) {
            rc = 0;
            break;
        }
        count++;
    }

    if (fs_closedir(&dir) < 0) {
        shell_printf(shell, "fs_closedir failed\n");
        rc = -1;
    }

err:
    return rc;
}

int storage_get_dir(const char *dirname, unsigned int n, char *subdirname, unsigned int len)
{
    struct fs_dirent info;
    int rc = -1;

    rc = storage_get(dirname, n, &info);
    if (rc < 0)
        goto err;

    if (info.type == FS_DIR_ENTRY_DIR) {
        if ((strlen(info.name) + 1) > len) {
            rc = -1;
            goto err;
        }

        memcpy(subdirname, info.name, strlen(info.name) + 1);
    } else {
        rc = -1;
        goto err;
    }

    return rc;

err:
    if (subdirname)
        subdirname[0] = '\0';
    return rc;
}

int storage_get_file(const char *dirname, unsigned int n, char *filename, unsigned int len)
{
    struct fs_dirent info;
    int rc = -1;

    rc = storage_get(dirname, n, &info);
    if (rc < 0)
        goto err;

    if (info.type == FS_DIR_ENTRY_FILE) {
        if ((strlen(info.name) + 1) > len) {
            rc = -1;
            goto err;
        }

        memcpy(filename, info.name, strlen(info.name) + 1);
    } else {
        rc = -1;
        goto err;
    }

    return rc;

err:
    if (filename)
        filename[0] = '\0';
    return rc;
}

static int rm_file(const char *filename)
{
    void *shell = storage.shell;
    int rc;

    rc = fs_unlink(filename);
    if (rc < 0)
        shell_printf(shell, "fs_unlink(%s) failed: %d\n", filename, rc);

    return rc;
}

static int rm_dir(char *dirname, unsigned int off, unsigned int end, struct fs_dirent *info)
{
    void *shell = storage.shell;
    struct fs_dir_t dir;
    int len;
    int rc;

    fs_dir_t_init(&dir);

    rc = fs_opendir(&dir, dirname);
    if (rc < 0) {
        shell_printf(shell, "fs_opendir(%s) failed: %d\n", dirname, rc);
        goto err;
    }

    /* iterate until end of directory */
    while (fs_readdir(&dir, info) == 0 && info->name[0] != 0) {
        len = h_snprintf_strict(dirname + off, end - off, "/%s", info->name);
        if (len < 0 || len >= (end - off)) {
            shell_printf(shell, "h_snprintf_strict failed: %d\n", len);
            rc = -1;
            goto err_close;
        }

        if (info->type == FS_DIR_ENTRY_FILE) {
            rm_file(dirname);
        } else if (info->type == FS_DIR_ENTRY_DIR) {
            rm_dir(dirname, off + len, end, info);
        }
    }

    dirname[off] = '\0';
    if (rm_file(dirname) < 0)
        goto err_close;

    rc = fs_closedir(&dir);
    if (rc < 0) {
        shell_printf(shell, "fs_closedir(%s) failed: %d\n", dirname, rc);
        goto err;
    }

    return rc;

err_close:
    fs_closedir(&dir);

err:
    return rc;
}

int storage_rm(const char *filename, bool recursive, bool force)
{
    void *shell = storage.shell;
    char path[MAX_PATH_LENGTH] = {0};
    struct fs_dirent info;
    int rc;

    if (h_snprintf_strict(path, MAX_PATH_LENGTH, "%s", filename) < 0)
        goto err;

    rc = fs_stat(path, &info);
    if (rc < 0) {
        if (!force) {
            shell_printf(shell, "fs_stat(%s) failed: %d\n", path, rc);
            goto err;
        }

        goto out;
    }

    if ((info.type == FS_DIR_ENTRY_FILE) || !recursive) {
        if (rm_file(path) < 0)
            goto err;
    } else if (info.type == FS_DIR_ENTRY_DIR) {
        if (rm_dir(path, strlen(path), MAX_PATH_LENGTH, &info) < 0)
            goto err;
    }

out:
    return 0;

err:
    return -1;
}

static int storage_mkdir_p(char *dirname)
{
    char partial_path[MAX_PWD_LENGTH] = {0};
    void *shell = storage.shell;
    struct fs_dirent info;
    char *dir;
    int rc, off = 0;

    dir = strtok(dirname, "/");
    while (dir) {
        rc = h_snprintf_strict(partial_path + off, MAX_PWD_LENGTH - off, "/%s", dir);
        if (rc < 0 || rc >= (MAX_PWD_LENGTH - off))
            goto err;

        off += rc;

        rc = fs_mkdir(partial_path);
        if (rc < 0) {
            if (rc == -EEXIST) {
                if (fs_stat(partial_path, &info) == 0) {
                    /* file or dir exists */
                    if (info.type == FS_DIR_ENTRY_FILE) {
                        /* file -> error */
                        shell_printf(shell, "fs_mkdir(%s) failed: file exists\n", partial_path);
                        goto err;
                    } else if (info.type == FS_DIR_ENTRY_DIR) {
                        /* directory -> continue */
                        goto next;
                    }
                }
            } else {
                shell_printf(shell, "fs_mkdir(%s) failed: %d\n", partial_path, rc);
                goto err;
            }
        }
next:
        dir = strtok(NULL, "/");
    }

    return 0;

err:
    return -1;
}

int storage_mkdir(const char *dirname, bool parent)
{
    void *shell = storage.shell;
    char path[MAX_PATH_LENGTH] = {0};
    int rc = 0;

    if (h_snprintf_strict(path, MAX_PATH_LENGTH, "%s", dirname) < 0) {
        rc = -1;
        goto out;
    }

    if (parent) {
        rc = storage_mkdir_p(path);
    } else {
        rc = fs_mkdir(path);
        if (rc < 0)
            shell_printf(shell, "fs_mkdir(%s) failed: %d\n", path, rc);
    }

out:
    return rc;
}

int storage_read(const char *dirname, const char *filename, char *buf, unsigned int len)
{
    void *shell = storage.shell;
    char path_buf[MAX_PATH_LENGTH] = {0};
    const char *path;
    struct fs_file_t file;
    int rc;

    if (dirname) {
        if (h_snprintf_strict(path_buf, MAX_PATH_LENGTH, "%s/%s", dirname, filename) < 0)
            return -1;
        path = path_buf;
    } else {
        path = filename;
    }

    fs_file_t_init(&file);

    rc = fs_open(&file, path, FS_O_READ);
    if (rc < 0) {
        goto err_open;
    }

    rc = fs_read(&file, buf, len);
    if (rc < 0) {
        shell_printf(shell, "fs_read() failed: %d\n", rc);
        goto err_read;
    }

    fs_close(&file);
    return rc;

err_read:
    fs_close(&file);

err_open:
    return -1;
}

int storage_write(const char *dirname, const char *filename, const char *buf, unsigned int len)
{
    void *shell = storage.shell;
    const char *path;
    char path_buf[MAX_PATH_LENGTH] = {0};
    struct fs_file_t file;
    int rc;

    if (dirname) {
        if (h_snprintf_strict(path_buf, MAX_PATH_LENGTH, "%s/%s", dirname, filename) < 0)
            return -1;
        path = path_buf;
    } else {
        path = filename;
    }

    fs_file_t_init(&file);

    rc = fs_open(&file, path, FS_O_CREATE | FS_O_WRITE | FS_O_TRUNC);
    if (rc < 0) {
         shell_printf(shell, "fs_open(%s) failed: %d\n", path, rc);
         goto err_open;
    }

    rc = fs_write(&file, buf, len);
    if (rc < 0) {
        shell_printf(shell, "fs_write(%s) failed: %d\n", path, rc);
        goto err_write;
    }

    fs_close(&file);
    return 0;

err_write:
    fs_close(&file);

err_open:
    return -1;
}

int storage_init(void)
{
    storage.mount = mountpoint;
    storage.shell = (void *)shell_backend_uart_get_ptr();

    /* Check if auto-mounted */
#if !(FSTAB_ENTRY_DT_MOUNT_FLAGS(PARTITION_NODE) & FS_MOUNT_FLAG_AUTOMOUNT)
    LOG_ERR("Filesystem should be automounted");
    goto err;
#else
    if (mountpoint != NULL && mountpoint->fs != NULL && mountpoint->mountp_len != 0) {
        LOG_INF("Filesystem automounted");
    } else {
        LOG_ERR("Filesystem automount failed (fs=NULL, mountp_len=0)");
        goto err;
    }

#endif

    return 0;

err:
    return -1;
}

void storage_exit(void)
{
    fs_unmount(storage.mount);
    storage.mount = NULL;
    storage.shell = NULL;
}
