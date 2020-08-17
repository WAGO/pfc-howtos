# HowTo use access retain variables PFC

## Every package is bundled to a specific firmware version: For Version 17 - FW:03.05.10(99)

This HowTo shows how to access the retain memory-area by using C or C++

# PREREQUISITES
This HowTo is / based on a clean installation of Ubuntu LTS, with an installed and working WAGO Board-Support Package for PFC200. Working means that you successfully built the standard image “sd.hdimg”.

# Build package "wretain_example" on development host:
1. Copy attached rule file and sources to given folder:
```
$cp ./ptxproj/rules/wago-demo-wretain_example.in    ~/wago/ptxproj/rules/
$cp ./ptxproj/rules/wago-demo-wretain_example.make  ~/wago/ptxproj/rules/
$cp ./ptxproj/src/wretain_example-1.0.0.tgz ~/wago/ptxproj/src/
```

2. Select "wretain_example" package for build
```
$cd ~/wago/ptxproj/
$ptxdist menuconfig
      Wago Specific                   ---> [ENTER]
      DEMO - Device Abstraction Layer  ---> [ENTER]
          [*]     Retain Variables API usage: C & C++

      <Exit>, <Exit>, <Exit> and <Yes>
```

3. Clean the package
```
$cd ~/wago/ptxproj
$ptxdist clean wretain_example
```

4. Build package or complete image
    1. Build package only
    ```
        $ptxdist targetinstall wretain_example
    ```
    Afterwards you should find the IPKG installer package file:
        ~/wago/ptxproj/platform-wago-pfcXXX/packages/wretain-example_1.0.0_armhf.ipk

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
- transfer package "wretain-example_1.0.0_armhf.ipk" into PFC's file system and call "opkg install <pkg-name>.ipk"
- utilize Web-Based-Management(WBM) feature "Software-Upload".

----------------------------------------------------------------------------------------------------------------------
# Using SSH for upload and installing OPKG package

1. On development host
```
    $cd ~/wago/ptxroj/platform-wago-pfcXXX/packages/
    $scp wretain-example_1.0.0_armhf.ipk root@<IP-of-PFC>:/root
    root@<IP-of-PFC>'s password:  # Enter the root password (default: wago)
    wretain-example_1.0.0_armhf.ipk               100%   23KB  23.3KB/s   00:00
```

2. On PFC Connect to PFC via SSH
```
    $ssh root@<IP-of-PFC>
    root@<IP-of-PFC>'s password: #Enter the root password (default: wago)

    WAGO Linux Terminal on PFC200V3-AABBCC

    root@PFC200V3-AABBCC:
```
You are now connected to the PFC.
Install kbusmodbusslave:
```
    $ opkg install wretain-example_1.0.0_armhf.ipk
    Installing wretain-example (1.0.0) on root.
    Configuring wretain-example.
```
Contratulations wretain-example is now installed on the PFC.
Test it on PFC:
```

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
    Select package to install or update, here "wretain-example_1.0.0_armhf.ipk".
```

4. Click on button [Start Upload].

    Transfer selected file into PFC file system and press button [Submit].

5. In newly shown section "Activate new software", press button [Submit] to install the package.
```
    Internally WBM just calls:
        >cd /home/
        >opkg install wretain-example_1.0.0_armhf.ipk
```
# Test wretain_example

1. Open ssh session to PFC
```
    $ssh root@<IP-of-PFC>
    root@<IP-of-PFC>'s password: #Enter the root password (default: wago)

    WAGO Linux Terminal on PFC200V3-AABBCC

    root@PFC200V3-AABBCC:
```

2. Execute C-Test or CPP-Test
```
    $/usr/sbin/wretain_example_c
```
or
```
    $/usr/sbin/wretain_example_cpp
```
It should give the following output:
```
Memory address: 0xb69b7000, memory size: 131072
NVRAM data:
        bool: 0
        byte: -14
        short: -5090
        unsigned short: 476
        int: 5000
        unsigned int: 50000
        float: 6.172500
        string: Value: false.
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
