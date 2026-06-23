# Endpoint configuration

This section describes each configurable setting that applies to Endpoint devices, how to change its value and what values are consistent.

## i.MX RT118x

## TSN Endpoint settings

This section describes each configurable setting that applies to the i.MX RT1180 **TSN Endpoint**, how to change its value and what values are consistent. The **TSN Endpoint** configuration is done through the Cortex-M33 shell interface, under the **/lfs/m7** directory. Note that the directories and parameters are not available by default. Users need to create the directories with ***mkdir*** and add the parameters with ***write*** shell commands.

<div align="center">
<table>
<caption><p>TSN Endpoint settings</p></caption>
<colgroup>
<col style="width: 13%" />
<col style="width: 18%" />
<col style="width: 24%" />
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
<td rowspan="12">/lfs/m7/tsn_app</td>
<td>mode</td>
<td></td>
<td>Type of application demonstrating capabilities</td>
<td><a href="#application-mode">Application Mode</a></td>
</tr>
<tr>
<td>role</td>
<td></td>
<td>Role of the endpoint across the network</td>
<td><a href="#board-role">Board Role</a></td>
</tr>
<tr>
<td>period_ns</td>
<td></td>
<td>Application period in nanoseconds</td>
<td><a href="#application-period">Application Period</a></td>
</tr>
<tr>
<td>priority</td>
<td></td>
<td>Choose network traffic priority</td>
<td><a href="#priority">Priority</a></td>
</tr>
<tr>
<td>packets</td>
<td></td>
<td>Choose number of packets to handle in the network</td>
<td><a href="#packets">Packets</a></td>
</tr>
<tr>
<td>zero_copy</td>
<td></td>
<td>Use socket zero-copy mechanism</td>
<td><a href="#zero_copy">Zero Copy</a></td>
</tr>
<tr>
<td>rx_tc_mask</td>
<td></td>
<td>Use traffic class bit mask to skip processing in socket context</td>
<td><a href="#rx_tc_mask">Rx TC Mask</a></td>
</tr>
<tr>
<td>num_io_devices</td>
<td>role == 0</td>
<td>Number of IO devices controlled by CONTROLLER board</td>
<td><a href="#number-of-io-devices-controlled">Number of IO Devices Controlled</a></td>
</tr>
<tr>
<td>tx_time_offset_ns</td>
<td></td>
<td>Packet transmit offset in nanoseconds</td>
<td><a href="#tx_time_offset_ns">Tx Time Offset (ns)</a></td>
</tr>
<tr>
<td>tx_time_enable</td>
<td></td>
<td>Enable/disable packet transmit time</td>
<td><a href="#tx_time_enable">Tx Time Enable</a></td>
</tr>
<tr>
<td>port_id</td>
<td>More than one interface available</td>
<td>Application logical port id</td>
<td><a href="#port_id">Port ID</a></td>
</tr>
<tr>
<td>offset</td>
<td>0 or 1</td>
<td>Increases the deadline for packet transmission</td>
<td><a href="#offset">Offset</a></td>
</tr>
<tr>
<td rowspan="2">/lfs/m7/portN</td>
<td></td>
<td>0 &lt;= N &lt; 2</td>
<td>Logical port number</td>
<td>Check logical ports mapping for each application in Evaluation Applications Section</td>
</tr>
<tr>
<td>hw_addr</td>
<td></td>
<td>MAC address of the port</td>
<td><a href="./03_common_configuration.md#hardware-address">Hardware Address</a></td>
</tr>
</tbody>
</table>
</div>

### Application mode

By default, this setting is set to 2, which corresponds to **NETWORK_ONLY** mode.

When compiling in **release** mode, this is the only application mode available, thus it should not be changed. When compiling in release motor mode, three application modes are available:

MOTOR_NETWORK = 0, which enables the controller to control 1 or 2 motors remotely.\
MOTOR_LOCAL = 1, which enables the controller to control 1 motor locally.\
NETWORK_ONLY = 2, which enables to observe time-sensitive traffic between the controller and 1 or 2 IO devices.

### Board role

By default, the board role is set to 0. This setting will determine the role of the board across the network. In the simplest network topology composed of 2 boards, one board must have the role CONTROLLER and the other the role IO_DEVICE_0.

The following board roles are available:

CONTROLLER_0 = 0, corresponds to the controller of a given scenario.\
IO_DEVICE_0 = 1, corresponds to the first IO device of a given scenario.\
IO_DEVICE_1 = 2, corresponds to the second IO device of a given scenario.

### Application period

By default, the period cycle is set at 100000 ns (100 µs).

The minimum value is 100000 ns, and it needs to be an integer divider of 1 second.

If the application mode is MOTOR_NETWORK or MOTOR_LOCAL only 2 values are possible, 100000 ns (100 µs) and 250000 ns (250 µs). This is to ensure correct operation with motor control algorithms.

### Packets

This parameter allows the user to send and receive multiple packets within a cycle.

By default, the endpoint only sends and receives one packet per cycle in the application. If Scheduled Traffic configuration is used, adaptations might be needed depending on the number of packets.

### Zero_copy

This parameter allows the user to use zero-copy mechanism on both transmit and receive, to improve performance (e,g, be able to use shorter cycles).

By default, the parameter is activated on the device.

### Rx_tc_mask

This parameter allows the user to skip certain queues for processing. For a correct use, it requires a previous knowledge about which queues the network traffic goes.

### Priority

This setting enables users to choose the network traffic priority. It is an important feature when using scheduled traffic together with frame preemption.

By default, this setting is set to 5 (QOS_VOICE_PRIORITY).

The following priorities are available:

----------------------------------------------

QOS_BEST_EFFORT_PRIORITY = 0\
QOS_BACKGROUND_PRIORITY = 1\
QOS_EXCELLENT_EFFORT_PRIORITY = 2\
QOS_CRITICAL_APPLICATIONS_PRIORITY = 3\
QOS_VIDEO_PRIORITY = 4

QOS_VOICE_PRIORITY = 5\
QOS_INTERNETWORK_CONTROL_PRIORITY = 6\
QOS_NETWORK_CONTROL_PRIORITY = 7

-----------------------------------------------

### Number of IO devices controlled

This setting is configurable only if the board role is set to **CONTROLLER_0**. This will tell the controller how many IO devices should be available on the network.

Currently 1 or 2 controlled IO devices are supported. By default, this setting is set to 2.

### Tx_time_offset_ns

When Time Specific Departure (TSD) feature is enabled, this parameter enables the user to specify the time when a frame is to be transmitted, in nanoseconds. Then, the port will delay the transmission of the frame so it can be transmitted at the precise time.

### Tx_time_enable

This flag parameter enables or disables Time Specific Departure (TSD) feature on the endpoint.

### Port_id

This parameter chooses the Endpoint network interface the application will use, provided that the device has more than one interface enabled for use.

### Offset

When set, this parameter increases the deadline for packet transmission.

Example of an application period of 250 µs and network budget of 15 µs:

- offset = 0: transmission deadline = 250/2 – 15 = 110 µs

- offset = 1: transmission deadline = 250 – 15 = 235 µs
