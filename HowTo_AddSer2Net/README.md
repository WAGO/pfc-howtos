#  Adding ser2net to the PFC

## Every package is bundled to a specific firmware version: For Version 2019.7.1 - FW:03.01.07(99)

This HowTo shows how to add and utilize ser2net on the PFC200.  
After installation on the PFC you will be able to access the onboard serial-port
via telnet.

# PREREQUISITES
This HowTo is based on a clean installation of Ubuntu LTS, with an installed 
and working WAGO Board-Support Package for PFC200.  
Working means that you successfully built the standard image “sd.hdimg”.


# Installation and usage

## 1) On development host (Ubuntu)
### 1.1) Copy rule files and sources to into ptxdist-rulefiles
```
>cp -v ptxproj/rules/* ~/<ptxproj>/rules/
>cp -v ptxproj/src/* ~/<ptxproj>/src/
```

### 1.2) Select package "ser2net" for build in ptxdist
```
>cd ~/<ptxproj>/
>ptxdist menuconfig
```

```
   Networking Tools                ---> [Enter]
     [*] ser2net (Select ser2net with [Space])

   [Exit], [Exit], and save configuration [Yes]
```

### 1.3) Clean "ser2net" package and other
```
>cd ~/<ptxproj>/
>ptxdist clean ser2net gensio libyaml
```

### 1.4) Build the "ser2net" package (package gensio and libyaml will also be build and needed)
```
>cd ~/<ptxproj>/
ptxdist targetinstall ser2net
```

If you see the "finished target ser2net.targetinstall.post" at the end of the run,it is time for a :)

You will find three needed IPKG package files:
- ~/<ptxproj>/platform-wago-pfcXXX/packages/ser2net_4.0_armhf.ipk
- ~/<ptxproj>/platform-wago-pfcXXX/packages/gensio_1.1_armhf.ipk
- ~/<ptxproj>/platform-wago-pfcXXX/packages/libyaml_0.2.2_armhf.ipk

### 1.5) Build complete firmware image "sd.hdimg" which includes perf package (optional)
You can build a complete image if you like. If you like follow these commands:
```
>cd ~/<ptxproj>/
>ptxdist images
```
As usual, you can:
- copy image file "sd.hdimg" with command "dd" to SD-Card and boot PFC200 from it.
- transfer packages into PFC200 file system and call "opkg install <pkg-name>.ipk
- utilize Web-Based-Management(WBM) feature "Software-Upload"

---

## 2.1) Copy IPKs to the PFC via ssh-copy
```
>cd ~/<ptxproj>/platform-wago-pfcXXX/packages/
>scp ser2net_4.0_armhf.ipk gensio_1.1_armhf.ipk libyaml_0.2.2_armhf.ipk root@<IP-PFC>:/root
```

## 2.2) Installing IPKG packages via  commandline
Open a terminal session (ssh or serial).
Login as "root" with password "wago" (default)
Install the package in the following order:
```
>cd /root
>opkg install libyaml_0.2.2_armhf.ipk
>opkg install gensio_1.1_armhf.ipk
>opkg install ser2net_4.0_armhf.ipk
```
---
# Testing the installed application
## 3.1) Open a (ssh or serial) terminal session to PFC200
```
>ssh root@<IP-PFC>
```

## 3.2) On PFC deactivate linux-console to be on serial port and reboot
```
>/usr/sbin/pfcXXX_config_rs232.sh owner=None
>reboot

```

## 3.3) Copy test configuration ser2net.yml and startup script to pfc
On development pc:
```
>scp -r <ser2net-dir>/pfc/etc/* root@<PFC-IP>:/etc
```
After the next reboot ser2net will be started automatically. Please do not reboot before
you've successfully tested the connection.

## 3.4) Test ser2net
Login via ssh to pfc
```
>ssh root@<IP-PFC>
```

on PFC start ser2net from commandline
```
>ser2net -p 3000 -d
```

This will start the ser2net application with an additional monitor/control session via telnet (port 3000)
it will also send any errors to stdout.

Connect the PFC-serial-port to your development pc. For example we will use /dev/ttyS0 as default port.
Also make sure that PC and PFC are connected to the same network.

```
+------+ serial +-------------------+
| PFC  |XXXXXXXX| PC /dev/ttyS0     |
+------+        +-------------------+
    X                    X
    X    +--------+      X
    XXXXX| Switch |XXXXXXX
Ethernet +--------+ Ethernet

```

Now to the funny part!
Open your favorite serial-terminal program on your development pc. And connect it to your serial-port.
For example we will use minicom.

```
>minicom -D /dev/ttyS0 -b 115200
```

Also open a telnet session (port 2000), on a separate terminal window, to your PFC:
```
>telnet <PFC-IP> 2000
```

After successfully creating both connection you are now able to test the communication in both directions.
Just simply go to the minicom window and type anything you like.
You should now receive on the telnet session the characters.
You also could use the telnet session to type in some characters and should receive this on the minicom session.
If so...everything works perfect and you are now able to connect any serial device to the pfc and communicate 
to it over a ethernet connection.

## 3.5) Enable auto start for "ser2net"
To start ser2net on start-up apply following steps.

1. Copy the start-up script to PFC via ssh. (If not allready done)
```
>scp <ser2net-dir>/pfc/etc/init.d/* root@<PFC-IP>:/etc/init.d/
```

2. Create symbolic link in /etc/rc.d/ for start-up ser2net on power-on.
```
>ssh root@<PFC-IP>
```
You need to enter the root password  
Now on PFC side, create the sym-link
```
>ln -s /etc/init.d/ser2net /etc/rc.d/S90_ser2net
```

3. Make /etc/init.d/ser2net executable
```
>chmod a+x /etc/init.d/ser2net
```

4. Reboot PFC to test the autostart
```
>reboot
```

## 4) Example configuration ser2net.yml
This is the example configuration for this howto.
It will setup a connection over telnet on port 2000 and claims /dev/serial, which is the onboard serial port.
Also it will set the serial connection to the baudrate of 115200 and the dataformat to 8N1.
Please refer to the manual ser2net.yaml.5 included in the sources if you need further information.

```
%YAML 1.1
---
#
# This is a ser2net configuration file, showing examples of all
# sorts of things.  It's all commented out so it's safe to put
# in /etc/ser2net/ser2net.yaml.
#
# This is described in ser2net.yaml(5)

# YAML gives an error on empty files, just add something so the
# error doesn't happen.
define: &confver 1.0

# # Set all baud rates to 115200n81 by default.
default:
   name: speed
   value: 115200n81

connection: &test
  accepter: telnet(rfc2217),tcp,2000
  connector: serialdev,/dev/serial,local

```
## 5) Virtual com port driver

If you need a virtual com port driver for your operating system and/or 
application. You can find some very useful information on this 
website: https://gist.github.com/DraTeots/e0c669608466470baa6c

---
# Compatibility list 
Software wise it is fully compatible to all. The list only shows 
devices which offers a onboard serial port

| PFC | Compatible |
|:-------------|:------------:|
| **PFC 100** | |
| 750-8100 | N |
| 750-8101 | N |
| 750-8102 | Y |
|  |  |
| **PFC 200** | |
| 750-8202 | Y |
| 750-8203 | N |
| 750-8204 | Y |
| 750-8206 | Y |
| 750-8207 | Y |
| 750-8208 | Y |
|  |  |
| **PFC 200 G2** | |
| 750-8212 | Y |
| 750-8213 | N |
| 750-8214 | Y |
| 750-8216 | Y |

