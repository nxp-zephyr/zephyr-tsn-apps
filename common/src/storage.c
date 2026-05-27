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

#include "rtos_apps/storage.h"

#include "genavb/helpers.h"

#include "shell_config.h"

LOG_MODULE_REGISTER(storage);

#define MAX_FILENAME_LENGTH    32
#define MAX_PWD_LENGTH         128
#define MAX_PATH_LENGTH        (MAX_PWD_LENGTH + MAX_FILENAME_LENGTH)

/* Mount configuration */
#define PARTITION_NODE DT_NODELABEL(lfs1)

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
static char current_dir[MAX_PWD_LENGTH] = "/";
static char old_dir[MAX_PWD_LENGTH] = "/";
static char absolute_path[MAX_PATH_LENGTH];

void *storage_get_lfs(void)
{
    return mountpoint;
}

static void canonical_path(void *shell, char *path)
{
    char *start, *end;
    unsigned int len;

    start = path;

    /* skip initial / */
    if (start[0] == '/')
        start++;

    while (true) {
        end = strchr(start, '/');

        if (!end) {
            len = strlen(start);
            if (!len)
                break;

            end = start + len - 1;
        } else {
            len = end - start;
        }

        if ((len == 2) && !memcmp(start, "..", 2)) {
           /* go back */
           if (start > path + 1) {
               start--;
               while (start > path) {
                   if ((start - 1)[0] == '/')
                       break;
                   start--;
               }
           }

           len = strlen(end + 1);
           memmove(start, end + 1, len + 1);
        } else if (((len == 1) && !memcmp(start, ".", 1)) ||
            (len == 0)) {
           /* skip */
           len = strlen(end + 1);
           memmove(start, end + 1, len + 1);
        } else {
            /* go forward */
            start = end + 1;
        }
    }
}


static int get_absolute_path(const char *pwd, const char *path, char *absolute_path, unsigned int len)
{
    unsigned int pwd_len;
    unsigned int path_len = strlen(path);

    if (path[0] == '/') {
        if (path_len >= MAX_PATH_LENGTH)
            return -1;

        memcpy(absolute_path, path, path_len + 1);
    } else {
        pwd_len = strlen(pwd);

        if ((pwd_len + 1 + path_len) >= MAX_PATH_LENGTH)
            return -1;

        memcpy(absolute_path, pwd, pwd_len);
        absolute_path[pwd_len] = '/';
        memcpy(absolute_path + pwd_len + 1, path, path_len + 1);
    }

    return 0;
}


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

int storage_pwd(void)
{
    void *shell = storage.shell;

    shell_printf(shell, "%s\n", current_dir);

    return 0;
}

int storage_cd(const char *dir, bool quiet)
{
    struct fs_dirent info;
    void *shell = storage.shell;
    int len;
    int rc;

    if (!strcmp(dir, "-"))
        dir = old_dir;

    if (get_absolute_path(current_dir, dir, absolute_path, MAX_PATH_LENGTH) < 0)
        goto err;

    canonical_path(shell, absolute_path);

    rc = fs_stat(absolute_path, &info);
    if (rc < 0) {
        if (!quiet)
            shell_printf(shell, "directory %s doesn't exist\n", absolute_path);

        goto err;
    }

    if (info.type != FS_DIR_ENTRY_DIR) {
        if (!quiet)
            shell_printf(shell, "directory %s not a directory\n", absolute_path);

        goto err;
    }

    len = strlen(absolute_path);
    if (len >= MAX_PWD_LENGTH)
        goto err;

    strcpy(old_dir, current_dir);
    strcpy(current_dir, absolute_path);

    return 0;

err:
    return -1;
}

static void ls_file(struct fs_dirent *info)
{
    void *shell = storage.shell;

    shell_printf(shell, "%8zu %s\r\n", info->size, info->name);
}

static int ls_dir(const char *dirname)
{
    void *shell = storage.shell;
    struct fs_dirent info;
    struct fs_dir_t dir;
    int rc;

    fs_dir_t_init(&dir);

    rc = fs_opendir(&dir, dirname);
    if (rc < 0) {
        shell_printf(shell, "fs_opendir(%s) failed: %d\n", dirname, rc);
        goto err;
    }

    /* iterate until end of directory */
    while (fs_readdir(&dir, &info) == 0 && info.name[0] != 0) {
        if (info.type == FS_DIR_ENTRY_FILE) {
            ls_file(&info);
        } else if (info.type == FS_DIR_ENTRY_DIR) {
            shell_printf(shell, "     DIR %s\r\n", info.name);
        } else {
            shell_printf(shell, "     ??? %s\r\n", info.name);
        }
    }

    rc = fs_closedir(&dir);
    if (rc < 0) {
        shell_printf(shell, "fs_closedir(%s) failed: %d\n", dirname, rc);
        goto err;
    }

    return 0;

err:
    return -1;
}

int storage_ls(const char *name)
{
    void *shell = storage.shell;
    struct fs_dirent info;
    int rc;

    if (get_absolute_path(current_dir, name, absolute_path, MAX_PATH_LENGTH) < 0)
        goto err;

    rc = fs_stat(absolute_path, &info);
    if (rc < 0) {
        shell_printf(shell, "fs_stat(%s) failed: %d\n", absolute_path, rc);
        goto err;
    }

    if (info.type == FS_DIR_ENTRY_FILE) {
        ls_file(&info);
    } else if (info.type == FS_DIR_ENTRY_DIR) {
        if (ls_dir(absolute_path) < 0)
            goto err;
    }

    return 0;

err:
    return -1;
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
    struct fs_dirent info;
    int rc;

    if (get_absolute_path(current_dir, filename, absolute_path, MAX_PATH_LENGTH) < 0)
        goto err;

    rc = fs_stat(absolute_path, &info);
    if (rc < 0) {
        if (!force) {
            shell_printf(shell, "fs_stat(%s) failed: %d\n", absolute_path, rc);
            goto err;
        }

        goto out;
    }

    if ((info.type == FS_DIR_ENTRY_FILE) || !recursive) {
        if (rm_file(absolute_path) < 0)
            goto err;
    } else if (info.type == FS_DIR_ENTRY_DIR) {
        if (rm_dir(absolute_path, strlen(absolute_path), MAX_PATH_LENGTH, &info) < 0)
            goto err;
    }

out:
    return 0;

err:
    return -1;
}

int storage_cat(const char *filename)
{
    void *shell = storage.shell;
    struct fs_file_t file;
    char buf[MAX_FILE_SIZE + 1];
    int rc;

    if (get_absolute_path(current_dir, filename, absolute_path, MAX_PATH_LENGTH) < 0)
        goto err_path;

    fs_file_t_init(&file);

    rc = fs_open(&file, absolute_path, FS_O_READ);
    if (rc < 0) {
        shell_printf(shell, "fs_open(%s) failed: %d\n", absolute_path, rc);
        goto err_open;
    }

    while (1) {
        rc = fs_read(&file, buf, MAX_FILE_SIZE);
        if (rc <= 0) {
            if (!rc)
                goto done;

            shell_printf(shell, "fs_read() failed: %d\n", rc);
            goto err_read;
        }

        buf[rc] = '\0';
        shell_printf(shell, "%s", buf);
    }

done:
    shell_printf(shell, "\n");
    fs_close(&file);
    return 0;

err_read:
    fs_close(&file);

err_open:
err_path:
    return -1;
}

static int storage_mkdir_p(char *dirname)
{
    char partial_path[MAX_PWD_LENGTH] = {0};
    void *shell = storage.shell;
    struct fs_dirent info;
    char *dir;
    int rc, off = 0;

    canonical_path(shell, dirname);

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
    int rc = 0;

    if (get_absolute_path(current_dir, dirname, absolute_path, MAX_PATH_LENGTH) < 0) {
        rc = -1;
        goto err_path;
    }

    if (parent) {
        rc = storage_mkdir_p(absolute_path);
    } else {
        rc = fs_mkdir(absolute_path);
        if (rc < 0)
            shell_printf(shell, "fs_mkdir(%s) failed: %d\n", absolute_path, rc);
    }

err_path:
    return rc;
}

int storage_read(const char *filename, char *buf, unsigned int len)
{
    void *shell = storage.shell;
    struct fs_file_t file;
    int rc;

    if (get_absolute_path(current_dir, filename, absolute_path, MAX_PATH_LENGTH) < 0)
        goto err_path;

    fs_file_t_init(&file);

    rc = fs_open(&file, absolute_path, FS_O_READ);
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
err_path:
    return -1;
}

int storage_write(const char *filename, const char *buf, unsigned int len)
{
    void *shell = storage.shell;
    struct fs_file_t file;
    int rc;

    if (get_absolute_path(current_dir, filename, absolute_path, MAX_PATH_LENGTH) < 0)
        goto err_path;

    fs_file_t_init(&file);

    rc = fs_open(&file, absolute_path, FS_O_CREATE | FS_O_WRITE | FS_O_TRUNC);
    if (rc < 0) {
         shell_printf(shell, "fs_open(%s) failed: %d\n", absolute_path, rc);
         goto err_open;
    }

    rc = fs_write(&file, buf, len);
    if (rc < 0) {
        shell_printf(shell, "fs_write(%s) failed: %d\n", absolute_path, rc);
        goto err_write;
    }

    fs_close(&file);
    return 0;

err_write:
    fs_close(&file);

err_open:
err_path:
    return -1;
}

int storage_init(void)
{
    storage.mount = mountpoint;

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

    /* Set current directory to mount point */
    strncpy(current_dir, mountpoint->mnt_point, sizeof(current_dir) - 1);
    current_dir[sizeof(current_dir) - 1] = '\0';
    strncpy(old_dir, current_dir, sizeof(old_dir) - 1);
    old_dir[sizeof(old_dir) - 1] = '\0';

    return 0;

err:
    return -1;
}

int storage_set_shell(void *shell)
{
    storage.shell = shell;

    return 0;
}

void storage_exit(void)
{
    fs_unmount(storage.mount);
    storage.mount = NULL;
}
