#pragma once

#include "esphome.h"
#include "esphome/components/button/button.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/climate/climate_mode.h"
#include "esphome/components/climate/climate_traits.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/application.h"
#include "esphome/core/component.h"
#include "registers.h"
#include "sensors.h"

namespace esphome {
namespace comfoair {

using binary_sensor::BinarySensor;
using button::Button;
using sensor::Sensor;
using text_sensor::TextSensor;

static const char* TAG = "comfoair";

struct Preferences {
  bool preheating_present = false;
  bool bypass_present = false;
  bool fireplace_present = false;
  bool kitchen_hood_present = false;
  bool postheating_present = false;
  bool postheating_pwm_mode_present = false;
  bool enthalpy_present = false;
  bool ewt_present = false;
};

class ComfoAirButton;

class ComfoAirComponent : public climate::Climate,
                          public PollingComponent,
                          public uart::UARTDevice {
 public:
  ComfoAirComponent() : Climate(), PollingComponent(1000), UARTDevice() {}

  climate::ClimateTraits traits() override;

  void control(const climate::ClimateCall& call) override;

  void dump_config() override;

  float get_setup_priority() const override;

  void setup() override;

  void loop() override;

  void update() override;

  void handle_action(const std::string& action);

 private:
  Sensor* make_sensor_(const char* id, const char* name, const char* unit,
                       int8_t accuracy, const char* device_class);
  BinarySensor* make_binary_sensor_(const char* id, const char* name);
  TextSensor* make_text_sensor_(const char* id, const char* name);
  ComfoAirButton* make_button_(const char* id, const char* name);

  void set_level_(int level);
  void set_comfort_temperature_(float temperature);

  void write_command_(const uint8_t command, const uint8_t* command_data,
                      uint8_t command_data_length);
  void write_escaped_byte_(uint8_t value);
  uint8_t comfoair_checksum_(uint8_t command, uint8_t length,
                             const uint8_t* command_data) const;
  optional<bool> check_byte_() const;
  void parse_data_();

  uint16_t get_uint16_(uint8_t start_index) const;

  uint8_t data_[30];
  uint8_t data_index_{0};
  int8_t update_counter_{-4};
  const int8_t num_update_counter_elements_{9};

  uint8_t bootloader_version_[13]{0};
  uint8_t firmware_version_[13]{0};
  uint8_t connector_board_version_[14]{0};

  ESPPreferenceObject pref_;
  Preferences preferences_;
  bool save_preference_lockout_ = false;

#define V(NAME, ...) Sensor* NAME = nullptr;
  SENSOR_DEFINITIONS(V);
#undef V
#define V(NAME, ...) BinarySensor* NAME = nullptr;
  BINARY_SENSOR_DEFINITIONS(V);
#undef V
#define V(NAME, ...) TextSensor* NAME = nullptr;
  TEXT_SENSOR_DEFINITIONS(V);
#undef V

#define V(CONF, NAME, ...) Sensor* NAME = nullptr;
  SENSOR_DEFINITIONS_OPT(V);
#undef V
#define V(CONF, NAME, ...) BinarySensor* NAME = nullptr;
  BINARY_SENSOR_DEFINITIONS_OPT(V)
#undef V
#define V(CONF, NAME, ...) TextSensor* NAME = nullptr;
  TEXT_SENSOR_DEFINITIONS_OPT(V);
#undef V
};

class ComfoAirButton : public Button {
 public:
  void press_action() override {
    if (parent_ && action_ != "") {
      parent_->handle_action(action_);
    }
  };
  void set_parent(ComfoAirComponent* parent) { parent_ = parent; }
  void set_action(const std::string& action) { action_ = action; }

 private:
  ComfoAirComponent* parent_ = nullptr;
  std::string action_;
};

}  // namespace comfoair
}  // namespace esphome
