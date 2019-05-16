# -*-makefile-*-
# $Id$
#
# Copyright (C) 2013 by 
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_KBUSDEMO) +=kbusdemo

#
# Paths and names
#
KBUSDEMO_VERSION	:= 3.0
KBUSDEMO			:= kbusdemo
KBUSDEMO_URL		:= file://$(SRCDIR)/src/kbusdemo
KBUSDEMO_DIR		:= $(BUILDDIR)/$(KBUSDEMO)
KBUSDEMO_SOURCE		:= $(SRCDIR)/$(KBUSDEMO)
KBUSDEMO_LICENSE	:= MIT

CDUP = ..

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

$(STATEDIR)/kbusdemo.get:
	@$(call targetinfo, $@)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

$(STATEDIR)/kbusdemo.extract:
	@$(call targetinfo, $@)
	@$(call clean, $(KBUSDEMO_DIR))
	cp -rd $(KBUSDEMO_SOURCE) $(KBUSDEMO_DIR)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

KBUSDEMO_PATH	:= PATH=$(CROSS_PATH)
KBUSDEMO_ENV 	:= $(CROSS_ENV)

$(STATEDIR)/kbusdemo.prepare:
	@$(call targetinfo, $@)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

$(STATEDIR)/kbusdemo.compile:
	@$(call targetinfo, $@)
	@cd $(KBUSDEMO_DIR) && \
		$(KBUSDEMO_ENV) $(KBUSDEMO_PATH) DIST_DIR=$(PTXDIST_PLATFORMDIR) \
		env \
		DIST_DIR=$(PTXDIST_PLATFORMDIR) CROSS_COMPILE=$(COMPILER_PREFIX) \
		$(MAKE)			
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

$(STATEDIR)/kbusdemo.install:
	@$(call targetinfo, $@)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/kbusdemo.targetinstall:
	@$(call targetinfo, $@)
	@$(call install_init, kbusdemo)
	@$(call install_fixup,kbusdemo,PRIORITY,optional)
	@$(call install_fixup,kbusdemo,VERSION,$(KBUSDEMO_VERSION))	
	@$(call install_fixup,kbusdemo,SECTION,base)
	@$(call install_fixup,kbusdemo,AUTHOR,"Thomas Brandt")
	@$(call install_copy, kbusdemo, 0, 0, 0755, $(KBUSDEMO_DIR)/kbusdemo, /usr/bin/kbusdemo)
	@$(call install_copy, kbusdemo, 0, 0, 0755, $(KBUSDEMO_DIR)/getkbusinfo, /usr/bin/getkbusinfo)
	@$(call install_finish,kbusdemo)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

kbusdemo_clean:
	rm -rf $(STATEDIR)/kbusdemo.*
	
# vim: syntax=make
