# Bridge configuration (i.MX RT118x)

This section describes each configurable setting that applies to Bridge devices, how to change its value and what values are consistent.

## VLAN Registration Configuration

IEEE 802.1Q VLAN Registration settings are defined per bridge's port and rely on command parameters.

**Note:** VLAN and port default VLAN (PVID) permanent entries are applied on system boot. If a port has no permanent entry, its PVID is set to 1. If there is no permanent entry for VLAN 1, the port is added to the VLAN 1 membership set. Untagged traffic is forwarded to all ports of the ingress port's PVID VLAN membership. Hence, by default, all ports' PVIDs are set to 1, and all ports are members of VLAN 1, with untagged traffic forwarded to all ports.

## VLAN Registration commands

### net genavb vlan update \<vid\> \<port_id\> \[-c \<control\>\] \[-u\] \[-p\]

Updates VLAN Filtering entry. A new entry is created if one doesn't exist.

Parameters:

\<vid\>

- VLAN ID (1 to 4094)

\<port_id\>

- Logical port index the VLAN entry is associated with.

Options:

> \[-c \<control\>\]

- Register admin control: 0:forbidden, 1:fixed (default).

\[-u\]

- Untagged transmit.

\[-p\]

- Update entry in permanent database.

### net genavb vlan delete \<vid\> \[-p\]

Deletes an entry from the VLAN Filtering Data Base.

Parameters:

\<vid\>

- VLAN ID (1 to 4094)

Options:

\[-p\]

- Updates entry in permanent database.

### net genavb vlan read \<vid\> \[-p\]

Reads an entry from the VLAN Filtering Data Base.

Parameters:

\<vid\>

- VLAN ID (1 to 4094)

Options:

\[-p\]

- Reads entry from permanent database.

### net genavb vlan dump \[-p\]

Dumps all entries from the VLAN Filtering Data Base and PVIDs of all ports.

Options:

\[-p\]

- Dumps entry from permanent database.

Following is an output example from an i.MX RT1180 TSN Bridge showing the dumping of VLAN entries:

```console
uart:~$ net genavb vlan dump
vid | dynamic | tagged | fixed            | forbidden        |
------+---------+--------+----------------+----------------+
2   | false   | true   | 2, 4           | 3, 5           |
312 | false   | true   | 3              | 2, 4, 5        |
31  | true    | true   |                | 2, 3, 4, 5     |

port |    2 |    3 |    4 |    5 |    6 |
------+------+------+------+------+------+
PVID |  123 |    2 |    1 |    1 |    1 |
```

1<sup>st</sup> entry: VID=2, the entry has been created statically, logical ports 2 and 4 are forwarded.

2<sup>nd</sup> entry: VID=312, the entry has been created statically, only logical port 3 is forwarded.

3<sup>rd</sup> entry: VID=31, the entry has been created dynamically, all ports are filtered.

### net genavb vlan set_pvid \<port_id\> \<vid\> \[-p\]

Sets port default vid (PVID).

Parameters:

\<port_id\>

- Logical port index

\<vid\>

- VLAN ID (1 to 4094)

Options:

\[-p\]

- Updates entry in permanent database.

### net genavb vlan get_pvid \<port_id\> \[-p\]

Gets port default vid (PVID).

Parameters:

\<port_id\>

- Logical port index

Options:

\[-p\]

- Dumps entry from permanent database.

## FDB configuration

IEEE 802.1Q FDB settings are defined per bridge's port and rely on command parameters.

## FDB commands

### net genavb fdb update \<mac\> \<vid\> \<port_id\> \[-c \<control\>\] \[-p\]

Adds port to an existing Filtering Data Base entry. A new entry is created if one doesn't exist.

Parameters:

> \<mac\>

- Ethernet MAC address in the form xx:xx:xx:xx:xx:xx

\<vid\>

- VLAN ID (1 to 4095)

\<port_id\>

- Logical port index the FDB entry is associated to.

Options:

> \[-c \<control\>\]

- Filtering control: 0:filtering, 1:forwarding (default).

\[-p\]

- Update entry in permanent database.

### net genavb fdb delete \<mac\> \<vid\> \[-p\]

Deletes an entry from the Filtering Data Base.

Parameters:

> \<mac\>

- Ethernet MAC address in the form xx:xx:xx:xx:xx:xx

\<vid\>

- VLAN ID (1 to 4095)

Options:

\[-p\]

- Update entry in permanent database.

### net genavb fdb read \<mac\> \<vid\> \[-p\]

Reads an entry from the Filtering Data Base.

Parameters:

> \<mac\>

- Ethernet MAC address in the form xx:xx:xx:xx:xx:xx

\<vid\>

- VLAN ID (1 to 4095)

Options:

\[-p\]

- Update entry in permanent database.

### net genavb fdb dump \[-p\]

Dumps all entries from the Filtering Data Base.

Options:

\[-p\]

- Update entry in permanent database.

Following is an output example from an i.MX RT1180 TSN Bridge showing the dumping of FDB entries:

```console
uart:~$ net genavb fdb dump
mac               | vid | dynamic | status | forwarding     | filtering      |
------------------+------+---------+--------+----------------+----------------+
00:00:11:22:33:cc | 55  | static  | 1      | 2, 3           | 4, 5           |
00:00:aa:bb:cc:dd | 99  | static  | 1      |                | 2, 3, 4, 5     |
```

1<sup>st</sup> entry: created dynamically, used (status = 1), logical ports 2 and 3 are forwarded.

2<sup>nd</sup> entry: created dynamically, used (status = 1), all ports are filtered.

## Frame Replication and Elimination for Reliability configuration

IEEE 802.1CB-2017 (FRER) settings are defined per bridge's port and rely on both filesystem configuration files and command parameters. FRER is applicable per stream and relies on the following features:

- Sequence Generation (seqg)

- Sequence Recovery (seqr)

- Sequence Identification (seqi)

## Sequence Generation settings

<div align="center">
<table>
<caption><p>Sequence Generation settings</p></caption>
<colgroup>
<col style="width: 13%" />
<col style="width: 23%" />
<col style="width: 16%" />
<col style="width: 33%" />
<col style="width: 12%" />
</colgroup>
<thead>
<tr>
<th>Path</th>
<th>Name</th>
<th>Condition</th>
<th>Brief description</th>
<th>Reference</th>
</tr>
</thead>
<tbody>
<tr>
<td rowspan="2">/lfs/seqg/N</td>
<td></td>
<td>0 &lt;= N &lt; 384</td>
<td>Sequence generation entry index</td>
<td></td>
</tr>
<tr>
<td>handle</td>
<td>&gt;= 0</td>
<td>Stream handle identifier</td>
<td><a href="#stream_handle">stream_handle</a></td>
</tr>
<tr>
<td rowspan="5">/lfs/seqi/N</td>
<td></td>
<td>0 &lt;= N &lt; 7</td>
<td>Sequence identification entry index</td>
<td></td>
</tr>
<tr>
<td>handle</td>
<td>&gt;= 0</td>
<td>Stream handle identifier</td>
<td><a href="#stream_handle">stream_handle</a></td>
</tr>
<tr>
<td>active</td>
<td>0 or 1</td>
<td>Redundancy TAG control</td>
<td><a href="#active">active</a></td>
</tr>
<tr>
<td>Encapsulation</td>
<td></td>
<td>Redundancy encapsulation Tag type</td>
<td><a href="#encapsulation">encapsulation</a></td>
</tr>
<tr>
<td>path_id_lan_id</td>
<td>0 or 1</td>
<td></td>
<td><a href="#path_id_lan_id">path_id_lan_id</a></td>
</tr>
<tr>
<td rowspan="6">/lfs/seqr/N</td>
<td></td>
<td>0 &lt;= N &lt;384</td>
<td>Sequence recovery entry index</td>
<td></td>
</tr>
<tr>
<td>handle</td>
<td>&gt;= 0</td>
<td>List of Stream handle identifier (comma separated)</td>
<td><a href="#stream_handle">stream_handle</a></td>
</tr>
<tr>
<td>port</td>
<td></td>
<td>List of logical ports (comma separated)</td>
<td><a href="#port">port</a></td>
</tr>
<tr>
<td>history_length</td>
<td>0 &lt; length &lt; 128</td>
<td>Recovery vector algorithm length</td>
<td><a href="#history_length">history_length</a></td>
</tr>
<tr>
<td>individual_recovery</td>
<td>0 or 1</td>
<td></td>
<td><a href="#individual_recovery">individual_recovery</a></td>
</tr>
<tr>
<td>take_no_sequence</td>
<td>0 or 1</td>
<td></td>
<td><a href="#take_no_sequence">take_no_sequence</a></td>
</tr>
</tbody>
</table>
</div>

### stream_handle

A list of stream handles identifying the Streams to which the packets belong to.

### Active

A boolean controlling if the redundancy TAG is removed or not on the specified ports.

### Encapsulation

Tag Encapsulation type, 1: R-TAG (default), 2: HSR

### path_id_lan_id

A boolean defining if path or LAN id shall be used.

### Port

A list of comma separated logicals port.

### history_length

Recovery vector algorithm length.

### individual_recovery

Enables individual recovery mode.

### take_no_sequence

Untagged frame acceptance, 0: disable (default), 1: enable.

## Sequence Generation commands

### net genavb seqg update \<index\> \[-h \<h1\>\[,\<h2\>,..,\<hn\>\]\] \[-p\]

Parameters:

\<index\>

- Entry index

Options:

\[-h \<h1\>\]

- List of stream handles (comma separated)

\[-p\]

- Update entry in permanent database

### net genavb seqg update \<index\> -r

Parameters:

\<index\>

- Entry index

Options:

\[-r\]

- Reset sequence generation

### net genavb seqg delete \<index\> \[-p\]

Parameters:

\<index\>

- entry index

Options:

\[-p\]

- delete entry from permanent database

### net genavb seqg read \<index\> \[-p\]

Parameters:

index: entry index

Options:

\[-p\] read entry from permanent database

## Sequence Identification commands

### net genavb seqi update \<port_id\> \[-h h1\[,\<h2\>,..,\<hn\>\]\] \[-a\] \[-e \<encapsulation\>\] \[-i \<id\>\] \[-p\]

Parameters:

\<port_id\>

- Logical port id

Options:

\[-h \<h1\>\]

- List of stream handles (comma separated)

\[-a\]

- Active

\[-e \<encapsulation\>\]

- Encapsulation tag, 1: R-TAG, 2: HSR

\[-i \<id\>\]

- Path or LAN id

\[-p\]

- Update entry in permanent database

### net genavb seqi delete \<port_id\> \[-p\]

Parameters:

\<port_id\>

- logical port id

Options:

\[-p\]

- delete entry from permanent database

### net genavb seqi read \<port_id\> \[-p\]

Parameters:

\<port_id\>

- logical port id

Options:

\[-p\]:

- read entry from permanent database

## Sequence Recovery commands

### net genavb seqr update \<index\> \[-h \<h1\>\[,\<h2\>,..,\<hn\>\]\] \[-P \<p1\>\[,\<p2\>,..,\<pn\>\]\] \[-a \<algorithm\>\] \[-r\] \[-H \<history\>\] \[-s \<enable\>\] \[-i \<enable\>\] \[-p\]

Parameters:

\<index\>

- entry index

Options:

\[-h \<h1\>\]

- list of stream handles (comma separated)

\[-P \<p1\>\]

- list of logical port ids (comma separated)

\[-a \<algorithm\>\]

- recovery algorithm, 0: Vector (default), 1: Match

\[-r\]

- reset sequence recovery

\[-H \<history\>\]

- history length

\[-s \<enable\>\]

- take no sequence, 0: disable (default), 1: enable

\[-i \<enable\>\]

- individual recovery, 0: disable (default), 1: enable

\[-p\]

- update entry in permanent database

### net genavb seqr delete \<index\> \[-p\]

Parameters:

\<index\>

- entry index

Options:

\[-p\]

- delete entry from permanent database

### net genavb seqr read \<index\> \[-p\]

Parameters:

\<index\>

- entry index

Options:

\[-p\]

- read entry from permanent database

## Stream Identification configuration

IEEE 802.1CB-2017 Stream Identification settings are defined per bridge's port and rely on both filesystem configuration files and command parameters.

## Stream Identification settings

<div align="center">
<table>
<caption><p>Stream Identification settings</p></caption>
<colgroup>
<col style="width: 13%" />
<col style="width: 22%" />
<col style="width: 15%" />
<col style="width: 35%" />
<col style="width: 13%" />
</colgroup>
<thead>
<tr>
<th>Path</th>
<th>Name</th>
<th>Condition</th>
<th>Brief description</th>
<th>Reference</th>
</tr>
</thead>
<tbody>
<tr>
<td rowspan="6">/lfs/si/N</td>
<td></td>
<td>0 &lt;= N &lt; 384</td>
<td>Stream Identification entry index</td>
<td></td>
</tr>
<tr>
<td>handle</td>
<td>&gt;= 0</td>
<td>Stream handle identifier</td>
<td><a href="#handle">handle</a></td>
</tr>
<tr>
<td>mac</td>
<td></td>
<td>MAC address</td>
<td><a href="#mac">mac</a></td>
</tr>
<tr>
<td>port_list</td>
<td></td>
<td>List of logical ports</td>
<td><a href="#port_list">port_list</a></td>
</tr>
<tr>
<td>tagged</td>
<td></td>
<td>Tag type used by the identification</td>
<td><a href="#tagged">tagged</a></td>
</tr>
<tr>
<td>type</td>
<td></td>
<td>Identification type</td>
<td><a href="#type">type</a></td>
</tr>
<tr>
<td></td>
<td>vlan</td>
<td></td>
<td>VID value</td>
<td><a href="#vlan">vlan</a></td>
</tr>
</tbody>
</table>
</div>

### Handle

An integer identifying the Stream to which the packet belongs.

### Mac

MAC address used for the identification. Depends on \<type\>.

### port_list

List of logical port ids (comma separated).

### Tagged

Tagged frame type used by the identification

1: Frame is tagged

2: Frame is untagged or tagged with vid = 0 (supported only untagged) (default)

3: Frame is tagged or not (unsupported)

### type

Identification type. If type is 1 (NULL identification) the identification is done based on the destination MAC address specified in the \<mac\> parameter. If type is 2 (Source MAC Vlan) the identification is done based on the source MAC address specified in the \<mac\> parameter.

### Vlan

VLAN ID value (default 0).

Note: This parameter must be specified (and matching an existing entry in the VLAN database) if both tagged and type parameters are set to 1.

## Stream Identification commands

### net genavb si update \<index\> \[-h \<handle\>\] \[-P \<p1\>\[,\<p2\>,...,\<pn\>\]\] \[-t \<type\>\] \[-m \<mac\>\] \[-T \<tagged\>\] \[-v \<vid\>\] \[-p\]

Parameters:

\<index\>

- stream identity table index

Options:

\[-h \<handle\>\]

- stream handle value (default: 0)

\[-P \<p1\>\]

- list of logical port ids (comma separated)

\[-t \<type\>\]

- stream identification type

1: Null (default)

2: Source MAC Vlan

\[-m \<mac\>\]

- stream identification source or destination mac address (default: 00:00:aa:bb:cc:dd)

if \<type\> is Null, identification done on Destination MAC

if \<type\> is Source MAC Vlan, identification done on Source MAC

\[-T \<tagged\>\]

- Tagged value:

1: Frame is tagged

2: Frame is untagged or tagged with vid = 0 (supported only untagged) (default)

3: Frame is tagged or not (unsupported)

\[-v \<vlan\>\]

- vid value (default: 0)

-p

- update entry in permanent database

### net genavb si delete \<index\> \[-p\]

Parameters:

\<index\>

- stream identity table index

Options:

\[-p\]

- delete entry from permanent database

### net genavb si read \<index\> \[-p\]

Parameters:

\<index\>

- stream identity table index

Options:

\[-p\]

- read entry from permanent database

## Per Stream Filtering and Policing configuration

IEEE 802.1Q PSFP settings are defined per bridge's port and rely on both filesystem configuration files and command parameters.

## Stream Filter settings

<div align="center">
<table>
<caption><p>Stream Filter settings</p></caption>
<colgroup>
<col style="width: 13%" />
<col style="width: 23%" />
<col style="width: 18%" />
<col style="width: 31%" />
<col style="width: 13%" />
</colgroup>
<thead>
<tr>
<th>Path</th>
<th>Name</th>
<th>Condition</th>
<th>Brief description</th>
<th>Reference</th>
</tr>
</thead>
<tbody>
<tr>
<td rowspan="6">/lfs/sf/N</td>
<td></td>
<td>0 &lt;= N &lt; 512</td>
<td>Stream Filter entry index</td>
<td></td>
</tr>
<tr>
<td>stream_handle</td>
<td>&gt;= 0</td>
<td>Stream handle identifier</td>
<td><a href="#stream_handle">stream_handle</a></td>
</tr>
<tr>
<td>flow_meter_enable</td>
<td>0 = disabled, 1 = enabled</td>
<td>Flow meter enable/disable</td>
<td><a href="#flow_meter_enable">flow_meter_enable</a></td>
</tr>
<tr>
<td>flow_meter_ref</td>
<td>&gt;= 0</td>
<td>Flow meter instance identifier</td>
<td><a href="#flow_meter_ref">flow_meter_ref</a></td>
</tr>
<tr>
<td>max_sdu_size</td>
<td>&gt;= 0</td>
<td>Maximum SDU allowed size</td>
<td><a href="#max_sdu_size">max_sdu_size</a></td>
</tr>
<tr>
<td>stream_gate_ref</td>
<td>&gt;= 0</td>
<td>Stream gate instance identifier</td>
<td><a href="#stream_gate_ref">stream_gate_ref</a></td>
</tr>
</tbody>
</table>
</div>

### stream_handle

An integer identifying the Stream to which the packet belongs.

### flow_meter_enable

A boolean that determines if the flow meter instance specified by flow_meter_ref is enabled or not. By default flow_meter_enable is set to 0.

### flow_meter_ref

Flow meter instance identifier.

### max_sdu_size

The maximum packet size expressed in bytes allowed by the filter. A packet with a size higher than max_sdu_size is dropped. If max_sdu_size is set to 0 size check filtering function is disabled (all packet sizes are accepted). Default value is 0.

### stream_gate_ref

Stream gate instance identifier.

## Stream Filter commands

### net genavb sf update \<index\> \[\<-h stream_handle\>\] \[\<-P \<priority_spec\>\] \[-m \<flow_meter_ref\>\] \[-M \<flow_meter_enable\>\] \] \[-g \<stream_gate_ref\>\] \[-S \<max_sdu_size\>\] \[-p\]

Adds filtering entry to the Ingress Filtering table. A new entry is created if one doesn't exist.

Parameters:

\<index\>

- user defined stream filter identifier

Options:

> \[-h \<stream_handle\>\]

- stream handle identifier (default 0)

\[-P \<priority\>\]

- vlan tag priority code port value (wildcarded if not specified)

\[-m \<flow_meter_ref\>\]

- flow meter instance identifier (default null entry 0xffffffff)

> \[-M \<flow_meter_enable\>\]\]

- flow meter enable, 0:disabled (default), 1:enabled

> \[-g \<stream_gate_ref\>\]

- stream gate instance identifier (default null entry 0xffffffff)

> \[-S \<max_sdu_size\>\]

- maximum service data unit in bytes, 0:disabled (default)

\[-p\]

- Update entry in permanent database.

### net genavb sf delete \<index\> \[-p\]

Deletes filtering entry from the Ingress Filtering table.

Parameters:

\<index\>

- user defined stream filter identifier

Options:

\[-p\]

- Delete entry from permanent database.

### net genavb sf read \<index\> \[-p\]

Reads filtering entry from the Ingress Filtering table.

Parameters:

\<index\>

- user defined stream filter identifier

Options:

\[-p\]

- Read entry from permanent database.

Following is an output example from an i.MX RT1180 TSN Bridge showing the content of a Filtering entry:

```console
uart:~$ net genavb sf read 1
Stream Filter 1
-----------------------
stream handle 0
priority spec 7
flow meter ref
flow meter enable 0
stream gate ref
max sdu size 512
matching frames count 0
passing frames count 0
not passing frames count 0
red frames count 0
not passing sdu count 0
```

## Stream Gate settings

<div align="center">
<table>
<caption><p>Stream Gate settings</p></caption>
<colgroup>
<col style="width: 13%" />
<col style="width: 23%" />
<col style="width: 18%" />
<col style="width: 31%" />
<col style="width: 13%" />
</colgroup>
<thead>
<tr>
<th>Path</th>
<th>Name</th>
<th>Condition</th>
<th>Brief description</th>
<th>Reference</th>
</tr>
</thead>
<tbody>
<tr>
<td rowspan="7">/lfs/sg/N</td>
<td></td>
<td>0 &lt;= N &lt; 32</td>
<td>StreamGate entry index</td>
<td></td>
</tr>
<tr>
<td>gate_enable</td>
<td></td>
<td>Stream gate enable</td>
<td><a href="#gate_enable">gate_enable</a></td>
</tr>
<tr>
<td>base_time</td>
<td></td>
<td>Stream gate base time</td>
<td><a href="#base_time">base_time</a></td>
</tr>
<tr>
<td>cycle_time</td>
<td></td>
<td>Stream gate cycle time</td>
<td><a href="#cycle_time">cycle_time</a></td>
</tr>
<tr>
<td>cycle_time_extension</td>
<td></td>
<td>Stream gate cycle time extension</td>
<td><a href="#cycle_time_extension">cycle_time_extension</a></td>
</tr>
<tr>
<td>list_length</td>
<td></td>
<td>Control list length</td>
<td><a href="#list_length">list_length</a></td>
</tr>
<tr>
<td>entryM</td>
<td>0 &lt;= M&lt; 6</td>
<td>Stream gate control list</td>
<td><a href="#entrym">entryM</a></td>
</tr>
</tbody>
</table>
</div>

### gate_enable

### base_time

Sets the absolute gPTP start time for the corresponding stream gate entry. 64bits value expressed in nanoseconds units. If in the past, an offset equal to N \* cycle_time is added, so actual base_time = base_time + N \* cycle_time.

### cycle_time

This setting defines the period over which the stream gate list sequence repeats. The cycle time is a 32bits value expressed in nanoseconds units and must be different from 0.

In order to set this setting, one must go into the folder **/lfs/sg/entryN** and set the setting **cycle_time** to the desired value.

```console
uart:~$ fs echo /lfs/sg/entryN/cycle_time 100000
```

### cycle_time_extension

This setting defines the maximum amount of time by which the cycle time is permitted to be extended when a new gate control sequence is being installed. The cycle time extension is used to extend the last complete cycle so that it ends at the new base time, or in other words, that the old and new cycles line up. The cycle time extension is a 32bits value expressed in nanoseconds units.

In order to set this setting, one must go into the folder **/lfs/sg/entryN** and set the setting **cycle_time_extension** to the desired value.

```console
uart:~$ fs echo /lfs/sg/entryN/cycle_time_extension 0
```

### list_length

This field indicates the number of entries in the stream gate control list.

### entryM

This file defines a stream gate control list composed of gates state, ipv, interval and octet.

### state

This field specifies the gate state for the gate control entry M. 0 the gate is closed, 1 the gate is open.

### ipv

Internal Priority Value for the gate control entry M that will be assigned to frames.

### interval

This field specifies the timer interval for the gate entry. The time is expressed in units of nanoseconds.

### octet

This field specifies the maximum bytes allowed to pass the gate entry.

## Stream Gate commands

### net genavb sg update \<index\> \[-e \<enable\>\] \[-c \<cycle_time\>\] \[-C \<cycle_time_ext\>\] \[-b \<base_time\>\] \[-l \<state,ipv,interval,octet\] \[-l …\] \[-p\]

Adds stream gate entry to the Stream Gate table. A new entry is created if one doesn't exist.

Parameters:

\<index\>

- user defined stream gate identifier

Options:

> \[-e \<enable\>\]

- gate enable, 0:disabled (default), 1: enabled

> \[-c \<cycle_time\>\]

- cycle time in ns, 0 to 4294967295, default: 100000

> \[-C \<cycle_time_ext\>\]

- cycle time extension in ns, 0 to 4294967295, default: 0

> \[-b \<base_time\>\]

- base time in ns, 0 to (2^64 - 1), default: 0

> \[-l \<state,ipv,interval,octet\>\]

- gate control list, default: 0,0,100000,0 (one option per list entry)

\[-p\]

- Update entry in permanent database.

### net genavb sg update \<index\> \[-i\] \[-x\]

Reset gate close parameters due to invalid receive or octets exceeded.

Parameters:

\<index\>

- user defined stream gate identifier

Options:

> \[-i\]

- reset gate closed due to invalid rx

> \[-x\]

- reset gate closed due to octets exceeded

### net genavb sg delete \<index\>

Deletes stream gate entry from the Stream Gate table.

Parameters:

\<index\>

- user defined stream gate identifier

Options:

\[-p\]

- Delete entry from permanent database.

### net genavb sg read \<index\>

Reads Stream Gate entry from the Stream Gate table.

Parameters:

\<index\>

- user defined stream gate identifier

Options:

\[-p\]

- Read entry from permanent database.

Following is an output example from an i.MX RT1180 TSN Bridge showing the content of a Stream Gate entry:

```console
uart:~$ net genavb sg read 1
Stream Gate 1
enable | base time             | cycle time | cycle_time ext | list length |
--------+-----------------------+------------+----------------+-------------+
1      | 876386600000          | 100000     | 0              | 1           |

Stream Gate Control List
entry | oper | state | ipv | interval (ns) | interval octet max |
-------+------+-------+-----+---------------+--------------------+
0     | 0    | 1     | 0   | 100000        | 16777215           |
```

## Flow Meter settings

<div align="center">
<table>
<caption><p>Flow Meter settings</p></caption>
<colgroup>
<col style="width: 13%" />
<col style="width: 23%" />
<col style="width: 18%" />
<col style="width: 31%" />
<col style="width: 13%" />
</colgroup>
<thead>
<tr>
<th>Path</th>
<th>Name</th>
<th>Condition</th>
<th>Brief description</th>
<th>Reference</th>
</tr>
</thead>
<tbody>
<tr>
<td rowspan="10">/lfs/fm/N</td>
<td></td>
<td>0 &lt;= N &lt; 512</td>
<td>Flow Meter entry index</td>
<td></td>
</tr>
<tr>
<td>cir</td>
<td></td>
<td>Committed rate</td>
<td><a href="#cir">cir</a></td>
</tr>
<tr>
<td>cbs</td>
<td></td>
<td>Committed burst size</td>
<td><a href="#cbs">cbs</a></td>
</tr>
<tr>
<td>eir</td>
<td></td>
<td>Excess rate</td>
<td><a href="#eir">eir</a></td>
</tr>
<tr>
<td>ebs</td>
<td></td>
<td>Excess bust size</td>
<td><a href="#ebs">ebs</a></td>
</tr>
<tr>
<td>cflag</td>
<td></td>
<td>Coupling flag</td>
<td><a href="#cflag">cflag</a></td>
</tr>
<tr>
<td>cmode</td>
<td></td>
<td>Color mode</td>
<td><a href="#cmode">cmode</a></td>
</tr>
<tr>
<td>dropy</td>
<td></td>
<td>Drop on yellow</td>
<td><a href="#dropy">dropy</a></td>
</tr>
<tr>
<td>markre</td>
<td></td>
<td>Mark all frames red enable</td>
<td><a href="#markre">markre</a></td>
</tr>
<tr>
<td>markr</td>
<td></td>
<td>Mark all frame red</td>
<td><a href="#markr">markr</a></td>
</tr>
</tbody>
</table>
</div>

### cir

Committed Information Rate

Expressed in units of bits per second, this parameter limits the average rate of frames declared green by the rate policer instance

### cbs

Committed Burst Size

Expressed in bytes, this is the maximum fill (number of bytes) allowed in the Committed (C) token bucket. This parameter limits the number of bytes allowed in a burst of frames that are declared green by the rate policer instance.

### eir

Excess Information Rate

Expressed in units of bits per second, this parameter limits the average rate of frames declared yellow by the rate policer instance.

### ebs

Excess Burst Size

Expressed in bytes, this is the maximum fill (number of bytes) allowed in the Excess (E) token bucket. This parameter limits the number of bytes allowed in a burst of frames that are declared yellow by the rate policer instance.

### cflag

Coupling flag

This field enables the feature of coupling the Committed (C) bucket and Excess (E) bucket. When the buckets are coupled, and the C bucket becomes full (C tokens = CBS), any overflow C tokens will be added to the E bucket, up to the max fill of the E bucket (EBS). 0: C and E token buckets are not coupled.

### cmode

Color mode

0: Color blind; pre-color information (encoded in the Drop Resilience (DR)) is not taken into account by the flow meter policer instance.1: = Color aware; pre-color information (encoded in the Drop Resilience (DR)) is taken into account by the rate policer instance.

### dropy

Drop on Yellow

0: Frames marked as yellow by the rate policer instance are not dropped.1: Frames marked as yellow by the rate policer instance are dropped.

### markre

Mark All Frames Red Enable

This field enables the "mark all frames red" blocking function.

### markr

Mark All Frames Red

When the "mark all frames red" feature is enabled, any frames dropped because they were marked red or yellow (if drop on yellow is enabled) will cause all subsequent frames to be marked red.

## Flow Meter commands

### net genavb fm update \<index\> \[-r \<cir\>\] \[-b \<cbs\>\] \[-R \<eir\>\] \[-B \<ebs\>\] \[-f \<flag\>\] \[-c \<cmode\>\] \[-y \<dropy\>\] \[-m \<markre\>\] \[-p\]

Adds flow meter entry to the Flow Meter table. A new entry is created if one doesn't exist.

Parameters:

\<index\>

- user defined flow meter identifier

Options:

> \[-r \<cir\>\]

- committed rate in units of bits per sec, (default 10000000)

> \[-b \<cbs\>\]

- committed burst size in bytes, (default 2000)

> \[-R \<eir\>\]

- excess rate in units of bits per sec, (default 0)

> \[-B \<ebs\>\]

- excess bust size in bytes, (default 0)

> \[-f \<flag\>\]

- couplng flag, 0: CIR and EIR buckets not coupled (default), 1: CIR and EIR buckets coupled

> \[-c \<cmode\>\]

- color mode, 0: color-blind mode (default), 1: color-aware mode

> \[-y \<dropy\>\]

- drop on yellow, 0: yellow frames eligible to drop (default), 1: yellow frames are dropped

> \[-m \<markre\>\]

- mark all frames red enabled, (default 0)

\[-p\]

- Update entry in permanent database.

### net genavb fm update \<index\> -M

Reset markAllFramesRed parameter for the specified flow meter entry.

Parameters:

\<index\>

- user defined flow meter identifier

Options:

> \[-M\]

- reset mark all frames red parameter

### net genavb fm delete \<index\>

Deletes flow meter entry from the Flow Meter Policer table.

Parameters:

\<index\>

- user defined flow meter identifier

Options:

\[-p\]

- Delete entry from permanent database.

### net genavb fm read \<index\>

Reads Flow Meter entry from the Flow Meter table.

Parameters:

\<index\>

- user defined flow meter identifier

Options:

\[-p\]

- Read entry from permanent database.

Following is an output example from an i.MX RT1180 TSN Bridge showing the content of a Flow Meter entry:

```console
uart:~$ net genavb fm read 1
Flow Meter 1
cir       | cbs        | eir       | ebs      | cflag | cmode  | dropy   | mren | mr   |
-----------+------------+-----------+----------+-------+--------+---------+------+------+
9999997   | 2000       | 0         | 0        | 0     | 0      | 0       | 0    | 0    |
```

## HSR configuration

HSR can be enabled and configured the HSR ports on filesystem configuration files. And HSR nodes can be set as different operation mode by command parameters at runtime.

## HSR ports settings

<div align="center">
<table>
<caption><p>HSR ports settings</p></caption>
<colgroup>
<col style="width: 11%" />
<col style="width: 3%" />
<col style="width: 18%" />
<col style="width: 3%" />
<col style="width: 13%" />
<col style="width: 19%" />
<col style="width: 8%" />
<col style="width: 7%" />
<col style="width: 13%" />
</colgroup>
<thead>
<tr>
<th>Path</th>
<th colspan="2">Name</th>
<th colspan="2">Condition</th>
<th colspan="2">Brief description</th>
<th colspan="2">Reference</th>
</tr>
</thead>
<tbody>
<tr>
<td colspan="2">/lfs/hsr</td>
<td colspan="2">hsr_enabled</td>
<td colspan="2"><p>0 = disabled,</p>
<p>1 = enabled</p></td>
<td colspan="2">Enable HSR</td>
<td></td>
</tr>
<tr>
<td colspan="2">/lfs/hsr/portN</td>
<td colspan="2"></td>
<td colspan="2">0 &lt;= N &lt;= 6</td>
<td colspan="2">Port index</td>
<td></td>
</tr>
<tr>
<td colspan="2"></td>
<td colspan="2">type</td>
<td colspan="2"><p>0 = reserved</p>
<p>1 = CPU host port</p>
<p>2 = HSR Ring 1 port</p>
<p>3 = HSR Ring 2 port</p>
<p>4 = Interlink port of Redbox</p>
<p>5 = Bridge management port</p></td>
<td colspan="2">Configure the type of each port.</td>
<td></td>
</tr>
</tbody>
</table>
</div>

HSR ports settings

To enable HSR and configure port types via the filesystem:

```console
uart:~$ fs echo /lfs/hsr/hsr_enabled 1
uart:~$ fs echo /lfs/hsr/portN/type 2
```

## HSR commands

### net genavb hsr mode_set \<mode\> \[-p\]

Parameters:

\<mode\>

- 0: Mode H (HSR-tagged forwarding)

- 1: Mode N (No forwarding)

- 2: Mode T (Transparent forwarding)

- 3: Mode U (Unicast forwarding)

Options:

\[-p\]

- Update entry in permanent database
