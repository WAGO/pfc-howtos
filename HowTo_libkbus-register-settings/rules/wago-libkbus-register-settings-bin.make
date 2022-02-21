# -*-makefile-*-

#
# We provide this package
#
PACKAGES-$(PTXCONF_LIBKBUS_REGISTER_SETTINGS) += libkbus-register-settings

#
# Paths and names
#
LIBKBUS_REGISTER_SETTINGS_VERSION	:= 0.1.0
LIBKBUS_REGISTER_SETTINGS			:= libkbus-register-settings
LIBKBUS_REGISTER_SETTINGS_URL		:= file://$(SRCDIR)/libkbus-register-settings-bin
LIBKBUS_REGISTER_SETTINGS_DIR		:= $(BUILDDIR)/$(LIBKBUS_REGISTER_SETTINGS)
LIBKBUS_REGISTER_SETTINGS_SOURCE	:= $(SRCDIR)/$(LIBKBUS_REGISTER_SETTINGS)-bin

# we use a plain makefile
LIBKBUS_REGISTER_SETTINGS_CONF_TOOL := NO

LIBRARY_CONF := 
ifdef PTXCONF_LIBKBUS_REGISTER_SETTINGS_SHARED
LIBRARY_CONF += build_shared=y
else
LIBRARY_CONF += build_shared=n
endif
ifdef PTXCONF_LIBKBUS_REGISTER_SETTINGS_STATIC
LIBRARY_CONF += build_static=y
else
LIBRARY_CONF += build_static=n
endif
ifdef PTXCONF_LIBKBUS_REGISTER_SETTINGS_DEBUG
LIBRARY_CONF += DEBUG=y
else
LIBRARY_CONF += DEBUG=n
endif


# explicitly move CROSS_ENV into MAKE_ENV, because ptxdist just doesn't do it ¯\_(ツ)_/¯
LIBKBUS_REGISTER_SETTINGS_MAKE_ENV := $(CROSS_ENV) 

# our own variables for the build:
LIBKBUS_REGISTER_SETTINGS_MAKE_ENV += LIBKBUS_REGISTER_SETTINGS_VERSION=$(LIBKBUS_REGISTER_SETTINGS_VERSION) $(LIBRARY_CONF)

# Get
# ----------------------------------------------------------------------------

$(STATEDIR)/libkbus-register-settings.get:
	@$(call targetinfo, $@)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

$(STATEDIR)/libkbus-register-settings.extract:
	@$(call targetinfo, $@)
	@$(call clean, $(KBUS_REGISTER_SETTINGS_DIR))
	cp -rd $(LIBKBUS_REGISTER_SETTINGS_SOURCE) $(LIBKBUS_REGISTER_SETTINGS_DIR)
	@$(call touch, $@)


# use ptxdist defaults for prepare, compile and install

$(STATEDIR)/libkbus-register-settings.targetinstall:
	@$(call targetinfo, $@)
	@$(call install_init, libkbus-register-settings)
	@$(call install_fixup,libkbus-register-settings,PRIORITY,optional)
	@$(call install_fixup,libkbus-register-settings,VERSION,$(LIBKBUS_REGISTER_SETTINGS_VERSION))	
	@$(call install_fixup,libkbus-register-settings,SECTION,base)
	@$(call install_fixup,libkbus-register-settings,AUTHOR,"cm")
ifdef PTXCONF_LIBKBUS_REGISTER_SETTINGS_SHARED
	@$(call install_lib, libkbus-register-settings, 0, 0, 0755, libkbus-register-settings)
endif
	@$(call install_finish,libkbus-register-settings)
	@$(call touch, $@)

# vim: syntax=make
