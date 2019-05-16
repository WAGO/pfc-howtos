##----------------------------------------------------------------------------------------------------------------------
### Copyright (c) WAGO Kontakttechnik GmbH & Co. KG
###
### PROPRIETARY RIGHTS of WAGO Kontakttechnik GmbH & Co. KG are involved in
### the subject matter of this material. All manufacturing, reproduction,
### use, and sales rights pertaining to this subject matter are governed
### by the license agreement. The recipient of this software implicitly
### accepts the terms of the license.
###
##----------------------------------------------------------------------------------------------------------------------
##----------------------------------------------------------------------------------------------------------------------
###
###  \file     Makefile
###
###  \version  $Revision: 6613 $
###
###  \brief    Makefile for a set of "C" examples application 
###            Using KBUS interface 
###
###  \authors  cm : WAGO Kontakttechnik GmbH & Co. KG
###            ow : FH Bielefeld, 30.03.2015,  add getkbusheader 
##----------------------------------------------------------------------------------------------------------------------

#
# add more flags here, but don't remove the "+="
#
CFLAGS += -O0 -g3  -Wall
#CFLAGS += -O2 -g   -Wsign-compare -Wfloat-equal -Wformat-security #-Werror

CFLAGS += -I$(SYSROOT)/usr/include/OsLinux/
CFLAGS += -I$(SYSROOT)/usr/include/dal/
#------------------------------------------------------------------------------
# Linker flags
#------------------------------------------------------------------------------
LDFLAGS += -ldal -llibloader -lpthread -lffi -lrt -ldbus-glib-1 -lglib-2.0 
LDFLAGS += -ltypelabel -loslinux -ldbuskbuscommon


all: kbusdemo getkbusinfo

kbusdemo: kbusdemo.o
	$(CC) -o kbusdemo kbusdemo.o $(LDFLAGS)

getkbusinfo: getkbusinfo.o
	$(CC) -o getkbusinfo getkbusinfo.o  $(LDFLAGS)

 
#$(EXTRALIBS)

clean:
	-rm -f kbusdemo
	-rm -f getkbusinfo

install:


.PHONY: all install clean

# end of development makefile

