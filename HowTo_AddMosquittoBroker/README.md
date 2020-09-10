# Adding Mosquitto Broker to the PFC or Touch Panel

## Every package is bundled to a specific firmware version: For Version 12.1- FW:03.00.39(99)

This HowTo shows how to build and install the Mosquitto Broker.

# PREREQUISITES
This HowTo is / based on a clean installation of Ubuntu LTS, with an installed and working WAGO Board-Support Package for PFC200. Working means that you successfully built the standard image “sd.hdimg”.

# Build package "mosquitto" on development host:
1. Copy attached rule file for "mosquitto.ipk" to given folder:
```
$cp ./ptxproj/rules/mosquitto.in    ~/wago/ptxproj-2.5.23/rules/
$cp ./ptxproj/rules/mosquitto.make  ~/wago/ptxproj-2.5.23/rules/

```

2. Select "mosquitto" package for build
```
$cd ~/wago/ptxproj-2.5.23/
$ptxdist menuconfig
      Networking Tools                   ---> [ENTER]
      [*]mosquitto                       ---> [ENTER]
      [*]     Broker
      [*]     Clients
      [*]     TLS support

      <Exit>, <Exit>, <Exit> and <Yes>
```

3. Clean the package
```
$cd ~/wago/ptxproj-2.5.23
$ptxdist clean mosquitto
```

4. Build package or complete image
    1. Build package only
    ```
        $ptxdist targetinstall mosquitto
    ```
    Afterwards you should find the IPKG installer package file:
    ```
        ~/wago/ptxproj-2.5.23/platform-wago-pfcXXX/packages/mosquitto_1.4.14_armhf.ipk
    ```

   2. Build complete firmware image "sd.hdimg"  (optional)
   ```
   $cd ~/wago/ptxproj-2.5.23/
   $ptxdist clean
   $ptxdist go -q
   $ptxdist images
   ```
   Afterwards you should find the firmware image "sd.hdimg"
   ```
        ~/wago/ptxproj-2.5.23/platform-wago-pfcXXX/images/sd.hdimg          #The firmware image
   ```

As usual, you can:
- copy image file "sd.hdimg" with command "dd" to SD-Card and boot PFC200 from it.
- transfer package "mosquittoBroker_1.4.14_armhf.ipk" into PFC's file system and call "ipkg install <pkg-name>.ipk"
- utilize Web-Based-Management(WBM) feature "Software-Upload".

<H1>Important to run Broker on PFC!

  Create your own IPK file.</H1>
  Unpack generated IPK and compress to a new Version.
  ```
        $ar -x mosquitto.ipk
        $tar -xvzf control.tar.gz
        $tar -xvzf data.tar.gz
        $tar -cvzf control.tar.gz ./control ./postinst
        $tar -cvzf data.tar.gz ./usr
        $ar -q mosquittoBroker_1.4.14_armhf.ipk debian.binary control.tar.gz data.tar.gz
  ```
----------------------------------------------------------------------------------------------------------------------
Using Web-Based-Management(WBM) feature "Software-Upload" for upload and installing OPKG packages

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
    Select package to install or update, here "mosquittoBroker_1.4.14_armhf.ipk".
```

4. Click on button [Start Upload].

    Transfer selected file into PFC file system and press button [Submit].

5. In newly shown section "Activate new software", press button [Submit] to install the package.
```
    Internally WBM just calls:
        >cd /home/
        >opkg install mosquittoBroker_1.4.14_armhf.ipk
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
