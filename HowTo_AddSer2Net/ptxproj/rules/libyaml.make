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
PACKAGES-$(PTXCONF_LIBYAML) += libyaml

#
# Paths and names
#
LIBYAML	       := libyaml-dist
LIBYAML_VERSION := 0.2.2
LIBYAML_SOURCE  := $(SRCDIR)/$(LIBYAML)-$(LIBYAML_VERSION).tar.gz
LIBYAML_DIR     := $(BUILDDIR)/$(LIBYAML)-$(LIBYAML_VERSION)
LIBYAML_LICENSE := MIT

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------
$(STATEDIR)/libyaml.get:
	@$(call targetinfo, $@)
	@$(call touch, $@)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------
#LIBYAML_PATH	:= PATH=$(CROSS_PATH)
#LIBYAML_ENV 	:= $(CROSS_ENV)

LIBYAML_CONF_TOOL := autoconf
LIBYAML_CONF_OPT := $(CROSS_AUTOCONF_USR)

#$(STATEDIR)/libyaml.prepare:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------
#$(STATEDIR)/libyaml.compile:
#	@$(call targetinfo)
#	@cd $(LIBYAML_DIR) && \
#		$(LIBYAML_ENV) $(LIBYAML_PATH) \
#		env \
#		CROSS_COMPILE=$(COMPILER_PREFIX) \
#		$(MAKE)
#	@$(call touch)
	
# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------
#$(STATEDIR)/libyaml.install:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/libyaml.targetinstall:
	@$(call targetinfo)

	@$(call install_init,  libyaml)
	@$(call install_fixup, libyaml,PRIORITY,optional)
	@$(call install_fixup, libyaml,VERSION,$(LIBYAML_VERSION))
	@$(call install_fixup, libyaml,SECTION,base)
	@$(call install_fixup, libyaml,AUTHOR,"Thomas.Brandt@wago.com")
	@$(call install_fixup, libyaml,DESCRIPTION,missing)

	@$(call install_lib,   libyaml, 0, 0, 0644, libyaml-0)

	
	@$(call install_finish, libyaml)
	@$(call touch)
# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

$(STATEDIR)/libyaml.clean:
	@$(call targetinfo)
	@$(call clean_pkg, LIBYAML)

# vim: syntax=make
 
