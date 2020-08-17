# -*-makefile-*-
#
# Copyright (C) 2019 by WAGO Kontakttechnik GmbH
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_WRETAIN_EXAMPLE) += wretain_example

#
# Paths and names
#
WRETAIN_EXAMPLE_VERSION := 1.0.0
WRETAIN_EXAMPLE		          := wretain_example
WRETAIN_EXAMPLE_SUFFIX        := .tgz
WRETAIN_EXAMPLE_MD5            = ddd51d28532a9fce1bb51626634b5058
WRETAIN_EXAMPLE_ARCHIVE       := $(WRETAIN_EXAMPLE)-$(WRETAIN_EXAMPLE_VERSION)$(WRETAIN_EXAMPLE_SUFFIX)
WRETAIN_EXAMPLE_SOURCE        := $(SRCDIR)/$(WRETAIN_EXAMPLE_ARCHIVE)
WRETAIN_EXAMPLE_DIR		      := $(BUILDDIR)/$(WRETAIN_EXAMPLE)
WRETAIN_EXAMPLE_LICENSE	      := MPL_2.0

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

$(STATEDIR)/wretain_example.get:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#WRETAIN_EXAMPLE_PATH	:= PATH=$(CROSS_PATH)
WRETAIN_EXAMPLE_CONF_TOOL	:= cmake
WRETAIN_EXAMPLE_MAKE_ENV	:= $(CROSS_ENV)
WRETAIN_EXAMPLE_CONF_OPT	:= $(CROSS_CMAKE_USR) $(WRETAIN_EXAMPLE_CMAKE_OPTS)

#(STATEDIR)/wretain_example.prepare:
#@$(call targetinfo)
#@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

$(STATEDIR)/wretain_example.install:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/wretain_example.targetinstall:
	@$(call targetinfo)

	@$(call install_init, wretain_example)
	@$(call install_fixup, wretain_example,PRIORITY,optional)
	@$(call install_fixup, wretain_example,SECTION,base)
	@$(call install_fixup, wretain_example,AUTHOR,"WAGO Kontakttechnik GmbH")
	@$(call install_fixup, wretain_example,DESCRIPTION,missing)

	@$(call install_copy, wretain_example, 0, 0, 0755, /$(WRETAIN_EXAMPLE_DIR)-build/wretain_example_c, /usr/sbin/wretain_example_c)
	@$(call install_copy, wretain_example, 0, 0, 0755, /$(WRETAIN_EXAMPLE_DIR)-build/wretain_example_cpp, /usr/sbin/wretain_example_cpp)

	@$(call install_finish, wretain_example)

	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

$(STATEDIR)/wretain_example.clean:
	@$(call targetinfo)
	@-cd $(WRETAIN_EXAMPLE_DIR)-build && \
		$(WRETAIN_EXAMPLE_ENV) $(WRETAIN_EXAMPLE_PATH) $(MAKE) clean
	@$(call clean_pkg, WRETAIN_EXAMPLE)

# vim: syntax=make
