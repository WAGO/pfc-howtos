# Adding Mono to the PFC 

## Every package is bundled to a specific firmware version: For Version 14 - FW:03.02.02(99)

This HowTo shows how to build and install "Mono" for PFC's.
mono is a runtime implementation of the ECMA Common Language Infrastructure. 
It can be used to run ECMA and .NET applications.

# CAUTION
> The mono package is very large package. Without additional action it will not fit into the default file system!  See below and Check "HowTo_CustomizeImageLayout".

> ptxdist command "images" would not work!

# PREREQUISITES
This HowTo is  based on a clean installation of Ubuntu LTS, with an installed and working WAGO Software-Devolopment-Kit for PFC. Working means that you successfully built the standard image “sd.hdimg”.

> You need a internet connection to download the source files.

> You will need a firmware image to be able to use Mono on the pfc. [See section "Install mono package on PFC"]

# Build package - On development host

1. Copy attached rule file and sources for "Mono" to given folder:
```
$cp ptxdist/rules/mono.in    ~/wago/ptxproj/rules/
$cp ptxdist/rules/mono.make  ~/wago/ptxproj/rules/
$cp ptxdist/rules/host-mono.in    ~/wago/ptxproj/rules/
$cp ptxdist/rules/host-mono.make  ~/wago/ptxproj-2.6.20/rules/
```

2. Select "mono" for build
```
$cd ~wago/ptxproj/
$ptxdist menuconfig
        Bytecode Engines / VMs             ---> [ENTER]
                 <*>     mono
             <Exit>, <Exit> and <Yes>
```

3. Clean the package only
```
$cd ~/wago/ptxproj/
$ptxdist clean mono
```

4. Build package only
```
$ptxdist targetinstall mono
    ```

    Afterwards you should find the IPKG installer package file:
        ~/wago/ptxproj/platform-wago-pfcXXX/packages/mono_6.4.0.198_armhf.ipk

# Install mono package on PFC

## Provide enough disk space in file system
Without further action mono do not fit into default image.
WBM-Feature "Create-Image" provide an easy way out ...

1. Use the previously created `ptxdist image`

2. Write image to SD-Card minimum 4GB
* Use tool "dd"
  ```
  $sudo dd if=sd.hdimg of=/dev/sda
  ```
4. Resize the the root partion of sd card.
  On Host-PC we need the following tools `e2fsck` `resize2fs` and `parted`

  In case it is not allready installed on the host-pc install it:
  ```
  $sudo apt install e2fsprogs system-config-lvm parted
  ```
  After successfully installation execute the following commands.
  ```
  $sudo umount /dev/[sdcard-second partiton]
  $sudo parted /dev/[sd-card] resizepart 2 100%
  $sudo e2fsck -f /dev/[sdcard-second patition]
  $sudo resize2fs /dev/[sdcard-second partiton]
  ```
  for example
  ```
  $sudo umount /dev/sda2
  $sudo parted /dev/sda resizepart 2 100%
  $sudo e2fsck -f /dev/sda2
  $sudo resize2fs /dev/sda2
  ```
3. Boot PFC from (standard) SD-Card

4. Check available disk space on PFC
  ```
  $df -h
  Filesystem                Size      Used Available Use% Mounted on
  /dev/root                 3.6G    433.0M      3.0G  12% /
  devtmpfs                245.3M      4.0K    245.3M   0% /dev
  none                    245.8M      8.0K    245.8M   0% /tmp
  none                    245.8M         0    245.8M   0% /media
  none                      4.0M     96.0K      3.9M   2% /var/log
  none                    245.8M     72.0K    245.8M   0% /var/run
  none                    245.8M         0    245.8M   0% /var/lock
  none                    245.8M         0    245.8M   0% /var/tmp
  none                    245.8M         0    245.8M   0% /sys/fs/cgroup
  tmpfs                   245.8M    116.0K    245.7M   0% /run
  /dev/mmcblk0p1           16.0M      2.6M     13.4M  16% /boot/loader
  ```

5. Install mono into large SD-Card-Image

  1. Copy file "mono_6.4.0.198_armhf.ipk" from development host into PFC filesystem
    ```
    $cd ~/wago/ptxproj/platform-wago-pfcXXX/packages
    $scp mono_6.4.0.198_armhf.ipk root@<IP-PFC>:/tmp/
    Enter the root password.
    ```

  2. Open a (ssh or serial) terminal session to PFC
    Login as "root" with password "wago" (default)

  3. Call installer tool "opkg" from command line.
    >The WBM-Feature "Software-Upload" will run into a TIMEOUT error!

    ```
    $opkg install /tmp/mono_6.4.0.198_armhf.ipk
    ```
    >REMARK: !!! No panic the task require approx. 4 minutes !!! 

  4. Check installation 
  ```
    mono --help
    Usage is: mono [options] program [program-options]

    Development:
        --aot[=<options>]      Compiles the assembly to native code
        --debug[=<options>]    Enable debugging support, use --help-debug for details
        --debugger-agent=options Enable the debugger agent
        --profile[=profiler]   Runs in profiling mode with the specified profiler module
        --trace[=EXPR]         Enable tracing, use --help-trace for details
        --jitmap               Output a jit method map to /tmp/perf-PID.map
        --help-devel           Shows more options available to developers

     Runtime:
        --config FILE          Loads FILE as the Mono config
        --verbose, -v          Increases the verbosity level
        --help, -h             Show usage information
        --version, -V          Show version information
        --version=number       Show version number
        --runtime=VERSION      Use the VERSION runtime, instead of autodetecting
        --optimize=OPT         Turns on or off a specific optimization
                               Use --list-opt to get a list of optimizations
        --attach=OPTIONS       Pass OPTIONS to the attach agent in the runtime.
                               Currently the only supported option is 'disable'.
        --llvm, --nollvm       Controls whenever the runtime uses LLVM to compile code.
        --gc=[sgen,boehm]      Select SGen or Boehm GC (runs mono or mono-sgen)
        --handlers             Install custom handlers, use --help-handlers for details.
        --aot-path=PATH        List of additional directories to search for AOT images.
  ```
