# KBUS Register Access

This HowTo describes the usage of the `libkbus-register-settings` library,
which allows you to configure (serial) terminals via KBUS.

## About

`libkbus_register_settings` provides an API to configure supported terminals over KBUS.
To do that, it provides a struct `KbusRegConfigurationSet`, which groups all possible settings.
By setting individual settings to the preferred option, those settings can be applied to a specific terminal 
with the `kbus_reg_apply_configuration()` function. 
To go in the opposite direction, the function `kbus_reg_extract_configuration()` allows to read
all supported/available options from a terminal.

### Representation of Options

All options are represented as enums to improve code readability. 
These enums are listed in the library header `kbus_register_settings_common.h`.
The available parity settings, for example, are defined in the following way:
```c
typedef enum {
    KBUS_REG_PARITY_NONE = 1,
    KBUS_REG_PARITY_EVEN = 2,
    KBUS_REG_PARITY_ODD = 3,
} KbusRegParity;
```

The `KbusRegConfigurationSet` struct allows to account for settings whose current value is not
supposed to get overwritten, by specifying the value 0, instead of a related enum variant.
Since usually only certain options are supposed to be changed, the `KbusRegConfigurationSet_Default` constant
provides a default `KbusRegConfigurationSet` instance with all values initialized to 0 (this is used in the example below).

Additionally, the value 0 in a `KbusRegConfigurationSet` that has been read signals that the respective option
is not available for the terminal that was inspected.

### Example: Reading the Baud Rate

The following excerpt reads the baud rate of the first terminal connected to the pfc and checks if it has a baud rate of 1200 set:

```c
    KbusRegErr kbus_error;

    KbusRegConfigurationSet configuration;
    if ((kbus_error = kbus_reg_extract_configuration(adi, 1, &configuration)).status != KBUS_REG_COM_OK) {
        // reading the settings of the terminal failed:
        handle_error(kbus_error, adi, kbusDeviceId); // see sample code for a possible implementation of this method
    }
    if (configuration.baud_rate == KBUS_REG_BAUD_1200) {
        printf("Baud rate is 1200\n");
    } else {
        printf("Baud rate is not 1200\n");
    }
```

### Example: Configuring the Baud Rate

The following excerpt sets a baud rate of 2400 for the second terminal connected to the pfc:

```c
    KbusRegErr kbus_error;

    KbusRegConfigurationSet configuration = KbusRegConfigurationSet_Default; // initialization with defaults
    configuration.baud_rate = KBUS_REG_BAUD_2400;

    if ((kbus_error = kbus_reg_apply_configuration(adi, 2, configuration)).status != KBUS_REG_COM_OK) {
        // writing the settings of the terminal failed:
        handle_error(kbus_error, adi, kbusDeviceId); // see sample code for a possible implementation of this method
    }
```

### Documentation

All data structures and functions are documented in the header files of the library.

## Prerequisites

This HowTo uses the [WAGO firmware SDK for PFC200-G1 and PFC100 family](https://github.com/WAGO/pfc-firmware-sdk).
Paths regarding SDK tools and files always refer to the standard locations, as detailed in the SDK's setup instructions.
The SDK is expected to work correctly, i. e. the standard image `sd.hdimg` has been successfully built.

Shell prompts either refer to the local computer, where you have the SDK installed or a remote shell on the PFC:
- `local$ <command>` refers to a local computer
- `pfc <command>` refers to a remote shell on the pfc

A dollar sign (`$`) at the beginning of a prompt shows a shell with a default/unprivileged user, while a hash sign (`#`) denotes a root user shell.

### ADI/DAL - CoDeSys-Runtime

The library uses ADI/DAL internally, which can only be used by one application, 
either CoDeSys-Runtime or your own application.

***Take care to stop the runtime before you run this demo*** (or you application): 

- temporarily: `root@PFC# /etc/init.d/runtime stop`
- permanent: `root@PFC# rm /etc/rc.d/S98_runtime` (this can be undone with: `root@PFC# ln -s /etc/init.d/runtime /etc/rc.d/S98_runtime`)

### Libary usage

The library does not provide setup of the ADI/DAL and KBUS interface,
which needs to be done before the library can be used.

Additionally, the kbus operating mode must not be `running`/`manual` during configuration operations.

More information about the underlying ADI/DAL / KBUS API can be found in the [My first KBUS application using ADI/DAL HowTo](https://github.com/WAGO/pfc-howtos/tree/master/HowTo_ADI-MyKBusApplikation#about-kbus).

## Building

1. Clone this repository and change your working directory to the HowTo folder:

```console
local$ git clone https://github.com/WAGO/pfc-howtos.git && cd HowTo_kbus-settings-65x
```

2. Copy the rule files, source files and the files of the `kbus-register-settings` library to the correct SDK folders:

```console
local$ cp ./rules/* ~/wago/ptxproj/rules/
local$ cp -r ./src/* ~/wago/ptxproj/src/
# for the source version (named `kbus-register-settings`):
local$ cp libkbus-register-settings/rules/* ~/wago/ptxproj/rules/
local$ cp -r libkbus-register-settings/src/* ~/wago/ptxproj/src/
# for the binary version (named `kbus-register-settings-bin`):
local$ cp libkbus-register-settings-bin/rules/* ~/wago/ptxproj/rules/
local$ cp -r libkbus-register-settings-bin/src/* ~/wago/ptxproj/src/
```

3. Select the `KBUS register access demo` package for building:

```console
local$ cd ~/wago/ptxproj/ # SDK location
local$ ptxdist menuconfig
        Wago Specific                   ---> [Enter]
        Demo                            ---> [Enter]
        KBUS register access demo            [Enter]

        [Exit], [Exit], [Exit], [Yes]
```

4. Clean the packages:

```console
local$ ptxdist clean libkbus-register-settings register-access-65x-demo
```

5. Build the packages:

```console
local$ ptxdist targetinstall libkbus-register-settings register-access-65x-demo
```

6. (Optional) Build a complete firmware image (`sd.hdimg`):

```console
local$ ptxdist images
```

## Installation (/Usage)

### Complete Firmware Image

Write the firmware image to an SD-Card and boot from it.
[Take a look at the SDK setup for instructions](https://github.com/WAGO/tp-firmware-sdk/#6-write-the-binary-image-file-sdhdimg-to-sd-card).

### Package Installation

- copy the built packages `platform-wago-pfcXXX/packages/register-access-65x-demo_0.1_armhf.ipk` and `platform-wago-pfcXXX/packages/libkbus-register-settings_0.1.0_armhf.ipk`
        onto the TP and manually install them
- install the packages via WBM's package upload

### Execution

Make sure the runtime is not running (see [Prerequisites](#Prerequisites)), then execute `pfc$ /usr/bin/register-access-65x-demo`.

## Dynamic/Static Linking

`libkbus-register-settings` can be build both as a static or shared library.
Therefore, the demo application can link statically or dynamically with it.
This can be controlled in the ptxdist menu where the package has been selected for building (Step 3 [Building](#Building)).
