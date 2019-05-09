# HowTo generate WUP-File

## Every package is bundled to a specific firmware version: For Version 2019.3.1 - FW:03.00.35(99)

This HowTo describes how to generate a WUP file, which can be used to update
the firmware of a device remotely.

# PREREQUISITES:
This HowTo is / based on a clean installation of Ubuntu LTS, with an installed 
and working WAGO Board-Support Package for PFC200.
Working means that you successfully built the standard image “sd.hdimg”.


# Generate WUP File:

## 1) Build complete firmware image
```
        $cd <workspace-bsp>/ptxproj/
        $ptxdist clean
        $ptxdist go -q
        $ptxdist images
```

## 2) Generate WUP File
```
        $cd <workspace-bsp>/ptxproj/
        $make dist
```

Afterwards you should find the WUP file:
```
 <workspace-bsp>/ptxproj/platform-wago-pfcXXX/images/PFC-Linux_update_V0300XX_XX_rXXXXX.wup
```


# About WUP Files:

WUP (WAGO Update Package) files are used by WAGO tools such as WAGOupload and e!Cockpit to update the firmware of PFC
devices remotely.

NOTE:
>The format of WUP file may be extended or changed in future. Therefore, this documentation is only valid for a specific firmware version. 

Technically, WUP files are ZIP archives, consisting of at least two files:
- a control file (package-info.xml)
- a RAUC bundle

Briefly spoken, WAGO tools such as WAGOupload and e!Cockpit extract the contents of a WUP file, perform some checks and
use the fwupdate config tool to perform the update. In doing so, only the RAUC bundle is transmitted to PFC, the WUP is
only used by the tool and is never transferred to PFC.

Detailed information about RAUC can be obtained from the projects home page: https://www.rauc.io/

# Use custom certificates:

RAUC uses a certificate based approach to authenticate the origin of a bundle. To ease creating of WUP files during
development, the BSP contains test certificates.

NOTE:
>In order to use RAUC's authentication mechanism to ensure the origin of a bundle, it is stricly recommanded to to use custom cerstifiactes.

## 1) Create certificate and key files
- follow the instruction at https://rauc.readthedocs.io/en/latest/advanced.html#security to create custom certficate and key files

## 2) Configure certificate files
```
        $cd <workspace-bsp>/ptxproj/
        $ptxdist menuconfig

        Applications --->  			[Enter]
		-*- Rauc Update Tool --->	[Enter]

        configure the paths of your custom certificate and key ring files
        Certificates for development and release purposes can be configured seperately. 
        To configure release cerstifiacte, use "Rauc release certificate". 
        To configure release key ring, use "Rauc release keyring".
 	
	[Exit], [Exit], [Exit], [YES]
 ```

## 3) Enable release build
```
        $cd <workspace-bsp>/ptxproj/
        $ptxdist setup

	Development Options 	---> [Enter]
		()  environment variable whitelist (space separated) [Enter]

	add "BUILDTYPE" to environment variable whitelist [Enter]


 	[Exit], [Exit], [YES]
```

## 4) Rebuild firmware image
```
        $export BUILDTYPE=release
        $cd <workspace-bsp>/ptxproj/
        $ptxdist clean
        $ptxdist go -q
        $ptxdist images
```

NOTE:
>Rebuild is necessary to apply changes. Note also, that BUILDTYPE environment variable is set.

## 5) Generate WUP file
```
        $cd <workspace-bsp>/ptxproj/
        $make dist
```
 Afterwards you should find the WUP file:
```
~/wago/ptxproj-3.0.XX/platform-wago-pfcXXX/images/PFC-Linux_update_V0300XX_XX_rXXXXX.wup
```

# Create a custom RAUC slot:

RAUC bundles consists of one or more slots. For example, PFC uses separate slots to install root file system and boot loaders separately. To customize you installation, e.g. install custom applications or use custom partitions, you can specify own slots.

Detailed information about slots and how to use them, can be found at
- https://rauc.readthedocs.io/en/latest/scenarios.html
- https://rauc.readthedocs.io/en/latest/integration.html#ptxdist.

The RAUC configuration files of the PFC is located at
- Slot configuration files:    <workspace-bsp>/ptxproj/configs/wago-pfcXXX/projectroot/etc/rauc
- Image configuration file:    <workspace-bsp>/ptxproj/configs/wago-pfcXXX/config/images/rauc.config
- Location of slot hook files: <workspace-bsp>/ptxproj/shared_public/fwupdate/rauc_bundle_conf/update

# Compatibility list:
| PFC | Compatible |
|:-------------|:------------:|
| **PFC 100** | |
| 750-8100 | Y |
| 750-8101 | Y |
| 750-8102 | Y |
|  |  |
| **PFC 200** | |
| 750-8202 | Y |
| 750-8203 | Y |
| 750-8204 | Y |
| 750-8206 | Y |
| 750-8207 | Y |
| 750-8208 | Y |
|  |  |
| **PFC 200 G2** | |
| 750-8212 | Y |
| 750-8213 | Y |
| 750-8214 | Y |
| 750-8216 | Y |

