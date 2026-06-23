# TSN network configuration

For network only and motor control TSN examples applications it’s important to have a coherent network configuration across all Endpoints and Bridges. In particular, the TSN Bridge(s) Scheduled Traffic or Frame Preemption configuration must be aligned with that of the Endpoints.

## Streams

<div align="center">
<table>
<caption><p>TSN streams definition</p></caption>
<thead>
<tr>
<th></th>
<th>Source</th>
<th>Destination</th>
<th>Unicast/Multicast</th>
<th>Destination MAC Address</th>
<th>Vlan ID</th>
<th>Vlan PCP<sup>2</a></sup></th>
<th>Frame Length (bytes)</th>
</tr>
</thead>
<tbody>
<tr>
<td><strong>Stream1</strong></td>
<td>Controller</td>
<td>IO device(s)</td>
<td>Multicast</td>
<td>91:e0:f0:00:fe:70</td>
<td>2</td>
<td>5</td>
<td>76</td>
</tr>
<tr>
<td><strong>Stream2</strong></td>
<td>IO device 1</td>
<td>Controller</td>
<td>Multicast</td>
<td>91:e0:f0:00:fe:71</td>
<td>2</td>
<td>5</td>
<td>136</td>
</tr>
<tr>
<td><strong>Stream3</strong></td>
<td>IO device 2</td>
<td>Controller</td>
<td>Multicast</td>
<td>91:e0:f0:00:fe:80</td>
<td>2</td>
<td>5</td>
<td>136</td>
</tr>
</tbody>
</table>
</div>

## Priority to Traffic Class mapping

The following table shows the mapping of Priority Code Point (PCP) to Traffic Class for different supported number of traffic classes and sr classes. The mapping follows 802.1Q-2018, section 34.5 Mapping priorities to traffic classes for time-sensitive streams. This information is required for proper configuration of Scheduled Traffic.

<div align="center">
<table style="width:98%;">
<caption><p>Priority Code Point (PCP) to Traffic Class mapping</p></caption>
<colgroup>
<col style="width: 13%" />
<col style="width: 7%" />
<col style="width: 5%" />
<col style="width: 5%" />
<col style="width: 5%" />
<col style="width: 5%" />
<col style="width: 5%" />
<col style="width: 5%" />
<col style="width: 5%" />
<col style="width: 5%" />
<col style="width: 29%" />
</colgroup>
<tbody>
<tr>
<td></td>
<td></td>
<td colspan="8" style="text-align: center;">Priority Code Point</td>
<td></td>
</tr>
<tr>
<td></td>
<td></td>
<td style="text-align: center;">0</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">2</td>
<td style="text-align: center;">3</td>
<td style="text-align: center;">4</td>
<td style="text-align: center;">5</td>
<td style="text-align: center;">6</td>
<td style="text-align: center;">7</td>
<td></td>
</tr>
<tr>
<td>Available Traffic Classes</td>
<td style="text-align: center;">4</td>
<td style="text-align: center;">0</td>
<td style="text-align: center;">0</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">2</td>
<td style="text-align: center;">2</td>
<td style="text-align: center;">3</td>
<td style="text-align: center;">3</td>
<td style="text-align: center;">i.MX RT1180 Endpoint ports</td>
</tr>
</tbody>
</table>
</div>

<div align="center">
<table style="width:98%;">
<caption><p>Priority to Traffic Class mapping (with no SR classes)</p></caption>
<colgroup>
<col style="width: 13%" />
<col style="width: 7%" />
<col style="width: 5%" />
<col style="width: 5%" />
<col style="width: 5%" />
<col style="width: 5%" />
<col style="width: 5%" />
<col style="width: 5%" />
<col style="width: 5%" />
<col style="width: 5%" />
<col style="width: 29%" />
</colgroup>
<tbody>
<tr>
<td></td>
<td></td>
<td colspan="8" style="text-align: center;">Priority Code Point</td>
<td></td>
</tr>
<tr>
<td></td>
<td></td>
<td style="text-align: center;">0</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">2</td>
<td style="text-align: center;">3</td>
<td style="text-align: center;">4</td>
<td style="text-align: center;">5</td>
<td style="text-align: center;">6</td>
<td style="text-align: center;">7</td>
<td></td>
</tr>
<tr>
<td>Available Traffic Classes</td>
<td style="text-align: center;">8</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">0</td>
<td style="text-align: center;">6</td>
<td style="text-align: center;">7</td>
<td style="text-align: center;">2</td>
<td style="text-align: center;">3</td>
<td style="text-align: center;">4</td>
<td style="text-align: center;">5</td>
<td style="text-align: center;">i.MX RT1180 Bridge ports</td>
</tr>
</tbody>
</table>
</div>

Priority to Traffic class mapping (with two SR classes)

## Bridge forwarding

If possible, configure FDB entries on the Bridge(s) so that stream data is only forwarded to ports that lead to the destination endpoint. One FDB entry is required per stream, using the above VLAN and destination mac address information.

## Scheduled Traffic configuration[^1]

## Endpoints

### 100μs period

Endpoints are running a schedule with a 100μs period. The base offset of the schedule is aligned to gPTP time modulo 1 second.

Controller transmit gate (for Stream1) opens at 35μs offset (relative to the period start).

IO device transmit gate (for Stream2/3) opens at 50μs + 35μs offset (relative to the period start).

The gate open interval is around 4μs (enough to accommodate the stream frame length plus some margin).

## Bridge

The schedule for all Bridges and all Bridge ports that transmit one of the streams above, must have a 100μs period and a base offset aligned to gPTP time modulo 1 second.

One possible schedule is to open transmit gate (for the ports and queues transmitting Stream 1) at offset 35μs and use a gate open interval that accommodates the worst propagation delay.

It’s also possible to use a fixed gate open interval but increase the transmit time offset at each hop along the stream path.

For ports and queues transmitting Stream 2 and 3, open the transmit gate at offset 50+40μs.

## Frame Preemption configuration[^2]

## Endpoints

Traffic class for frame priority equal 7 is configured as Express. All higher Traffic classes are also configured as Express. Other traffic classes are configured as Preemptable.

The priority value can be configurated by the user. The values described here are default values, but user can override them.

Verify function, as described in IEEE 802.3br-2016, section 99.4.3, is disabled, so preemption will always be enabled even if the peer port has preemption disabled.

## Bridge

All Bridge ports, connected to an Endpoint must have Frame preemption enabled.

Traffic class Express/Preemptible configuration should match the Endpoint’s one.

[^1]: If Scheduled Traffic is not disabled.

[^2]: If Frame Preemption is enabled.
