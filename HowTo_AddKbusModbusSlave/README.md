# Adding KbusModbusPFCSlave to the PFC

## Every package is bundled to a specific firmware version: For Version 14 - FW:03.02.02(99)

This HowTo shows how to build and install the Kbus Modbus Slave application 
"kbusmodbusslave" who acting like a Modbus-Coupler 750-352.
It also acting as counter part for WAGO-IO-Check.

The application "kbusmodbusslave" can be interessting for two reason:
1. Direct access to KBus-IO-Modules for any third party process (Mono, PHP, Node.js, ...) who's able to send modbus requests.
2. As an extensive real world example for coding with ADI/DAL-Interface.

# CAUTION
>Take care that there is no other ADI/DAL-Application(like PLC-Runtime) active while using this application. Because ADI/DAL-Interface offer a single process support only!

# PREREQUISITES
This HowTo is / based on a clean installation of Ubuntu LTS, with an installed and working WAGO Board-Support Package for PFC200. Working means that you successfully built the standard image “sd.hdimg”.

# Build package "kbusmodbusslave" on development host:
1. Copy attached rule file and sources for "kbusmodbusslave" to given folder:
```
$cp ./ptxproj/rules/kbusmodbusslave.in    ~/wago/ptxproj/rules/
$cp ./ptxproj/rules/kbusmodbusslave.make  ~/wago/ptxproj/rules/
$cp ./ptxproj/src/kbusmodbusslave-1.4.1.tar.bz2 ~/wago/ptxproj/src/
```

2. Select "kbusmodbusslave" package for build
```
$cd ~/wago/ptxproj/
$ptxdist menuconfig
      Wago Specific                   ---> [ENTER]
      DAL - Device Abstraction Layer  ---> [ENTER]
          [*]     Modbus-PFC-Slave

      <Exit>, <Exit>, <Exit> and <Yes>
```

3. Clean the package
```
$cd ~/wago/ptxproj
$ptxdist clean kbusmodbusslave
```

4. Build package or complete image
    1. Build package only
    ```
        $ptxdist targetinstall kbusmodbusslave
    ```
    Afterwards you should find the IPKG installer package file:
        ~/wago/ptxproj/platform-wago-pfcXXX/packages/kbusmodbusslave_1.4.1_arm.ipk

  2. Build complete firmware image "sd.hdimg"  (optional)
  ```
        $cd ~/wago/ptxproj/
        $ptxdist clean
        $ptxdist go -q
        $ptxdist images
  ```
   Afterwards you should find the firmware image "sd.hdimg":
        ~/wago/ptxproj/platform-wago-pfcXXX/images/sd.hdimg          #The firmware image

As usual, you can:
- copy image file "sd.hdimg" with command "dd" to SD-Card and boot PFC200 from it.
- transfer package "kbusmodbusslave_1.4.1_arm.ipk" into PFC's file system and call "opkg install <pkg-name>.ipk"
- utilize Web-Based-Management(WBM) feature "Software-Upload".

----------------------------------------------------------------------------------------------------------------------
# Using SSH for upload and installing OPKG package

1. On development host
```
    $cd ~/wago/ptxroj/platform-wago-pfcXXX/packages/
    $scp kbusmodbusslave_1.4.1_armhf.ipk root@<IP-of-PFC>:/root
    root@<IP-of-PFC>'s password:  # Enter the root password (default: wago)
    kbusmodbusslave_1.4.1_armhf.ipk               100%   23KB  23.3KB/s   00:00
```

2. On PFC
Connect to PFC via SSH
```
    $ssh root@<IP-of-PFC>
    root@<IP-of-PFC>'s password: #Enter the root password (default: wago)

    WAGO Linux Terminal on PFC200V3-AABBCC

    root@PFC200V3-AABBCC:
```
You are now connected to the PFC.
Install kbusmodbusslave:
```
    $ opkg install kbusmodbusslave_1.4.1_armhf.ipk
    Installing kbusmodbusslave (1.4.1) on root.
    Configuring kbusmodbusslave.
```
Contratulations kbusmodbusslave is now installed on the PFC.
Test it on PFC:
```
    $ /etc/init.d/runtime stop
    Terminate CoDeSys v2.3...1403
    Terminating CoDeSys v2.3, killing runtime ...done
    $ kbusmodbusslave -v 7 --nodaemon

    ======= CONFIGURATION =======
    PORT: 502
    MAX CONNECTIONS: 5
    OPERATION MODE: 0
    MODBUS DELAY MS: 0
    KBUS CYCLE TIME MS: 50
    KBUS PRIORITY: 60
    ==============================
    verbosity level is 7
    Not running in background
    kbusmodbusslave running...
    ADI Device[0]: libpbdpm
    Modbus: Wait for KBUS to be initialized
    ADI Device[1]: libpackbus
    Found kbus device on: 1
    KBUS device open OK
    KBUS set to application state: 1
    [...]
```

----------------------------------------------------------------------------------------------------------------------
# Using Web-Based-Management(WBM) feature "Software-Upload" for upload and installing OPKG packages

1. Start your local browser, and navigate of PFC's default homepage(WBM)
```
        https://192.168.1.17
```
    Ignore Cert-Warning ...

2. Select "Software-Upload" in left hand "Navigation" pane, You will be requested to authenticate!
```
    Login as "admin" with password "wago" (default)
```

3. Press button [Browser] to open the local file dialogue.
```
    Browse to attached folder "./packages/"
    Select package to install or update, here "kbusmodbusslave_1.4.1_armhf.ipk".
```

4. Click on button [Start Upload].

    Transfer selected file into PFC file system and press button [Submit].

5. In newly shown section "Activate new software", press button [Submit] to install the package.
```
    Internally WBM just calls:
        >cd /home/
        >opkg install kbusmodbusslave_1.4.1_armhf.ipk
```

6. Open a (ssh or serial) terminal session to PFC
```
        Login as "root" with password "wago" (default)
```

7. Kill possible running CoDeSys2 runtime "plclinux_rt" or e!COCKPIT runtime "codesys3" (Because ADI/DAL can only be used by ONE application)
```
    $/etc/init.d/runtime stop
```

8. Run application with option "--nodaemon -v7", to see some output on console.
    Starts program not as background daemon and let it be most verbose.
```
    $kbusmodbusslave --nodaemon -v7
    ======= CONFIGURATION =======
    PORT: 502
    MAX CONNECTIONS: 5
    COUPLER MODE: 0
    MODBUS DELAY MS: 0
    KBUS CYCLE TIME MS: 50
    KBUS PRIORITY: 60
    ==============================
    verbosity level is 7
    kbusmodbusslave running...
    ADI Device[0]: libcanlayer2
    Modbus: Wait for KBUS to be initialized
    ADI Device[1]: libpbdpm
    ADI Device[2]: libpackbus
    Found kbus device on: 2
    KBUS device open OK
    KBUS set to application state: 1

            .KbusBitCount: 224
            .TerminalCount: 8
            .ErrorCode: 0
            .ErrorArg: 0
            .ErrorPos: 0
            .BitCountAnalogInput: 288
            .BitCountAnalogOutput: 224
            .BitCountDigitalInput: 12
            .BitCountDigitalOutput: 6
    Offset: IN: 36 - OUT: 28

     Pos:1:  Type: 750-5XX / 4DO-DIAG   BitOffsetOut:224;   BitSizeOut:4;   BitOffsetIn:288; BitSizeIn:4;    Channels:0;     PiFormat:0;
     Pos:2:  Type: 750-4XX / 4DI        BitOffsetOut:0;     BitSizeOut:0;   BitOffsetIn:292; BitSizeIn:4;    Channels:0;     PiFormat:0;
     Pos:3:  Type: 750-453 / 0-0        BitOffsetOut:0;     BitSizeOut:0;   BitOffsetIn:0;   BitSizeIn:64;   Channels:4;     PiFormat:0;
     Pos:4:  Type: 750-478 / 0-0        BitOffsetOut:0;     BitSizeOut:0;   BitOffsetIn:64;  BitSizeIn:32;   Channels:2;     PiFormat:0;
     Pos:5:  Type: 750-4XX / 4DI        BitOffsetOut:0;     BitSizeOut:0;   BitOffsetIn:296; BitSizeIn:4;    Channels:0;     PiFormat:0;
     Pos:6:  Type: 753-647 / 0-0        BitOffsetOut:0;     BitSizeOut:192; BitOffsetIn:96;  BitSizeIn:192;  Channels:1;     PiFormat:0;
     Pos:7:  Type: 750-5XX / 2DO        BitOffsetOut:228;   BitSizeOut:2;   BitOffsetIn:0;   BitSizeIn:0;    Channels:0;     PiFormat:0;
     Pos:8:  Type: 753-556 / 0-0        BitOffsetOut:192;   BitSizeOut:32;  BitOffsetIn:0;   BitSizeIn:0;    Channels:2;     PiFormat:0;

    Watchdog Init
    Modbus config Init
    Modbus Config MAC Init
    Modbus KBUS Info Init
    Modbus const Init
    Modbus ShortDesctiption Init
    Modbus-Init complete - Ready for take off
```

9. Alternatively you can use netstat to check that modbus port 502 is open
```
        root@PFC200-405C59:~ netstat -tl
        Active Internet connections (only servers)
        Proto Recv-Q Send-Q Local Address           Foreign Address         State
        tcp        0      0 localhost:8000          (null):*                LISTEN
        tcp        0      0 (null):wago-serv-ser    (null):*                LISTEN
        tcp        0      0 (null):wago-serv-tcp    (null):*                LISTEN
        tcp        0      0 (null):www              (null):*                LISTEN
        tcp        0      0 (null):502              (null):*                LISTEN
        tcp        0      0 (null):ssh              (null):*                LISTEN
        tcp        0      0 (null):https            (null):*                LISTEN
```

--------------------------------------------------------------------------------------

# Enable auto start for "kbusmodbusslave"
To start kbusmodbusslave on start-up apply following steps.

1. Copy the start-up script to PFC via ssh.
```
$scp ~/Downloads/HowTo_AddModbusPFCSlave/pfc/etc_init.d/kbusmodbusslave    root@<IP-PFCX00>:/etc/init.d/kbusmodbusslave
```
   You need to enter the root password

2. Create symbolic link in /etc/rc.d/ for start-up kbusmodbusslave on power-on.

 Open a terminal session to PFC
```
  $ssh root@<IP-PFCX00>
``
  You need to enter the root password

  Now on PFC side, create the sym-link
  ```
  $ln -s /etc/init.d/kbusmodbusslave /etc/rc.d/S90_kbusmodbusslave
  ```

3. Make /etc/init.d/kbusmodbusslave executable
  ```
  $chmod a+x /etc/init.d/kbusmodbusslave
  ```

4. Check for any ADI/DAL-Application(like PLC-Runtime) to be not started during start-up

   Check for link to start up PLC-Runtime
   ```
   $cd /etc/rc.d/
   $ll
   ```
   If link S98_runtime is present delete it
   ```
   $rm S98_runtime
   ```
5. Reboot PFC to test the autostart
```
   $reboot
```

# Configuration file "/etc/kbusmodbusslave.conf"
The file contains configuration information for kbusmodbusslave.
The configuration file is a free-form ASCII text file.
Keywords in the file are case-sensitive.
Comments an be placed on every line beginning and has to start with '#'.

Default configuration file: /etc/kbusmodbusslave.conf

    #CONFIGURATION FILE FOR KBUSMODBUSSLAVE DAEMON

    #DEFAULT PORT IS 502
    modbus_port 502

    #MAXIMUM ALLOWED TCP CONNECTIONS
    max_tcp_connections 5

    #SET MODBUSMODE ASYNCHRONUS MODE (MODE 0)
    #OR AS SYNCHRONUS MODE (MODE 1)
    operation_mode 0

    #SET MODBUS RESPONSE DELAY (Default: 0)
    modbus_delay_ms 0

    #SET KBUS PRIORITY (Default: 60)
    kbus_priority 60

    #SET KBUS CYCLE MS (Default: 50)
    kbus_cycle_ms 50

--------------------------------------------------------------------------------------
# Debug Modes
Via Parameter "verbose" you are able to generate some debug information.
A higher value will generate noisier output.

Currently the following stages are implemented:

| Name          | Value |
| ------------- |:-----:|
| VERBOSE_STD   |  1    |
| VERBOSE_INFO  |  2    |
| VERBOSE_GOSSIPY | 3   |
| VERBOSE_DEBUG |  7    |

## Special Information
In VERBOS_DEBUG mode the raw modbus telegram for RX and TX will be shown for debug purpose.

--------------------------------------------------------------------------------------
# Operration Mode

1. Synchronus-Mode:
On a incomming modbus request a KBUS cycle is initiated. So that new data will be written
synchronously. The response will deliver updated KBUS-data. Also the KBUS and modbus data
is updated cyclically, just to keep the output alive. (kbus_cycle_ms)

```

                +                                         +
                |                                         | KBUS & modbus update
                |                                         |
  Req(WD, RD)   |                                         |
  +------------>+                                         |
                |                                         |
               +++                                        |
               | |Modbus Write                            |
               +++                                        |
                |                                         |
                +--------------------------------------> +++
                                 KBUS Write/Read         | | KBUS Cycle
                +<-------------------------------------+ +++
                |                                         |
               +++                                        |
               | |Modbus Read                             |
               +++                                        |
                |                                         |
  <-------------+                                         |
  Resp          |                                         |
                |                                         |
                |                                         | t
                +                                         v
```

**PRO:** Low Jitter

**CONTRA:** Higher response time, because of Modbus+KBUS execution time. More CPU power is needed.


2. Asynchronus-Mode:
On an incomming modbus request a KBUS a response is done immediately. The data will be
updated on every KBUS cycle. (kbus_cycle_ms)

```
                +                                         +
                |                                         | Cyclic Execution
                |                                         | of KBUS update
  Req(WD, RD)   |                                        +++
  +------------>+                                        | | KBUS Cycle
                |                                        +++
               +++                                        |
               | |Modbus Write/Read                       |
               +++                                       +++
  Resp          |                                        | | KBUS Cycle
  <-------------+                                        +++
                |                                         | t
                +                                         v
```
**PRO:** Minimal response time with minimal CPU load.

**CONTRA:** High Jitter in the range of +/- kbus_cycle_ms

# WAGO-IO-CHECK / Operation Manual Switch

To be able to communicate with WAGO-IO-CHECK please make sure to switch the operation 
manual switch (OMS) to position "STOP". Otherwise WAGO-IO-CHECK will recognize that
an PFC applikation is running and will not allow to communicate.

OMS Positions:
     RUN: kbusmodbusslave is running and will answer incomming modbus telegrams.
    STOP: kbusmodbusslave is stoped and incomming modbus telegrams will be answered with
          the exception code 06 "Slave Device Busy".
   RESET: If you hold for minimum 3 seconds the switch in that position, kbusmodbuslave 
          will be restarted and initialized.

# Modus register definition
For further details about modbus, please refer to the WAGO 750-352
chapter Modbus.

### READ (FC3 and FC4)

| dec         | hex           |                                                                     |
| -----------:| ------------- |---------------------------------------------------------------------|
|      0..255 | 0x0000..0x00FF | Physical-Input-Area 1  - First 256 Words of physical input data    |
|    512..767 | 0x0200..0x02FF | Physical-Output-Area 1 - First 256 Words of physical output data   |
| 4096..12287 | 0x1000..0x2FFF | Configuration-Register (see. Configuration Register)               |
|24576..25339 | 0x6000..0x62FB | Physical-Input-Area 2  - Additional 764 Words of physical input data |
|28672..29435 | 0x7000..0x72FB | Physical-Output-Area 2 - Additional 764 Words of physical output data |

### WRITE (FC6 and FC16)

| dec         | hex           |                                                                     |
| -----------:| ------------- |---------------------------------------------------------------------|
|    0..255   | 0x0000..0x00FF | Physical-Output-Area 1 - First 256 Words of physical output data |
|  512..767   | 0x0200..0x02FF | Physical-Output-Area 1 - First 256 Words of physical output data - Mirror |
| 4096..12287 | 0x1000..0x2FFF | Configuration-Register (see. Configuration Register) |
|24576..25339 | 0x6000..0x62FB | Physical-Output-Area 2 - Additional 764 Words of physical output data|
|28672..29435 | 0x7000..0x72FB | Physical-Output-Area 2 - Additional 764 Words of physical output data - Mirror|

### Bit access read (FC1 and FC2)

| dec         | hex           |                                                                     |
| -----------:| ------------- |---------------------------------------------------------------------|
|    0..511   | 0x0000..0x01FF | Physical-Input-Area 1  - First 512 digital input |
|  512..1023  | 0x0200..0x03FF | Physical-Output-Area 1 - First 512 digital output |
| 32768..34295 | 0x8000..0x8f57 | Physical-Input-Area 2  - Bit 513 to Bit 2039 input|
| 36864..38391 | 0x9000..0x9f57 | Physical-Output-Area 2 - Bit 513 to Bit 2039 output |

### Bit access write (FC5 and FC15)

| dec         | hex           |                                                                     |
| -----------:| ------------- |---------------------------------------------------------------------|
|    0..511   | 0x0000..0x01FF | Physical-Input-Area 1 - First 512 digital output |
|  512..1023  | 0x0200..0x03FF | Physical-Input-Area 1 - First 512 digital output - Mirror |
| 32768..34295 | 0x8000..0x8f57 | Physical-Input-Area 2 - Bit 513 to Bit 2039 output |
| 36864..38391 | 0x9000..0x9f57 | Physical-Input-Area 2 - Bit 513 to Bit 2039 output  - Mirror |

## CONFIGURATION - REGISTER

### Modbus Watchdog

|hex | [R/W] | [Words] | [Description] |
| -- | ----- | :-------: | ------------- | 
| 0x1000 | R/W | 1 | Watchdog time read/write |
| 0x1003 | R/W | 1 | Watchdog trigger / Watchdog start |
| 0x1004 | R | 1 | Minimum trigger time |
| 0x1006 | R | 1 | Watchdog status (0:off 1:on) |
| 0x1008 | R/W | 1 | Stop watchdog (Write sequence 0x55AA or 0xAA55) |

### Process Image Information

|hex | [R/W] | [Words] | [Description] |
| -- | ----- | :-------: | ------------- |
| 0x1022 | R | 1 | Count of analogue process-image output data (bits) |
| 0x1023 | R | 1 | Count of analogue process-image input data (bits) |
| 0x1024 | R | 1 | Count of digital process-image output data (bits) |
| 0x1025 | R | 1 | Count of digital process-image input data (bits) |

### MAC-ID

|hex | [R/W] | [Words] | [Description] |
| -- | ----- | :-------: | ------------- |
| 0x1031 | R | 1 | MAC-Address of device |

### Constants

|hex | [R/W] | [Words] | [Description] |
| -- | ----- | :-------: | ------------- |
| 0x2000 | R | 1 | Constant 0x0000 |
| 0x2001 | R | 1 | Constant 0xFFFF |
| 0x2002 | R | 1 | Constant 0x1234 |
| 0x2003 | R | 1 | Constant 0xAAAA |
| 0x2004 | R | 1 | Constant 0x5555 |
| 0x2005 | R | 1 | Constant 0x7FFF |
| 0x2006 | R | 1 | Constant 0x8000 |
| 0x2007 | R | 1 | Constant 0x3FFF |
| 0x2008 | R | 1 | Constant 0x4000 |

### Short description
|hex | [R/W] | [Words] | [Description] |
| -- | ----- | :-------: | ------------- |
| 0x2020 | R | 1..16 | Short description of device. (ASCII-coded values) |

### I/O Modules

|hex | [R/W] | [Words] | [Description] |
| -- | ----- | :-------: | ------------- |
| 0x2030 | R | 1..65 | Description of the connected I/O modules (module 0....64) |
| 0x2031 | R | 1..64 | Description of the connected I/O modules (module 64...129)|
| 0x2030 | R | 1..64 | Description of the connected I/O modules (module 130..194)|
| 0x2030 | R | 1..64 | Description of the connected I/O modules (module 195..255)|

# FILESYSTEM ACCESS

In /tmp/KBUS/ those files could be found:
* termCount: Count of connected I/O-modules
* termInfo: Textfile wich mirrors the I/O-module assembly

```
    e.g:
     Pos:1:  Type: 750-5XX / 4DO-DIAG   BitOffsetOut:224;   BitSizeOut:4;   BitOffsetIn:288; BitSizeIn:4;    Channels:0;     PiFormat:0;
     Pos:2:  Type: 750-4XX / 4DI        BitOffsetOut:0;     BitSizeOut:0;   BitOffsetIn:292; BitSizeIn:4;    Channels:0;     PiFormat:0;
     Pos:3:  Type: 750-453 / 0-0        BitOffsetOut:0;     BitSizeOut:0;   BitOffsetIn:0;   BitSizeIn:64;   Channels:4;     PiFormat:0;
     Pos:4:  Type: 750-478 / 0-0        BitOffsetOut:0;     BitSizeOut:0;   BitOffsetIn:64;  BitSizeIn:32;   Channels:2;     PiFormat:0;
     Pos:5:  Type: 750-4XX / 4DI        BitOffsetOut:0;     BitSizeOut:0;   BitOffsetIn:296; BitSizeIn:4;    Channels:0;     PiFormat:0;

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
