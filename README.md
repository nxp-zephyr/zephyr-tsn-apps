# Zephyr TSN Apps
TSN reference applications for Zephyr, running on i.MX RT1180.

## Supported Hardware

This release supports the following i.MX RTxxx EVK boards:

| SoC            | Board Name        | Description                    |
|----------------|-------------------|--------------------------------|
| i.MX RT1180    | evkmimxrt1180     | i.MX RT1180 Evaluation Kit     |


## Getting Started

### Prerequisites

#### ARM GNU Toolchain

Download and install ARM GCC toolchain version 14.2.Rel1:

**Linux:**
```bash
wget https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz
sudo tar -C /opt/ -xf arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz
export ARMGCC_DIR=/opt/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi
```

**Windows:**

Download the installer from [ARM Developer](https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-mingw-w64-i686-arm-none-eabi.exe) and install it, then set the environment variable:
```cmd
set ARMGCC_DIR=C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\14.2 Rel1
```

#### Host Tools

**Linux (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install git python3 python3-pip cmake ninja-build
pip3 install west # you might need a virtual environment here
```

**Windows:**
- Install [Git for Windows](https://git-scm.com/download/win)
- Install [Python 3.10+](https://www.python.org/downloads/windows/)
- Install [CMake 3.30.0+](https://cmake.org/download/) - use the .msi installer
- Install west:
```cmd
pip install west
```

## Cloning the Repository

Initialize a west workspace to start the development environment:

```bash
west init -m https://github.com/nxp-zephyr/zephyr-tsn-apps <workspace>
cd <workspace>/zephyr-tsn-apps
west update
```

## Repository Structure

```bash
.
├── apps
│   └── networking                                      # Basic networking application
│       ├── boards                                      # Boards overlay and configurations
│       ├── build_all.sh
│       ├── CMakeLists.txt                              # Application's CMakeLists.txt
│       ├── prj.conf                                    # Application-specific configuration
│       └── src                                         # Source files
└── west.yml
```

### Building Applications

Install dependencies:
**Linux:**
```bash
pip3 install -r <workspace>/zephyr/scripts/requirements.txt
```

**Windows:**
```cmd
pip3 install -r <workspace>\zephyr\scripts\requirements.txt
```

Navigate to the Application root directory and use the `west build` command:

**Linux:**
```bash
cd <workspace>/zephyr-tsn-apps

# Use ARMGCC Toolchain
export ZEPHYR_TOOLCHAIN_VARIANT=cross-compile
export CROSS_COMPILE=$ARMGCC_DIR/bin/arm-none-eabi-

west build -p always -b mimxrt1180_evk/mimxrt1189/cm33 apps/networking -d build_rt1180_cm33
```

**Windows:**
```cmd
cd <workspace>\zephyr-tsn-apps

# Use ARMGCC Toolchain
set ZEPHYR_TOOLCHAIN_VARIANT=cross-compile
set CROSS_COMPILE=%ARMGCC_DIR%\bin\arm-none-eabi-

west build -p always -b mimxrt1180_evk/mimxrt1189/cm33 apps/networking -d build_rt1180_cm33
```

Build artifacts will be in the `build/` directory.

**Build Command Parameters:**
- `-p always` - Pristine build (clean before building)
- `-b <board>` - Target board name (e.g., 'mimxrt1180_evk/mimxrt1189/cm33')
- `-d <dir>` - Build directory (optional, defaults to `build/`)
- positional argument - Application path (e.g., `apps/networking`)

Or run the build script:
```bash
cd <workspace>/zephyr-tsn-apps/apps/networking
./build.sh
```
