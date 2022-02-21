# HowTo: Utilize serial terminal 750-652 via DAL/ADI

This HowTo descripes steps for using serial interface terminal "750-652" 
within your C/C++ application via DAL/ADI-API.

Terminal 750-652 able to operate as RS232, RS422 or RS485.
Terminal 750-652 use 6, 22 or 46 bytes in process image for data exchange.
Data exchange controlled/monitored by 2 additional bytes.

Use tool "Wago-IO-Check.exe" to configure terminals data width:
- " 8byte": 2 control/status bytes +  6 data bytes(default)
- "24byte": 2 control/status bytes + 22 data bytes
- "48byte": 2 control/status bytes + 46 data bytes

Use tool "Wago-IO-Check.exe" to configure terminals:
- Operating-Mode: RS232, RS422 or RS485(default)
- Baudrate, Databytes, Parity, Stopbits, FlowControl, Duplex-Mode

File "kbus652.h" offer a set of functions for using serial interface terminal 750-652.

IMPORTANT:
Set "PISIZE_652" depending on terminals current data width.

The KBUS could only be used by one master, it could be the CoDeSys-Runtime
or your own application. But not both.
Take care to stop CoDeSys-Runtime "plclinux_rt" before run this demo.

The KBUS-API self is published in "adi_application_interface.h".

## 1) Copy content of attached folder "rules" into ~/\<ptxproj\>/rules.
```
    >cp rules/* ~/wago/<ptxproj>/rules/
```

## 2) Copy folder "kbusdemo652" into ~/wago/\<ptxproj\>/src/
```
   >cp -R ./src/kbusdemo652 ~/wago/<ptxproj>/src/kbusdemo652
```

## 3) Take a look into the source code files for additional information

## 4) Select kbusdemo652 in ptxdist to be build
```
    >cd ~/wago/<ptxproj>
    >ptxdist menuconfig

    Wago Specific -->
        Demo        -->
            [*] KBUS demo on using 750-652

    Save changes and exit.
```

## 5) Build examples
```
    >ptxdist clean kbusdemo652 && ptxdist targetinstall kbusdemo652
```
## 6) Copy binaries to PFC200 and install it.
```
    >cd ~/wago/<ptxproj>
    scp platform-wago-pfcXXX/packages/kbusdemo652_0.1.6_armhf.ipk root@<IP-PFC>:/root
```
Login to PFC via SSH

```
    $ssh root@<IP-PFC>

    >opkg install kbusdemo652_0.1.6_armhf.ipk
```

## 7) Run executable
```
    >/usr/bin/kbusdemo652
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

