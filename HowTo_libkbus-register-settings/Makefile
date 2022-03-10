
ifndef PTXPROJ_PATH
PTXPROJ_PATH := ~/wago/ptxproj 
$(warning PTXPROJ_PATH is not set; assuming default of ~/wago/ptxproj)
endif

ifndef PTXPROJ_PLATFORM_DIR
PTXPROJ_PLATFORM_DIR := platform-wago-pfcXXX
endif

all: copy

build:
	@echo Building library with ptxdist:
	cd $(PTXPROJ_PATH) && ptxdist targetinstall libkbus-register-settings

copy: build
	@echo Copying shared library and headers [if this fails, make sure both static and shared build is selected]:
	cp $(PTXPROJ_PATH)/$(PTXPROJ_PLATFORM_DIR)/sysroot-target/usr/lib/libkbus-register-settings.so* src/libkbus-register-settings-bin
	cp $(PTXPROJ_PATH)/$(PTXPROJ_PLATFORM_DIR)/sysroot-target/usr/lib/libkbus-register-settings.a src/libkbus-register-settings-bin
	cp $(PTXPROJ_PATH)/$(PTXPROJ_PLATFORM_DIR)/sysroot-target/usr/include/kbus_register_settings.h src/libkbus-register-settings-bin
	cp $(PTXPROJ_PATH)/$(PTXPROJ_PLATFORM_DIR)/sysroot-target/usr/include/kbus_register_settings_common.h src/libkbus-register-settings-bin
	# remove symlinked files (the wildcard variant + subsequent removal is used so that it's not needed to pin a specific library version for this makefile :p)
	rm src/libkbus-register-settings-bin/libkbus-register-settings.so
	rm src/libkbus-register-settings-bin/libkbus-register-settings.so.?

clean:
	-rm src/libkbus-register-settings-bin/kbus_register_settings*.h
	-rm src/libkbus-register-settings-bin/libkbus-register-settings.a
	-rm src/libkbus-register-settings-bin/libkbus-register-settings.so*
