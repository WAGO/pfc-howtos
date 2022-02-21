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
###  \file     makefile
###
###  \version  $Revision:  $
###
###  \brief    Makefile of the "C" Application
###
###  \author   cm : WAGO Kontakttechnik GmbH & Co. KG
##----------------------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Defines
#------------------------------------------------------------------------------
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld

# Executabels name
EXE = kbusdemo652
# Source files
SRC = kbusdemo652.c kbus652.c

# Debug (compile with debugging information and without optimization)
DEBUG = y

#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------
# Debug
ifeq ($(DEBUG),n)
CFLAGS += -O2
else
CFLAGS += -O0 -g3
endif
# warning flags
CFLAGS += -Wall -Wstrict-prototypes -fno-strict-aliasing
# use thread save versions
CFLAGS += -D_GNU_SOURCE -pthread
# Compiler includes
CFLAGS += -I$(DIST_DIR)/sysroot-target/usr/include 
CFLAGS += -I$(DIST_DIR)/sysroot-target/usr/include/glib-2.0
CFLAGS += -I$(DIST_DIR)/sysroot-target/usr/lib/glib-2.0/include

#------------------------------------------------------------------------------
# Linker flags
#------------------------------------------------------------------------------
LDFLAGS += -ldal -lglib-2.0 -lpthread -lgthread-2.0 -lffi -llibloader -ldl
# Linker search directories
EXTRALIBS += $(DIST_DIR)/sysroot-target/usr/lib/libdal.a

#------------------------------------------------------------------------------
# Generate a object list from the source files
#------------------------------------------------------------------------------
OBJ = $(SRC:.c=.o)

#------------------------------------------------------------------------------
# Rules
#------------------------------------------------------------------------------

all: $(OBJ) $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) $(EXTRALIBS) -o $@

%.c%.o: 
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	@rm -f $(EXE) *.o 
