# -*-makefile-*-
#
# Copyright (C) 2017 by Georg Schöffl <georgschoeffl91@hotmail.com>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
HOST_PACKAGES-$(PTXCONF_HOST_MONO) += host-mono

#
# Paths and names
#
#http://download.mono-project.com/sources/mono/mono-6.4.0.198.tar.xz
HOST_MONO_VERSION   := 6.4.0.198
HOST_MONO_MD5       := e7936bca6838002daf5018bc382adb11
HOST_MONO		    := mono-$(HOST_MONO_VERSION)
HOST_MONO_SUFFIX	:= tar.xz
HOST_MONO_URL		:= http://download.mono-project.com/sources/mono/$(HOST_MONO).$(HOST_MONO_SUFFIX)
HOST_MONO_SOURCE	:= $(SRCDIR)/$(HOST_MONO).$(HOST_MONO_SUFFIX)
HOST_MONO_DIR		:= $(HOST_BUILDDIR)/$(HOST_MONO)
HOST_MONO_LICENSE	:= unknown

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

$(STATEDIR)/host-mono.extract:
	@$(call targetinfo)
	@$(call clean, $(HOST_MONO_DIR))
	@$(call extract, HOST_MONO)
	@$(call patchin, HOST_MONO)
	@$(call touch)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

HOST_MONO_CONF_ENV	:= \
	$(HOST_ENV) \
	CPPFLAGS="$(HOST_CPPFLAGS)"
#
# autoconf
#
HOST_MONO_CONF_TOOL	:= autoconf
HOST_MONO_CONF_OPT	:= \
	$(HOST_AUTOCONF_USR) \
	--with-sigaltstack=no \
	--disable-gtk-doc \
	--with-mcs-docs=no \
	--with-moonlight=no \
	--with-ikvm-native=no \
	--enable-minimal=profiler,debug
#	mono_cv_uscore=no \

# ----------------------------------------------------------------------------
# Host-Install
# ----------------------------------------------------------------------------
# The host build is needed by MONO. Installation see MONO package.

# vim: syntax=make
