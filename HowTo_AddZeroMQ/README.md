# Add ZeroMQ library (libzmq-4.3.4)

## This HowTo describes the procedure of compiling "libzmq" for PFC.

# PREREQUISITES:
This HowTo is  based on a clean installation of Ubuntu LTS, with an installed
and working WAGO Board-Support Package for PFC200.
Working means that you successfully built the standard image “sd.hdimg”.

# Installation and usage

## 1) On development host (Ubuntu)
### 1.1) Copy rule files and sources to into ptxdist-rulefiles
```
>cp -v ptxproj/rules/* ~/<ptxproj>/rules/
```

### 1.2) Select package "libzmq" for build in ptxdist
```
>cd ~/<ptxproj>/
>ptxdist menuconfig
```

```
     Networking Tools -->

        [*] libzmq  ( Select with [Space] and press [enter] )

        Leave window with [Exit] [Exit], [Exit], and [Yes]
```

### 1.3) Clean the "libzmq" package
```
>cd ~/<ptxproj>/
>ptxdist clean libzmq
```

### 1.4) Build the "libzmq" package
```
>cd ~/<ptxproj>/
>ptxdist targetinstall libzmq       # Build libzmq package only.
```

If you see the "finished target libzmq.targetinstall.post" at the end of the run,it is time for a :)

Afterward you will find the OPKG installer package file:
-  ~/<ptxproj>/platform-wago-pfcXXX/packages/libzmq_4.0.4_arm.ipk

### 1.5) Build complete firmware image "sd.hdimg" which includes packages libzmq and libsodium.
```
>cd ~/<ptxproj>/
>ptxdist images
```

As usual, you can:
- copy image file "sd.hdimg" with command "dd" to SD-Card and boot PFC200 from it.
- transfer package into PFC200 file system and call "opkg install < pkg-name >.ipk
- utilize Web-Based-Management(WBM) feature "Software-Upload" 

---
# Testing the library

What you have to do is  write a ZeroMQ application for PFC
Feelfree to take a look at https://github.com/zeromq 
There you will find many examples in different languages.

