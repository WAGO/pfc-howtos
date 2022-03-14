#ifndef KBUS_REGISTER_SETTINGS_H
#define KBUS_REGISTER_SETTINGS_H

/**
 * @file
 * @brief Configure (serial) terminals over kbus.
 * 
 * @note All kbus_reg_set_<...>() methods do *not* immediately affect the terminal.
 * Use kbus_reg_write_settings() to apply the settings to the terminal.
 * 
 * @note All kbus_reg_get_<...>() methods do *not* read the current settings from the terminal,
 * but the settings the terminal had the last time kbus_reg_read_settings() has been called.
 * That is, unless kbus_reg_read_settings() has been called manually, the settings at 
 * initialization of the \link KbusRegTerminal settings instance \endlink.
 * 
 * @warning All kbus_reg_get_<...>() and kbus_reg_set_<...>() methods internally check if the terminal type
 * of the settings instance is supported. If not, they do not apply or get the setting.
 * 
 * @warning Some settings may have options that are not supported on all terminals (e. g. certain baudrates).
 * All kbus_reg_set_<...>() methods check if the passed option is available for the passed terminal and return
 * an error if not.
 * 
 * The configuration method names correspond to the setting names in IO-Check.
 *
 * Addressing
 * ----------
 * 
 * - slot number starts at 1, e. g. the first terminal connected has slot number 1
 * - slot numbers do not exclude unsupported terminals, 
 *      e. g. if an unsupported terminal is connected at position 1 and a 750-652 terminal at position 2,
 *      the 750-652 terminal has slot number 2
 *
 * Thread Safety
 * -------------
 * None, do not use the structures and methods provided across multiple threads.
 *
 * @author F. Warzecha: WAGO Kontakttechnik GmbH & Co. KG
 */

#include <dal/adi_application_interface.h>

#include "kbus_register_settings_common.h"

/**
 * @def
 * @brief declare three fields of the same type postfixed for all 3 phases of 3pm modules
 */
#define CONFIGURATION_SET_FIELDS_ALL_PHASES(type, name) type name ## _pl1 ; \
    type name ## _pl2 ; \
    type name ## _pl3 ; 

/**
 * @brief Collection of all possible configuration values.
 * 
 * To leave a setting unconfigured, set its value to 0.
 * 
 * @see An initialized variant is available as #KbusRegConfigurationSet_Default
 */
typedef struct {
    KbusRegOperatingMode operating_mode;
    KbusRegBaudRate baud_rate;
    KbusRegDatabits databits;
    KbusRegParity parity;
    KbusRegStopbits stopbits;
    KbusRegFlowcontrol flowcontrol;
    KbusRegContinuous continuous_send;
    KbusRegContinuous continuous_receive;
    KbusRegContinuousReceiveTimeout continuous_receive_timeout;
    KbusRegSwitchoverTimeRS485 switchover_time_rs485;
    KbusRegOutputFormat output_format;
    KbusRegRtsCts rts_cts;
    KbusRegXonXoff xon_xoff_send;
    KbusRegXonXoff xon_xoff_receive;
    KbusRegCopyStatusbyte copy_statusbyte;
    KbusRegWatchdogTimer watchdog_timer;
    KbusRegDcFilter dc_filter;
    CONFIGURATION_SET_FIELDS_ALL_PHASES (KbusRegUserScaling, user_scaling)
    CONFIGURATION_SET_FIELDS_ALL_PHASES (KbusRegProcessImage, process_image)
    CONFIGURATION_SET_FIELDS_ALL_PHASES (KbusRegEnergySign, energy_sign)
    CONFIGURATION_SET_FIELDS_ALL_PHASES (KbusRegClearMinMaxValues, clear_min_max_values)
    CONFIGURATION_SET_FIELDS_ALL_PHASES (KbusRegEnergyMeasurementScaling, energy_measurement_scaling)
} KbusRegConfigurationSet;

#undef CONFIGURATION_SET_FIELDS_ALL_PHASES

/**
 * @brief #KbusRegConfigurationSet with all values initialized with 0 (e. g. unconfigured).
 *
 * @internal this (ab)uses the default initialization rules of static objects, which states that arithmetic fields are initialized with 0 (cf 6.7.9.10)
 */
const static KbusRegConfigurationSet KbusRegConfigurationSet_Default = {};

/**
 * @brief Apply a configuration set to a terminal.
 *
 * Thread Safety
 * -------------
 * Do not call this method from multiple threads.
 * 
 * @param[in] adi An initialized ADI-interface.
 * @param[in] terminalSlot The index of the terminal to configure.
 * @param[in] configuration The configuration to apply to the terminal.
 * @return If the configuration has been applied successfully.
 */
KbusRegErr kbus_reg_apply_configuration(tApplicationDeviceInterface *adi, uint8_t terminalSlot, KbusRegConfigurationSet configuration);

/**
 * @brief Extract the configuration set of a terminal.
 *
 * Fills all available configuration values with the values set in the terminal.
 * All unavailable values will have the value '0'.
 *
 * Thread Safety
 * -------------
 * Do not call this method from multiple threads.
 *
 * @param[in] adi An initialized ADI-interface.
 * @param[in] terminalSlot The index of the terminal to read from.
 * @param[out] configuration The configuration of the terminal.
 * @return If the configuration has been extracted successfully.
 */
KbusRegErr kbus_reg_extract_configuration(tApplicationDeviceInterface *adi, uint8_t terminalSlot, KbusRegConfigurationSet *configuration);

/**
 * @brief Initialize a new settings instance.
 * 
 * @warning The \a adi instance is saved in the initialized instance. 
 * Therefore, the adi interface must not be closed while the settings instance is used.
 * 
 * @note This method allocates memory via malloc(). Even if an error is returned,
 * there may be some memory already allocated. Use kbus_reg_close_settings() to
 * clean up the settings instance and free the allocated memory.
 * 
 * Determines the terminal type and reads the terminal's settings 
 * (e. g. there is no need to call kbus_reg_read_settings() directly after initialization).
 *
 * Thread Safety
 * -------------
 * Do not use \a settings across multiple threads.
 * 
 * @param[in] adi An initialized ADI-interface.
 * @param[out] settings The settings variable to initialize.
 * @param[in] terminalSlot The index of the terminal the new instance configures.
 * @return The success of the operation.
 */ 
KbusRegErr kbus_reg_open_settings(tApplicationDeviceInterface *adi, KbusRegTerminal **settings, uint8_t terminalSlot);

/**
 * @brief Clean up an initialized settings instance.
 * 
 * @warning The \a settings instance must not be used after this function has been called, even if
 * an error is returned.
 * 
 * @note This method frees the memory allocated in kbus_reg_open_settings(), 
 * but only if no error occurred during clean up.
 * 
 * @param[in] settings The instance to clean up.
 * @return If the settings instance has been cleaned up.
 */
KbusRegErr kbus_reg_close_settings(KbusRegTerminal *settings);

/**
 * @brief Re-read the terminal's settings.
 * 
 * @param[in,out] settings An initialized settings instance.
 * @return If the settings have been read.
 */
KbusRegErr kbus_reg_read_settings(KbusRegTerminal *settings);

/**
 * @brief Write the terminal's settings.
 * 
 * @note Overwrites all settings of the terminal 
 * (not just the modified ones).
 * 
 * @warning Due to limitations of the kbus register API,
 * it is not possible to verify the settings on the terminal.
 * Although this API tries its best to make sure only valid
 * settings can be/are written, it is recommended to check if 
 * the correct settings are applied, before relying on correctly modified settings.
 * 
 * @param[in] settings The settings to write.
 * @return If the settings have been written.
 */
KbusRegErr kbus_reg_write_settings(KbusRegTerminal *settings);

/**
 * @brief Get the #KbusRegTerminalType of a settings instance.
 * 
 * @param[in] settings An initialized settings instance.
 * @return The terminal type.
 */
KbusRegErr kbus_reg_terminal_type(KbusRegTerminal *settings, KbusRegTerminalType *type);

/**
 * @brief Set the operating mode of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] mode The #KbusRegOperatingMode to set.
 * @return If the setting has been successfully set. 
 */
KbusRegSettingResult kbus_reg_set_operating_mode(KbusRegTerminal *settings, KbusRegOperatingMode mode);

/**
 * @brief Get the operating mode of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] mode The #KbusRegOperatingMode mode.
 * @return If the setting has been successfully read. 
 */
KbusRegSettingResult kbus_reg_get_operating_mode(KbusRegTerminal *settings, KbusRegOperatingMode *mode);

/**
 * @brief Set the baud rate of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] rate The baud rate to set.
 * @return If the setting has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_baud_rate(KbusRegTerminal *settings, KbusRegBaudRate rate);

/**
 * @brief Get the baud rate of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] rate The baud rate.
 * @return If the setting has been successfully read. 
 */
KbusRegSettingResult kbus_reg_get_baud_rate(KbusRegTerminal *settings, KbusRegBaudRate *rate);

/**
 * @brief Set the databits setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] bits The databits setting to set.
 * @return If the setting has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_databits(KbusRegTerminal *settings, KbusRegDatabits bits);

/**
 * @brief Get the databits setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] bits The databits setting.
 * @return If the setting has been successfully read.
 */
KbusRegSettingResult kbus_reg_get_databits(KbusRegTerminal *settings, KbusRegDatabits *bits);

/**
 * @brief Set the parity setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] parity The parity setting to set.
 * @return If the setting has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_parity(KbusRegTerminal *settings, KbusRegParity parity);

/**
 * @brief Get the parity setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] parity The parity setting.
 * @return If the setting has been successfully read.
 */
KbusRegSettingResult kbus_reg_get_parity(KbusRegTerminal *settings, KbusRegParity *parity);

/**
 * @brief Set the stopbits setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] bits The stopbits setting to set.
 * @return If the setting has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_stopbits(KbusRegTerminal *settings, KbusRegStopbits bits);

/**
 * @brief Get the stopbits setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] bits The stopbits setting.
 * @return If the setting has been successfully read.
 */
KbusRegSettingResult kbus_reg_get_stopbits(KbusRegTerminal *settings, KbusRegStopbits *bits);

/**
 * @brief Set the flowcontrol setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] flow The flowcontrol setting to set.
 * @return If the setting has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_flowcontrol(KbusRegTerminal *settings, KbusRegFlowcontrol flow);

/**
 * @brief Get the flowcontrol setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] flow The flowcontrol setting.
 * @return If the setting has bee successfully read.
 */
KbusRegSettingResult kbus_reg_get_flowcontrol(KbusRegTerminal *settings, KbusRegFlowcontrol *flow);

/**
 * @brief Set the continuous receive setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] continuous The continuous receive setting to set.
 * @return If the setting has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_continuous_receive(KbusRegTerminal *settings, KbusRegContinuous continuous);

/**
 * @brief Get the continuous receive setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] continuous The continuous receive setting.
 * @return If the setting has been successfully read.
 */
KbusRegSettingResult kbus_reg_get_continuous_receive(KbusRegTerminal *settings, KbusRegContinuous *continuous);

/**
 * @brief Set the continuous send setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] continuous The continuous send setting to set.
 * @return If the setting has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_continuous_send(KbusRegTerminal *settings, KbusRegContinuous continuous);

/**
 * @brief Get the continuous send setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] continuous The continuous send setting.
 * @return If the setting has been successfully read.
 */
KbusRegSettingResult kbus_reg_get_continuous_send(KbusRegTerminal *settings, KbusRegContinuous *continuous);

/**
 * @brief Set the continuous receive timeout setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] timeout The timeout setting to set.
 * @return If the setting has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_continuous_receive_timeout(KbusRegTerminal *settings, KbusRegContinuousReceiveTimeout timeout);

/**
 * @brief Get the continuous receive timeout setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] timeout The timeout setting.
 * @return If the setting has been successfully read.
 */
KbusRegSettingResult kbus_reg_get_continuous_receive_timeout(KbusRegTerminal *settings, KbusRegContinuousReceiveTimeout *timeout);

/**
 * @brief Set the switchover time for RS485 setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] time The switchover time setting to set.
 * @return If the setting has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_switchover_time_rs485(KbusRegTerminal *settings, KbusRegSwitchoverTimeRS485 time);

/**
 * @brief Get the switchover time for RS485 setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] time The switchover time setting.
 * @return If the setting has been successfully read.
 */
KbusRegSettingResult kbus_reg_get_switchover_time_rs485(KbusRegTerminal *settings, KbusRegSwitchoverTimeRS485 *time);

/**
 * @brief Set the rts pre-delay setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] time_ms The pre-delay time to set in ms.
 * @return If the pre-delay has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_rts_pre_delay(KbusRegTerminal *settings, uint16_t time_ms);

/**
 * @brief Get the rts pre-delay setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] time_ms The pre-delay time in ms.
 * @return If the pre-delay has been successfully read.
 */
KbusRegSettingResult kbus_reg_get_rts_pre_delay(KbusRegTerminal *settings, uint16_t *time_ms);

/**
 * @brief Set the rts post-delay setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] time_ms The post-delay time to set in ms.
 * @return If the post-delay has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_rts_post_delay(KbusRegTerminal *settings, uint16_t time_ms);

/**
 * @brief Get the rts post-delay setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] time_ms The post-delay time in ms.
 * @return If the post-delay time has been successfully read.
 */
KbusRegSettingResult kbus_reg_get_rts_post_delay(KbusRegTerminal *settings, uint16_t *time_ms);

/**
 * @brief Set the dmx start channel setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] channel_no The start channel number to set.
 * @return If the start channel has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_dmx_start_channel(KbusRegTerminal *settings, uint16_t channel_no);

/**
 * @brief Get the dmx start channel setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] channel_no The start channel number.
 * @return If the start channel has been successfully read.
 */
KbusRegSettingResult kbus_reg_get_dmx_start_channel(KbusRegTerminal *settings, uint16_t *channel_no);

/**
 * @brief Set the output format setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] format The output format to set.
 * @return If the output format has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_output_format(KbusRegTerminal *settings, KbusRegOutputFormat format);

/**
 * @brief Get the output format setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] format The output format setting.
 * @return If the output format has been successfully read.
 */
KbusRegSettingResult kbus_reg_get_output_format(KbusRegTerminal *settings, KbusRegOutputFormat *format);

/**
 * @brief Set the rts/cts setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] rts_cts The rts/cts setting to set.
 * @return If the rts/cts setting has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_rts_cts(KbusRegTerminal *settings, KbusRegRtsCts rts_cts);

/**
 * @brief Get the rts/cts setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] rts_cts The rts/cts settings.
 * @return If the rts/cts settings has been successfully read.
 */
KbusRegSettingResult kbus_reg_get_rts_cts(KbusRegTerminal *settings, KbusRegRtsCts *rts_cts);

/**
 * @brief Set the xon/xoff receive setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] xon_xoff The xon/xoff receive setting to set.
 * @return If the xon/xoff receive setting has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_xon_xoff_receive(KbusRegTerminal *settings, KbusRegXonXoff xon_xoff);

/**
 * @brief Get the xon/off receive setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] xon_xoff The xon/xoff receive setting.
 * @return If the xon/xoff receive setting has been successfully read.
 */
KbusRegSettingResult kbus_reg_get_xon_xoff_receive(KbusRegTerminal *settings, KbusRegXonXoff *xon_xoff);

/**
 * @brief Set the xon/xoff send setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] xon_xoff The xon/xoff send setting to set.
 * @return If the xon/xoff send setting has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_xon_xoff_send(KbusRegTerminal *settings, KbusRegXonXoff xon_xoff);

/**
 * @brief Get the xon/off send setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] xon_xoff The xon/xoff send setting.
 * @return If the xon/xoff send setting has been successfully read.
 */
KbusRegSettingResult kbus_reg_get_xon_xoff_send(KbusRegTerminal *settings, KbusRegXonXoff *xon_xoff);

/**
 * @brief Set the statusbyte copy setting of a terminal.
 * 
 * @param[in,out] settings The settings instance to modify.
 * @param[in] copy The statusbyte copy setting to set.
 * @return If the statusbyte copy setting has been successfully set.
 */
KbusRegSettingResult kbus_reg_set_copy_statusbyte(KbusRegTerminal *settings, KbusRegCopyStatusbyte copy);

/**
 * @brief Get the statusbyte copy setting of a terminal.
 * 
 * @param[in] settings The settings instance to read from.
 * @param[out] copy The statusbyte copy setting.
 * @return If the statusbyte copy setting has been successfully read.
 */
KbusRegSettingResult kbus_reg_get_copy_statusbyte(KbusRegTerminal *settings, KbusRegCopyStatusbyte *copy);

KbusRegSettingResult kbus_reg_set_watchdog_timer(KbusRegTerminal *settings, KbusRegWatchdogTimer watchdog);

KbusRegSettingResult kbus_reg_get_watchdog_timer(KbusRegTerminal *settings, KbusRegWatchdogTimer *watchdog);

KbusRegSettingResult kbus_reg_set_dc_filter(KbusRegTerminal *settings, KbusRegDcFilter filter);

KbusRegSettingResult kbus_reg_get_dc_filter(KbusRegTerminal *settings, KbusRegDcFilter *filter);

KbusRegSettingResult kbus_reg_set_user_scaling(KbusRegTerminal *settings, KbusRegPhase phase, KbusRegUserScaling scaling);

KbusRegSettingResult kbus_reg_get_user_scaling(KbusRegTerminal *settings, KbusRegPhase phase, KbusRegUserScaling *scaling);

KbusRegSettingResult kbus_reg_set_process_image(KbusRegTerminal *settings, KbusRegPhase phase, KbusRegProcessImage image);

KbusRegSettingResult kbus_reg_get_process_image(KbusRegTerminal *settings, KbusRegPhase phase, KbusRegProcessImage *image);

KbusRegSettingResult kbus_reg_set_energy_sign(KbusRegTerminal *settings, KbusRegPhase phase, KbusRegEnergySign sign);

KbusRegSettingResult kbus_reg_get_energy_sign(KbusRegTerminal *settings, KbusRegPhase phase, KbusRegEnergySign *sign);

KbusRegSettingResult kbus_reg_set_clear_min_max_values(KbusRegTerminal *settings, KbusRegPhase phase, KbusRegClearMinMaxValues clear);

KbusRegSettingResult kbus_reg_get_clear_min_max_values(KbusRegTerminal *settings, KbusRegPhase phase, KbusRegClearMinMaxValues *clear);

KbusRegSettingResult kbus_reg_set_energy_measurement_scaling(KbusRegTerminal *settings, KbusRegPhase phase, KbusRegEnergyMeasurementScaling scaling);

KbusRegSettingResult kbus_reg_get_energy_measurement_scaling(KbusRegTerminal *settings, KbusRegPhase phase, KbusRegEnergyMeasurementScaling *scaling);

#endif /* KBUS_REGISTER_SETTINGS_H */
