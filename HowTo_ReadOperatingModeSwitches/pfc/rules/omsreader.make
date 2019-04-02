# -*-makefile-*-
#
# Copyright (C) 2018 by <ThomasBrandt@wago.com>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_OMSREADER) += omsreader

#
# Paths and names
#
OMSREADER_VERSION	:= 1.0
OMSREADER		:= omsreader
OMSREADER_URL		:= file://src/$(OMSREADER)
OMSREADER_DIR		:= $(BUILDDIR)/$(OMSREADER)
OMSREADER_LICENSE	:= MIT

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#OMSREADER_PATH	:= PATH=$(CROSS_PATH)
OMSREADER_CONF_TOOL	:= NO
OMSREADER_MAKE_ENV	:= $(CROSS_ENV)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/omsreader.targetinstall:
	@$(call targetinfo)

	@$(call install_init, omsreader)
	@$(call install_fixup, omsreader,PRIORITY,optional)
	@$(call install_fixup, omsreader,SECTION,base)
	@$(call install_fixup, omsreader,AUTHOR,"Thomas.Brandt@wago.com")
	@$(call install_fixup, omsreader,DESCRIPTION,missing)

	@$(call install_copy, omsreader, 0, 0, 0755, $(OMSREADER_DIR)/omsreader, /usr/bin/omsreader)

	@$(call install_finish, omsreader)

	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

$(STATEDIR)/omsreader.clean:
	@$(call targetinfo)
	@-cd $(OMSREADER_DIR) && \
		$(OMSREADER_ENV) $(OMSREADER_PATH) $(MAKE) clean
	@$(call clean_pkg, OMSREADER)

# vim: syntax=make
