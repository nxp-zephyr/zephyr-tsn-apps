# Common configuration

Multiple configurations settings and commands are available. They enable changing the behavior of the application at runtime.

This section describes each configurable setting or command that applies to both Endpoint and Bridge devices, how to change its value and what values are consistent.

## Common commands

## help

Lists all available commands with their name and arguments.

```console
uart:~$ help
You can try to call commands with <-h> or <--help> parameter for more information.

Shell supports following meta-keys:
  Ctrl + (a key from: abcdefklnptuw)
  Alt  + (a key from: bf)
Please refer to shell documentation for more details.

Available commands:
  fs     : File system commands
  help   : Prints the help message.
  net    : Networking commands
  zperf  : Zperf commands
```

## net genavb

Lists all available GenAVB/TSN stack configuration and management commands.

```console
uart:~$ net genavb
genavb - GenAVB/TSN stack configuration and management commands
Subcommands:
  fdb         : Forwarding database configuration and management commands
  fm          : Flow Meter, Per-Stream Filtering and Policing (PSFP) configuration commands
  fp          : Frame Preemption configuration and management commands
  log         : <component_id> <level>
                    <component_id>
                        all, app, avtp, avdecc, srp, maap, common, os, fgptp, api or mgmt
                    <level>
                        crit, err, init, info or dbg
  port_stats  : <port_id>
                    parameters:
                        port_id: logical port index
  qbv         : Time-Aware Shaper (Qbv) configuration and management commands
  seqg        : Sequence Generation, Frame Replication and Elimination for Reliability (FRER) commands
  seqi        : Sequence Identification, Frame Replication and Elimination for Reliability (FRER) commands
  seqr        : Sequence Recovery, Frame Replication and Elimination for Reliability (FRER) commands
  sf          : Stream Filter, Per-Stream Filtering and Policing (PSFP) configuration commands
  sg          : Stream Gate, Per-Stream Filtering and Policing (PSFP) configuration commands
  si          : Stream Identification table configuration and management commands
  vlan        : VLAN configuration and management commands
```

## net genavb log \<component_id\> \<level\>

Sets a component log level.

> \<component_id\>
- all: set log level to all components
- app: application
- avdecc: AVDECC GenAVB/TSN stack component
- srp: SRP GenAVB/TSN stack component
- map: MAAP GenAVB/TSN stack component
- common: common code of GenAVB/TSN stack
- os: OS specific code of GenAVB/TSN stack component
- fgptp: gPTP GenAVB/TSN stack component
- mgmt: management GenAVB/TSN stack component
> \<level\>
- crit: critical (almost no output)
- err: error (only errors)
- init: init (errors and initialization logs)
- info: informative (default)
- dbg: debug (very noisy)

## net genavb port_stats \<port_id\>

Outputs Ethernet MAC statistics

The output is specific to the MAC and its driver.


### Example on NETC interface of i.MX RT1180

```console
in-bytes 7597303498
in-valid-bytes 7597303354
in-pause-frames 0
in-valid-frames 118705114
in-vlan-frames 18510184
in-uc-frames 100174645
in-mc-frames 18530469
in-bc-frames 0
in-frames 118705127
in-min-frames 0
in-64-frames 118686589
in-65-127-frames 18525
in-128-255-frames 0
in-256-511-frames 0
in-512-1023-frames 0
in-1024-1522-frames 0
in-1523-max-frames 0
in-control-frames 0
out-bytes 295878873466
out-valid-bytes 295878873466
out-pause-frames 0
out-valid-frames 212735014
out-vlan-frames 18506291
out-uc-frames 194128220
out-mc-frames 18606792
out-bc-frames 2
out-frames 212735014
out-min-frames 0
out-64-frames 18546641
out-65-127-frames 60170
out-128-255-frames 0
out-256-511-frames 0
out-512-1023-frames 0
out-1024-1522-frames 194128203
out-1523-max-frames 0
out-control-frames 0
in-valid-reassembled-frames 52192
in-additional-mPackets 52192
in-error-frame-reassembly 26
in-error-frame-smd 78
out-additional-mPackets 0
out-hold-transitions 0
in-error 6
in-error-undersize 0
in-error-oversize 0
in-error-fcs 6
in-error-fragment 7
in-error-jabber 0
in-error-discard 0
in-error-dicard-no-truncated 0
out-error-fcs 0
out-error-undersize 0
in-discard-count 3961
in-discard-reason0 1024
in-discard-table-id 15
in-discard-entry-id 7
out-discard-count 8
out-discard-reason0 256
out-discard-table-id 0
out-discard-entry-id 0
bridge-discard-count 463
bridge-discard-reason0 256
bridge-discard-table-id 2
bridge-discard-entry-id 0
q0-rejected-bytes 12144
q0-rejected-frames 8
q0-dequeue-bytes 304162930088
q0-dequeue-frames 201027083
q0-dropped-bytes 0
q0-dropped-frames 0
q0-frames 0
...
q6-rejected-bytes 0
q6-rejected-frames 0
q6-dequeue-bytes 7857450
q6-dequeue-frames 100501
q6-dropped-bytes 0
q6-dropped-frames 0
q6-frames 0
q7-rejected-bytes 0
q7-rejected-frames 0
q7-dequeue-bytes 1184402688
```

## gPTP settings

<div align="center">
<table>
<caption><p>gPTP settings</p></caption>
<colgroup>
<col style="width: 13%" />
<col style="width: 18%" />
<col style="width: 23%" />
<col style="width: 30%" />
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
<td rowspan="8">/lfs/gptp</td>
<td>is_bridge</td>
<td></td>
<td>Stack operating mode</td>
<td><a href="#stack-operating-mode">Stack Operating Mode</a></td>
</tr>
<tr>
<td>force_2011</td>
<td></td>
<td>Force 802.1AS-2011 standard only</td>
<td><a href="#force-8021as-2011-standard">Force 802.1AS-2011 Standard</a></td>
</tr>
<tr>
<td>gmCapable</td>
<td>domain 0</td>
<td>Capability of the endpoint to be a gPTP grand master</td>
<td><a href="#grand-master-capability">Grand Master Capability</a></td>
</tr>
<tr>
<td>priority1</td>
<td>domain 0</td>
<td>Priority 1 field of gPTP configuration</td>
<td><a href="#gptp-priority-1">gPTP Priority 1</a></td>
</tr>
<tr>
<td>priority2</td>
<td>domain 0</td>
<td>Priority 2 field of gPTP configuration</td>
<td><a href="#gptp-priority-2">gPTP Priority 2</a></td>
</tr>
<tr>
<td>clockClass</td>
<td>domain 0</td>
<td>Class value of the gPTP clock</td>
<td><a href="#gptp-clockclass">gPTP clockClass</a></td>
</tr>
<tr>
<td>clockAccuracy</td>
<td>domain 0</td>
<td>Accuracy value of the gPTP clock</td>
<td><a href="#gptp-clockaccuracy">gPTP clockAccuracy</a></td>
</tr>
<tr>
<td>OffsetScaledLogVariance</td>
<td>domain 0</td>
<td>Variance of the gPTP clock</td>
<td><a href="#gptp-offsetscaledlogvariance">gPTP offsetScaledLogVariance</a></td>
</tr>
<tr>
<td rowspan="3">/lfs/gptp/domainN</td>
<td></td>
<td>0 &lt; N &lt;= 1</td>
<td>gPTP domain index</td>
<td><a href="#gptp-domainn">gPTP domainN</a></td>
</tr>
<tr>
<td>domain_number</td>
<td></td>
<td>gPTP domain number</td>
<td><a href="#gptp-domain_number">gPTP domain_number</a></td>
</tr>
<tr>
<td>…</td>
<td>Same parameters as for domain 0</td>
<td></td>
<td></td>
</tr>
<tr>
<td rowspan="8">/lfs/gptp/portN</td>
<td></td>
<td>0 &lt;= N &lt; 4</td>
<td>gPTP logical port index</td>
<td>Check logical ports mapping for each application in Evaluation Applications Section</td>
</tr>
<tr>
<td>rxDelayCompensation</td>
<td></td>
<td>Rx timestamp compensation</td>
<td><a href="#rx-phy-delay-compensation">Rx PHY Delay Compensation</a></td>
</tr>
<tr>
<td>txDelayCompensation</td>
<td></td>
<td>Tx timestamp compensation</td>
<td><a href="#tx-phy-delay-compensation">Tx PHY Delay Compensation</a></td>
</tr>
<tr>
<td style="text-align: center;">initialLogPdelayReqInterval</td>
<td></td>
<td style="text-align: center;">Pdelay request initial interval</td>
<td><a href="#gptp-initiallogpdelayreqinterval">gPTP InitialLogPdelayReqInterval</a></td>
</tr>
<tr>
<td style="text-align: center;">initialLogSyncInterval</td>
<td></td>
<td style="text-align: center;">Sync transmit initial interval</td>
<td><a href="#gptp-initiallogsyncinterval">gPTP InitialLogSyncInterval</a></td>
</tr>
<tr>
<td style="text-align: center;">initialLogAnnounceInterval</td>
<td></td>
<td style="text-align: center;">Initial announce transmit interval</td>
<td><a href="#gptp-initiallogannounceinterval">gPTP initialLogAnnounceInterval</a></td>
</tr>
<tr>
<td style="text-align: center;">allowedLostResponses</td>
<td></td>
<td style="text-align: center;">Pdelay_Req messages without valid responses</td>
<td><a href="#gptp-allowedlostresponses">gPTP allowedLostResponses</a></td>
</tr>
<tr>
<td style="text-align: center;">delayMechanism</td>
<td>domain 0</td>
<td>Peer delay mechanism</td>
<td><a href="#gptp-delaymechanism">gPTP delayMechanism</a></td>
</tr>
<tr>
<td rowspan="3">/lfs/gptp/domainX/portN</td>
<td style="text-align: center;"></td>
<td>0 &lt; X &lt;= 1</td>
<td>gPTP domain index</td>
<td><a href="#gptp-domainn">gPTP domainN</a></td>
</tr>
<tr>
<td style="text-align: center;"></td>
<td>0 &lt;= N &lt; 4</td>
<td>gPTP logical port index</td>
<td>Check logical ports mapping for each application in Evaluation Applications Section</td>
</tr>
<tr>
<td style="text-align: center;">delayMechanism</td>
<td></td>
<td>Peer delay mechanism</td>
<td><a href="#gptp-delaymechanism">gPTP delayMechanism</a></td>
</tr>
</tbody>
</table>
</div>

## Stack operating mode

This setting can take value 0 or 1. By default, this setting is set to 1, which means the gPTP stack operates as a gPTP bridge. If set to 0 the stack operates as a gPTP endpoint.

Note: in this release only gPTP bridge mode is supported.

## Force 802.1AS-2011 standard

This setting can take value 0 or 1. By default, this setting is set to 0, which means the gPTP stack supports IEEE 802.1AS-2020 standard.

This setting defines if the gPTP Stack operates following the IEEE 802.1AS-2011 standard, i.e. disabling the IEEE 802.1AS-2020 specifics features such as Multiple Domain support. The use of this option may, in some cases, improve compatibility with gPTP equipment not supporting IEEE 802.1AS-2020 standard.

## Grand master capability

This setting can take value 0 or 1. By default, this setting is set to 0 for a **TSN Endpoint** (meaning the endpoint is not grand master capable) and set to 1 for a **TSN Bridge**.

In order to set this setting, one must go into the folder **/lfs/gptp** and set the setting **gmCapable** to the desired value.

```console
uart:~$ fs echo /lfs/gptp/gmCapable 1
```

## gPTP priority 1

This setting can take value between 0 to 255. By default, this setting is set to 248 for a **TSN Endpoint** and 246 for a **TSN Bridge**.

In order to set this setting, one must go into the folder **/lfs/gptp** and set the setting **priority1** to the desired value.

```console
uart:~$ fs echo /lfs/gptp/priority1 248
```

## gPTP priority 2

This setting can take value between 0 to 255. By default, this setting is set to 248.

In order to set this setting, one must go into the folder **/lfs/gptp** and set the setting **priority2** to the desired value.

```console
uart:~$ fs echo /lfs/gptp/priority2 248
```

## gPTP clockClass

This setting can take value between 0 to 255. By default, this setting is set to 248.

In order to set this setting, one must go into the folder **/lfs/gptp** and set the setting **clockClass** to the desired value.

```console
uart:~$ fs echo /lfs/gptp/clockClass 248
```

## gPTP clockAccuracy

This setting can take value between 0 to 0xFF. By default, this setting is set to 0xFE.

In order to set this setting, one must go into the folder **/lfs/gptp** and set the setting **clockAccuracy** to the desired value.

```console
uart:~$ fs echo /lfs/gptp/clockAccuracy 0xFE
```

## gPTP offsetScaledLogVariance

This setting can take value from 0 to 0xFFFF. By default, this setting is set to 0x436A.

In order to set this setting, one must go into the folder **/lfs/gptp** and set the setting **offsetScaledLogVariance** to the desired value.

```console
uart:~$ fs echo /lfs/gptp/offsetScaledLogVariance 0x436A
```

## gPTP domainN

This setting can take value from 1 to GPTP_MAX_DOMAINS.

In order to set parameters for domainN settings, one must go into the folder **/lfs/gptp/domainN** and set the gPTP parameters as for domain 0.

## gPTP domain_number

This setting can take value from -1 to 127. By default, this setting is set to -1. Applies only to domains index \> 0.

In order to set parameters for domain_number setting, one must go into the folder **/lfs/gptp/domainN/domain_number** and set the gPTP domain number.

```console
uart:~$ fs echo /lfs/gptp/domainN/domain_number 1
```

## PHY delay compensation

The following table shows the measured Rx and Tx PHY delay values for the supported NXP boards. For optimized gPTP synchronization PHY delay compensation should be applied at run time.

<div align="center">
<table style="width:93%;">
<caption><p>Measured Rx and Tx PHY delay values for supported NXP boards</p></caption>
<colgroup>
<col style="width: 22%" />
<col style="width: 16%" />
<col style="width: 10%" />
<col style="width: 10%" />
<col style="width: 16%" />
<col style="width: 16%" />
</colgroup>
<thead>
<tr>
<th style="text-align: center;"><strong>Device</strong></th>
<th style="text-align: center;"><strong>PHY name</strong></th>
<th style="text-align: center;"><strong>Speed (Mbps)</strong></th>
<th style="text-align: center;"><strong>Logical port on<br />
EVK</strong></th>
<th style="text-align: center;"><strong>Rx PHY Delay (ns)</strong></th>
<th style="text-align: center;"><strong>Tx PHY Delay (ns)</strong></th>
</tr>
</thead>
<tbody>
<tr>
<td style="text-align: center;">i.MX RT1180</td>
<td style="text-align: center;">RTL8211F</td>
<td style="text-align: center;">1000</td>
<td style="text-align: center;">3, 4, 5</td>
<td style="text-align: center;">580</td>
<td style="text-align: center;">252</td>
</tr>
<tr>
<td style="text-align: center;">i.MX RT1180</td>
<td style="text-align: center;">RTL8211F</td>
<td style="text-align: center;">100</td>
<td style="text-align: center;">3, 4, 5</td>
<td style="text-align: center;">783</td>
<td style="text-align: center;">1347</td>
</tr>
<tr>
<td style="text-align: center;">i.MX RT1180</td>
<td style="text-align: center;">RTL8201F</td>
<td style="text-align: center;">100</td>
<td style="text-align: center;">0, 2</td>
<td style="text-align: center;">794</td>
<td style="text-align: center;">4024</td>
</tr>
</tbody>
</table>
</div>

<span id="_Ref152866341" class="anchor"></span>Measured Rx and Tx delay for different PHY's.

\*NM = Not Measured

The NXP boards already have Rx compensation and Tx compensation applied in software. The values applied in software are computed using [the table above](#phy-delay-compensation) and applying a margin of 50 ns for the total propagation delay.

Refer to below section to understand how to change the setting for the boards listed on the above table.

### Rx PHY delay compensation

This setting can take value from -1000000 to 1000000 in ns unit. By default, this setting is set to 0. This setting sets the Rx timestamp compensation, subtracted from the receive timestamps.\
\
In order to set this setting, one must go into the folder **/lfs/gptp/portN** and set the setting **rxDelayCompensation** to the desired value.

```console
uart:~$ fs echo /lfs/gptp/portN/rxDelayCompensation 580
```

### Tx PHY delay compensation

This setting can take value from -1000000 to 1000000 in ns unit. By default, this setting is set to 0. This setting sets the Tx timestamp compensation, added to transmit timestamps.

In order to set this setting, one must go into the folder **/lfs/gptp/portN** and set the setting **txDelayCompensation** to the desired value.

```console
uart:~$ fs echo /lfs/gptp/portN/txDelayCompensation 252
```

## gPTP InitialLogPdelayReqInterval

This setting can take value from 0 to 3 in log2 unit. By default, this setting is set to 0. This setting sets the pdelay request initial interval.

In order to set this setting, one must go into the folder **/lfs/gptp/portN** and set the setting **initialLogPdelayReqInterval** to the desired value.

```console
uart:~$ fs echo /lfs/gptp/portN/initialLogPdelayReqInterval 0
```

##  gPTP InitialLogSyncInterval

This setting can take value from -5 to 0 in log2 unit. By default, this setting is set to -3. This setting sets the sync transmit initial interval.

In order to set this setting, one must go into the folder **/lfs/gptp/portN** and set the setting **initialLogSyncInterval** to the desired value.

```console
uart:~$ fs echo /lfs/gptp/portN/initialLogSyncInterval -3
```

##  gPTP initialLogAnnounceInterval

This setting can take value from 0 to 3 in log2 unit. By default, this setting is set to 0. This setting sets the initial announce transmit interval.

In order to set this setting, one must go into the folder **/lfs/gptp/portN** and set the setting **initialLogAnnounceInterval** to the desired value.

```console
uart:~$ fs echo /lfs/gptp/portN/initialLogAnnounceInterval 0
```

##  gPTP allowedLostResponses

This setting can take value from 1 to 255. By default, this setting is set to 3 if gPTP setting force_2011 is set to 1, otherwise it is set to 9. This setting sets the number of Pdelay_Req messages without valid responses above which a PTP Port is considered to be not exchanging peer delay messages with its neighbor.

In order to set this setting, one must go into the folder **/lfs/gptp/portN** and set the setting **allowedLostResponses** to the desired value.

```console
uart:~$ fs echo /lfs/gptp/portN/allowedLostResponses 9
```

##  gPTP delayMechanism

This setting can take values of 2 (P2P mechanism) or 3 (COMMON_P2P mechanism) for domain number 0. This setting can take value of 3 (COMMON_P2P mechanism) for domain number greater than 0. This setting sets the peer delay mechanism associated with the Port.

In order to set this setting for domain 0, one must go into the folder **/lfs/gptp/portN** and set the setting **delayMechanism** to the desired value.

```console
uart:~$ fs echo /lfs/gptp/portN/delayMechanism 2
```

In order to set parameters for domainX settings, one must go into the folder **/lfs/gptp/domainX/portN** and set the setting **delayMechanism** to the desired value.

```console
uart:~$ fs echo /lfs/gptp/domainX/portN/delayMechanism 3
```

## Network settings

*Scheduled Traffic configuration paths*

<div align="center">
<table>
<caption><p>Network settings</p></caption>
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
<td rowspan="5">/lfs/portN</td>
<td></td>
<td>0 &lt;= N &lt; 7</td>
<td>Logical port number</td>
<td>Check logical ports mapping for each application in Evaluation Applications Section</td>
</tr>
<tr>
<td>hw_addr</td>
<td></td>
<td>MAC address of the port</td>
<td><a href="#hardware-address">Hardware Address</a></td>
</tr>
<tr>
<td>ip_addr</td>
<td></td>
<td>IPv4 address of the endpoint</td>
<td><a href="#ip-address">IP Address</a></td>
</tr>
<tr>
<td>gw_addr</td>
<td></td>
<td>Gateway address of the endpoint</td>
<td><a href="#gateway-address">Gateway Address</a></td>
</tr>
<tr>
<td>net_mask</td>
<td></td>
<td>Network mask of the endpoint</td>
<td><a href="#network-mask">Network Mask</a></td>
</tr>
</tbody>
</table>
</div>

**Note**: These settings don't apply for the i.MX RT1180 board Cortex-M7, since it doesn't have access to the filesystem. For more information about setting the i.MX RT1180 Cortex-M7 parameters, see section [TSN Endpoint Settings](04_endpoint_configuration.md#tsn-endpoint-settings). Also, the condition for **/lfs/portN** can vary depending on how many logical ports a device has available.

## Hardware address

In order to set this parameter, go into the folder **/lfs/portN** and set the **hw_addr** to the desired value.

The format of this parameter is the one of a MAC address: **xx:xx:xx:xx:xx:xx**

```console
uart:~$ fs echo /lfs/portN/hw_addr 00:11:22:33:44:05
```

### i.MX RT1180

The following table indicates the default MAC address of each logical port for Cortex-M33.

<div align="center">
<table>
<caption><p>Network settings for Cortex-M33 of i.MX RT1180 EVK</p></caption>
<thead>
<tr>
<th>Logical Port</th>
<th>Default hw_addr</th>
</tr>
</thead>
<tbody>
<tr><td>0</td><td>00:BB:CC:DD:EE:00</td></tr>
<tr><td>1</td><td>00:BB:CC:DD:EE:01</td></tr>
<tr><td>2</td><td>00:11:22:33:44:02</td></tr>
<tr><td>3</td><td>00:11:22:33:44:03</td></tr>
<tr><td>4</td><td>00:11:22:33:44:04</td></tr>
<tr><td>5</td><td>00:11:22:33:44:05</td></tr>
</tbody>
</table>
  </div>

The following table indicates the default MAC address of each logical port for Cortex-M7.

<div align="center">
<table>
<caption><p>Network settings for Cortex-M7 of i.MX RT1180 EVK</p></caption>
<thead>
<tr>
<th>Logical Port</th>
<th>Default hw_addr</th>
</tr>
</thead>
<tbody>
<tr><td>0</td><td>00:BB:CC:DD:EF:10</td></tr>
</tbody>
</table>
  </div>

## IP address

In order to set this parameter, go into the folder **/lfs/portN** and set the **ip_addr** to the desired value. This parameter is applicable only to endpoint ports.

The format of this parameter is the one of an IPv4 address, such as: **192.168.1.10**

```console
uart:~$ fs echo /lfs/portN/ip_addr 192.168.1.10
```

## Gateway address

In order to set this parameter, go into the folder **/lfs/portN** and set the **gw_addr** to the desired value. This parameter is applicable only to endpoint ports.

The format of this parameter is the one of an IPv4 address, such as: **192.168.1.254**

```console
uart:~$ fs echo /lfs/portN/gw_addr 192.168.1.254
```

## Network mask

In order to set this parameter, go into the folder **/lfs/portN** and set the **net_mask** to the desired value. This parameter is applicable only to endpoint ports.

The format of this parameter is the one of an IPv4 address, such as: **255.255.255.0**

```console
uart:~$ fs echo /lfs/portN/net_mask 255.255.255.0
```

## Scheduled Traffic (Qbv) configuration

## Scheduled Traffic settings

IEEE 802.1Qbv settings are defined per port and rely on both filesystem configuration files and command parameters.

<div align="center">
<table>
<caption><p>Scheduled traffic settings</p></caption>
<colgroup>
<col style="width: 23%" />
<col style="width: 17%" />
<col style="width: 20%" />
<col style="width: 25%" />
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
<td rowspan="6">/lfs/qbv/portN</td>
<td></td>
<td>0 &lt;= N &lt; 8</td>
<td>Logical port index</td>
<td>Check logical ports mapping for each application in Evaluation Applications Section</td>
</tr>
<tr>
<td>enabled</td>
<td></td>
<td>Optional, Qbv enabled on portN after the reboot</td>
<td><a href="#enabled">Enabled</a></td>
</tr>
<tr>
<td>base_time</td>
<td></td>
<td>Qbv gate list base time</td>
<td><a href="#base-time">Base Time</a></td>
</tr>
<tr>
<td>cycle_time</td>
<td></td>
<td>Qbv gate list cycle time</td>
<td><a href="#cycle-time">Cycle Time</a></td>
</tr>
<tr>
<td>cycle_time_ext</td>
<td></td>
<td>Qbv gate list cycle time extension</td>
<td><a href="#cycle-time-extension">Cycle Time Extension</a></td>
</tr>
<tr>
<td>entryM</td>
<td>0 &lt;= M &lt; 256</td>
<td>Qbv gate list entry</td>
<td><a href="#gate-list-entry">Gate List Entry</a></td>
</tr>
<tr>
<td>/lfs/qbv/portN/max_sdu</td>
<td>qM</td>
<td>0 &lt;= M &lt; 8</td>
<td>Service Data Unit size per queue (i.MX RT1180 only)</td>
<td><a href="#imx-rt1180">i.MX RT1180</a></td>
</tr>
</tbody>
</table>
</div>

### Enabled

If set to 1, Qbv configuration for particular port will be applied automatically after the boot. If not existing or set to 0, Qbv is disabled by default. This configuration is updated by `net genavb qbv set` and `net genavb qbv disable` commands, meaning ***net genavb qbv set/net genavb qbv disable*** will enable/disable Qbv for desired port immediately and update "enabled" file.

### Base time

Sets the absolute gPTP start time for the corresponding 802.1Qbv entries for the port. 64bits value expressed in nanoseconds units. If in the past, an offset equal to N \* cycle_time is added, so actual base_time = base_time + N \* cycle_time.

### Cycle time

This setting defines the period of time over which the 802.1Qbv gate list sequence repeats. The cycle time is a 32bits value expressed in nanoseconds units and must be different from 0.

In order to set this setting, one must go into the folder **/lfs/qbv/portN** and set the setting **cycle_time** to the desired value

```console
uart:~$ fs echo /lfs/qbv/portN/cycle_time 100000
```

### Cycle time extension

This setting defines the maximum amount of time by which the cycle time is permitted to be extended when a new gate control sequence is being installed. The cycle time extension is used to extend the last complete cycle so that it ends at the new base time, or in other words, that the old and new cycles line up. The cycle time extension is a 32bits value expressed in nanoseconds units.

In order to set this setting, one must go into the folder **/lfs/qbv/portN** and set the setting **cycle_time_ext** to the desired value

```console
uart:~$ fs echo /lfs/qbv/portN/cycle_time_ext 0
```

### Gate list entry

This file defines an entry for 802.1Q gate list composed of gates state, state interval and gate operation.

### Gates state

The gate list state define the open/close state for each traffic class transmit queue. The gate list state is expressed as a 8 bits field defined as follow:

> Bit 0: Gate state for traffic class 0.
>
> Bit 1: Gate state for traffic class 1.
>
> Bit 2: Gate state for traffic class 2.
>
> Bit 3: Gate state for traffic class 3.
>
> Bit 4: Gate state for traffic class 4.
>
> Bit 5: Gate state for traffic class 5.
>
> Bit 6: Gate state for traffic class 6.
>
> Bit 7: Gate state for traffic class 7.

For each of the traffic classes above, the following values apply:

0b = The gate is closed, data is not transmitted.

1b = The gate is open, data is transmitted.

### Gate state interval

The gate list interval defines the period during which the defined gate list entry state is applied. The gate list entry interval is a 32 bits value expressed in nanoseconds.

### Gate operation (optional)

The gate list operation defines the operation performed on the gate during the state interval. It has three possible values:

- 0 - which represents the operation ***SetGateStates***, default value used when only scheduled traffic is enabled.

When frame preemption is enabled with scheduled traffic, it can have two extra values:

- 1 - which represents the operation ***Set-And-Hold-MAC***

- 2 - which repesents the operation ***Set-And-Release-MAC***.

When scheduled traffic is used without frame preemption, the only gate operation used is ***SetGateStates***, which sets the open or closed state, for each of the Port's queues.

When frame preemption is used with scheduled traffic, the "Hold and Release" mechanism can be used to protect the express traffic class window from interference from preemptable traffic. This is acomplished by the operations:

- ***Set-And-Hold-MAC*** : Performs all of the actions defined for the ***SetGateStates*** operation, and in addition issues a hold request to the MAC, action which will prevent transmission of preemptable frames by the scheduled traffic scheduler.

- ***Set-And-Release-MAC :*** Performs all of the actions defined for the ***SetGateStates*** operation, and in addition issues a release request to the MAC, action which will allow transmission of preemptable frames by the scheduled traffic scheduler.

The file format is:

**gates state**,**state interval**\<, **gate operation** \>

Where:

- gates state is a one byte represented as two hexadecimal digits.

- state interval is a decimal unsigned integer.

- \< **gate operation** \> is an optional unsigned integer value to represent the gate operation. When not written, its default value is used.

For example:

```console
uart:~$ fs echo /lfs/qbv/portN/entry0 20,20000
```

Defines an entry 0 with only gate 5 open for 20000ns

```console
uart:~$ fs echo /lfs/qbv/portN/entry1 db,225000
```

Defines an entry 1 with gates 2 and 5 closed for 225000ns

```console
uart:~$ fs echo /lfs/qbv/portN/entry2 0F,225000,1
```

Defines an entry 2 with gates 0 to 3 open for 225000ns, and the preemptable frames from those open gates won't be transmitted.

```console
uart:~$ fs echo /lfs/qbv/portN/entry3 f7,95000,2
```

Defines an entry 3 with only gate 3 closed for 95000ns. Also, the preemptable frames will resume their transmissions.

## Scheduled Traffic commands

### net genavb qbv set \<port_id\> \[-b \<base_time\>\] \[-c \<cycle_time\>\] \[-C \<cycle_time_ext\>\] \[-l \<gate_states\>,\<time_interval\>\[,\<gate_operation\>\] \[-l ...\] \[-p\]

Enable 802.1Qbv operations on a port immediately and save that state, Qbv will be automatically enabled after each reboot.

\<port_id\>

- Logical port index the 802.1Qbv operations are enabled on.

\[-b \<base_time\>\]

- base time in ns, 0 to (2^64 - 1), default: 0

\[-c \<cycle_time\>\]

- cycle time in ns, 0 to 4294967295, default: 100000

\[-C \<cycle_time_ext\>\]

- Cycle time extension in ns, 0 to 4294967295, default: 0

\[-l \<gate_states\>,\<time_interval\>\[,\<gate_operation\>\]\]

- gate control list, default: ff,100000,0 (one option per list entry)

\[-p\]

- update configuration in permanent database.

### net genavb qbv disable \<port_id\> \[-p\]

Disable 802.1Qbv operations on a port immediately and save that state, Qbv will be disabled after each reboot.

\<port_id\>

- Logical port index the 802.1Qbv operations are disabled on.

\[-p\]

- Update configuration in permanent database.

### net genavb qbv get \<port_id\> \[-t \<oper_type\>\] \[-p\]

Output a port's 802.1Qbv settings

\<port_id\>

- Logical port index the 802.1Qbv settings are requested for.

> \<oper_type\>

- Qbv configuration type: 0=OPER, 1=ADMIN (default OPER)

\[-p\]

- Update configuration in permanent database.

When specified the \<-p\> option retrieve the permanent Qbv settings directly from the file system (configuration storage). By default the Qbv settings are read from the hardware registers.

Following is an output example from an i.MX RT1180 TSN Bridge showing the Qbv settings on bridge's first port (logical port id 2):

```console
uart:~$ net genavb qbv get 2
port_id 2
enable 1
base_time 1552773327101 (ns)
cycle_time 250000 (ns)
cycle_time_ext 0
list_length 2
gate list:
entry | oper | gate | interval (ns)
-------+-------+------+---------
0 | 0x00 | 0x20 | 20000
1 | 0x00 | 0xdf | 230000
```

## i.MX RT1180

The i.MX RT1180 board supports extra scheduled traffic settings and commands, that are not supported by other platforms

<div align="center">
<table>
<caption><p>i.MX RT1180 Scheduled traffic extra setting</p></caption>
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
<td>/lfs/qbv/portN/max_sdu</td>
<td>qM</td>
<td>0 &lt;= M &lt; 8</td>
<td>Maximum Service Data Unit (SDU) size per queue</td>
<td><a href="#maximum-service-data-unit">Maximum Service Data Unit</a></td>
</tr>
</tbody>
</table>
</div>

i.MX RT1180 Scheduled traffic extra setting

### Scheduled Traffic settings

### Maximum Service Data Unit

For a chosen traffic class, q0 to q8, is supported setting the maximum Service Data Unit (SDU) the traffic class can support for a given port. It means that if a certain packet goes through a traffic class whose SDU is smaller than the packet, the packet is discarded.

### Scheduled Traffic commands

### net genavb qbv set_max_sdu \<port_id\> \[-l \<traffic class queue\>,\<maxSDU\> \[-l ...\]\] \[-p\]

Enable max Service Data Unit (SDU) configuration on a traffic class on a port immediately and save that state. It will be automatically enabled after each reboot.

\<port_id\>

- Logical port index the operation is enabled on.

\[-l \<traffic class queue\>,\<maxSDU\>\]\[-l ...\]\]

- List of traffic classes with their respectives SDU values, default: N,0 (0 \<= N \< 8, one option per list entry)

\[-p\]

- update configuration in permanent database.

###  net genavb qbv get_max_sdu \<port_id\> \[-p\]

Output a port's max SDU settings

\<port_id\>

- Logical port index the operation is enabled on.

\[-p\]

- update configuration in permanent database.

## Frame Preemption (Qbu) configuration

## Frame Preemption settings

IEEE 802.1Qbu settings are defined per port and rely on both filesystem configuration files and command parameters.

**Notes**: By default, Qbu is disabled. The user needs to activate it manually with ***net genavb fp set*** command.

<div align="center">
<table>
<caption><p>Frame Preemption settings</p></caption>
<colgroup>
<col style="width: 16%" />
<col style="width: 16%" />
<col style="width: 16%" />
<col style="width: 16%" />
<col style="width: 16%" />
<col style="width: 16%" />
</colgroup>
<thead>
<tr>
<th><p>Path</p>
<p>(parent)</p></th>
<th>Path</th>
<th>Name</th>
<th>Condition</th>
<th>Brief description</th>
<th>Reference</th>
</tr>
</thead>
<tbody>
<tr>
<td>/lfs/fp/portN</td>
<td></td>
<td></td>
<td>N=0</td>
<td>Logical port index</td>
<td></td>
</tr>
<tr>
<td></td>
<td rowspan="3"><strong>/802_1Q</strong></td>
<td>admin_status</td>
<td>Hexadecimal bitmask</td>
<td>Queue's traffic class priority</td>
<td><a href="#admin_status">admin_status</a></td>
</tr>
<tr>
<td></td>
<td>preemption_active</td>
<td>Read-only</td>
<td>preemption is both supported by the MAC and is currently active.</td>
<td><a href="#preemption_active">preemption_active</a></td>
</tr>
<tr>
<td></td>
<td>hold_request</td>
<td>Read-only</td>
<td>A hold request was issued</td>
<td><a href="#hold_request">hold_request</a></td>
</tr>
<tr>
<td></td>
<td rowspan="7"><strong>/802_3</strong></td>
<td>support</td>
<td>Read-only</td>
<td>given device supports a MAC Merge sublayer</td>
<td><a href="#support">support</a></td>
</tr>
<tr>
<td></td>
<td>status_verify</td>
<td>Read-only</td>
<td>status of the MAC Merge sublayer verification on the given device</td>
<td><a href="#status_verify">status_verify</a></td>
</tr>
<tr>
<td></td>
<td>status_tx</td>
<td>Read-only</td>
<td>status of the MAC Merge sublayer on the given device in the transmit direction</td>
<td><a href="#status_tx">status_tx</a></td>
</tr>
<tr>
<td></td>
<td>enable_tx</td>
<td>&gt;= 0 &amp;&amp; &lt;3</td>
<td>status of the MAC Merge sublayer on the given device in the transmit direction</td>
<td><a href="#enable_tx">enable_tx</a></td>
</tr>
<tr>
<td></td>
<td>verify_disable_tx</td>
<td>&lt; 2</td>
<td>status of the Verify function of MAC Merge sublayer on the given device in the transmit direction.</td>
<td><a href="#verify_disable_tx">verify_disable_tx</a></td>
</tr>
<tr>
<td></td>
<td>verify_time</td>
<td>&gt;= 1 &amp;&amp; &lt;=128</td>
<td>defines the nominal wait time between verification attempts in milliseconds.</td>
<td><a href="#verify_time">verify_time</a></td>
</tr>
<tr>
<td></td>
<td>add_frag_size</td>
<td>2-bit size</td>
<td>A 2-bit integer value used to indicate the value of addFragSize variable used by the Transmit Processing</td>
<td><a href="#add_frag_size">add_frag_size</a></td>
</tr>
</tbody>
</table>
</div>

### admin_status

This parameter is the administrative value of the preemption status for the priority. The hardware has 8 transmit queues, which can be split between preemptable and express, when frame preemptable is enabled.

The queue assigned as express will have higher priority than those preemptable and transmitted by the express MAC.

### preemption_Active

Boolean that is TRUE if preemption is supported by the MAC and it is currently active.

### hold_request

The hold_request object contains an enumerated integer value, with hold (1) indicating that a MM_CTL.request(HOLD) has been issued, and release (2) indicating that a MM_CTL.request(RELEASE) has been issued. This object exists per Port.

**Note:** this attribute only works when used with scheduled traffic (Qbv), and currently it is not supported.

### support

An entry that has one of the following values:

supported (1) MAC Merge sublayer is supported on the device

not supported (0) MAC Merge sublayer is not supported on the device

### status_verify

A read-only entry that has one of the following values: *unknown* (5), *initial* (1), *verifying* (2), *succeeded* (3), *failed* (4), *disabled* (0).

### status_tx

A read only entry that has one of the following values:

Inactive (0) transmit preemption is inactive

Active (1) transmit preemption is active

**Note**: Using the `net genavb fp get` operation, you likely won't see this value as active, since it is only activated when preempting packet, which is hard to predict when exactly it occurs.

### enable_tx

An ENUMERATED VALUE that has entry *disabled* (transmit preemption is disabled) or *enabled* (transmit preemption is enabled). This attribute indicates (when accessed via a GET operation) the status of the MAC Merge sublayer on the given device in the transmit direction. The status of the MAC Merge sublayer may be modified to the indicated value via a SET operation.

### verify_disable_tx

An ENUMERATED VALUE that has as entry the value *disabled* (verify is disabled) and *enabled* (verify is enabled).

### verify_time

The integer value of this attribute defines the nominal wait time between verification attempts in milliseconds. Valid range is 1 to 128 inclusive. The default value is 10.

### add_frag_size

A 2-bit integer value used to indicate the value of the non-final fragment size when a packet is preempted.

## Frame Preemption commands

### net genavb fp set \<port_id\> \[-q\] \[-t 0x\<express mask\>\] \[-e \<enable/disable\>\] \[-d \<enable/disable\>\] \[-v \<time ms\>\] \[-a \<frag size\>\] \[-p\]

It enables or disables frame preemption on a given logical port's directory on the filesystem. It creates a new one if it doesn't exist.

Parameters:

\<port_id\>

- Logical port index

\[-q\]

- Only set 802.1Q standard (when not used, it will only set 802.3 standard)

\[-t 0x\<express mask\>\]

- Frame preemption status bitmask in hexadecimal : 0 (Preemptable) and 1 (Express). It represents the frame preemption status table on the standard.

\[-e \<enable/disable\>\]

- Enable frame preemption : enable 4B boundaries (2), enable 64B boundaries (1), disable (0) (default)

\[-d \<enable/disable\>\]

- Verify disable state machine: disable (1) (default), enable (0)

\[-v \<time ms\>\]

- Verify time (microseconds) : valid range 1 to 128, default: 10

\[-a \<fragment size\>\]

- Additional fragment size (AFS) : minimum size of non-final fragments supported by the remote port \[units of 64 bytes -\> (AFS+1)\*64\]. Default value : 0.

\[-p\]

- Save *set* configuration on the filesystem (permanent).

### net genavb fp get \<port_id\> \[-p\]

Get frame preemption configuration from the hardware or from the permanent database (filesystem).

Parameters:

\<port_id\>

- Logical port index

\[-p\]

- read configuration from the filesystem (permanent database).

The following is an output example from an i.MX RT1180 TSN Bridge showing the get command from the hardware:

```console
uart:~$ net genavb fp get 3
Reading config from the hardware
802.1Q:
admin status : ppEEppEE
Preemption active : 1
Hold request : RELEASE
802.3:
support : 1
status verify : 3
status tx : 0
enable tx : 1
verify disable tx : 0
verify time : 10
add frag size : 0
```

## Frame Preemption + Scheduled Traffic configuration

## i.MX RT1180 (as TSN Bridge)

Frame Preemption and Scheduled Traffic can be used together on bridge ports, with a small update to gate control list entries.

User can make use of the "Hold and Release" mechanism to better control the scheduling of preemptable packets as explained in [Gate Operation](#gate-operation-optional) section.
