TARGET = KbusQtDemo
TEMPLATE = app

SOURCES += main.cpp

contains(DEFINES, WAGO_PFC){ # Check if we defined WAGO_PFC so we know its meant for this device, we defined it during the Qt-Build-Process
    CONFIG += wago_pfc
}

wago_pfc{ # Wago specific code, like the inlude Paths and Libraries
    QT -= core gui # Is included by default, we dont want that!

    QMAKE_PLATFORM += wago_pfc
    # Include Pfad für andere Libs noch anpassen
    exists($$SYSROOT_TARGET){
        message("Target_Sysroot found: $$SYSROOT_TARGET")
        INCLUDEPATH += $${SYSROOT_TARGET}/usr/include/
        INCLUDEPATH += $${SYSROOT_TARGET}/usr/include/OsLinux/
        INCLUDEPATH += $${SYSROOT_TARGET}/usr/include/dal/
        INCLUDEPATH += $${SYSROOT_TARGET}/usr/include/diagnostic/
        INCLUDEPATH += $${SYSROOT_TARGET}/usr/include/dbus-1.0/

        LIBS += -L$$[QT_SYSROOT]
        LIBS += -L$${SYSROOT_TARGET}/usr/lib
    } else {
      error("Target-Sysroot cannot be set. Set Environment Variable "SYSROOT_TARGET"(Default is /opt/wago_pfc/sysroot)")
    }

    # Build-Type = Debug or Release
    CONFIG(release, debug|release) {
        BUILD_TYPE = Release
    } else {
        BUILD_TYPE = Debug
    }
    message("Build-Typ: $$BUILD_TYPE")

    # System Libraries we are linking against so the ADI/DAL is working correctly
    LIBS += -ldal -llibloader -lpthread -lffi -lrt -lgobject-2.0 -ldbus-glib-1 -lglib-2.0 -lpcre -lz
    LIBS += -ltypelabel -loslinux -ldbuskbuscommon -ldbus-1 -lgio-2.0 -lwago_dbus -lgmodule-2.0

    # The install path where we will copy the Application to
    target.path= /root/
    message("Install Path on Wago_PFC: '$$target.path'")
    INSTALLS += target
}
