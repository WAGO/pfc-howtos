#  Adding perf tool to the PFC

## Every package is bundled to a specific firmware version: For Version 2019.3.1 - FW:03.00.35(99)

This HowTo shows how to add the perf tool to the PFC

# PREREQUISITES
This HowTo is / based on a clean installation of Ubuntu LTS, with an installed 
and working WAGO Board-Support Package for PFC200.  
Working means that you successfully built the standard image “sd.hdimg”.


# Installation and usage

## 1) On development host (Ubuntu)
### 1.1) Copy rule files to into ptxdist-rulefiles
```
>cp -v ptxproj/rules/* ~/<ptxproj>/rules/
```

### 1.2) Select package "perf" for build in ptxdist
```
>cd ~/<ptxproj>/
>ptxdist menuconfig
```

```
   Core (libc, locales)            ---> [Enter]
     -*- kernel tools                    ---> [Enter]
       [*]   perf (Select perf with [Space]

   [Exit], [Exit], [Exit], and save configuration [Yes]
```

### 1.3) Clean "kernel" package
```
>cd ~/<ptxproj>/
>ptxdist clean kernel
```

### 1.4) Build the "kernel" package only
```
>cd ~/<ptxproj>/
ptxdist targetinstall kernel
```

If you see the "finished target kernel.targetinstall.post" at the end of the run,it is time for a :)

You will find three needed IPKG package files:  
- ~/<ptxproj>/platform-wago-pfcXXX/packages/kernel_4.9.115_armhf.ipk
- ~/<ptxproj>/platform-wago-pfcXXX/packages/slang_2.3.0_armhf.ipk
- ~/<ptxproj>/platform-wago-pfcXXX/packages/libelf_0.170_armhf.ipk

### 1.5) Build complete firmware image "sd.hdimg" which includes perf package (optional)
You can build a complete image if you like. If you like follow these commands:
```
>cd ~/<ptxproj>/
>ptxdist images
```
As usual, you can:
- copy image file "sd.hdimg" with command "dd" to SD-Card and boot PFC200 from it.
- transfer packags into PFC200 file system and call "opkg install <pkg-name>.ipk
- utilize Web-Based-Management(WBM) feature "Software-Upload"

---

## 2.1) Copy IPKs to the PFC via ssh-copy
```
>cd ~/<ptxproj>/platform-wago-pfcXXX/packages/
>scp kernel_4.9.115_armhf.ipk slang_2.3.0_armhf.ipk libelf_0.170_armhf.ipk root@<IP-PFC>:/root
```

## 2.2) Installing IPKG package by commandline
Open a terminal session (ssh or serial).  
Login as "root" with password "wago" (default)  
```
>cd /root
>opkg --force-reinstall install libelf_0.170_armhf.ipk
>opkg install slang_2.3.0_armhf.ipk
>opkg --force-reinstall install kernel_4.9.115_armhf.ipk
```
---
# Testing the installed application
## 3.1) Open a (ssh or serial) terminal session to PFC200
Login as "root" with password "wago" (default)
an execute _perf_
```
>perf

 usage: perf [--version] [--help] [OPTIONS] COMMAND [ARGS]

  The most commonly used perf commands are:
     annotate        Read perf.data (created by perf record) and display annotated code
     archive         Create archive with object files with build-ids found in perf.data file
     bench           General framework for benchmark suites
     buildid-cache   Manage build-id cache.
     buildid-list    List the buildids in a perf.data file
     config          Get and set variables in a configuration file.
     data            Data file related processing
     diff            Read perf.data files and display the differential profile
     evlist          List the event names in a perf.data file
     inject          Filter to augment the events stream with additional information
     kmem            Tool to trace/measure kernel memory properties
     kvm             Tool to trace/measure kvm guest os
     list            List all symbolic event types
     lock            Analyze lock events
     mem             Profile memory accesses
     record          Run a command and record its profile into perf.data
     report          Read perf.data (created by perf record) and display the profile
     sched           Tool to trace/measure scheduler properties (latencies)
     script          Read perf.data (created by perf record) and display trace output
     stat            Run a command and gather performance counter statistics
     test            Runs sanity tests.
     timechart       Tool to visualize total system behavior during a workload
     top             System profiling tool.
     probe           Define new dynamic tracepoints

 See 'perf help COMMAND' for more information on a specific command.
```

CPU counter statistics for the entire system, for 5 seconds:
```
>perf stat -a sleep 5

 Performance counter stats for 'system wide':

       5008,331120      cpu-clock (msec)          #    1,000 CPUs utilized
            11.601      context-switches          #    0,002 M/sec
                 0      cpu-migrations            #    0,000 K/sec
                67      page-faults               #    0,013 K/sec
     5.008.262.333      cycles                    #    1,000 GHz
        58.300.050      instructions              #    0,01  insn per cycle
        10.372.129      branches                  #    2,071 M/sec
         2.889.067      branch-misses             #   27,85% of all branches

      5,008560565 seconds time elapsed
```

CPU counter statistics for the specified command:
```
perf stat <command>
```

If your interested in a more detailed description see: http://www.brendangregg.com/perf.html

---
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

