# -*-makefile-*-
#
# Copyright (C) 2017 by <wago.com>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_KBUSMODBUSSLAVE) += kbusmodbusslave

#
# Paths and names
#
KBUSMODBUSSLAVE			:= kbusmodbusslave
KBUSMODBUSSLAVE_VERSION	:= 1.4.1
KBUSMODBUSSLAVE_SOURCE	:= $(SRCDIR)/$(KBUSMODBUSSLAVE)-$(KBUSMODBUSSLAVE_VERSION).tar.bz2
KBUSMODBUSSLAVE_DIR		:= $(BUILDDIR)/$(KBUSMODBUSSLAVE)
KBUSMODBUSSLAVE_LICENSE	:= MPL_2.0
KBUSMODBUSSLAVE_MD5		:= 62bd7b145313cd6419d7ee98711bc2fa

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------
$(STATEDIR)/kbusmodbusslave.get:
	@$(call targetinfo, $@)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------
KBUSMODBUSSLAVE_PATH	:= PATH=$(CROSS_PATH)
KBUSMODBUSSLAVE_ENV 	:= $(CROSS_ENV)

$(STATEDIR)/kbusmodbusslave.prepare:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------
$(STATEDIR)/kbusmodbusslave.compile:
	@$(call targetinfo)
	@cd $(KBUSMODBUSSLAVE_DIR) && \
		$(KBUSMODBUSSLAVE_ENV) $(KBUSMODBUSSLAVE_PATH) \
		env \
		CROSS_COMPILE=$(COMPILER_PREFIX) \
		$(MAKE)
	@$(call touch)
	
# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------
$(STATEDIR)/kbusmodbusslave.install:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/kbusmodbusslave.targetinstall:
	@$(call targetinfo)

	@$(call install_init,  kbusmodbusslave)
	@$(call install_fixup, kbusmodbusslave,PRIORITY,optional)
	@$(call install_fixup, kbusmodbusslave,VERSION,$(KBUSMODBUSSLAVE_VERSION))
	@$(call install_fixup, kbusmodbusslave,SECTION,base)
	@$(call install_fixup, kbusmodbusslave,AUTHOR,"BrT")
	@$(call install_fixup, kbusmodbusslave,DESCRIPTION,missing)
	@$(call install_copy,  kbusmodbusslave, 0, 0, 0755, $(KBUSMODBUSSLAVE_DIR)/$(KBUSMODBUSSLAVE), /usr/bin/$(KBUSMODBUSSLAVE))
	@$(call install_copy,  kbusmodbusslave, 0, 0, 0644, $(KBUSMODBUSSLAVE_DIR)/kbusmodbusslave.conf, /etc/kbusmodbusslave.conf)
	@$(call install_finish, kbusmodbusslave)
	@$(call touch)
# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

$(STATEDIR)/kbusmodbusslave.clean:
	@$(call targetinfo)
	rm -rf $(STATEDIR)/kbusmodbusslave.*
	rm -rf $(PKGDIR)/kbusmodbusslave_*
	rm -rf $(KBUSMODBUSSLAVE_DIR)
	@$(call clean_pkg, NODE)

# vim: syntax=make
 
