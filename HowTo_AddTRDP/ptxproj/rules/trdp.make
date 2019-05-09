# -*-makefile-*-
#
# Copyright (C) 2016 by <TCNopen>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_TRDP) += trdp

#
# Paths and names
#
TRDP		   := trdp
TRDP_VERSION	:= 1.4.1.0
#TRDP_URL	:= svn export https://svn.code.sf.net/p/tcnopen/trdp/tags/trdp/1.4.1.0
TRDP_MD5		:=
TRDP_URL		:= file://local_src/$(TRDP)-$(TRDP_VERSION)
TRDP_DIR		:= $(BUILDDIR)/$(TRDP)
TRDP_LICENSE	:= Mozilla Public License v2.0

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

$(STATEDIR)/trdp.extract:
	@$(call targetinfo)
	@$(call clean, $(TRDP_DIR))
	@$(call extract, TRDP)
	@$(call patchin, TRDP)
   # Generate suitable 'config.mk' WAGO PFC family
	cd $(TRDP_DIR) && make ./WAGO_PFC_config	
	@$(call touch)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#TRDP_PATH	:= PATH=$(CROSS_PATH)
TRDP_CONF_TOOL	:= NO
TRDP_MAKE_ENV	:= $(CROSS_ENV)

#$(STATEDIR)/trdp.prepare:
#	@$(call targetinfo)
#	@$(call world/prepare, TRDP)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

#$(STATEDIR)/trdp.compile:
#	@$(call targetinfo)
#	@$(call world/compile, TRDP)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

$(STATEDIR)/trdp.install:
	@$(call targetinfo)
#	@$(call world/install, TRDP)
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/trdp.targetinstall:
	@$(call targetinfo)

	@$(call install_init, trdp)
	@$(call install_fixup, trdp,PRIORITY,optional)
	@$(call install_fixup, trdp,SECTION,base)
	@$(call install_fixup, trdp,AUTHOR,"<TCNopen>")
	@$(call install_fixup, trdp,DESCRIPTION,missing)

#
# TODO: Add here all files that should be copied to the target
# Note: Add everything before(!) call to macro install_finish
#
#Examples
	@$(call install_copy, trdp, 0, 0, 0755, $(TRDP_DIR)/bld/output/linux-rel/echoCallback, /usr/bin/echoCallback)
	@$(call install_copy, trdp, 0, 0, 0755, $(TRDP_DIR)/bld/output/linux-rel/receivePolling, /usr/bin/receivePolling)
	@$(call install_copy, trdp, 0, 0, 0755, $(TRDP_DIR)/bld/output/linux-rel/sendHello, /usr/bin/sendHello)
	@$(call install_copy, trdp, 0, 0, 0755, $(TRDP_DIR)/bld/output/linux-rel/receiveHello, /usr/bin/receiveHello)
	@$(call install_copy, trdp, 0, 0, 0755, $(TRDP_DIR)/bld/output/linux-rel/sendData, /usr/bin/sendData)
	@$(call install_copy, trdp, 0, 0, 0755, $(TRDP_DIR)/bld/output/linux-rel/sourceFiltering, /usr/bin/sourceFiltering)
# Tests
	@$(call install_copy, trdp, 0, 0, 0755, $(TRDP_DIR)/bld/output/linux-rel/getStats, /usr/bin/getStats)

	@$(call install_finish, trdp)

	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

$(STATEDIR)/trdp.clean:
	@$(call targetinfo)
	@-cd $(TRDP_DIR) && \
		$(TRDP_ENV) $(TRDP_PATH) $(MAKE) clean
	@$(call clean_pkg, TRDP)

# vim: syntax=make
