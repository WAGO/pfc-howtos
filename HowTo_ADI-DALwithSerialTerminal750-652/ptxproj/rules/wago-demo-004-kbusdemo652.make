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
PACKAGES-$(PTXCONF_KBUSDEMO652) +=kbusdemo652

#
# Paths and names
#
KBUSDEMO652_VERSION	:= 0.1.6
KBUSDEMO652			   := kbusdemo652
KBUSDEMO652_URL		:= file://$(SRCDIR)/src/kbusdemo652
KBUSDEMO652_DIR		:= $(BUILDDIR)/$(KBUSDEMO652)
KBUSDEMO652_SOURCE	:= $(SRCDIR)/$(KBUSDEMO652)

CDUP = ..

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

$(STATEDIR)/kbusdemo652.get:
	@$(call targetinfo, $@)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

$(STATEDIR)/kbusdemo652.extract:
	@$(call targetinfo, $@)
	@$(call clean, $(KBUSDEMO652_DIR))
	cp -rd $(KBUSDEMO652_SOURCE) $(KBUSDEMO652_DIR)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

KBUSDEMO652_PATH	:= PATH=$(CROSS_PATH)
KBUSDEMO652_ENV 	:= $(CROSS_ENV)

$(STATEDIR)/kbusdemo652.prepare:
	@$(call targetinfo, $@)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

$(STATEDIR)/kbusdemo652.compile:
	@$(call targetinfo, $@)
	@cd $(KBUSDEMO652_DIR) && \
		$(KBUSDEMO652_ENV) $(KBUSDEMO652_PATH) DIST_DIR=$(PTXDIST_PLATFORMDIR) \
		env \
		DIST_DIR=$(PTXDIST_PLATFORMDIR) CROSS_COMPILE=$(COMPILER_PREFIX) \
		$(MAKE)			
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

$(STATEDIR)/kbusdemo652.install:
	@$(call targetinfo, $@)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/kbusdemo652.targetinstall:
	@$(call targetinfo, $@)
	@$(call install_init, kbusdemo652)
	@$(call install_fixup,kbusdemo652,PRIORITY,optional)
	@$(call install_fixup,kbusdemo652,VERSION,$(KBUSDEMO652_VERSION))	
	@$(call install_fixup,kbusdemo652,SECTION,base)
	@$(call install_fixup,kbusdemo652,AUTHOR,"cm")
	@$(call install_copy, kbusdemo652, 0, 0, 0755, $(KBUSDEMO652_DIR)/kbusdemo652, /usr/bin/kbusdemo652)
	@$(call install_finish,kbusdemo652)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

kbusdemo652_clean:
	rm -rf $(STATEDIR)/kbusdemo652.*
	
# vim: syntax=make
