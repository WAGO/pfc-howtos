# -*-makefile-*-
#
# Copyright (C) 2010 by Robert Schwebel <r.schwebel@pengutronix.de>
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
PACKAGES-$(PTXCONF_MONO) += mono

#
# Paths and names
#
MONO_VERSION    := 6.4.0.198
MONO_MD5        := e7936bca6838002daf5018bc382adb11
MONO		    := mono-$(MONO_VERSION)
MONO_SUFFIX	    := tar.xz
MONO_URL	    := http://download.mono-project.com/sources/mono/$(MONO).$(MONO_SUFFIX)
MONO_SOURCE	    := $(SRCDIR)/$(MONO).$(MONO_SUFFIX)
MONO_DIR	    := $(BUILDDIR)/$(MONO)
MONO_LICENSE	:= unknown

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

$(STATEDIR)/mono.extract:
	@$(call targetinfo)
	@$(call clean, $(MONO_DIR))
	@$(call extract, MONO)
	@$(call patchin, MONO)
	@$(call touch)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

MONO_CONF_ENV	:= \
	$(CROSS_ENV) \
	CPPFLAGS="$(CROSS_CPPFLAGS)" \
	mono_cv_uscore=yes

#
# autoconf
#
MONO_CONF_TOOL	:= autoconf
MONO_CONF_OPT	:= \
	$(CROSS_AUTOCONF_USR) \
#	--disable-gtk-doc \
	--with-mcs-docs=no \
#	--with-moonlight=no \
	--with-ikvm-native=no \
	--enable-minimal=profiler,debug \
	--enable-shared=yes \
	--disable-mcs-build 
#	--enable-boehm \
#	--with-sgen=no 


# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/mono.targetinstall:
	@$(call targetinfo)

	@$(call install_init,  mono)
	
	@$(call install_fixup, mono,PACKAGE,mono)
	@$(call install_fixup, mono,PRIORITY,optional)
	@$(call install_fixup, mono,VERSION,$(MONO_VERSION))
	@$(call install_fixup, mono,SECTION,base)
	@$(call install_fixup, mono,AUTHOR,"Georg Schöffl <georgschoeffl91@hotmail.com>")
	@$(call install_fixup, mono,DEPENDS,)
	@$(call install_fixup, mono,DESCRIPTION,missing)

	@$(call targetinfo)

	# Install runtime
	@$(call install_tree, mono, -, -, $(MONO_PKGDIR)/usr/bin, /usr/bin)

	#install dlls (from HOST_MONO build)
	@rsync -av --exclude=*.so --exclude=*.mdb \
		$(PTXDIST_SYSROOT_HOST)/usr/local/lib/mono $(MONO_PKGDIR)/usr/lib/

	@rsync -av --exclude=*.so --exclude=*.mdb \
		$(PTXDIST_SYSROOT_HOST)/usr/local/etc/mono $(MONO_PKGDIR)/etc

	@$(call install_tree, mono, -, -, $(MONO_PKGDIR)/usr/lib, /usr/lib)
	@$(call install_tree, mono, -, -, $(MONO_PKGDIR)/etc/mono, /etc/mono)

	@$(call install_copy, mono, 0, 0, 0755, \
		$(MONO_DIR)/mcs/class/lib/monolite-linux/62731146-81CF-4D61-826D-9A8DDED14432/mscorlib.dll, \
		/usr/lib/mono/4.5/mscorlib.dll)


	@$(call install_finish, mono)

	@$(call touch)

# vim: syntax=make
