# -*-makefile-*-
#
# Copyright (C) 2021 by WAGO Kontakttechnik GmbH & Co. KG, author Thomas Brandt
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_LIBZMQ) += libzmq

#
# Paths and names
#
LIBZMQ_VERSION		:= 4.3.4
LIBZMQ_MD5		:= c897d4005a3f0b8276b00b7921412379
LIBZMQ			:= zeromq-$(LIBZMQ_VERSION)
LIBZMQ_SUFFIX		:= tar.gz
LIBZMQ_URL		:= https://github.com/zeromq/libzmq/releases/download/v$(LIBZMQ_VERSION)/$(LIBZMQ).$(LIBZMQ_SUFFIX)
LIBZMQ_SOURCE		:= $(SRCDIR)/$(LIBZMQ).$(LIBZMQ_SUFFIX)
LIBZMQ_DIR		:= $(BUILDDIR)/$(LIBZMQ)
LIBZMQ_LICENSE		:= LGPL-3.0+

#
# autoconf
#
LIBZMQ_CONF_TOOL	:= autoconf
LIBZMQ_CONF_OPT		:= \
	$(CROSS_AUTOCONF_USR) \
	--enable-shared \
	--disable-static \
	--with-gnu-ld \
	--without-documentation

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/libzmq.targetinstall:
	@$(call targetinfo)

	@$(call install_init, libzmq)
	@$(call install_fixup, libzmq,PRIORITY,optional)
	@$(call install_fixup, libzmq,SECTION,base)
	@$(call install_fixup, libzmq,AUTHOR,"thomas.brandt@wago.com")
	@$(call install_fixup, libzmq,DESCRIPTION,missing)

	@$(call install_lib, libzmq, 0, 0, 0644, libzmq)
	@$(call install_copy, libzmq, 0, 0, 0755, -, /usr/bin/curve_keygen)

	@$(call install_finish, libzmq)

	@$(call touch)

# vim: syntax=make
