# GenAVB/TSN stack logs

Logs are output to the serial terminal.

The format is as follow:

- Log level: INIT, INFO, ERR, DEBUG

- Time

- Stack component prefix: avdecc, avtp, fgptp, srp, freertos (os specific component)

- Function name

- Log

## gPTP logs

- Stack component log prefix: fgptp

- If the stack is configured in automotive mode, then the log contains:

Running gPTP stack in automotive profile on interface eth0

- *Port Role*, *Port AS-capability* and *link Status* are reported each time there is a change in the link state (link is 802.1AS capable or not) or upon Grand Master (GM) change. This information is also displayed regularly along with current synchronization and pdelay statistics

Port 0: role changed from DISABLED to SLAVE

…

> Port 0 domain(0,0): Role: Slave Link: Up AS_Capable: Yes neighborGptpCapable: Yes

- Selected Grand Master (GM) capabilities are reported upon new GM selection. *Root Identity* represents the clock ID of the currently selected GM. *Priority1, Priority2, Class* and *Accuracy* describe the clock quality of the selected GM. Finally, the *Peer Master Identity* of the nearer peer master endpoint the current device is connected to (e.g. a bridge placed between the slave device and the GM):

Grand master: root identity 00049ffffe039e35

Grand master: priority1 245 priority2

Grand master: class 248 accuracy 248

Grand master: variance 17258

Grand master: source port identity 0001f2fffe0025fe, port number 2

- *Synchronization State* is reported upon GM selection (SYNCHRONIZED) or when no GM are detected (NOT SYNCHRONIZED). Synchronization *Time* expressed in ms represents the time it took to the local clock to reach synchronization threshold starting from the first SYNC message received.

Port(0) domain(0,0): SYNCHRONIZED – synchronization time (ms): 250

- *Pdelay* (propagation delay) and local clock adjustments are printed out every 5 seconds. *PDelay* is expressed in ns units and represents the one-way delay from the endpoint and its peer master. *Correction* is expressed in parts per billion and represents the frequency adjustment performed to the local clock. *Offset* is expressed in ns represents the resulting difference between the locally adjusted clock and the reference gPTP Master’s clock. (Min/Max/Avg and Variance are computed for both Correction and Offset statistics)

> Port 0 domain(0,0): Propagation delay (ns): 37.60 min 34 avg 36 max 45 variance 17
>
> Port 0 domain(0,0): Correction applied to local clock (ppb): min -5603 avg 5572 max 5538 variance 148
>
> Port 0 domain(0,0): Offset between GM and local clock (ns) min -12 avg 4 max 22 variance 111

- The following port statistics (32 bits counters) are printed out every 15 seconds on slave and master entities:

<div align="center">
<table>
<caption><p>gPTP counters</p></caption>
<colgroup>
<col style="width: 44%" />
<col style="width: 55%" />
</colgroup>
<thead>
<tr>
<th colspan="2">Receive counters</th>
</tr>
</thead>
<tbody>
<tr>
<td>PortStatRxPkts</td>
<td>Number of gPTP packets received (ether type 0x88F7)</td>
</tr>
<tr>
<td>PortStatRxSyncCount</td>
<td>Number of SYNC packets received</td>
</tr>
<tr>
<td>PortStatRxSyncReceiptTimeouts</td>
<td>Number of FOLLOW-UP packets timeout</td>
</tr>
<tr>
<td>PortStatRxFollowUpCount</td>
<td>Number of FOLLOW-UP packets received</td>
</tr>
<tr>
<td>PortStatRxAnnounce</td>
<td>Number of ANNOUNCE packets received</td>
</tr>
<tr>
<td>PortStatAnnounceReceiptTimeouts</td>
<td>Number of ANNOUNCE packets timeout</td>
</tr>
<tr>
<td>PortStatAnnounceReceiptDropped</td>
<td>Number of ANNOUNCE packets dropped by the entity</td>
</tr>
<tr>
<td>PortStatRxSignaling</td>
<td>Number of SIGNALING packets received</td>
</tr>
<tr>
<td>PortStatRxPdelayRequest</td>
<td>Number of PDELAY REQUEST packets received</td>
</tr>
<tr>
<td>PortStatRxPdelayResponse</td>
<td>Number of PDELAY RESPONSE packets received</td>
</tr>
<tr>
<td>PortStatPdelayAllowedLostResponsesExceeded</td>
<td>Number of excess of allowed lost responses to PDELAY requests</td>
</tr>
<tr>
<td>PortStatRxPdelayResponseFollowUp</td>
<td>Number of PDELAY FOLLOW-UP packets received</td>
</tr>
<tr>
<td>PortStatRxErrEtype</td>
<td>Number of ether type errors (not 0x88F7)</td>
</tr>
<tr>
<td>PortStatRxErrPortId</td>
<td>Number or port ID errors</td>
</tr>
<tr>
<td colspan="2">Transmit counters</td>
</tr>
<tr>
<td>PortStatTxPkts</td>
<td>Number of gPTP packets transmitted</td>
</tr>
<tr>
<td>PortStatTxSyncCount</td>
<td>Number of SYNC packets transmitted</td>
</tr>
<tr>
<td>PortStatTxFollowUpCount</td>
<td>Number of FOLLOW-UP packets transmitted</td>
</tr>
<tr>
<td>PortStatTxAnnounce</td>
<td>Number of ANNOUNCE packets transmitted</td>
</tr>
<tr>
<td>PortStatTxSignaling</td>
<td>Number of SIGNALING packets transmitted</td>
</tr>
<tr>
<td>PortStatTxPdelayReques</td>
<td>Number of PDELAY REQUEST packets transmitted</td>
</tr>
<tr>
<td>PortStatTxPdelayResponse</td>
<td>Number of PDELAY RESPONSE packets transmitted</td>
</tr>
<tr>
<td>PortStatTxPdelayResponseFollowUp</td>
<td>Number of PDELAY FOLLOW-UP packets transmitted</td>
</tr>
<tr>
<td>PortStatTxErr</td>
<td>Number of transmit errors</td>
</tr>
<tr>
<td>PortStatTxErrAlloc</td>
<td>Number of transmit packets allocation errors</td>
</tr>
<tr>
<td colspan="2">Miscellaneous counters</td>
</tr>
<tr>
<td>PortStatAdjustOnSync</td>
<td>Number of adjustments performed upon SYNC received</td>
</tr>
<tr>
<td>PortStatMdPdelayReqSmReset</td>
<td>Number of reset of the PDELAY REQUEST state machine</td>
</tr>
<tr>
<td>PortStatMdSyncRcvSmReset</td>
<td>Number of reset of the SYNC RECEIVE state machine</td>
</tr>
<tr>
<td>PortStatHwTsRequest</td>
<td>Number of egress timestamp requests</td>
</tr>
<tr>
<td>PortStatHwTsHandler</td>
<td>Number of egress timestamp notification</td>
</tr>
<tr>
<td>PortStatNumSynchronizationLoss</td>
<td>Number or synchronization loss on the slave endpoint (e.g. GM change, GM reference clock discontinuity...)</td>
</tr>
<tr>
<td>PortStatNumNotAsCapable</td>
<td>Number of transition from AS_Capable=TRUE to AS_Capable=FALSE</td>
</tr>
</tbody>
</table>
</div>

