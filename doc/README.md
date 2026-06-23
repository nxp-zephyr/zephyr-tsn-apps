# GenAVB/TSN Stack Evaluation Documentation

The GenAVB/TSN Stack is a set of software components which provides Time Sensitive Network (TSN) functionality on NXP SoC and hardware platforms.

This document provides information on how to set up evaluation use cases of the GenAVB/TSN Stack. In that context, it provides information on the SoC and boards that can be used, how to set up the hardware platforms and configuration information of the evaluation software.

---

## Table of Contents

### 1. Initial Preparation

This chapter covers the prerequisites and one-time setup steps required before running any evaluation use case, including board configuration, flashing, serial terminal setup, and mandatory TSN network settings.

- [1.1 Evaluation Boards Description and Supported Roles](01_initial_preparation/01_evaluation_boards_description_and_supported_roles.md)
- [1.2 Flashing Application Images](01_initial_preparation/02_flashing_application_images.md)
- [1.3 Serial Terminal](01_initial_preparation/03_serial_terminal.md)
- [1.4 Preparing Boards for TSN Operations](01_initial_preparation/04_preparing_boards_for_tsn_operations.md)
- [1.5 TSN Network Configuration Settings](01_initial_preparation/05_tsn_network_configuration_settings.md)

### 2. Example Applications Description

This chapter describes the software applications available in this repository. It is organized in two parts: the application components, which describe the individual software building blocks, and the evaluation applications, which describe the full system images per board and configuration (singlecore/multicore).

#### 2.1 Application Components

- [2.1.1 Networking Application](02_example_applications/2_01_application_components/01_zephyr_networking.md)
- [2.1.2 TSN Bridge Application](02_example_applications/2_01_application_components/02_tsn_bridge_tsn_init.md)
- [2.1.3 TSN Isochronous Application](02_example_applications/2_01_application_components/03_tsn_app.md)

#### 2.2 Evaluation Applications

- [2.2.1 Networking Application (i.MX RT118x)](02_example_applications/2_02_evaluation_applications/01_zephyr_networking_i_mx_rt118x.md)
- [2.2.2 TSN Bridge Application (i.MX RT118x)](02_example_applications/2_02_evaluation_applications/02_tsn_bridge_tsn_init_i_mx_rt118x.md)
- [2.2.3 TSN Isochronous Application (i.MX RT118x)](02_example_applications/2_02_evaluation_applications/03_tsn_app_i_mx_rt118x.md)

### 3. Evaluation Use Cases

This chapter provides step-by-step instructions to run and evaluate the supported use cases on the i.MX RT118x hardware. Each use case describes the required hardware setup, configuration commands, and expected results.

- [3.1 Networking Application (i.MX RT118x)](03_evaluation_use_cases/01_zephyr_networking_i_mx_rt118x.md)
- [3.2 TSN Bridge Application (i.MX RT118x)](03_evaluation_use_cases/02_tsn_bridge_tsn_init_i_mx_rt118x.md)
- [3.3 TSN Network Only with Two TSN Bridges](03_evaluation_use_cases/03_tsn_app_i_mx_rt118x.md)

### 4. More on Evaluation Usage

This chapter provides detailed reference documentation for the shell commands, configuration settings, and advanced features available in the GenAVB/TSN evaluation applications.

- [4.1 Using the Built-in Shell](04_more_on_evaluation_usage/01_using_the_built_in_shell.md)
- [4.2 Using Configuration Settings](04_more_on_evaluation_usage/02_using_configuration_settings.md)
- [4.3 Common Configuration](04_more_on_evaluation_usage/03_common_configuration.md)
- [4.4 Endpoint Configuration](04_more_on_evaluation_usage/04_endpoint_configuration.md)
- [4.5 Bridge Configuration (i.MX RT118x)](04_more_on_evaluation_usage/05_bridge_configuration_i_mx_rt118x.md)
- [4.6 TSN Network Configuration](04_more_on_evaluation_usage/06_tsn_network_configuration.md)
- [4.7 Pulse Per Second (PPS) Signal](04_more_on_evaluation_usage/07_pulse_per_second_pps_signal.md)
- [4.8 GenAVB/TSN Stack Logs](04_more_on_evaluation_usage/08_genavb_tsn_stack_logs.md)
- [4.9 Example Applications Logs](04_more_on_evaluation_usage/09_example_applications_logs.md)
