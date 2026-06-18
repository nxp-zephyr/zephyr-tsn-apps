# Zephyr TSN Apps
TSN reference applications for Zephyr.

## Supported Hardware

This release supports the following i.MX RTxxx EVK boards:

| SoC            | Board Name        | Description                    |
|----------------|-------------------|--------------------------------|
| i.MX RT1180    | mimxrt1180_evk    | i.MX RT1180 Evaluation Kit     |


## Getting Started

### Prerequisites

Host tools (and optionally: GNU ARM Toolchain) can be installed using the [MCUXpresso Installer from nxp.com](https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-installer:MCUXPRESSO-INSTALLER) utility for both Linux and Windows Hosts.

From the NXP MCUXpresso Installer main menu select and install the following components:
* Debug Probes Software: LinkServer and/or SEGGER JLink
* (Optional) Arm GNU Toolchain v14.2.1

For i.MX RT1180 EVK, refer to: [i.MX RT1180 EVK - Programming and Debugging](https://docs.zephyrproject.org/4.4.0/boards/nxp/mimxrt1180_evk/doc/index.html#programming-and-debugging)

Follow the Zephyr documentation, [Getting Started](https://docs.zephyrproject.org/4.4.0/develop/getting_started/index.html) to install the required host tools:
- Select and Update OS
- Install dependencies
- Get Zephyr and install Python dependencies
  - For Step 4. fetch this repository instead of cloning the official Zephyr repository
```bash
  export revision=main
  west init -m https://github.com/nxp-zephyr/zephyr-tsn-apps --mr ${revision} <workspace>
  west update
  cd <workspace>
```
Replace ```${revision}``` with any zephyr-tsn-apps release you wish to use. This can also be ```main``` if you want the latest state, or any commit SHA.
- Install the Zephyr SDK

## Repository Structure

```bash
.
├── apps
│   ├── networking      # Basic networking application
│   ├── tsn_app         # TSN application
│   └── tsn_init        # TSN initialization application
├── common              # Common libraries, sources and build dependencies
└── west.yml
```

## Building Applications

Make sure to enable the virtual environment setup in [Host Tools](#Host-Tools). 

Run the `west build` command from the repository root. The generic command format is:

```bash
west build -p always -b <board> apps/<app> -d <build_dir> [--sysbuild]
```

**Build Command Parameters:**
- `-p always` - Pristine build (clean before building)
- `-b <board>` - Target board name (e.g., 'mimxrt1180_evk/mimxrt1189/cm33')
- `--sysbuild` - Build the multi-core configuration
- `-d <build_dir>` - Build directory (optional, defaults to `build/`)
- positional argument - Application path (e.g., `apps/networking`)


> **_NOTE:_** By default, the used toolchain is Zephyr SDK. Using other toolchains (ARM GNU toolchain) is also supported as described in the Zephyr documentation on [Using Other Cross Compilers](https://docs.zephyrproject.org/latest/develop/toolchains/other_x_compilers.html#other-cross-compilers) with the following limitations:
> - tsn_app with multicore build uses CONFIG_BUILD_OUTPUT_ADJUST_LMA and requires a custom objcopy binary from Zephyr SDK. Refer to [Zephyr issue #58080](https://github.com/zephyrproject-rtos/zephyr/issues/58080) and [Zephyr SDK-ng PR #729](https://github.com/zephyrproject-rtos/sdk-ng/pull/729)

### Supported Builds

| **app**    | **board**                      | **multicore**         |
|------------|--------------------------------|-----------------------|
| networking | mimxrt1180_evk/mimxrt1189/cm33 | no                    |
| tsn_init   | mimxrt1180_evk/mimxrt1189/cm33 | yes (with --sysbuild) |
| tsn_app    | mimxrt1180_evk/mimxrt1189/cm33 | yes (with --sysbuild) |

## Flashing Applications

Use the 'west flash' command to flash the built application to the target board:
```bash
west flash --runner <linkserver | jlink>  -d <build_dir>
```
