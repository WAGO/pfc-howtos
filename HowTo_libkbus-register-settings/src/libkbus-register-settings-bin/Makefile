ifdef LIBKBUS_REGISTER_SETTINGS_VERSION
VERSION := $(LIBKBUS_REGISTER_SETTINGS_VERSION)
else 
$(error LIBKBUS_REGISTER_SETTINGS_VERSION not defined)
endif
MAJOR_VERSION := $(firstword $(subst ., ,$(VERSION)))

STATIC_LIB = libkbus-register-settings.a
DYN_LIB = libkbus-register-settings.so

PUBLIC_API = kbus_register_settings.h kbus_register_settings_common.h

#------------------------------------------------------------------------------
# Rules
#------------------------------------------------------------------------------

ifndef build_static
build_static := n
endif
ifndef build_shared
build_shared := y
endif

ifeq ($(build_static),y)
all: $(STATIC_LIB) 
install: $(STATIC_LIB) 
endif
ifeq ($(build_shared),y)
all: $(DYN_LIB).$(VERSION)
install: $(DYN_LIB).$(VERSION)
endif

# no need to build anything here, since it's already compiled

install: 
	cp $(PUBLIC_API) $(DESTDIR)/usr/include/
ifeq ($(build_static),y)
	cp $(STATIC_LIB) $(DESTDIR)/usr/lib/ 
endif
ifeq ($(build_shared),y)
	cp $(DYN_LIB).$(VERSION) $(DESTDIR)/usr/lib/
	ln -s $(DESTDIR)/usr/lib/$(DYN_LIB).$(VERSION) $(DESTDIR)/usr/lib/$(DYN_LIB)
	ln -s $(DESTDIR)/usr/lib/$(DYN_LIB).$(VERSION) $(DESTDIR)/usr/lib/$(DYN_LIB).$(MAJOR_VERSION)
endif

clean:
	@rm -f $(DYN_LIB)* $(STATIC_LIB) 
