#pragma once

#define SENSOR_DEFINITIONS(V) \
  V(intake_fan_speed, "Intake Fan Speed", "%", 0, "speed") \
  V(exhaust_fan_speed, "Exhaust Fan Speed", "%", 0, "speed") \
  V(intake_fan_speed_rpm, "Intake Fan Speed RPM", "rpm", 0, "speed") \
  V(exhaust_fan_speed_rpm, "Exhaust Fan Speed RPM", "rpm", 0, "speed") \
  V(ventilation_level, "Ventilation Level", "", 0, "") \
  V(outside_air_temperature, "Outside Air Temperature", "°C", 1, "temperature") \
  V(supply_air_temperature, "Supply Air Temperature", "°C", 1, "temperature") \
  V(return_air_temperature, "Return Air Temperature", "°C", 1, "temperature") \
  V(exhaust_air_temperature, "Exhaust Air Temperature", "°C", 1, "temperature") \
  V(return_air_level, "Return Air Level", "m³", 1, "volume") \
  V(supply_air_level, "Supply Air Level", "m³", 1, "volume") \
  V(level0_hours, "Level 0 Hours", "h", 0, "") \
  V(level1_hours, "Level 1 Hours", "h", 0, "") \
  V(level2_hours, "Level 2 Hours", "h", 0, "") \
  V(level3_hours, "Level 3 Hours", "h", 0, "") \
  V(frost_protection_hours, "Frost Protection Hours", "h", 0, "") \
  V(frost_protection_minutes, "Frost Protection Minutes", "min", 0, "") \
  V(filter_hours, "Filter Hours", "h", 0, "") \
  V(bathroom_switch_on_delay_minutes, "Bathroom Switch On Delay Minutes", "", 0, "") \
  V(bathroom_switch_off_delay_minutes, "Bathroom Switch Off Delay Minutes", "", 0, "") \
  V(l1_switch_off_delay_minutes, "L1 Switch Off Delay Minutes", "", 0, "") \
  V(boost_ventilation_minutes, "Boost Ventilation Minutes", "", 0, "") \
  V(filter_warning_weeks, "Filter Warning Weeks", "weeks", 0, "") \
  V(rf_high_time_short_minutes, "RF High Time Short Minutes", "", 0, "") \
  V(rf_high_time_long_minutes, "RF High Time Long Minutes", "", 0, "") \

#define BINARY_SENSOR_DEFINITIONS(V) \
  V(frost_protection_active, "Frost Protection Active") \
  V(summer_mode, "Summer Mode") \
  V(supply_fan_active, "Supply Fan Active") \

#define TEXT_SENSOR_DEFINITIONS(V) \
  V(filter_status, "Filter Status") \
  V(frost_protection_level, "Frost Protection Level")

#define BUTTON_DEFINITIONS(V) \
  V(filter_reset, "Filter Reset") \
  V(error_reset, "Error Reset")

#define SENSOR_DEFINITIONS_OPT(V) \
  V(bypass_present, bypass_valve, "Bypass Valve", "%", 0, "volume") \
  V(bypass_present, bypass_factor, "Bypass Factor", "", 0, "volume") \
  V(bypass_present, bypass_step, "Bypass Step", "", 0, "volume") \
  V(bypass_present, bypass_correction, "Bypass Correction", "", 0, "volume") \
  V(bypass_present, bypass_open_hours, "Bypass Open Hours", "h", 0, "") \
  V(bypass_present, motor_current_bypass, "Motor Current Bypass", "A", 1, "current") \
  V(preheating_present, motor_current_preheating, "Motor Current Preheating", "A", 1, "current") \
  V(preheating_present, preheating_hours, "Preheating Hours", "h", 0, "") \
  V(enthalpy_present, enthalpy_temperature, "Enthalpy Temperature", "°C", 1, "temperature") \
  V(ewt_present, ewt_temperature, "EWT Temperature", "°C", 1, "temperature") \
  V(postheating_present, reheating_temperature, "Reheating Temperature", "°C", 1, "temperature") \
  V(kitchen_hood_present, kitchen_hood_temperature, "Kitchen Hood Temperature", "°C", 1, "temperature") \
  V(kitchen_hood_present, extractor_hood_switch_off_delay_minutes, "Extractor Hood Switch Off Delay Minutes", "", 0, "") \

#define BINARY_SENSOR_DEFINITIONS_OPT(V) \
  V(bypass_present, bypass_valve_open, "Bypass Valve Open") \
  V(preheating_present, preheating_state, "Preheating State") \

#define TEXT_SENSOR_DEFINITIONS_OPT(V) \
  V(preheating_present, preheating_valve, "Preheating Valve")
