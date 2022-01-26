# -*-makefile-*-
#
# Copyright (C) 2022 by WAGO GmbH <Thomas.Brandt@wago.com>
# Copyright (C) 2022 by Bernard Rodriguez <bernard.rodriguez@infrabel.be>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_CERTMONGER) += certmonger

#
# Paths and names
#
CERTMONGER_VERSION	:= 0.79.15
CERTMONGER_MD5		:= e775fe8ae7d0aa5bd1334c83bbd6f482
CERTMONGER		:= certmonger-$(CERTMONGER_VERSION)
CERTMONGER_SUFFIX	:= tar.gz
CERTMONGER_URL		:= https://pagure.io/certmonger/archive/$(CERTMONGER)/certmonger-$(CERTMONGER).$(CERTMONGER_SUFFIX)
CERTMONGER_SOURCE	:= $(SRCDIR)/$(CERTMONGER).$(CERTMONGER_SUFFIX)
CERTMONGER_DIR		:= $(BUILDDIR)/$(CERTMONGER)
CERTMONGER_SRCDIR	:= $(CERTMONGER_DIR)/src
CERTMONGER_LICENSE	:= GPLv3 or later (with an exception that it can be linked with OpenSSL).

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

#$(CERTMONGER_SOURCE):
#	@$(call targetinfo)
#	@$(call get, CERTMONGER)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#CERTMONGER_CONF_ENV	:= $(CROSS_ENV)

#
# autoconf
#
CERTMONGER_CONF_TOOL	:= autoconf
CERTMONGER_CONF_OPT	:=	$(CROSS_AUTOCONF_USR) \
	PKG_CONFIG_PATH=$(PTXDIST_SYSROOT_TARGET)/usr/lib/pkgconfig \
	KRB5_CONFIG=$(PTXDIST_SYSROOT_TARGET)/usr/bin/krb5-config \
	--without-idn

$(STATEDIR)/certmonger.prepare:
	@$(call targetinfo)
	@$(call clean, $(CERTMONGER_DIR)/config.cache)
	cd $(CERTMONGER_DIR) && autoreconf -i -f && \
		$(CERTMONGER_PATH) $(CERTMONGER_ENV) \
		./configure $(CERTMONGER_CONF_OPT)
	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

#$(STATEDIR)/certmonger.compile:
#	@$(call targetinfo)
#	@$(call world/compile, CERTMONGER)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

#$(STATEDIR)/certmonger.install:
#	@$(call targetinfo)
#	@$(call world/install, CERTMONGER)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/certmonger.targetinstall:
	@$(call targetinfo)

	@$(call install_init, certmonger)
	@$(call install_fixup, certmonger,PRIORITY,optional)
	@$(call install_fixup, certmonger,SECTION,base)
	@$(call install_fixup, certmonger,AUTHOR,"Thomas Brandt <Thomas.Brandt@wago.com>")
	@$(call install_fixup, certmonger,DESCRIPTION,missing)

	@$(call install_copy, certmonger, 0, 0, 0755, $(CERTMONGER_SRCDIR)/certmonger.conf, /etc/certmonger/certmonger.conf)

	@$(call install_copy, certmonger, 0, 0, 0755, $(CERTMONGER_SRCDIR)/getcert, /usr/bin/getcert)
	@$(call install_copy, certmonger, 0, 0, 0755, $(CERTMONGER_SRCDIR)/ipa-getcert, /usr/bin/ipa-getcert)
	@$(call install_copy, certmonger, 0, 0, 0755, $(CERTMONGER_SRCDIR)/local-getcert, /usr/bin/local-getcert)
	@$(call install_copy, certmonger, 0, 0, 0755, $(CERTMONGER_SRCDIR)/selfsign-getcert, /usr/bin/selfsign-getcert)

	@$(call install_copy, certmonger, 0, 0, 0755, $(CERTMONGER_SRCDIR)/certmonger-session, /usr/libexec/certmonger/certmonger-session)
	@$(call install_copy, certmonger, 0, 0, 0755, $(CERTMONGER_SRCDIR)/dogtag-ipa-renew-agent-submit, /usr/libexec/certmonger/dogtag-ipa-renew-agent-submit)
	@$(call install_copy, certmonger, 0, 0, 0755, $(CERTMONGER_SRCDIR)/dogtag-submit, /usr/libexec/certmonger/dogtag-submit)
	@$(call install_copy, certmonger, 0, 0, 0755, $(CERTMONGER_SRCDIR)/ipa-submit, /usr/libexec/certmonger/ipa-submit)
	@$(call install_copy, certmonger, 0, 0, 0755, $(CERTMONGER_SRCDIR)/scep-submit, /usr/libexec/certmonger/scep-submit)

	@$(call install_copy, certmonger, 0, 0, 0755, $(CERTMONGER_SRCDIR)/certmonger, /usr/sbin/certmonger)


	@$(call install_finish, certmonger)

	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/certmonger.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, CERTMONGER)

# vim: syntax=make
