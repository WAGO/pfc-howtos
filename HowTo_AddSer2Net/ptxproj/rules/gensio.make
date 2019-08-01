# -*-makefile-*-
#
# Copyright (C) 2019 by <wago.com>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_GENSIO) += gensio

#
# Paths and names
#
GENSIO	       := gensio
GENSIO_VERSION := 1.1
GENSIO_SOURCE  := $(SRCDIR)/$(GENSIO)-$(GENSIO_VERSION).tar.gz
GENSIO_DIR     := $(BUILDDIR)/$(GENSIO)-$(GENSIO_VERSION)
GENSIO_LICENSE := GPLv2

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------
$(STATEDIR)/gensio.get:
	@$(call targetinfo, $@)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------
#GENSIO_PATH	:= PATH=$(CROSS_PATH)
#GENSIO_ENV 	:= $(CROSS_ENV)

GENSIO_CONF_OPT := $(CROSS_AUTOCONF_USR)
GENSIO_CONF_OPT += --with-python=no

#$(STATEDIR)/gensio.prepare:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------
#$(STATEDIR)/gensio.compile:
#	@$(call targetinfo)
#	@cd $(GENSIO_DIR) && \
#		$(GENSIO_ENV) $(GENSIO_PATH) \
#		env \
#		CROSS_COMPILE=$(COMPILER_PREFIX) \
#		$(MAKE)
#	@$(call touch)
	
# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------
#$(STATEDIR)/gensio.install:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/gensio.targetinstall:
	@$(call targetinfo)

	@$(call install_init,  gensio)
	@$(call install_fixup, gensio,PRIORITY,optional)
	@$(call install_fixup, gensio,VERSION,$(GENSIO_VERSION))
	@$(call install_fixup, gensio,SECTION,base)
	@$(call install_fixup, gensio,AUTHOR,"Thomas.Brandt@wago.com")
	@$(call install_fixup, gensio,DESCRIPTION,missing)

	@$(call install_lib,   gensio, 0, 0, 0644, libgensio)

	
	@$(call install_finish, gensio)
	@$(call touch)
# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

$(STATEDIR)/gensio.clean:
	@$(call targetinfo)
#	rm -rf $(STATEDIR)/kbusmodbusslave.*
#	rm -rf $(PKGDIR)/kbusmodbusslave_*
#	rm -rf $(KBUSMODBUSSLAVE_DIR)
	@$(call clean_pkg, GENSIO)

# vim: syntax=make
 
