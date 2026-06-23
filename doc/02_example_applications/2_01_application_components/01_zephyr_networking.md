# Networking Application

This application allows evaluation of the networking capabilities of the i.MX RT118x platform. It relies on Zephyr's Net Shell and Zperf utilities, as well as native switch and endpoint drivers, to perform network throughput and performance measurements.

The application supports the following features:

- Static and DHCP-based IPv4 address configuration via the Zephyr Net Shell
- TCP and UDP throughput evaluation using Zperf, compatible with iPerf 2.x on the host side
- Network interface status inspection and statistics

The Networking Evaluation application targets any of the external interfaces of the i.MX RT1180 EVK and is intended to be used in conjunction with a host PC running iPerf as a client or server counterpart.
