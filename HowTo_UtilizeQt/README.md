 # HowTo: ADI - KBUS application using Qt
 
 ## You can choose which Qt-Version you want to build for: This HowTo used Qt Version 5.13
 
 This HowTo describes how to build a Qt-Cross-Compile Environment so you can remotely run/debug
 your application on a PFC device.
 
 Find within 1 simple demo program using the default Qt qmake generator for C/C++ builds
 - kbusQtDemo : Has three different operation modes you can call by executing ./kbusQtDemo 1, 2 or 3
            1 : kbusdemo
            2 : getkbusinfo
            3 : getkbusHeader
            
# ATTENTION:
The ADI/DAL can only be used by one master, it can either be the CoDeSys-Runtime
or your own application. But not both.
Please make sure to stop the Runtime before running this demo.
- Temporally:
```
	>/etc/init.d/runtime stop
```
- Permanent:
```
	#//Delete symbolic link
	>rm /etc/rc.d/S98_runtime
	#Recover symbolic link
	>ln -s /etc/init.d/runtime /etc/rc.d/S98_runtime
```

# PREREQUISITES
This HowTo uses Qt-Creator to remotely deploy the application.
You need a working Cross-Compile Environment, see https://github.com/WAGO/pfc-firmware-sdk
Working means that you successfully followed the example to step 6.) and have the sysroot folders
within the platform folder(...)

# About KBUS:

The Kbus is working like a shifting register, means reading inputs and
writing outputs are capsulated within one Kbus-cycle.

A Kbus-cycle triggered by a call of device specific function "libpackbus_Push()".

Operating modes for driving the Kbus:
## 1) Operating mode “running”, a better name would be “manual”

In mode “running” Kbus have to be driven by your application.
Kbus data only read or written on calling function "libpackbus_Push()".

## Kbus operating mode "running":
- Highly deterministic(RealTimePrio)
- More program code needed.
- You have to think about Kbus-TimeOut of Output-IO-Terminals

## What the hell means “Kbus-TimeOut of Output-IO-Terminals”?
Every WAGO Output-IO-Terminals monitors elapsed time since last Kbus-cycle.
If elapsed time exceed 50ms, IO-Terminal shut down the outputs.

## And what is ADI/DAL's "WatchDog" functions good for?
Function „adi->WatchdogTrigger()“ only needed for PROFIBUS and CANopen
where fieldbus driven by a separate CHIP(DPC31) or software-stack.
“WatchdogTrigger()” is used to inform external fieldbus stack(CHIP)
about a living PLC program.
For KBUS, where entire fieldbus controlled by software, there is
no need to call “WatchdogTrigger()”.

# Step by Step Ubuntu:
----------------------------------------------------------------------------

## 0) Follow the steps in the Official Guide “https://github.com/WAGO/pfc-firmware-sdk” until you reach step 6.)

## 1.0) Get the latest(desired) Qt-Version 

   Either by downloading the Source directly from the Website:
   * [Qt-Everywhere](https://download.qt.io/archive/qt/5.13/5.13.0/single/) - Qt-Version 5.13.0
   
   OR by downloading the Qt-Version with git:
```
      git clone -b 5.13.0 git://code.qt.io/qt/qt5.git    
```
  Afterwards you need to call init_repository, optionally with the Modules you would like
  otherwise all modules will be downloaded
  -Also see * [Qt-Source](https://wiki.qt.io/Get_the_Source) 

## 1.2) Copy the folder "linux-arm-PFCXXXX-g++" into the downloaded Qt Folder: qtbase -> mkspecs -> devices

## 1.1) Copy the file "qtbase/mkspecs/devices/linux-rasp-pi2-g++/" into the Folder we just created "linux-arm-PFCXXXX-g++"
        
  Copy the File from "qtbase/mkspecs/devices/linux-rasp-pi2-g++/qplatformdefs.h
  into the "linux-arm-PFCXXXX-g++ Folder.
  --> It is just a simple include file but cannot be added to this Tutorial, due 
      to licensing.

## 1.3) Open a Terminal in the basefolder of the qt-source. You should see a file named configure in the folder.

  Now we need to export some Variables for the Configuration, you should adjust the Paths to fit your paths
  If you plan to copy the Sysroot folders from your VM to your host System via scp or rsync
  make sure to use -L to deference the Symbol Links

```
  export TOOLCHAIN=*PATH_TO_YOUR_TOOLCHAIN*
```
  For example the LINARO Toolchain used in the WAGO Tutorial
  → opt/wago/PFCXXXX/toolchain/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-

```
  export HOST_SYSROOT=*PATH_TO_HOST_SYSROOT*
```
  The host sysroot is the folder of your Toolchain Sysroot, if you don’t have one follow either:   
  - Download the precompiled sysroot from wago(“https://github.com/WAGO/gcc-linaro.toolchain-2017-precompiled”) 
  Build it yourself("https://github.com/WAGO/gcc-linaro.toolchain-2017").
  → opt/wago/PFCXXXX/toolchain/arm-linux-gnueabihf-sysroot

```
  export TARGET_SYSROOT=*PATH_TO_TARGET_SYSROOT*
```
  The Target sysroot is also the Folder we created in the Wago Tutorial
   You will find the Folder in ptxproj → platform-wago-pfcXXX → sysroot-target

```
  export PREFIX=*PATH_YOU_WANT_THE_QT_VERSION_TO_INSTALL_TO*
```
  The Path the newly build Qt-Version will install to.
  For example: /opt/Qt/5.13.0/wago_pfcxxx

```
  export EXTPREFIX=*PATH_YOU_WANT_THE_SYSROOT_TO_INSTALL_TO*
```
  By default the sysroot will be installed into the Toolchain folder. If you want the Sysroot
to install into a different folder you should set this Value, otherwise leave it out in the Configure.

## 1.4) Execute the command
```
./configure -device linux-arm-PFCXXXX-g++ -device-option CROSS_COMPILE=$TOOLCHAIN -device-option SYSROOT_TARGET=$TARGET_SYSROOT -sysroot $HOST_SYSROOT -prefix $PREFIX -extprefix $EXTPREFIX -skip qtandroidextras -skip qtcharts -skip qtwinextras -skip qtlocation -skip qtwebengine -skip qtwebview -no-opengl -D WAGO_PFC
```
  Note: For further options see “https://doc.qt.io/archives/qtextended4.4/buildsystem/over-configure-options-1.html”
  
   → Follow the Prompts you will receive and check for a “success” message.
   ▪ -device: The Device for which we will configure the build. Contains information about the Architecture and so on.

   ▪ skip … : Skips the Qt-Module so it will not be build, you can enter all the Qt-Modules you do not want to include into your build.

   ▪ No-opengl: The Wago-PFCXXX does not support opengl and it will not work either. So we need to tell Qt not to use it.

   ▪ -D Add an explicit define to the build, so we can later determine our system in make/qmake
              
   
## 1.5) Build and Install Qt
  After the configure, run make -jn, n is the amount of threads your PC has + 1

  Afterwards the Qt-Version will be build, this will take some time even on faster machines, expect it to take around 30-90 minutes.

  When the make is done, double check that no errors are listed in the last lines, if everything worked you are ready to install with: “make install” 
  → This will install the build into the Path provided by $PREFIX

## 2.0) Setting up the Qt-Enviroment

   Check out the PFD-File "Qt for WagoPFCXXXX.pfd" for a detailed description
   with a Picture Guide on HowTo do it.
   
   Summary:
   
   1.) Install Qt-Creator
   
   2.) Navigate to Tools -> Options -> Kits
   
     2.1) Navigate to Debugger and add the gdb Debugger within your Toolchain
     
     2.2) Navigate to Compiler and add the C/C++ Compiler within your Toolchain
     
     2.3) Navigate to Qt Versions and add the Qt-Version we just build in Step 1
     
     2.4) Navigate to Kits and create a new Kit using our Qt-Version, Compiler
          and Debugger.
          
   3.) Navigate to Devices, Add a new Generic Linux Device, follow the Instruction
   
     3.1) Navigate back to Kits and add the Generic Linux Device
     
     
## 3.0) Build the example

   Within the Qt-Creator load a project and navigate to the folder containing this Tutorial, open KBusQtDemo.pro.
   
   Follow the Project Settings within the Qt-Creator.
   
   If everything was setup correctly you should now be able to run the simple
   demo Program remotely and debug/build from your Developer Machine


# Compatibility list:
| PFC | Qt-Version | Operating Systems |
|:-------------|:-------------|:------------:|
| **PFC200** |  | |
| 8212 | 5.13.0 | Ubuntu 19.04, Windows 10(Build Toolchain yourself)|

