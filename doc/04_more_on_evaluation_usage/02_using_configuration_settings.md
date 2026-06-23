# Using configuration settings

A **configuration setting** is a file stored in a specific directory that will be read by the application at startup (or runtime) and override default settings used by the application.

By default, the filesystem is empty, so each directory and each file necessary for a given configuration must be created.

Some commands will create/delete files and directories automatically.

If a configuration setting file is missing, then the default value is used.

Here is an example of how to configure the IP address:

First, open the shell and press "CTRL+T" to halt console logs.

After a reset, the shell always returns to the root directory. The current directory can be printed with the command ***fs pwd***:

```console
uart:~$ fs pwd
/
```

As stated before, if this is the first time the shell is used, the filesystem is empty. This can be verified with the command ***fs ls***:

```console
uart:~$ fs ls /lfs
```

By referring to section [Network Settings](03_common_configuration.md#network-settings), which summarizes all the network configuration settings available, the IP address corresponds to the setting **/lfs/port0/ip_addr**.

Hence, create the directory **port0**. Then go to this directory and write the setting **ip_addr**:

```console
uart:~$ fs mkdir /lfs/port0
uart:~$ fs cd /lfs/port0
uart:~$ fs echo ip_addr 192.168.1.10
uart:~$ fs cd ..
```
It’s also possible to write a setting from the top directory:

```console
uart:~$ fs mkdir /lfs/port0
uart:~$ fs echo /lfs/port0/ip_addr 192.168.1.10
```

Make sure to reset the board after changing one or multiple configuration settings so that the changes are taken into account by the application.

In order to check the value of an existing configuration setting, go to its directory and use the command ***fs cat*** in order to read the setting:

```console
uart:~$ fs cd /lfs/port0
uart:~$ fs cat ip_addr
192.168.1.10uart:~$
uart:~$ fs cd ..
```

It’s also possible to read a setting from the top directory:

 ```console
uart:~$ fs cat /lfs/port0/ip_addr
192.168.1.10
```

Note that the command ***help*** can be used to list all available commands in the shell.

