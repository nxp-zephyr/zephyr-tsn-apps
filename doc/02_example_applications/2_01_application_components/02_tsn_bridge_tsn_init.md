# TSN Bridge Application

The TSN Bridge application runs on the Cortex-M33 core and configures the GenAVB/TSN stack in bridge mode, providing User Interface (UI) to configure TSN Bridge functionalities of the i.MX RT1180 bridge ports. It also exposes two Zephyr network interfaces allowing TCP/IP communication over both the standalone ENETC0 port and the integrated endpoint connected to the internal bridge port.

In the multicore configuration, the Cortex-M7 core runs a companion application that launches the GenAVB/TSN stack as a TSN-capable Endpoint, sharing the internal bridge port (ENETC1) with the Cortex-M33 Endpoint via a virtual interface. Both cores operate independently and communicate with the network through their respective Endpoint interfaces.
