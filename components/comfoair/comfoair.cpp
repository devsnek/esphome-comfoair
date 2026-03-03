#include "comfoair.h"

#include "sensors.h"

namespace esphome {
namespace comfoair {

using binary_sensor::BinarySensor;
using button::Button;
using sensor::Sensor;
using text_sensor::TextSensor;

climate::ClimateTraits ComfoAirComponent::traits() {
  auto traits = climate::ClimateTraits();
  traits.add_feature_flags(climate::CLIMATE_SUPPORTS_CURRENT_TEMPERATURE);
  traits.set_supported_modes({climate::CLIMATE_MODE_FAN_ONLY});
  traits.clear_feature_flags(
      climate::CLIMATE_REQUIRES_TWO_POINT_TARGET_TEMPERATURE);
  traits.clear_feature_flags(climate::CLIMATE_SUPPORTS_ACTION);
  traits.set_visual_min_temperature(12);
  traits.set_visual_max_temperature(29);
  traits.set_visual_temperature_step(1);
  traits.set_supported_fan_modes({
      climate::CLIMATE_FAN_LOW,
      climate::CLIMATE_FAN_MEDIUM,
      climate::CLIMATE_FAN_HIGH,
      climate::CLIMATE_FAN_OFF,
  });
  return traits;
}

/// Override control to change settings of the climate device.
void ComfoAirComponent::control(const climate::ClimateCall& call) {
  if (call.get_fan_mode().has_value()) {
    int level;

    fan_mode = *call.get_fan_mode();
    switch (fan_mode.value()) {
      case climate::CLIMATE_FAN_HIGH:
        level = 0x04;
        break;
      case climate::CLIMATE_FAN_MEDIUM:
        level = 0x03;
        break;
      case climate::CLIMATE_FAN_LOW:
        level = 0x02;
        break;
      case climate::CLIMATE_FAN_OFF:
        level = 0x01;
        break;
      case climate::CLIMATE_FAN_ON:
      case climate::CLIMATE_FAN_MIDDLE:
      case climate::CLIMATE_FAN_DIFFUSE:
      default:
        level = -1;
        break;
    }

    if (level >= 0) {
      set_level_(level);
    }
  }

  if (call.get_target_temperature().has_value()) {
    target_temperature = *call.get_target_temperature();
    set_comfort_temperature_(target_temperature);
  }

  publish_state();
}

void ComfoAirComponent::dump_config() {
  uint8_t* p;
  ESP_LOGCONFIG(TAG, "ComfoAir:");
  LOG_UPDATE_INTERVAL(this);
  p = bootloader_version_;
  ESP_LOGCONFIG(TAG, "  Bootloader %.10s v%0d.%02d b%2d", p + 3, *p, *(p + 1),
                *(p + 2));
  p = firmware_version_;
  ESP_LOGCONFIG(TAG, "  Firmware %.10s v%0d.%02d b%2d", p + 3, *p, *(p + 1),
                *(p + 2));
  p = connector_board_version_;
  ESP_LOGCONFIG(TAG, "  Connector Board %.10s v%0d.%02d", p + 2, *p, *(p + 1));

  if (*(p + 12) != 0) {
    ESP_LOGCONFIG(TAG, "  CC-Ease v%0d.%02d", *(p + 12) >> 4, *(p + 12) & 0x0f);
  }
  if (*(p + 13) != 0) {
    ESP_LOGCONFIG(TAG, "  CC-Luxe v%0d.%02d", *(p + 13) >> 4, *(p + 13) & 0x0f);
  }
  check_uart_settings(9600);
}

float ComfoAirComponent::get_setup_priority() const {
  return setup_priority::DATA;
}

void ComfoAirComponent::setup() {
  pref_ = make_entity_preference<Preferences>();
  if (!pref_.load(&preferences_)) {
    preferences_ = {false, false, false, false, false, false, false, false};
  }

#define MAKE_SENSOR(var, name, unit, acc, dc) \
  var = make_sensor_(#var, name, unit, acc, dc);
#define MAKE_BINARY_SENSOR(var, name) var = make_binary_sensor_(#var, name);
#define MAKE_TEXT_SENSOR(var, name) var = make_text_sensor_(#var, name);
#define MAKE_BUTTON(var, name) make_button_(#var, name);
  SENSOR_DEFINITIONS(MAKE_SENSOR);
  BINARY_SENSOR_DEFINITIONS(MAKE_BINARY_SENSOR);
  TEXT_SENSOR_DEFINITIONS(MAKE_TEXT_SENSOR);
  BUTTON_DEFINITIONS(MAKE_BUTTON);

#define MAKE_SENSOR_OPT(pref, ...) \
  if (preferences_.pref) MAKE_SENSOR(__VA_ARGS__)
#define MAKE_BINARY_SENSOR_OPT(pref, ...) \
  if (preferences_.pref) MAKE_BINARY_SENSOR(__VA_ARGS__)
#define MAKE_TEXT_SENSOR_OPT(pref, ...) \
  if (preferences_.pref) MAKE_TEXT_SENSOR(__VA_ARGS__)
  SENSOR_DEFINITIONS_OPT(MAKE_SENSOR_OPT);
  BINARY_SENSOR_DEFINITIONS_OPT(MAKE_BINARY_SENSOR_OPT);
  TEXT_SENSOR_DEFINITIONS_OPT(MAKE_TEXT_SENSOR_OPT);

#undef MAKE_TEXT_SENSOR_OPT
#undef MAKE_BINARY_SENSOR_OPT
#undef MAKE_SENSOR_OPT
#undef MAKE_BUTTON
#undef MAKE_TEXT_SENSOR
#undef MAKE_BINARY_SENSOR
#undef MAKE_SENSOR
}

void ComfoAirComponent::loop() {
  while (available() != 0) {
    read_byte(&data_[data_index_]);
    auto check = check_byte_();
    if (!check.has_value()) {
      // finished
      if (data_[COMMAND_ID_ACK] != COMMAND_ACK) {
        parse_data_();
      }
      data_index_ = 0;
    } else if (!*check) {
      // wrong data
      ESP_LOGV(TAG, "Byte %i of received data frame is invalid.", data_index_);
      data_index_ = 0;
    } else {
      // next byte
      data_index_++;
    }
  }
}

void ComfoAirComponent::update() {
  switch (update_counter_) {
    case -4:
      write_command_(CMD_GET_BOOTLOADER_VERSION, nullptr, 0);
      break;
    case -3:
      write_command_(CMD_GET_FIRMWARE_VERSION, nullptr, 0);
      break;
    case -2:
      write_command_(CMD_GET_CONNECTOR_BOARD_VERSION, nullptr, 0);
      break;
    case -1:
      write_command_(CMD_GET_STATUS, nullptr, 0);
      break;
    case 0:
      write_command_(CMD_GET_FAN_STATUS, nullptr, 0);
      break;
    case 1:
      write_command_(CMD_GET_VALVE_STATUS, nullptr, 0);
      break;
    case 2:
      write_command_(CMD_GET_SENSOR_DATA, nullptr, 0);
      break;
    case 3:
      write_command_(CMD_GET_VENTILATION_LEVEL, nullptr, 0);
      break;
    case 4:
      write_command_(CMD_GET_TEMPERATURES, nullptr, 0);
      break;
    case 5:
      write_command_(CMD_GET_FAULTS, nullptr, 0);
      break;
    case 6:
      if (preferences_.bypass_present) {
        write_command_(CMD_GET_BYPASS_CONTROL_STATUS, nullptr, 0);
      }
      break;
    case 7:
      write_command_(CMD_GET_OPERATION_HOURS, nullptr, 0);
      break;
    case 8:
      if (preferences_.preheating_present) {
        write_command_(CMD_GET_PREHEATING_STATUS, nullptr, 0);
      }
      break;
    case 9:
      write_command_(CMD_GET_TIME_DELAY, nullptr, 0);
      break;
  }

  update_counter_++;
  if (update_counter_ > num_update_counter_elements_) update_counter_ = 0;
}

void ComfoAirComponent::handle_action(const std::string& action) {
  if (action == "filter_reset") {
    uint8_t reset_cmd[4] = {0, 0, 0, 1};
    write_command_(CMD_RESET_AND_SELF_TEST, reset_cmd, sizeof(reset_cmd));
  } else if (action == "error_reset") {
    uint8_t reset_cmd[4] = {1, 0, 0, 0};
    write_command_(CMD_RESET_AND_SELF_TEST, reset_cmd, sizeof(reset_cmd));
  }
}

Sensor* ComfoAirComponent::make_sensor_(const char* id, const char* name,
                                        const char* unit, int8_t accuracy,
                                        const char* device_class) {
  auto* s = new Sensor();
  s->set_name(name);
  if (unit[0] != '\0') s->set_unit_of_measurement(unit);
  s->set_accuracy_decimals(accuracy);
  if (device_class[0] != '\0') s->set_device_class(device_class);
  s->set_state_class(sensor::STATE_CLASS_MEASUREMENT);
  App.register_sensor(s);
  return s;
}

BinarySensor* ComfoAirComponent::make_binary_sensor_(const char* id,
                                                     const char* name) {
  auto* s = new BinarySensor();
  s->set_name(name);
  App.register_binary_sensor(s);
  return s;
}

TextSensor* ComfoAirComponent::make_text_sensor_(const char* id,
                                                 const char* name) {
  auto* s = new TextSensor();
  s->set_name(name);
  App.register_text_sensor(s);
  return s;
}

ComfoAirButton* ComfoAirComponent::make_button_(const char* id,
                                                const char* name) {
  auto* b = new ComfoAirButton();
  b->set_name(name);
  b->set_action(id);
  b->set_parent(this);
  App.register_button(b);
  return b;
}

void ComfoAirComponent::set_level_(int level) {
  if (level < 0 || level > 5) {
    ESP_LOGI(TAG, "Ignoring invalid level request: %i", level);
    return;
  }

  ESP_LOGI(TAG, "Setting level to: %i", level);
  {
    uint8_t command[1] = {(uint8_t)level};
    write_command_(CMD_SET_LEVEL, command, sizeof(command));
  }
}

void ComfoAirComponent::set_comfort_temperature_(float temperature) {
  if (temperature < 12.0f || temperature > 29.0f) {
    ESP_LOGI(TAG, "Ignoring invalid temperature request: %i", temperature);
    return;
  }

  ESP_LOGI(TAG, "Setting temperature to: %i", temperature);
  {
    uint8_t command[1] = {(uint8_t)((temperature + 20.0f) * 2.0f)};
    write_command_(CMD_SET_COMFORT_TEMPERATURE, command, sizeof(command));
  }
}

void ComfoAirComponent::write_command_(const uint8_t command,
                                       const uint8_t* command_data,
                                       uint8_t command_data_length) {
  write_byte(COMMAND_PREFIX);
  write_byte(COMMAND_HEAD);
  write_byte(0x00);

  uint16_t checksum = 173;
  checksum += command;
  checksum += command_data_length;

  write_escaped_byte_(command);
  write_escaped_byte_(command_data_length);

  for (uint8_t i = 0; i < command_data_length; i++) {
    uint8_t data_byte = command_data[i];
    checksum += data_byte;
    write_escaped_byte_(data_byte);
  }

  uint8_t checksum_byte = static_cast<uint8_t>(checksum & 0xFF);
  write_escaped_byte_(checksum_byte);

  write_byte(COMMAND_PREFIX);
  write_byte(COMMAND_TAIL);
  flush();
}

void ComfoAirComponent::write_escaped_byte_(uint8_t value) {
  write_byte(value);
  if (value == COMMAND_PREFIX) {
    write_byte(value);
  }
}

uint8_t ComfoAirComponent::comfoair_checksum_(
    uint8_t command, uint8_t length, const uint8_t* command_data) const {
  uint16_t sum = 173;
  sum += command;
  sum += length;
  if (command_data != nullptr) {
    for (uint8_t i = 0; i < length; i++) {
      sum += command_data[i];
    }
  }
  return static_cast<uint8_t>(sum & 0xFF);
}

optional<bool> ComfoAirComponent::check_byte_() const {
  uint8_t index = data_index_;
  uint8_t byte = data_[index];

  if (index == 0) {
    return byte == COMMAND_PREFIX;
  }

  if (index == 1) {
    if (byte == COMMAND_ACK) {
      return {};
    } else {
      return byte == COMMAND_HEAD;
    }
  }

  if (index == 2) {
    return byte == 0x00;
  }

  if (index < COMMAND_LEN_HEAD) {
    return true;
  }

  uint8_t data_length = data_[COMMAND_IDX_DATA];

  if ((COMMAND_LEN_HEAD + data_length + COMMAND_LEN_TAIL) > sizeof(data_)) {
    ESP_LOGW(TAG, "ComfoAir message too large");
    return false;
  }

  if (index < COMMAND_LEN_HEAD + data_length) {
    return true;
  }

  if (index == COMMAND_LEN_HEAD + data_length) {
    // checksum is without checksum bytes
    uint8_t checksum = comfoair_checksum_(
        data_[COMMAND_IDX_MSG_ID], data_length, data_ + COMMAND_LEN_HEAD);
    if (checksum != byte) {
      // ESP_LOGW(TAG, "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X
      // %02X", data_[0], data_[1], data_[2], data_[3], data_[4], data_[5],
      // data_[6], data_[7], data_[8], data_[9], data_[10]);
      ESP_LOGW(TAG, "ComfoAir Checksum doesn't match: 0x%02X!=0x%02X", byte,
               checksum);
      return false;
    }
    return true;
  }

  if (index == COMMAND_LEN_HEAD + data_length + 1) {
    return byte == COMMAND_PREFIX;
  }

  if (index == COMMAND_LEN_HEAD + data_length + 2) {
    if (byte != COMMAND_TAIL) {
      return false;
    }
  }

  return {};
}

void ComfoAirComponent::parse_data_() {
  status_clear_warning();
  uint8_t* msg = &data_[COMMAND_LEN_HEAD];

  switch (data_[COMMAND_IDX_MSG_ID]) {
    case RES_GET_BOOTLOADER_VERSION: {
      memcpy(bootloader_version_, msg, data_[COMMAND_IDX_DATA]);
      break;
    }
    case RES_GET_FIRMWARE_VERSION: {
      memcpy(firmware_version_, msg, data_[COMMAND_IDX_DATA]);
      break;
    }
    case RES_GET_CONNECTOR_BOARD_VERSION: {
      memcpy(connector_board_version_, msg, data_[COMMAND_IDX_DATA]);
      break;
    }
    case RES_GET_FAN_STATUS: {
      if (intake_fan_speed != nullptr) {
        intake_fan_speed->publish_state(msg[0]);
      }
      if (exhaust_fan_speed != nullptr) {
        exhaust_fan_speed->publish_state(msg[1]);
      }
      if (intake_fan_speed_rpm != nullptr) {
        intake_fan_speed_rpm->publish_state(
            static_cast<int>(1875000.0f / get_uint16_(2)));
      }
      if (exhaust_fan_speed_rpm != nullptr) {
        exhaust_fan_speed_rpm->publish_state(
            static_cast<int>(1875000.0f / get_uint16_(4)));
      }
      break;
    }
    case RES_GET_VALVE_STATUS: {
      if (bypass_valve != nullptr) {
        bypass_valve->publish_state(msg[0]);
      }
      if (bypass_valve_open != nullptr) {
        bypass_valve_open->publish_state(msg[0] != 0);
      }

      if (preheating_state != nullptr) {
        preheating_state->publish_state(msg[1] != 0);
      }

      if (motor_current_bypass != nullptr) {
        motor_current_bypass->publish_state(msg[2]);
      }
      if (motor_current_preheating != nullptr) {
        motor_current_preheating->publish_state(msg[3]);
      }

      break;
    }
    case RES_GET_BYPASS_CONTROL_STATUS: {
      if (bypass_factor != nullptr) {
        bypass_factor->publish_state(msg[2]);
      }
      if (bypass_step != nullptr) {
        bypass_step->publish_state(msg[3]);
      }
      if (bypass_correction != nullptr) {
        bypass_correction->publish_state(msg[4]);
      }

      if (summer_mode != nullptr) {
        summer_mode->publish_state(msg[6] != 0);
      }
      break;
    }
    case RES_GET_TEMPERATURE_STATUS: {
      // T1 / outside air
      if (outside_air_temperature != nullptr) {
        outside_air_temperature->publish_state((float)msg[0] / 2.0f - 20.0f);
      }
      // T2 / supply air
      if (supply_air_temperature != nullptr) {
        supply_air_temperature->publish_state((float)msg[1] / 2.0f - 20.0f);
      }
      // T3 / return air
      if (return_air_temperature != nullptr) {
        return_air_temperature->publish_state((float)msg[2] / 2.0f - 20.0f);
      }
      // T4 / exhaust air
      if (exhaust_air_temperature != nullptr) {
        exhaust_air_temperature->publish_state((float)msg[3] / 2.0f - 20.0f);
      }

      break;
    }
    case RES_GET_SENSOR_DATA: {
      if (enthalpy_temperature != nullptr) {
        enthalpy_temperature->publish_state((float)msg[0] / 2.0f - 20.0f);
      }

      break;
    }
    case RES_GET_VENTILATION_LEVEL: {
      if (return_air_level != nullptr) {
        return_air_level->publish_state(msg[6]);
      }
      if (supply_air_level != nullptr) {
        supply_air_level->publish_state(msg[7]);
      }

      if (ventilation_level != nullptr) {
        ventilation_level->publish_state(msg[8] - 1);
      }

      // Fan Speed
      switch (msg[8]) {
        case 0x00:
          fan_mode.reset();
          mode = climate::CLIMATE_MODE_FAN_ONLY;
          break;
        case 0x01:
          fan_mode = climate::CLIMATE_FAN_OFF;
          mode = climate::CLIMATE_MODE_OFF;
          break;
        case 0x02:
          fan_mode = climate::CLIMATE_FAN_LOW;
          mode = climate::CLIMATE_MODE_FAN_ONLY;
          break;
        case 0x03:
          fan_mode = climate::CLIMATE_FAN_MEDIUM;
          mode = climate::CLIMATE_MODE_FAN_ONLY;
          break;
        case 0x04:
          fan_mode = climate::CLIMATE_FAN_HIGH;
          mode = climate::CLIMATE_MODE_FAN_ONLY;
          break;
      }

      publish_state();

      // Supply air fan active (1 = active / 0 = inactive)
      if (supply_fan_active != nullptr) {
        supply_fan_active->publish_state(msg[9] == 1);
      }
      break;
    }
    case RES_GET_FAULTS: {
      if (filter_status != nullptr) {
        uint8_t status = msg[8];
        filter_status->publish_state(
            status == 0 ? "Ok" : (status == 1 ? "Full" : "Unknown"));
      }
      break;
    }
    case RES_GET_TEMPERATURES: {
      // comfort temperature
      target_temperature = (float)msg[0] / 2.0f - 20.0f;
      publish_state();

      // T1 / outside air
      if (outside_air_temperature != nullptr && msg[5] & 0x01) {
        outside_air_temperature->publish_state((float)msg[1] / 2.0f - 20.0f);
      }
      // T2 / supply air
      if (supply_air_temperature != nullptr && msg[5] & 0x02) {
        supply_air_temperature->publish_state((float)msg[2] / 2.0f - 20.0f);
      }
      // T3 / exhaust air
      if (return_air_temperature != nullptr && msg[5] & 0x04) {
        return_air_temperature->publish_state((float)msg[3] / 2.0f - 20.0f);
        current_temperature = (float)msg[3] / 2.0f - 20.0f;
      }
      // T4 / continued air
      if (exhaust_air_temperature != nullptr && msg[5] & 0x08) {
        exhaust_air_temperature->publish_state((float)msg[4] / 2.0f - 20.0f);
      }
      // EWT
      if (ewt_temperature != nullptr && msg[5] & 0x10) {
        ewt_temperature->publish_state((float)msg[6] / 2.0f - 20.0f);
      }
      // reheating
      if (reheating_temperature != nullptr && msg[5] & 0x20) {
        reheating_temperature->publish_state((float)msg[7] / 2.0f - 20.0f);
      }
      // kitchen hood
      if (kitchen_hood_temperature != nullptr && msg[5] & 0x40) {
        kitchen_hood_temperature->publish_state((float)msg[8] / 2.0f - 20.0f);
      }

      break;
    }
    case RES_GET_STATUS: {
      Preferences preferences{false, false, false, false,
                              false, false, false, false};
      preferences.preheating_present = msg[0];
      preferences.bypass_present = msg[1];

      const char* type =
          msg[2] == 1 ? "Left" : (msg[2] == 2 ? "Right" : "Unknown");
      const char* size =
          msg[3] == 1 ? "Large" : (msg[3] == 2 ? "Small" : "Unknown");

      preferences.fireplace_present = msg[4] & 0x01;
      preferences.kitchen_hood_present = msg[4] & 0x02;
      preferences.postheating_present = msg[4] & 0x04;
      preferences.postheating_pwm_mode_present = msg[4] & 0x40;
      bool p10_active = msg[6] & 0x01;
      bool p11_active = msg[6] & 0x02;
      bool p12_active = msg[6] & 0x04;
      bool p13_active = msg[6] & 0x08;
      bool p14_active = msg[6] & 0x10;
      bool p15_active = msg[6] & 0x20;
      bool p16_active = msg[6] & 0x40;
      bool p17_active = msg[6] & 0x80;
      bool p18_active = msg[7] & 0x01;
      bool p19_active = msg[7] & 0x02;
      bool p90_active = msg[8] & 0x01;
      bool p91_active = msg[8] & 0x02;
      bool p92_active = msg[8] & 0x04;
      bool p93_active = msg[8] & 0x08;
      bool p94_active = msg[8] & 0x10;
      bool p95_active = msg[8] & 0x20;
      bool p96_active = msg[8] & 0x40;
      bool p97_active = msg[8] & 0x80;
      preferences.enthalpy_present = msg[9];
      preferences.ewt_present = msg[10];

      if (!save_preference_lockout_) {
        if (memcmp(&preferences, &preferences_, sizeof(Preferences)) != 0) {
          save_preference_lockout_ = true;
          if (pref_.save(&preferences)) {
            ESP_LOGW(TAG, "Updated preferences, you should reboot!");
          } else {
            // we don't want to repeatedly call save on flash memory.
            ESP_LOGW(TAG, "Unable to save prefs! this is not good!");
          }
        } else {
          ESP_LOGI(TAG, "Preferences are up to date!");
        }
      }

      break;
    }
    case RES_GET_OPERATION_HOURS: {
      if (level0_hours != nullptr) {
        level0_hours->publish_state((msg[0] << 16) | (msg[1] << 8) | msg[2]);
      }

      if (level1_hours != nullptr) {
        level1_hours->publish_state((msg[3] << 16) | (msg[4] << 8) | msg[5]);
      }

      if (level2_hours != nullptr) {
        level2_hours->publish_state((msg[6] << 16) | (msg[7] << 8) | msg[8]);
      }

      if (level3_hours != nullptr) {
        level3_hours->publish_state((msg[17] << 16) | (msg[18] << 8) | msg[19]);
      }

      if (frost_protection_hours != nullptr) {
        frost_protection_hours->publish_state((msg[9] << 8) | msg[10]);
      }

      if (bypass_open_hours != nullptr) {
        bypass_open_hours->publish_state((msg[13] << 8) | msg[14]);
      }

      if (preheating_hours != nullptr) {
        preheating_hours->publish_state((msg[11] << 8) | msg[12]);
      }

      if (filter_hours != nullptr) {
        filter_hours->publish_state((msg[15] << 8) | msg[16]);
      }

      break;
    }

    case RES_GET_PREHEATING_STATUS: {
      if (preheating_valve != nullptr) {
        std::string name_preheating_valve;
        switch (msg[0]) {
          case 0:
            name_preheating_valve = "Closed";
            break;

          case 1:
            name_preheating_valve = "Open";
            break;

          default:
            name_preheating_valve = "Unknown";
            break;
        }
        preheating_valve->publish_state(name_preheating_valve);
      }

      if (frost_protection_active != nullptr) {
        frost_protection_active->publish_state(msg[1] != 0);
      }

      if (preheating_state != nullptr) {
        preheating_state->publish_state(msg[2] != 0);
      }

      if (frost_protection_minutes != nullptr) {
        frost_protection_minutes->publish_state((msg[3] << 8) | msg[4]);
      }

      if (frost_protection_level != nullptr) {
        std::string name_frost_protection_level;
        switch (msg[5]) {
          case 0:
            name_frost_protection_level = "GuaranteedProtection";
            break;

          case 1:
            name_frost_protection_level = "HighProtection";
            break;

          case 2:
            name_frost_protection_level = "NominalProtection";
            break;

          case 3:
            name_frost_protection_level = "Economy";
            break;

          default:
            name_frost_protection_level = "Unknown";
            break;
        }
        frost_protection_level->publish_state(name_frost_protection_level);
      }

      break;
    }
    case RES_GET_TIME_DELAY: {
      if (bathroom_switch_on_delay_minutes != nullptr) {
        bathroom_switch_on_delay_minutes->publish_state(msg[0]);
      }

      if (bathroom_switch_off_delay_minutes != nullptr) {
        bathroom_switch_off_delay_minutes->publish_state(msg[1]);
      }

      if (l1_switch_off_delay_minutes != nullptr) {
        l1_switch_off_delay_minutes->publish_state(msg[2]);
      }

      if (boost_ventilation_minutes != nullptr) {
        boost_ventilation_minutes->publish_state(msg[3]);
      }

      if (filter_warning_weeks != nullptr) {
        filter_warning_weeks->publish_state(msg[4]);
      }

      if (rf_high_time_short_minutes != nullptr) {
        rf_high_time_short_minutes->publish_state(msg[5]);
      }

      if (rf_high_time_long_minutes != nullptr) {
        rf_high_time_long_minutes->publish_state(msg[6]);
      }

      if (extractor_hood_switch_off_delay_minutes != nullptr) {
        extractor_hood_switch_off_delay_minutes->publish_state(msg[7]);
      }

      break;
    }
  }
}

uint16_t ComfoAirComponent::get_uint16_(uint8_t start_index) const {
  return (uint16_t(data_[COMMAND_LEN_HEAD + start_index + 1] |
                   data_[COMMAND_LEN_HEAD + start_index] << 8));
}

}  // namespace comfoair
}  // namespace esphome
