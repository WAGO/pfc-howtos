# -*-makefile-*-
#
# Copyright (C) 2012 by Bernhard Walle <bernhard@bwalle.de>
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_OPENLDAP) += openldap

#
# Paths and names
#
OPENLDAP_VERSION	:= 2.4.59
OPENLDAP_LIBVERSION	:= 2.4
OPENLDAP_MD5		:= 6036a03b3a67b4a1fe1246e0a2c7265a
OPENLDAP		:= openldap-$(OPENLDAP_VERSION)
OPENLDAP_SUFFIX		:= tgz
OPENLDAP_URL		:= https://www.openldap.org/software/download/OpenLDAP/openldap-release/$(OPENLDAP).$(OPENLDAP_SUFFIX)
OPENLDAP_SOURCE		:= $(SRCDIR)/$(OPENLDAP).$(OPENLDAP_SUFFIX)
OPENLDAP_DIR		:= $(BUILDDIR)/$(OPENLDAP)
OPENLDAP_LICENSE	:= OLDAP-2.8
OPENLDAP_LICENSE_FILES	:= \
	file://COPYRIGHT;md5=b6dea6c170362fc46381fe3690c722cb \
	file://LICENSE;md5=153d07ef052c4a37a8fac23bc6031972

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

OPENLDAP_CONF_ENV	:= \
	$(CROSS_ENV) \
	ac_cv_header_unicode_utypes_h=no \
	ac_cv_header_uuid_uuid_h=no

OPENLDAP_TLS_CONF_OPT-$(PTXCONF_OPENLDAP_TLS_GNUTLS)	:= gnutls
OPENLDAP_TLS_CONF_OPT-$(PTXCONF_OPENLDAP_TLS_OPENSSL)	:= openssl
OPENLDAP_TLS_CONF_OPT-$(PTXCONF_OPENLDAP_TLS_DISABLED)	:= no

#
# autoconf
#
OPENLDAP_CONF_TOOL	:= autoconf
OPENLDAP_CONF_OPT	:= \
	$(CROSS_AUTOCONF_USR) \
	--disable-debug \
	--enable-dynamic \
	--enable-syslog \
	--enable-proctitle \
	--$(call ptx/endis, PTXCONF_GLOBAL_IPV6)-ipv6 \
	--enable-local \
	\
	--$(call ptx/endis, PTXCONF_OPENLDAP_SLAPD)-slapd \
	--disable-dynacl \
	--disable-aci \
	--enable-cleartext \
	--enable-crypt \
	--disable-lmpasswd \
	--disable-spasswd \
	--disable-modules \
	--enable-rewrite \
	--disable-rlookups \
	--disable-slapi \
	--disable-slp \
	--disable-wrappers \
	\
	--enable-bdb=no \
	--enable-dnssrv=no \
	--enable-hdb=no \
	--enable-ldap=yes \
	--enable-mdb=yes \
	--enable-meta=no \
	--enable-monitor=yes \
	--enable-ndb=no \
	--enable-null=yes \
	--enable-passwd=yes \
	--enable-perl=no \
	--enable-relay=yes \
	--enable-shell=no \
	--enable-sock=no \
	--enable-sql=no \
	\
	--enable-overlays=no \
	\
	--without-cyrus-sasl \
	--without-fetch \
	--with-threads \
	--with-tls=no \
	--with-yielding_select=yes


# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/openldap.targetinstall:
	@$(call targetinfo)

	@$(call install_init, openldap)
	@$(call install_fixup, openldap,PRIORITY,optional)
	@$(call install_fixup, openldap,SECTION,base)
	@$(call install_fixup, openldap,AUTHOR,"Bernhard Walle <bernhard@bwalle.de>")
	@$(call install_fixup, openldap,DESCRIPTION,missing)

#	slapd

ifdef PTXCONF_OPENLDAP_SLAPD
	@$(call install_copy, openldap, 0, 0, 0755, -, /usr/libexec/slapd)
	@$(call install_link, openldap, ../libexec/slapd, /usr/bin/slapacl)
	@$(call install_link, openldap, ../libexec/slapd, /usr/bin/slapadd)
	@$(call install_link, openldap, ../libexec/slapd, /usr/bin/slapauth)
	@$(call install_link, openldap, ../libexec/slapd, /usr/bin/slapcat)
	@$(call install_link, openldap, ../libexec/slapd, /usr/bin/slapdn)
	@$(call install_link, openldap, ../libexec/slapd, /usr/bin/slapindex)
	@$(call install_link, openldap, ../libexec/slapd, /usr/bin/slappasswd)
	@$(call install_link, openldap, ../libexec/slapd, /usr/bin/slapschema)
	@$(call install_link, openldap, ../libexec/slapd, /usr/bin/slaptest)
endif

#	libraries

	@$(call install_lib, openldap, 0, 0, 0644, liblber-$(OPENLDAP_LIBVERSION))
	@$(call install_lib, openldap, 0, 0, 0644, libldap-$(OPENLDAP_LIBVERSION))
	@$(call install_lib, openldap, 0, 0, 0644, libldap_r-$(OPENLDAP_LIBVERSION))

#	tools

ifdef PTXCONF_OPENLDAP_INSTALL_UTILITIES
	@$(call install_link, openldap, ldapmodify, /usr/bin/ldapadd)
	@$(call install_copy, openldap, 0, 0, 0755, -, /usr/bin/ldapcompare)
	@$(call install_copy, openldap, 0, 0, 0755, -, /usr/bin/ldapdelete)
	@$(call install_copy, openldap, 0, 0, 0755, -, /usr/bin/ldapexop)
	@$(call install_copy, openldap, 0, 0, 0755, -, /usr/bin/ldapmodify)
	@$(call install_copy, openldap, 0, 0, 0755, -, /usr/bin/ldapmodrdn)
	@$(call install_copy, openldap, 0, 0, 0755, -, /usr/bin/ldappasswd)
	@$(call install_copy, openldap, 0, 0, 0755, -, /usr/bin/ldapsearch)
	@$(call install_copy, openldap, 0, 0, 0755, -, /usr/bin/ldapurl)
	@$(call install_copy, openldap, 0, 0, 0755, -, /usr/bin/ldapwhoami)
endif

#	configuration files

	@$(call install_tree, openldap, 0, 0, -, /etc/openldap)
	@$(call install_alternative, openldap, 0, 0, 0644, /etc/openldap/ldap.conf)

	@$(call install_finish, openldap)

	@$(call touch)

# vim: syntax=make
