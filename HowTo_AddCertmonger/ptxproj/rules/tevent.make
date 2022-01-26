# -*-makefile-*-
#
# Copyright (C) 2022 by WAGO GmbH (Thomas.Brandt@wago.com)
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_TEVENT) += tevent

#
# Paths and names
#
TEVENT_VERSION		:= 0.9.8
TEVENT_MD5		:= edc29f20fb508b6b860d70f0f07ccd3f
TEVENT			:= tevent-$(TEVENT_VERSION)
TEVENT_SUFFIX		:= tar.gz
TEVENT_URL		:= https://www.samba.org/ftp/tevent/$(TEVENT).$(TEVENT_SUFFIX)
TEVENT_SOURCE		:= $(SRCDIR)/$(TEVENT).$(TEVENT_SUFFIX)
TEVENT_DIR		:= $(BUILDDIR)/$(TEVENT)
TEVENT_LICENSE		:= GPL-3.0-or-later AND LGPL-3.0-or-later
# cross-compile runtime checks. Initial file generated with
# --cross-execute=$(PTXDIST_SYSROOT_CROSS)/bin/qemu-cross
TEVENT_CONFIG		 = $(call ptx/get-alternative, config/tevent, cross-answers)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

TEVENT_CONF_TOOL	:= autoconf
TEVENT_CONF_OPT	:=	$(CROSS_AUTOCONF_USR) \
	PKG_CONFIG_PATH=$(PTXDIST_SYSROOT_TARGET)/usr/lib/pkgconfig

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/tevent.targetinstall:
	@$(call targetinfo)

	@$(call install_init, tevent)
	@$(call install_fixup, tevent,PRIORITY,optional)
	@$(call install_fixup, tevent,SECTION,base)
	@$(call install_fixup, tevent,AUTHOR,"Thomas Brandt <thomas.brandt@wago.com>")
	@$(call install_fixup, tevent,DESCRIPTION,missing)

	@$(call install_lib, tevent, 0, 0, 0644, libtevent)

	@$(call install_finish, tevent)

	@$(call touch)

# vim: syntax=make
