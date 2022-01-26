# -*-makefile-*-
#
# Copyright (C) 2021 by Michael Olbrich <m.olbrich@pengutronix.de>
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_TALLOC) += talloc

#
# Paths and names
#
TALLOC_VERSION		:= 2.3.2
TALLOC_MD5		:= 3376a86bdf9dd4abc6b8d8d645390902
TALLOC			:= talloc-$(TALLOC_VERSION)
TALLOC_SUFFIX		:= tar.gz
TALLOC_URL		:= https://www.samba.org/ftp/talloc/$(TALLOC).$(TALLOC_SUFFIX)
TALLOC_SOURCE		:= $(SRCDIR)/$(TALLOC).$(TALLOC_SUFFIX)
TALLOC_DIR		:= $(BUILDDIR)/$(TALLOC)
TALLOC_LICENSE		:= GPL-3.0-or-later AND LGPL-3.0-or-later
# cross-compile runtime checks. Initial file generated with
# --cross-execute=$(PTXDIST_SYSROOT_CROSS)/bin/qemu-cross
TALLOC_CONFIG		 = $(call ptx/get-alternative, config/talloc, cross-answers)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

TALLOC_CONF_TOOL	:= NO
TALLOC_CONF_OPT	:=  \
	--without-gettext \
	--disable-python \
	--prefix=/usr \
	--sysconfdir=/etc \
	--localstatedir=/var \
	--libdir=/usr/lib \
	--bundled-libraries=NONE \
	--disable-rpath \
	--disable-rpath-install \
	--disable-rpath-private-install \
	--pidl-developer \
	--disable-warnings-as-errors \
	--fatal-errors \
	--enable-gccdeps \
	--pedantic \
	--cross-compile \
	--cross-execute=/does/not/exist/and/triggers/exceptions \
	--cross-answers=$(TALLOC_DIR)/cross-answers


$(STATEDIR)/talloc.prepare:
	@$(call targetinfo)
	@UNAME_M=$(PTXCONF_ARCH_STRING) \
		UNAME_R=$(KERNEL_VERSION) \
		UNAME_V=$(if $(KERNEL_HEADER_VERSION),$(KERNEL_HEADER_VERSION),$(KERNEL_VERSION)) \
		HAS_64BIT=$(call ptx/ifdef,PTXCONF_ARCH_LP64,OK,NO) \
		ptxd_replace_magic $(TALLOC_CONFIG) > $(TALLOC_DIR)/cross-answers
	@echo -----------------------------------------
	@echo $(HOST_SYSTEM_PYTHON)
	@echo -----------------------------------------
	@$(call world/execute, TALLOC, $(SYSTEMPYTHON3) ./buildtools/bin/waf configure $(TALLOC_CONF_OPT))
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/talloc.targetinstall:
	@$(call targetinfo)

	@$(call install_init, talloc)
	@$(call install_fixup, talloc,PRIORITY,optional)
	@$(call install_fixup, talloc,SECTION,base)
	@$(call install_fixup, talloc,AUTHOR,"Michael Olbrich <m.olbrich@pengutronix.de>")
	@$(call install_fixup, talloc,DESCRIPTION,missing)

	@$(call install_lib, talloc, 0, 0, 0644, libtalloc)

	@$(call install_finish, talloc)

	@$(call touch)

# vim: syntax=make
