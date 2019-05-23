# Add SQLite Command-Line-Tool "sqlite3"

## Every package is bundled to a specific firmware version: For Version 2019.3.1 - FW:03.00.35(99)

This HowTo describes the procedure of compiling the sqlite package which also install the command-Line-Tool "sqlite3".

# PREREQUISITES
This HowTo is  based on a clean installation of Ubuntu Desktop 16.04 AMD64,
with an installed and working WAGO Board-Support Package for PFC200.
Working means that you successfully build the standard image “sd.hdimg”.
The “HowTo install WAGO-PFC-BSP-2019.3.1” on Ubuntu16 LTS is shipped with BSP.

# 1) On development host (Ubuntu)

## 1.1) Select additional option "install sqlite" to build package "sqlite" with CLI tool
```
 >cd ~/wago/ptxproj-3.0.35/
 >ptxdist menuconfig

  System Libraries   --->
      -*- sqlite     --->

    --- sqlite
    ---   build options
    [*]   Safe for use within a multi-threaded program
    [ ]   external extensions
    [ ]   use readline
    [ ]   set synchronous WAL mode to NORMAL
    [*]   disable tracking memory usage
    [*]   LIKE operator does not match BLOB operands
    [ ]   disable checking of expression parse-tree depth
    [ ]   omit deprecated interfaces and features
    [*]   disable shared cache
    [ ]   use alloca
    ---   install options
    [*]   install sqlite


 Select option "install sqlite" with [Space] and leave window with [Exit] [Exit], [Exit], and [Yes]
```

## 1.2) Clean the "sqlite" package
```
 >cd ~/wago/ptxproj-3.0.35/
 >ptxdist clean sqlite
```

## 1.3) Build the "mtconnect-agent" package
```
 >cd ~/wago/ptxproj-3.0.35/
 # Build package only.
 >ptxdist targetinstall sqlite

 Afterward you should find the OPKG installer package file:
 #The OPKG installer package
 ~/wago/ptxproj-3.0.35/platform-wago-pfc200/packages/sqlite_3220000_armhf.ipk

 If something else is reported: Get yourself a coffee and start doing research!
 Check http://www.ptxdist.org/software/ptxdist/appnotes/OSELAS.BSP-Pengutronix-Generic-arm-Quickstart.pdf
```

## 1.4) Build complete firmware image "sd.hdimg" which includes sqlite package
```
 >cd ~/wago/ptxproj-3.0.35/
 >ptxdist images
```

# 2) Update PFC Firmware
As usual, you can:
- copy image file "sd.hdimg" with command "dd" to SD-Card and boot PFC200 from it.
- transfer package <pkg-name>.ipk" somehow into PFC200 file system and call "opkg install <pkg-name>.ipk
- utilize Web-Based-Management(WBM) feature "Software-Upload" with option "force" to reinstall the package

Using command line tools for upload and installing IPK packages:

## 2.1) Use "scp" to transfer ipk file into pfc filesystem
```
 >cd ~/Downloads/HowTo_Add-SQLiteCommandLineTool/packages
 >scp sqlite_3220000_armhf.ipk root@192.168.1.17:/tmp/
```
Default password for user "root" is "wago"

## 2.2) Open a (ssh or serial) terminal session to PFC200
Login as "root" with password "wago" (default)

### 2.2.1) Install the package with option "--force-reinstall".
```
 >cd /tmp
 >opkg --force-reinstall install sqlite_3220000_armhf.ipk

 Reinstalling sqlite (3220000) on root.
 Installing sqlite (3220000) on root.
 Configuring sqlite.
```

### 2.2.2) Check installation of sqlite command line tool
```
 >sqlite3 --help

 Usage: sqlite3 [OPTIONS] FILENAME [SQL]
 FILENAME is the name of an SQLite database. A new database is created
 if the file does not previously exist.
 OPTIONS include:
    -ascii               set output mode to 'ascii'
    -bail                stop after hitting an error
    -batch               force batch I/O
    -column              set output mode to 'column'
    -cmd COMMAND         run "COMMAND" before reading stdin
    -csv                 set output mode to 'csv'
    -echo                print commands before execution
    -init FILENAME       read/process named file
    -[no]header          turn headers on or off
    -help                show this message
    -html                set output mode to HTML
    -interactive         force interactive I/O
    -line                set output mode to 'line'
    -list                set output mode to 'list'
    -lookaside SIZE N    use N entries of SZ bytes for lookaside memory
    -mmap N              default mmap size set to N
    -newline SEP         set output row separator. Default: '\n'
    -nullvalue TEXT      set text string for NULL values. Default ''
    -pagecache SIZE N    use N slots of SZ bytes each for page cache memory
    -quote               set output mode to 'quote'
    -separator SEP       set output column separator. Default: '|'
    -stats               print memory stats before each finalize
    -version             show SQLite version
    -vfs NAME            use NAME as the default VFS
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


