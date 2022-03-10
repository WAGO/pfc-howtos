#ifndef KBUS_REGISTER_SETTINGS_COMMON_H
#define KBUS_REGISTER_SETTINGS_COMMON_H

/**
 * @file 
 *
 * To get a string representation of an enum variant representing an option,
 * you can use the variant as an index into the respective <tt>KBUS_REG_<option name></tt> array.
 * 
 * @internal
 * @warning All enums modelling configuration options may not use 0 for any variant's value, due to
 * 0 being used as an unofficial 'unconfigured' value in kbus_reg_apply_configuration().
 *
 * @warning Make sure that the content of the arrays matches the discrimantors of the respective enums.
 * @endinternal
 *
 * @author F. Warzecha: WAGO Kontakttechnik GmbH & Co. KG
 */

#include <stdint.h>

/**
 * @brief Error variants returned when writing/reading setting values.
 */
typedef enum {
    /**
     * @brief THe setting has been successfully written/read.
     */
    KBUS_REG_SETTING_OK = 0,
    /**
     * @brief The setting is not available for the terminal type its used on.
     * 
     * @note In certain cases an option is not available due to the value of another option
     * (e. g. the 650 does not support parity=None + databits=7).
     */
    KBUS_REG_SETTING_UNAVAILABLE = 1, 
    /**
     * @brief The option passed in or read from a terminal cannot be matched to any of the enum variants.
     * 
     * This error can only occurr when reading from a terminal or when passing in integers instead of enum variants.
     */
    KBUS_REG_SETTING_UNKNOWN = 2, 
    /**
     * @brief Internal error that is not supposed to happen.
     */
    KBUS_REG_SETTING_BUG = 3, 
    /**
     * @brief The #KbusRegTerminal pointer is invalid.
     */
    KBUS_REG_SETTING_INVALID_HANDLE = 4,
} KbusRegSettingResult;

/**
 * @brief Error variants for all kbus register communication.
 */
typedef enum {
    /**
     * @brief The communication was successful.
     */
    KBUS_REG_COM_OK = 0,
    /**
     * @brief A generic error whose cause is not further known occurred.
     */
    KBUS_REG_COM_ERR = 1,
    /**
     * @brief An error occurred calling a function from adi/dal.
     */
    KBUS_REG_COM_ERR_DAL = 2, 
    /**
     * @brief An error occurred calling a function from libpackbus.
     * 
     * These functions are called via the CallDeviceSpecificFunction() function from adi/dal.
     */
    KBUS_REG_COM_ERR_LIBPACKBUS = 3,
    /**
     * @brief An unknown terminal type has been encountered.
     */
    KBUS_REG_COM_UNKNOWN_TERMINAL = 4,
    /**
     * @brief A required memory allocation has failed.
     */
    KBUS_REG_COM_NO_MEM = 5,
    /**
     * @brief The #KbusRegTerminal pointer is invalid.
     */
    KBUS_REG_COM_INVALID_HANDLE = 6,
    /**
     * @brief A setting from #KbusRegConfigurationSet could not be applied.
     */
    KBUS_REG_ERR_SETTING = 7, 
    /**
     * @brief The terminal type is known, but the hardware version of the terminal is not supported.
     */
    KBUS_REG_COM_UNSUPPORTED_HARDWARE_VERSION = 8,
    /**
     * @brief The terminal type is known and has a supported hardware version, but the software version on the terminal is not supported.
     */
    KBUS_REG_COM_UNSUPPORTED_SOFTWARE_VERSION = 9,
} KbusRegErrStatus;

/**
 * @brief Error information returned by all kbus register communication methods.
 */
typedef struct {
    /**
     * @brief If the operation was successful.
     */
    KbusRegErrStatus status;
    /**
     * @brief The error description of an underlying API.
     * 
     * @warning This field is only valid if a #KBUS_REG_COM_ERR_DAL, a #KBUS_REG_COM_ERR_LIBPACKBUS or a #KBUS_REG_ERR_SETTING error occurred.
     * 
     * @todo should the return code lists of adi/dal + libpackbus be listed here? Or only a link to documentation?
     */
    int32_t underlying_error;
} KbusRegErr;

/**
 * @brief Software version of a terminal.
 */
typedef struct {
    uint8_t patch;
    uint8_t minor;
    uint8_t major;
} KbusRegTerminalSoftwareVersion;

/**
 * @brief Detailed representation of a terminal type.
 */
typedef struct {
    uint16_t series;
    uint16_t value;
    uint16_t spec1;
    uint16_t spec2;
    uint8_t hardware_version;
} KbusRegTerminalInfo;

/**
 * @brief String representation of #KbusRegTerminalType.
 */
const static char __attribute__((unused)) *KBUS_REG_TERMINAL_TYPES[] = { 
    "Unknown",
    "750-650/003-000",
    "750-652",
    "750-493",
};

/**
 * @brief Implemented terminal types.
 */
typedef enum {
    /**
     * Supported SW/HW versions: all
     */
    KBUS_REG_TERMINAL_750_650_003 = 1,
    /**
     * Supported SW/HW versions: all
     */
    KBUS_REG_TERMINAL_750_652 = 2,
    /**
     * Supported SW/HW versions: SW >= 1.1.75; HW >= 20
     */
    KBUS_REG_TERMINAL_750_493 = 3,
} KbusRegTerminalType;

/**
 * @brief Terminal settings instance.
 */
struct sKbusRegTerminal;

/**
 * @see #sKbusRegTerminal
 */
typedef struct sKbusRegTerminal KbusRegTerminal;

/**
 * @brief Available phases of 3phase measurment modules (e. g. 750-493).
 */
typedef enum {
    KBUS_REG_PHASE_L1,
    KBUS_REG_PHASE_L2,
    KBUS_REG_PHASE_L3,
} KbusRegPhase;

/**
 * @brief String representation of #KbusRegOperatingMode.
 */
const static char __attribute__((unused)) *KBUS_REG_OPERATING_MODES[] = {
    "Unknown",
    "RS-232",
    "RS-485 half-duplex",
    "RS-422 full-duplex",
    "Data Exchange RS-422",
    "DMX half-duplex/250k",
};

/**
 * @brief Operating modes for serial terminals.
 */
typedef enum {
    KBUS_REG_MODE_RS232 = 1,
    KBUS_REG_MODE_RS485 = 2, // halbduplex
    KBUS_REG_MODE_RS422 = 3, // vollduplex
    KBUS_REG_MODE_EXCHANGE_RS422 = 4,
    KBUS_REG_MODE_DMX = 5, // halbduxplex/250k
} KbusRegOperatingMode;

/**
 * @brief String representation of #KbusRegBaudRate.
 */
const static char __attribute__((unused)) *KBUS_REG_BAUD_RATES[] = {
    "Unknown",
    "300",
    "600",
    "1200",
    "2400",
    "4800",
    "9600",
    "19200",
    "38400",
    "57600",
    "115200",
};

/**
 * @brief Baud rate setting for serial terminals.
 */
typedef enum {
    KBUS_REG_BAUD_300 = 1,
    KBUS_REG_BAUD_600 = 2,
    KBUS_REG_BAUD_1200 = 3,
    KBUS_REG_BAUD_2400 = 4,
    KBUS_REG_BAUD_4800 = 5,
    KBUS_REG_BAUD_9600 = 6,
    KBUS_REG_BAUD_19200 = 7,
    KBUS_REG_BAUD_38400 = 8,
    KBUS_REG_BAUD_57600 = 9,
    KBUS_REG_BAUD_115200 = 10,
} KbusRegBaudRate;

/**
 * @brief String representation of #KbusRegDatabits.
 */
const static char __attribute__((unused)) *KBUS_REG_DATABITS[] = {
    "Unknown",
    "8",
    "7",
};

/**
 * @brief Databits setting for serial terminals.
 */
typedef enum {
    KBUS_REG_DATABITS_8 = 1,
    KBUS_REG_DATABITS_7 = 2,
} KbusRegDatabits;

/**
 * @brief String representation of #KbusRegParity.
 */
const static char __attribute__((unused)) *KBUS_REG_PARITIES[] = {
    "Unknown",
    "None",
    "Even",
    "Odd",
};

/**
 * @brief Parity setting for serial terminals.
 */
typedef enum {
    KBUS_REG_PARITY_NONE = 1,
    KBUS_REG_PARITY_EVEN = 2,
    KBUS_REG_PARITY_ODD = 3,
} KbusRegParity;

/**
 * @brief String representation of #KbusRegStopbits.
 */
const static char __attribute__((unused)) *KBUS_REG_STOPBITS[] = {
    "Unknown",
    "1",
    "2",
};

/**
 * @brief Stopbits setting for serial terminals.
 */
typedef enum {
    KBUS_REG_STOPBITS_1 = 1,
    KBUS_REG_STOPBITS_2 = 2,
} KbusRegStopbits;

/**
 * @brief String representation of #KbusRegFlowcontrol.
 */
const static char __attribute__((unused)) *KBUS_REG_FLOWCONTROLS[] = {
    "Unknown",
    "disabled",
    "XON/XOFF",
    "RTS/CTS",
    "RTS with Pre-/Post-Delay",
};

/**
 * @brief Flowcontrol setting for serial terminals.
 * 
 * @note Some terminals have a flowcontrol setting,
 * other terminals have specific #KbusRegXonXoff or #KbusRegRtsCts settings.
 */
typedef enum {
    KBUS_REG_FLOWCTRL_OFF = 1,
    KBUS_REG_FLOWCTRL_XON_XOFF = 2,
    KBUS_REG_FLOWCTRL_RTS_CTS = 3,
    KBUS_REG_FLOWCTRL_RTS_PRE_POST_DELAY = 4,
} KbusRegFlowcontrol;

/**
 * @brief String representation of #KbusRegContinuous.
 */
const static char __attribute__((unused)) *KBUS_REG_CONTINUOUS[] = {
    "Unknown",
    "activated",
    "deactivated",
};

/**
 * @brief Continuous send/receive setting.
 */
typedef enum {
    KBUS_REG_CONTINUOUS_ON = 1,
    KBUS_REG_CONTINUOUS_OFF = 2,
} KbusRegContinuous;

/**
 * @brief String representation of #KbusRegContinuousReceiveTimeout.
 */
const static char __attribute__((unused)) *KBUS_REG_CONTINUOUS_RECEIVE_TIMEOUTS[] = {
    "Unknown",
    "2 Characters",
    "4 Characters",
};

/**
 * @brief Continuous receive timeout setting.
 */
typedef enum {
    KBUS_REG_CNT_RECEIVE_TIMEOUT_2_CHARS = 1,
    KBUS_REG_CNT_RECEIVE_TIMEOUT_4_CHARS = 2,
} KbusRegContinuousReceiveTimeout;

/**
 * @brief String representation of #KbusRegSwitchoverTimeRS485.
 */
const static char __attribute__((unused)) *KBUS_REG_SWITCHOVERS_RS485[] = {
    "Unknown",
    "100 us",
    "2 Characters",
    "4 Characters",
};

/**
 * @brief Switchover time setting for rs485.
 */
typedef enum {
    KBUS_REG_SWITCHOVER_RS485_100US = 1,
    KBUS_REG_SWITCHOVER_RS485_2_CHARS =2 ,
    KBUS_REG_SWITCHOVER_RS485_4_CHARS = 3,
} KbusRegSwitchoverTimeRS485;

/**
 * @brief String representation of #KbusRegOutputFormat.
 */
const static char __attribute__((unused)) *KBUS_REG_OUTPUT_FORMATS[] = {
    "Unknown",
    "Standard",
    "Alternative",
};

/**
 * @brief Output format.
 */
typedef enum {
    KBUS_REG_OUTPUT_STANDARD = 1,
    KBUS_REG_OUTPUT_ALTERNATIVE = 2,
} KbusRegOutputFormat;

/**
 * @brief String representation of #KbusRegRtsCts.
 */
const static char __attribute__((unused)) *KBUS_REG_RTS_CTS[] = {
    "Unknown",
    "activated",
    "deactivated",
};

/**
 * @brief RTS/CTS setting.
 */
typedef enum {
    KBUS_REG_RTS_CTS_OFF = 1,
    KBUS_REG_RTS_CTS_ON = 2,
} KbusRegRtsCts;

/**
 * @brief String representation of #KbusRegXonXoff.
 */
const static char __attribute__((unused)) *KBUS_REG_XON_XOFF[] = {
    "Unknown",
    "activated",
    "deactivated",
};

/**
 * @brief XON/XOFF Send/Receive setting.
 */
typedef enum {
    KBUS_REG_XON_XOFF_OFF = 1,
    KBUS_REG_XON_XOFF_ON = 2,
} KbusRegXonXoff;

/**
 * @brief String representation of #KbusRegCopyStatusbyte.
 */
const static char __attribute__((unused)) *KBUS_REG_COPY_STATUSBYTE[] = {
    "Unknown",
    "Immediately",
    "Delayed",
};

/**
 * @brief Copy status byte setting.
 */
typedef enum {
    KBUS_REG_COPY_STATUSBYTE_DIRECT = 1,
    KBUS_REG_COPY_STATUSBYTE_DELAYED = 2,
} KbusRegCopyStatusbyte;

/**
 * @brief String representation of #KbusRegUserScaling.
 */
const static char __attribute__((unused)) *KBUS_REG_USER_SCALING[] = {
    "Unknown",
    "disabled",
    "enabled",
};

/**
 * @brief User scaling setting of power measurement modules.
 */
typedef enum {
    KBUS_REG_USER_SCALING_DISABLED = 1,
    KBUS_REG_USER_SCALING_ENABLED = 2,
} KbusRegUserScaling;

/**
 * @brief String representation of #KbusRegWatchdogTimer.
 */
const static char __attribute__((unused)) *KBUS_REG_WATCHDOG_TIMER[] = {
    "Unknown",
    "enabled",
    "disabled",
};

/**
 * @brief Watchdog timer setting of power measurement modules.
 */
typedef enum {
    KBUS_REG_WATCHDOG_TIMER_ENABLED = 1,
    KBUS_REG_WATCHDOG_TIMER_DISABLED = 2,
} KbusRegWatchdogTimer;

/**
 * @brief String representation of #KbusRegProcessImage.
 */
const static char __attribute__((unused)) *KBUS_REG_PROCESS_IMAGE[] = {
    "Unknown",
    "simple", // TODO could also use compat here or simple/compat (though I'm not certain I want to have things such as '/' in the string, to keep them easily parsable)
    "flexible",
};

/**
 * @brief Process image setting of power measurement modules.
 */
typedef enum {
    KBUS_REG_PROCESS_IMAGE_SIMPLE = 1,
    KBUS_REG_PROCESS_IMAGE_FLEXIBLE = 2,
} KbusRegProcessImage;

/**
 * @brief String representation of #KbusRegDcFilter.
 */
const static char __attribute__((unused)) *KBUS_REG_DC_FILTER[] = {
    "Unknown",
    "bypassed",
    "activated",
};

/**
 * @brief DC filter settings for power measurement modules.
 */
typedef enum {
    KBUS_REG_DC_FILTER_BYPASSED = 1,
    KBUS_REG_DC_FILTER_ACTIVATED = 2,
} KbusRegDcFilter;

/**
 * @brief String representation of #KbusRegEnergySign.
 */
const static char __attribute__((unused)) *KBUS_REG_ENERGY_SIGN[] = {
    "Unknown",
    "not inverted",
    "inverted",
};

/**
 * @brief Energy consumption invert setting for power measurement modules.
 */
typedef enum {
    KBUS_REG_ENERGY_SIGN_NOT_INVERTED = 1,
    KBUS_REG_ENERGY_SIGN_INVERTED = 2,
} KbusRegEnergySign;

/**
 * @brief String representation of #KbusRegClearMinMaxValues.
 */
const static char __attribute__((unused)) *KBUS_REG_CLEAR_MIX_MAX_VALUES[] = {
    "Unknown",
    "deactivated",
    "activated",
};

/**
 * @brief Automatic clearing min/max values of power measurement modules.
 */
typedef enum {
    KBUS_REG_CLEAR_MIN_MAX_VALUES_DEACTIVATED = 1,
    KBUS_REG_CLEAR_MIN_MAX_VALUES_ACTIVATED = 2,
} KbusRegClearMinMaxValues;

/**
 * @brief String representation of #KbusRegEnergyMeasurementScaling (750-49X variant, e. g. 1Wh).
 */
const static char __attribute__((unused)) *KBUS_REG_ENERGY_MEASUREMENT_SCALING_1A[] = {
    "Unknown",
    "1 mWh",
    "0.01 Wh",
    "0.1 Wh",
    "1 Wh",
    "0.01 kWh",
    "0.1 kWh",
    "1 kWh",
};

/**
 * @brief String representation of #KbusRegEnergyMeasurementScaling (750-49X/000-001 variant, e. g. 5Wh).
 */
const static char __attribute__((unused)) *KBUS_REG_ENERGY_MEASUREMENT_SCALING_5A[] = {
    "Unknown",
    "5 mWh",
    "0.05 Wh",
    "0.5 Wh",
    "5 Wh",
    "0.05 kWh",
    "0.5 kWh",
    "5 kWh",
};

/**
 * @brief Scaling factors for measuring energy (3-phase power measurment terminals).
 *
 * Values with the same order of magnitude represent the same setting (e. g. 1A_1mWh is the same as 5A_5mWh). 
 * Both values exist, due to slight differences in the scaling. 
 * E. g. the 750-493 model allows for up to 1A and has scaling values with multiple of 1Wh, while
 * the 750-493/000-001 has the exact same orders of magnitude, the difference simply being the base of 5Wh.
 */
typedef enum {
    KBUS_REG_ENERGY_MEASUREMENT_SCALING_1A_1mWh = 1,
    KBUS_REG_ENERGY_MEASUREMENT_SCALING_5A_5mWh = 1,
    KBUS_REG_ENERGY_MEASUREMENT_SCALING_1A_10mWh = 2,
    KBUS_REG_ENERGY_MEASUREMENT_SCALING_5A_50mWh = 2,
    KBUS_REG_ENERGY_MEASUREMENT_SCALING_1A_100mWh = 3,
    KBUS_REG_ENERGY_MEASUREMENT_SCALING_5A_500mWh = 3,
    KBUS_REG_ENERGY_MEASUREMENT_SCALING_1A_1Wh = 4,
    KBUS_REG_ENERGY_MEASUREMENT_SCALING_5A_5Wh = 4,
    KBUS_REG_ENERGY_MEASUREMENT_SCALING_1A_10Wh = 5,
    KBUS_REG_ENERGY_MEASUREMENT_SCALING_5A_50Wh = 5,
    KBUS_REG_ENERGY_MEASUREMENT_SCALING_1A_100Wh = 6,
    KBUS_REG_ENERGY_MEASUREMENT_SCALING_5A_500Wh = 6,
    KBUS_REG_ENERGY_MEASUREMENT_SCALING_1A_1kWh = 7,
    KBUS_REG_ENERGY_MEASUREMENT_SCALING_5A_5kWh = 7,
} KbusRegEnergyMeasurementScaling;

#endif /* KBUS_REGISTER_SETTINGS_COMMON_H */
