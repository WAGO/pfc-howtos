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
PACKAGES-$(PTXCONF_OPENDDS) += opendds

#
# Paths and names
#
OPENDDS_VERSION	:= 3.16
OPENDDS := OpenDDS-$(OPENDDS_VERSION)
OPENDDS_MD5 := eb2fa1ffdcecf9a709ba700bf02c9c1f
OPENDDS_SUFFIX	:= tar.gz
OPENDDS_URL		:= https://github.com/objectcomputing/OpenDDS/releases/download/DDS-$(OPENDDS_VERSION)/$(OPENDDS).$(OPENDDS_SUFFIX)
OPENDDS_SOURCE	:= $(SRCDIR)/$(OPENDDS).$(OPENDDS_SUFFIX)
OPENDDS_DIR		:= $(BUILDDIR)/$(OPENDDS)
OPENDDS_LICENSE	:= Opendds License

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------
#$(OPENDDS_SOURCE):
#	@$(call targetinfo)
#	@$(call get, OPENDDS)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------
OPENDDS_CONF_ENV :=
OPENDDS_CONF_OPT := --target=linux-cross \
                    --target-compiler=arm-linux-gnueabihf-g++ \
                    --no-tests \
                    --prefix=/usr

#-----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------
OPENDDS_DIR_HOST := $(OPENDDS_DIR)/build/host
OPENDDS_DIR_TARGET := $(OPENDDS_DIR)/build/target
OPENDDS_MAKE_ENV := $(CROSS_ENV)
OS := $(shell uname -s)
OPENDDS_MAKE_OPT := "-j$(NPROCS)"
NPROCS := 1

#figure out how many procces could be startet for parallel build
ifeq ($(OS),Linux)
	NPROCS:=$(shell grep -c ^processor /proc/cpuinfo)
endif

$(STATEDIR)/opendds.compile:
	@$(call targetinfo)
#to compile properly we have to remove the "ARCH" setting
	@unset ARCH && \
		cd $(OPENDDS_DIR_HOST) && \
		$(MAKE) $(OPENDDS_MAKE_OPT) && \
		cd $(OPENDDS_DIR_TARGET) && \
		$(OPENDDS_PATH) $(OPENDDS_MAKE_ENV)\
		$(MAKE) $(OPENDDS_MAKE_OPT)

	@$(call touch)
# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------
$(STATEDIR)/opendds.install:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------
OPENDDS_INSTALL_DIR := /usr/lib
$(STATEDIR)/opendds.targetinstall:
	@$(call targetinfo)

	@$(call install_init, opendds)
	@$(call install_fixup, opendds,PRIORITY,optional)
	@$(call install_fixup, opendds,SECTION,base)
	@$(call install_fixup, opendds,AUTHOR,"thomas.brandt@wago.com")
	@$(call install_fixup, opendds,DESCRIPTION,missing)

	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/dds/libOpenDDS_Dcps.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Dcps.so.3.16.0)
	@$(call install_link, opendds, libOpenDDS_Dcps.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Dcps.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/dds/libOpenDDS_Dcps.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Dcps.so.3.16.0)
	@$(call install_link, opendds, libOpenDDS_Dcps.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Dcps.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/dds/FACE/libOpenDDS_FACE.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_FACE.so.3.16.0)
	@$(call install_link, opendds, libOpenDDS_FACE.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_FACE.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/dds/InfoRepo/libOpenDDS_Federator.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Federator.so.3.16.0)
	@$(call install_link, opendds, libOpenDDS_Federator.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Federator.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/dds/DCPS/InfoRepoDiscovery/libOpenDDS_InfoRepoDiscovery.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_InfoRepoDiscovery.so.3.16.0)
	@$(call install_link, opendds, libOpenDDS_InfoRepoDiscovery.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_InfoRepoDiscovery.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/dds/InfoRepo/libOpenDDS_InfoRepoLib.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_InfoRepoLib.so.3.16.0)
	@$(call install_link, opendds, libOpenDDS_InfoRepoLib.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_InfoRepoLib.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/dds/InfoRepo/libOpenDDS_InfoRepoServ.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_InfoRepoServ.so.3.16.0)
	@$(call install_link, opendds, libOpenDDS_InfoRepoServ.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_InfoRepoServ.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/tools/modeling/codegen/model/libOpenDDS_Model.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Model.so.3.16.0)
	@$(call install_link, opendds, libOpenDDS_Model.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Model.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/dds/monitor/libOpenDDS_monitor.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_monitor.so.3.16.0)
	@$(call install_link, opendds, libOpenDDS_monitor.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_monitor.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/dds/DCPS/transport/multicast/libOpenDDS_Multicast.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Multicast.so.3.16.0)
	@$(call install_link, opendds, libOpenDDS_Multicast.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Multicast.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/dds/DCPS/RTPS/libOpenDDS_Rtps.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Rtps.so.3.16.0)
	@$(call install_link, opendds, libOpenDDS_Rtps.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Rtps.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/dds/DCPS/transport/rtps_udp/libOpenDDS_Rtps_Udp.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Rtps_Udp.so.3.16.0)
	@$(call install_link, opendds, libOpenDDS_Rtps_Udp.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Rtps_Udp.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/dds/DCPS/transport/shmem/libOpenDDS_Shmem.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Shmem.so.3.16.0)
	@$(call install_link, opendds, libOpenDDS_Shmem.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Shmem.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/dds/DCPS/transport/tcp/libOpenDDS_Tcp.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Tcp.so.3.16.0)
	@$(call install_link, opendds, libOpenDDS_Tcp.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Tcp.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/dds/DCPS/transport/udp/libOpenDDS_Udp.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Udp.so.3.16.0)
	@$(call install_link, opendds, libOpenDDS_Udp.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libOpenDDS_Udp.so)
# --- TAO / ACE --- #
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/ace/libACE.so.6.2a_p19, $(OPENDDS_INSTALL_DIR)/libACE.so.6.2a_p19)
	@$(call install_link, opendds, libACE.so.6.2a_p19, $(OPENDDS_INSTALL_DIR)/libACE.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/AnyTypeCode/libTAO_AnyTypeCode.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_AnyTypeCode.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_AnyTypeCode.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_AnyTypeCode.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/IORTable/libTAO_Async_IORTable.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_Async_IORTable.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_Async_IORTable.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_Async_IORTable.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/BiDir_GIOP/libTAO_BiDirGIOP.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_BiDirGIOP.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_BiDirGIOP.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_BiDirGIOP.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/CodecFactory/libTAO_CodecFactory.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_CodecFactory.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_CodecFactory.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_CodecFactory.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/Codeset/libTAO_Codeset.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_Codeset.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_Codeset.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_Codeset.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/CSD_Framework/libTAO_CSD_Framework.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_CSD_Framework.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_CSD_Framework.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_CSD_Framework.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/CSD_ThreadPool/libTAO_CSD_ThreadPool.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_CSD_ThreadPool.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_CSD_ThreadPool.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_CSD_ThreadPool.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/DynamicInterface/libTAO_DynamicInterface.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_DynamicInterface.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_DynamicInterface.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_DynamicInterface.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/TAO_IDL/libTAO_IDL_FE.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_IDL_FE.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_IDL_FE.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_IDL_FE.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/ImR_Client/libTAO_ImR_Client.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_ImR_Client.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_ImR_Client.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_ImR_Client.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/IORManipulation/libTAO_IORManip.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_IORManip.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_IORManip.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_IORManip.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/IORTable/libTAO_IORTable.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_IORTable.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_IORTable.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_IORTable.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/Messaging/libTAO_Messaging.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_Messaging.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_Messaging.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_Messaging.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/PI/libTAO_PI.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_PI.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_PI.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_PI.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/PortableServer/libTAO_PortableServer.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_PortableServer.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_PortableServer.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_PortableServer.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/libTAO.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO.so.2.2a_p19)
	@$(call install_link, opendds, libTAO.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/orbsvcs/orbsvcs/libTAO_Svc_Utils.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_Svc_Utils.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_Svc_Utils.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_Svc_Utils.so)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/ACE_wrappers/TAO/tao/Valuetype/libTAO_Valuetype.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_Valuetype.so.2.2a_p19)
	@$(call install_link, opendds, libTAO_Valuetype.so.2.2a_p19, $(OPENDDS_INSTALL_DIR)/libTAO_Valuetype.so)


#copy examples if selected
ifdef PTXCONF_OPENDDS_EXAMPLE
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/dds/InfoRepo/DCPSInfoRepo, /bin/DCPSInfoRepo)

	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/DevGuideExamples/DCPS/Messenger/publisher, /usr/share/OpenDDS/Messenger/publisher)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/DevGuideExamples/DCPS/Messenger/subscriber, /usr/share/OpenDDS/Messenger/subscriber)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR)/DevGuideExamples/DCPS/Messenger/run_test.sh, /usr/share/OpenDDS/Messenger/run_test.sh)

	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/DevGuideExamples/DCPS/Messenger.minimal/publisher, /usr/share/OpenDDS/Messenger.minimal/publisher)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/DevGuideExamples/DCPS/Messenger.minimal/subscriber, /usr/share/OpenDDS/Messenger.minimal/subscriber)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR)/DevGuideExamples/DCPS/Messenger.minimal/run_test.sh, /usr/share/OpenDDS/Messenger.minimal/run_test.sh)

	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/DevGuideExamples/DCPS/Messenger_ZeroCopy/publisher, /usr/share/OpenDDS/Messenger_ZeroCopy/publisher)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/DevGuideExamples/DCPS/Messenger_ZeroCopy/subscriber, /usr/share/OpenDDS/Messenger_ZeroCopy/subscriber)
	@$(call install_copy, opendds, 0, 0, 755, $(OPENDDS_DIR_TARGET)/DevGuideExamples/DCPS/Messenger_ZeroCopy/run_test.sh, /usr/share/OpenDDS/Messenger_ZeroCopy/run_test.sh)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/DevGuideExamples/DCPS/Messenger_ZeroCopy/libMessenger_ZeroCopy_Idl.so.3.16.0 , $(OPENDDS_INSTALL_DIR)/libMessenger_ZeroCopy_Idl.so.3.16.0)
	@$(call install_link, opendds, libMessenger_ZeroCopy_Idl.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libMessenger_ZeroCopy_Idl.so)
endif

ifdef PTXDIST_NOT_SELECTED
#FACE-Example
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/DevGuideExamples/FACE/Simple/publisher, /usr/share/OpenDDS/FACE/Simple/publisher)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/DevGuideExamples/FACE/Simple/subscriber, /usr/share/OpenDDS/FACE/Simple/subscriber)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/DevGuideExamples/FACE/Simple/callback_subscriber, /usr/share/OpenDDS/FACE/Simple/callback_subscriber)
	@$(call install_copy, opendds, 0, 0, 775, $(OPENDDS_DIR_TARGET)/DevGuideExamples/FACE/Simple/libFaceMessenger_Idl.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libFaceMessenger_Idl.so.3.16.0)
	@$(call install_link, opendds, libFaceMessenger_Idl.so.3.16.0, $(OPENDDS_INSTALL_DIR)/libFaceMessenger_Idl.so)
endif

	@$(call install_finish, opendds)
	@$(call touch)
# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

$(STATEDIR)/opendds.clean:
	@$(call targetinfo)
	@$(call clean_pkg, opendds)

# vim: syntax=make
 
