# Executabels name
EXE = register-access-65x-demo
# Source files
SRC = register-access-65x-demo.c

# Debug (compile with debugging information and without optimization)
ifndef DEBUG
DEBUG = y
endif

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
CFLAGS += -I$(SYSROOT)/usr/include 
CFLAGS += -I$(SYSROOT)/usr/include/glib-2.0
CFLAGS += -I$(SYSROOT)/usr/lib/glib-2.0/include

ifndef force_link_static
force_link_static := n
endif

#------------------------------------------------------------------------------
# Linker flags
#------------------------------------------------------------------------------
LDFLAGS += -ldal -lglib-2.0 -lpthread -lgthread-2.0 -lffi -llibloader -ldl 
# usually, the linker prefers shared libaries. 
# E. g., if both shared and static version are available, the linker automatically
# links against the shared libary.
# Because that may not be wanted, force the linker to use the static library with the syntax below
ifeq ($(force_link_static),y)
LDFLAGS += -l:libkbus-register-settings.a
else
LDFLAGS += -lkbus-register-settings
endif
# Linker search directories
EXTRALIBS += $(SYSROOT)/usr/lib/libdal.a 

#------------------------------------------------------------------------------
# Generate a object list from the source files
#------------------------------------------------------------------------------
OBJ = $(SRC:.c=.o)

#------------------------------------------------------------------------------
# Rules
#------------------------------------------------------------------------------

all: $(EXE)

install:
	cp $(EXE) $(DESTDIR)/usr/bin/

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) $(EXTRALIBS) -o $@

clean:
	@rm -f $(EXE) *.o 
