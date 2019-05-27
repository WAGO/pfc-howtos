# Add Train-Realtime-Data-Protocol(TRDP) support 
  
## Every package is bundled to a specific firmware version: For Version 2019.3.1 - FW:03.00.35(99)

This Howto describes needed steps to make TRDP protocol from TCNOpen available 
for PFC.

# ATTENTION: 
- PFC do not fit all required specification to be useful in trains! 
- Check TCNopen license

# PREREQUISITES
This HowTo is / based on a clean installation of Ubuntu LTS, with an installed 
and working WAGO Board-Support Package for PFC200.
Working means that you successfully built the standard image “sd.hdimg”.

# Step by Step: Build package "trdp"

## 1) Copy attached rule files 
```
      >cd <workspace>/HowTo_AddTRDP/ptxproj
      >cp rules/trdp.in <workspace-bsp>/ptxproj/rules/ 
      >cp rules/trdp.make <workspace-bsp>/ptxproj/rules/ 
```

## 2) Copy attached source code 
```
      >cd <workspace>/HowTo_AddTRDP/ptxproj
      >cp -r src/trdp-1.4.1.0 <workspace-bsp>/ptxproj/local_src/ 
```
	
## 3) Select package for build
```
	>cd <workspace-bsp>/ptxproj/
	>ptxdist menuconfig

	   Networking Tools --->
		   [*] trdp
                                          
	   Save changes and exit.
```

## 5) Rebuild package
```
	>cd <workspace-bsp>/ptxproj/
	>ptxdist clean trdp	
	>ptxdist targetinstall trdp
	
	You should now find:
	<workspace-bsp>/ptxproj/platform-wago-pfcXXX/packages/trdp-1.4.1.0_armhf.ipk	
```

## 6) Optional build complied firmware image "sd.hdimg" 
```
	>cd <workspace-bsp>/ptxproj/
	>ptxdist images

	You should now find:
	<workspace-bsp>/ptxproj/platform-wago-pfcXXX/images/sd.hdimg	
```	

# Step by Step: Use package "trdp"
As usual, you can:
- Copy image file "sd.hdimg" with command "dd" to SD-Card and boot PFC200 from it.
- Transfer package "<pkg-name>.ipk" into PFC200 file system and call "ipkg install <pkg-name>.ipk.
- Utilize Web-Based-Management(WBM) feature "Software-Upload" to upload and apply "<pkg-name>.ipk". 
- Transfer executable to PFC by FTP/scp or whatever and make it executable.
- Configure Eclipse-CDT for file transfer and remote debugging. 
CAUTION: Every package or binary is bundled to a specific firmware version!


## Using Web-Based-Management(WBM) feature "Software-Upload" for upload and installing IPKG packages


### 1) Start your local browser, and navigate of PFC200's default homepage(WBM)  
```
	https://192.168.1.17
	Ignore Cert-Warning ...
```
	
### 2) Select "Software-Upload" in left hand "Navigation" pane, You will be requested to authenticate!
```
	Login as "admin" with password "wago" (default)
```
### 3) Press button [Browser] to open the local file dialogue.	
	Browse to folder "~/wago/ptxproj-2.4.22/platform-wago-pfcXXX/packages/" 
	Select package to install or update, here "trdp_1.4.1.0_arm.ipk".

### 4) Click on button [Start Upload].
```
	Transfer selected file into PFC200 file system and press button [Submit].
```
	
### 5) In newly shown section "Activate new software", press button [Submit] to install the package.
```
	Internally WBM just calls:
		>cd /home/
		>opkg install trdp_1.4.1.0_arm.ipk
```

### 6) Open a (ssh or serial) terminal session to PFC200
```
	 Login as "root" with password "wago" (default)
```

### 7) Run example applications on PFC 
#### 7.1) Run 'sendHello'
```
	>sendHello
```
	
#### 7.2) Open a second terminal session to PFC200 and run 'receiveHello'
```
	>receiveHello	
```
	
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



