# -*-makefile-*-

#
# We provide this package
#
PACKAGES-$(PTXCONF_REGISTER_ACCESS_65X_DEMO) += register-access-65x-demo

#
# Paths and names
#
REGISTER_ACCESS_65X_DEMO_VERSION	:= 0.1
REGISTER_ACCESS_65X_DEMO			:= register-access-65x-demo
REGISTER_ACCESS_65X_DEMO_URL		:= file://$(SRCDIR)/register-access-65x-demo
REGISTER_ACCESS_65X_DEMO_DIR		:= $(BUILDDIR)/$(REGISTER_ACCESS_65X_DEMO)
REGISTER_ACCESS_65X_DEMO_SOURCE	:= $(SRCDIR)/$(REGISTER_ACCESS_65X_DEMO)

# we use a plain makefile
REGISTER_ACCESS_65X_DEMO_CONF_TOOL := NO

EXE_CONF :=
ifdef PTXCONF_REGISTER_ACCESS_65X_DEMO_SHARED
EXE_CONF += force_link_static=n
endif
ifdef PTXCONF_REGISTER_ACCESS_65X_DEMO_STATIC
EXE_CONF += force_link_static=y
endif
ifdef PTXCONF_REGISTER_ACCESS_65X_DEMO_SHARED
ifdef PTXCONF_REGISTER_ACCESS_65X_DEMO_STATIC
$(error cannot link both static and dynamic (both PTXCONF_KBUS_SETTINGS_65X_SHARED and PTXCONF_KBUS_SETTINGS_65X_STATIC are defined))
endif
endif
ifdef PTXCONF_REGISTER_ACCESS_65X_DEMO_DEBUG
EXE_CONF += DEBUG=y
else
EXE_CONF += DEBUG=n
endif

# explicitly move CROSS_ENV into MAKE_ENV, because ptxdist just doesn't do it ¯\_(ツ)_/¯
REGISTER_ACCESS_65X_DEMO_MAKE_ENV := $(CROSS_ENV)

# our own variables for the build
REGISTER_ACCESS_65X_DEMO_MAKE_ENV += $(EXE_CONF)


# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

$(STATEDIR)/register-access-65x-demo.get:
	@$(call targetinfo, $@)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

$(STATEDIR)/register-access-65x-demo.extract:
	@$(call targetinfo, $@)
	@$(call clean, $(REGISTER_ACCESS_65X_DEMO_DIR))
	cp -rd $(REGISTER_ACCESS_65X_DEMO_SOURCE) $(REGISTER_ACCESS_65X_DEMO_DIR)
	@$(call touch, $@)

# we use ptxdist's default prepare, compile and install stages

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/register-access-65x-demo.targetinstall:
	@$(call targetinfo, $@)
	@$(call install_init, register-access-65x-demo)
	@$(call install_fixup,register-access-65x-demo,PRIORITY,optional)
	@$(call install_fixup,register-access-65x-demo,VERSION,$(REGISTER_ACCESS_65X_DEMO_VERSION))	
	@$(call install_fixup,register-access-65x-demo,SECTION,base)
	@$(call install_fixup,register-access-65x-demo,AUTHOR,"cm")
	@$(call install_copy,register-access-65x-demo, 0, 0, 0755, -, /usr/bin/register-access-65x-demo)
	@$(call install_finish,register-access-65x-demo)
	@$(call touch, $@)

# vim: syntax=make
