# HowTo generate WUP-File

This HowTo describes how to generate a WUP file, which can be used to update
the firmware of a device remotely.

- [PREREQUISITES](#prerequisites)
- [Generate WUP File](#generate-wup-file)
	- [1) Build complete firmware image](#1-build-complete-firmware-image)
	- [2) Generate WUP File](#2-generate-wup-file)
- [About WUP Files:](#about-wup-files)
- [Use custom certificates:](#use-custom-certificates)
	- [1) Create certificate and key files](#1-create-certificate-and-key-files)
	- [2) Enter docker image pfc-builder provided by SDK](#2-enter-docker-image-pfc-builder-provided-by-sdk)
	- [3) Configure certificate files](#3-configure-certificate-files)
	- [4) Enable release build](#4-enable-release-build)
	- [5) Rebuild firmware image](#5-rebuild-firmware-image)
	- [6) Generate WUP file](#6-generate-wup-file)
	- [7) Exit docker image pfc-builder provided by SDK](#7-exit-docker-image-pfc-builder-provided-by-sdk)
- [Create a custom RAUC slot](#create-a-custom-rauc-slot)
- [Compatibility list](#compatibility-list)


## PREREQUISITES
This HowTo is based on a clean installation of Ubuntu LTS, with an installed 
and working WAGO [pfc-firmware-sdk-G2](https://github.com/WAGO/pfc-firmware-sdk-G2).
Working means that you successfully built the standard image “sd.hdimg”.


## Generate WUP File

### 1) Build complete firmware image
```bash
cd pfc-firmware-sdk-G2
make init
make images
```

### 2) Generate WUP File
```bash
cd pfc-firmware-sdk-G2
make wup
```

Afterwards you should find the WUP file:
```
pfc-firmware-sdk-G2/ptxproj/platform-wago-pfcXXX/images/PFC-Linux_update_VXXXXXX_XX_rXXXXXXXXXX.wup
```


## About WUP Files:

WUP (WAGO Update Package) files are used by WAGO tools such as WAGOupload to update the firmware of PFC devices remotely.

> [!NOTE]
> The format of WUP file may be extended or changed in future. Therefore, this documentation is only valid for a specific firmware version. 

Technically, WUP files are ZIP archives, consisting of at least two files:
- a control file (package-info.xml)
- a RAUC bundle

Briefly spoken,
WAGO tools such as WAGOupload extract the contents of a WUP file,
perform some checks and use the fwupdate config tool to perform the update.
In doing so, only the RAUC bundle is transmitted to PFC,
the WUP is only used by the tool and is never transferred to PFC.

Detailed information about RAUC can be obtained from the projects home page: https://www.rauc.io/

## Use custom certificates:

RAUC uses a certificate based approach to authenticate the origin of a bundle.
To ease creating WUP files during development, the SDK contains test certificates.

> [!NOTE]
> In order to use RAUC's authentication mechanism to ensure the origin of a bundle, it is strictly recommended to to use custom certificates.

### 1) Create certificate and key files
- follow the instruction at https://rauc.readthedocs.io/en/latest/advanced.html#security to create custom certificate and key files

### 2) Enter docker image pfc-builder provided by SDK

If the sdk was not initialized yet, initialize PTXdist project in ptxproj directory

```bash
cd pfc-firmware-sdk-G2
make init
```

Enter a bash in the docker image pfc-builder

```bash
cd pfc-firmware-sdk-G2
make bash
```

### 3) Configure certificate files

Inside the bash of the pfc-builder, open `ptxdist menuconfig`

```bash
ptxdist menuconfig
```

Navigate to `RAUC Key Selection`

```
Applications --->				[Select]
	RAUC Key Selection  --->	[Select]
```

Configure the paths of your custom certificate and key file.
- to configure key, use "RAUC key"
- to configure certificate, use "RAUC certificate"

Exit `ptxdist menuconfig` and save made changes
```
[Exit], [Exit], [Exit], [YES]
```

### 4) Enable release build

Open PTXDist Setup Menu

```bash
ptxdist setup
```

Navigate to the environment whitelist

```
Development Options 	---> [Select]
	()  environment variable whitelist (space separated) [Select]
```

Add `BUILDTYPE` to environment variable whitelist and confirm the updated whitelist with [Enter]

```
[Exit], [Exit], [YES]
```

### 5) Rebuild firmware image

Rebuild the sd.hdimage with `BUILDTYPE` set to `release`

```bash
export BUILDTYPE=release
ptxdist clean
ptxdist go -q
ptxdist images
```

> [!NOTE]
> Rebuild is necessary to apply changes. Note also, that BUILDTYPE environment variable is set.

### 6) Generate WUP file
```
make wup
```

Afterwards you should find the WUP file:

```
~/ptxproj/platform-wago-pfcXXX/images/PFC-Linux_update_VXXXXXX_XX_rXXXXXXXXXX.wup
```

If you didn't set `BUILDTYPE` set to `release`, the WUP file and the sd.hdimg file both have the suffix `_development` in their names.

### 7) Exit docker image pfc-builder provided by SDK

Changes are made in the locally stored files.
You can safely leave the pfc-builder.

```bash
exit
```

## Create a custom RAUC slot

RAUC bundles consists of one or more slots. For example, PFC uses separate slots to install root file system and boot loaders separately. To customize you installation, e.g. install custom applications or use custom partitions, you can specify own slots.

Detailed information about slots and how to use them, can be found at
- https://rauc.readthedocs.io/en/latest/scenarios.html
- https://rauc.readthedocs.io/en/latest/integration.html#ptxdist.

The RAUC configuration files of the PFC is located at
- Slot configuration files: `ptxproj/configs/wago-pfcXXX/projectroot/etc/rauc`
- Image configuration file: `ptxproj/configs/wago-pfcXXX/config/images/rauc.config`
- Location of slot hook files: `ptxproj/shared_public/fwupdate/rauc_bundle_conf/update`

## Compatibility list
| PFC | Compatible |
|:-|:-:|
| PFC 100 G2 | Y |
| PFC 200 G2 | Y |
| PFC 300 | Y |
| CC 100 | Y |
| TP 600 | Y |
| WP 400 | Y |

