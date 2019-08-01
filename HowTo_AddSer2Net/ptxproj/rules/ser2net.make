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
PACKAGES-$(PTXCONF_SER2NET) += ser2net

#
# Paths and names
#
SER2NET		:= ser2net
SER2NET_VERSION := 4.0
SER2NET_SOURCE	:= $(SRCDIR)/$(SER2NET)-$(SER2NET_VERSION).tar.gz
SER2NET_DIR     := $(BUILDDIR)/$(SER2NET)
SER2NET_LICENSE	:= GPLv2

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------
$(STATEDIR)/ser2net.get:
	@$(call targetinfo, $@)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------
SER2NET_PATH	:= PATH=$(CROSS_PATH)
SER2NET_ENV 	:= $(CROSS_ENV)

#$(STATEDIR)/ser2net.prepare:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------
$(STATEDIR)/ser2net.compile:
	@$(call targetinfo)
	@cd $(SER2NET_DIR) && \
		$(SER2NET_ENV) $(SER2NET_PATH) \
		env \
		CROSS_COMPILE=$(COMPILER_PREFIX) \
		$(MAKE)
	@$(call touch)
	
# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------
$(STATEDIR)/ser2net.install:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/ser2net.targetinstall:
	@$(call targetinfo)

	@$(call install_init,  ser2net)
	@$(call install_fixup, ser2net,PRIORITY,optional)
	@$(call install_fixup, ser2net,VERSION,$(SER2NET_VERSION))
	@$(call install_fixup, ser2net,SECTION,base)
	@$(call install_fixup, ser2net,AUTHOR,"Thomas.Brandt@wago.com")
	@$(call install_fixup, ser2net,DESCRIPTION,missing)
	@$(call install_copy,  ser2net, 0, 0, 0644, $(SER2NET_DIR)/ser2net.yaml, /etc/ser2net/ser2net.yaml)
	@$(call install_copy,  ser2net, 0, 0, 0755, $(SER2NET_DIR)/ser2net, /usr/sbin/ser2net)
	@$(call install_finish, ser2net)
	@$(call touch)
# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

$(STATEDIR)/ser2net.clean:
	@$(call targetinfo)
#	rm -rf $(STATEDIR)/kbusmodbusslave.*
#	rm -rf $(PKGDIR)/kbusmodbusslave_*
#	rm -rf $(KBUSMODBUSSLAVE_DIR)
	@$(call clean_pkg, NODE)

# vim: syntax=make
 
