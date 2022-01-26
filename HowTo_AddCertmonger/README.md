# Add certmonger (certmonger-0.79.15)

## This HowTo describes the procedure of compiling "certmonger" for PFC-G2.

# Introduction
The certmonger daemon monitors certificates for impending expiration, and can optionally refresh soon-to-be-expired certificates with the help of a CA. If told to, it can drive the entire enrollment process from key generation through enrollment and refresh.

It can work with either flat files, like those used by OpenSSL, or with NSS databases.

# PREREQUISITES:
This HowTo is  based on a clean installation of Ubuntu LTS, with an installed
and working WAGO Board-Support Package for PFC200-G2.
Working means that you successfully built the standard image “sd.hdimg”.

# Installation and usage

## 1) On development host (Ubuntu)
### 1.1.1 sudo apt install libparse-yapp-perl
### 1.1) Copy rule and patch files to your ptxdist-project directory
```
>cp -v ptxproj/rules/* ~/<ptxproj>/rules/
>cp -vR ptxproj/patches/* -/<ptxproj>/patches/
```

### 1.2) Select package "certmonger" for build in ptxdist
```
>cd ~/<ptxproj>/
>ptxdist menuconfig
```

```
     Networking Tools -->

        [*] certmonger  ( Select with [Space] and press [enter] )

        Leave window with [Exit], [Exit], and [Yes]
```

### 1.3) Clean the "certmonger" package
```
>cd ~/<ptxproj>/
>ptxdist clean certmonger
```

### 1.4) Build the "certmonger" package
```
>cd ~/<ptxproj>/
>ptxdist targetinstall certmonger       # Build certmonger package only.
```

If you see the "finished target certmonger.targetinstall.post" at the end of the run,it is time for a :)

Afterward you will find the OPKG installer package file:
-  ~/<ptxproj>/platform-wago-pfcXXX/packages/certmonger_0.79.15_armhf.ipk

## ATTENTION
Keep in mind certmonger depends on several libraries and tools. For example:
 * TALLOC
 * TEVENT
 * DBUS
 * NSS
 * LIBXML2
 * LIBCURL
 * JANSSON
 * KRB5
 * OPENLDAP
 * LIBPOPT

To use certmonger all of theses packages has to be install separately on the PFC. There are also more
dependencies for the above listed ones. So the best idea is to build a complete image to deploy to the
PFC-G2

### 1.5) Build complete firmware image "sd.hdimg" which includes packages certmonger and all dependencies.
```
>cd ~/<ptxproj>/
>ptxdist images
```

After successfully build copy the image file "sd.hdimg" with command "dd" to SD-Card and boot the PFC200-G2 from it.

# Compatibility list:
It's only compatible with PFC-G2 because of memory-limitation of PFC-G1 and PFC100

| PFC | Compatible |
|:-------------|:------------:|
| **PFC 100** | |
| 750-8100 | N |
| 750-8101 | N |
| 750-8102 | N |
|  |  |
| **PFC 200** | |
| 750-8202 | N |
| 750-8203 | N |
| 750-8204 | N |
| 750-8206 | N |
| 750-8207 | N |
| 750-8208 | N |
|  |  |
| **PFC 200 G2** | |
| 750-8212 | Y |
| 750-8213 | Y |
| 750-8214 | Y |
| 750-8216 | Y |
