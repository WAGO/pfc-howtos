# Add OpenDDS

## This HowTo shows how to build and install the OpenDDS libraries and OpenDDS-Example

# PREREQUISITES:
This HowTo is  based on a clean installation of Ubuntu LTS, with an installed
and working WAGO Board-Support Package for PFC200.
Working means that you successfully built the standard image “sd.hdimg”.

# Installation and usage

## 1) On development host (Ubuntu)
### 1.1) Copy attached rule file, sources and patches for "opendds" to given folder:
```
$cp ./ptxproj/rules/* ~/<ptxproj>/rules/
$cp -r ./ptxproj/patches/OpenDDS-3.16 ~/<ptxproj>/patches/
```
### 1.2) Select "opendds" package for build
```
        $cd ~/<ptxproj>
        $ptxdist menuconfig
            Networking Tools                ---> [ENTER]
            [*] OpenDDS
            [*]   OpenDDS-Example (optional)

                <Exit>, <Exit> and <Yes>
```
### 1.3) Clean the package
```
        $cd ~/<ptxproj>
        $ptxdist clean opendds
```
### 1.4) Build package only
```
        $ptxdist targetinstall opendds
```

Afterwards you should find the IPKG installer package file:
- ~/< ptxproj >/platform-wago-pfcXXX/packages/opendds_3.16_arm.ipk

### 1.5) Build complete firmware image "sd.hdimg"  (optional)
```
>cd ~/<ptxproj>/
>ptxdist images
```
 Afterwards you should find the firmware image "sd.hdimg":
- ~/< ptxproj >/platform-wago-pfcXXX/images/sd.hdimg          #The firmware image

As usual, you can:
- copy image file "sd.hdimg" with command "dd" to SD-Card and boot PFC200 from it.
- transfer package "opendds_1.0.0_arm.ipk" into PFC's file system and call "ipkg install <pkg-name>.ipk"
- utilize Web-Based-Management(WBM) feature "Software-Upload".

---
# Executing examples

## 1) Install package opendds_3.16_arm.ipk your prefered way

## 2) Open a (ssh or serial) terminal session to PFC
Login as "root" with password "wago" (default)
Change directory to example location.
```
$cd /usr/share/OpenDDS/
```

        In the directory are three examples located:
        1. Messenger
        2. Messenger.minimal
        3. Messenger_ZeroCopy

        For source code please refer to OpenDDS-3.16 sources.
        In every directory you will find two little programs:
        1. subscriber
        2. publisher
        and a script:
        1. run_test.sh

        To execute an example:
        for e.g. Example-Messenger

        $cd Messenger

        $./run_test.sh

## 3) Expected output:
```
	TAO (486|3048595456) - Completed initializing the process-wide service context
	TAO (486|3048595456) - Default ORB services initialization begins
	TAO (486|3048595456) - ORBInitializer_Registry::register_orb_initializer 0 @0x237248
	TAO (486|3048595456) - ORBInitializer_Registry::register_orb_initializer 1 @0x234af0
	TAO (486|3048595456) - Default ORB services initialization completed
	TAO (486|3048595456) - We are the default ORB ...
	TAO (486|3048595456) - Initializing the orb-specific services
	SampleInfo.sample_rank = 0
	SampleInfo.instance_state = 1
	Message: subject    = Review
			 subject_id = 99
			 from       = Comic Book Guy
			 count      = 0
			 text       = Worst. Movie. Ever.
			 SampleInfo.sample_rank = 0
			 SampleInfo.instance_state = 1
	Message: subject    = Review
			 subject_id = 100
			 from       = Comic Book Guy
			 count      = 1
			 text       = Worst. Movie. Ever.
			 SampleInfo.sample_rank = 0
			 SampleInfo.instance_state = 1
	Message: subject    = Review
			 subject_id = 101
			 from       = Comic Book Guy
			 count      = 2
			 text       = Worst. Movie. Ever.
			 SampleInfo.sample_rank = 0
			 SampleInfo.instance_state = 1
	Message: subject    = Review
			 subject_id = 102
			 from       = Comic Book Guy
			 count      = 3
			 text       = Worst. Movie. Ever.
			 SampleInfo.sample_rank = 0
			 SampleInfo.instance_state = 1
	Message: subject    = Review
			 subject_id = 103
			 from       = Comic Book Guy
			 count      = 4
			 text       = Worst. Movie. Ever.
			 SampleInfo.sample_rank = 0
			 SampleInfo.instance_state = 1
	Message: subject    = Review
			 subject_id = 104
			 from       = Comic Book Guy
			 count      = 5
			 text       = Worst. Movie. Ever.
			 SampleInfo.sample_rank = 0
			 SampleInfo.instance_state = 1
	Message: subject    = Review
			 subject_id = 105
			 from       = Comic Book Guy
			 count      = 6
			 text       = Worst. Movie. Ever.
			 SampleInfo.sample_rank = 0
			 SampleInfo.instance_state = 1
	Message: subject    = Review
			 subject_id = 106
			 from       = Comic Book Guy
			 count      = 7
			 text       = Worst. Movie. Ever.
			 SampleInfo.sample_rank = 0
			 SampleInfo.instance_state = 1
	Message: subject    = Review
			 subject_id = 107
			 from       = Comic Book Guy
			 count      = 8
			 text       = Worst. Movie. Ever.
			 SampleInfo.sample_rank = 0
			 SampleInfo.instance_state = 1
	Message: subject    = Review
			 subject_id = 108
			 from       = Comic Book Guy
			 count      = 9
			 text       = Worst. Movie. Ever.
	SampleInfo.sample_rank = 0
	SampleInfo.instance_state = 2
	SampleInfo.sample_rank = 0
	SampleInfo.instance_state = 2
	SampleInfo.sample_rank = 0
	SampleInfo.instance_state = 2
	SampleInfo.sample_rank = 0
	SampleInfo.instance_state = 2
	SampleInfo.sample_rank = 0
	SampleInfo.instance_state = 2
	SampleInfo.sample_rank = 0
	SampleInfo.instance_state = 2
	SampleInfo.sample_rank = 0
	SampleInfo.instance_state = 2
	SampleInfo.sample_rank = 0
	SampleInfo.instance_state = 2
	SampleInfo.sample_rank = 0
	SampleInfo.instance_state = 2
	SampleInfo.sample_rank = 0
	SampleInfo.instance_state = 2
	..:: EXIT ::..
```
Example was executed sucessfull and OpenDDS is installed correctly.

# Compatibiliy-List
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

