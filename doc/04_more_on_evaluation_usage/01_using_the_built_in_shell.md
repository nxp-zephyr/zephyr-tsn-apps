# Using the built-in shell

In GenAVB/TSN applications, the Zephyr shell is enabled with filesystem support (mounted at /lfs ) to create, modify or remove files and directories.

To use this shell it is recommended to press "CTRL+t" on the PC serial terminal, to halt the logging.

## Filesystem commands

There are several commands available to manage files and directories. These commands are a very basic implementation of what could be found on any Unix system.
You can refer to the [Zephyr File System Commands Documentation](https://docs.zephyrproject.org/latest/samples/subsys/shell/fs/README.html#files-system-shell-commands)

All file system related commands are available as sub-commands of `fs`.

**write:** write hexadecimal numbers to a given file. Optionally an offset in the file can be given.

```console
fs write <path> [-o <offset>] <hex number> ...
```

**echo:** write a string to a given file.

```console
fs echo <path> <string> ...
```

**cat:** concatenate files and print on the standard output
```console
fs cat <path> [<path> ...]
```

**read:** read a file and dump in hex and ASCII format

```console
fs read <path>
```

**ls:** list all files and directories in a given path

```console
fs ls [path]
```

**rm:** remove a file or directory

```console
fs rm <path>
```

**cd:** change current working directory to given path

```console
fs cd [path]
```

**pwd:** list current working directory

```console
fs pwd
```

**mkdir:** create a directory

```console
fs mkdir <path>
```

**trunc:** truncate a given file

```console
fs trunc <path>
```

Here is an example of how these commands could be used:

```console
uart:~$ fs pwd
/
uart:~$ fs ls
       0 lfs/
uart:~$ fs cd lfs
uart:~$ fs mkdir foo
uart:~$ fs ls
       0 foo/
uart:~$ fs cd foo
uart:~$ fs pwd
/lfs/foo
uart:~$ fs echo bar Hello!
uart:~$ fs ls
       6 bar
uart:~$ fs cat bar
Hello!uart:~$
uart:~$ fs rm bar
uart:~$ fs ls
```
