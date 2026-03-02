"""Creates module ComfoAir"""

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.cpp_generator import AssignmentExpression
from esphome.components import uart, climate, sensor, binary_sensor, text_sensor, button
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_SPEED,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLUME,
    STATE_CLASS_MEASUREMENT,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_CUBIC_METER,
    UNIT_PERCENT,
    UNIT_REVOLUTIONS_PER_MINUTE,
)

comfoair_ns = cg.esphome_ns.namespace("comfoair")
ComfoAirComponent = comfoair_ns.class_(
    "ComfoAirComponent", climate.Climate, cg.Component, uart.UARTDevice
)

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["climate", "sensor", "binary_sensor", "text_sensor", "button"]
REQUIRED_KEY_NAME = "name"

ComfoAirButton = comfoair_ns.class_("ComfoAirButton", button.Button)

entities = {
    "text_sensor": {
        "filter_status": text_sensor.text_sensor_schema(),
        "frost_protection_level": text_sensor.text_sensor_schema(),
        "preheating_valve": text_sensor.text_sensor_schema(),
    },
    "binary_sensor": {
        "frost_protection_active": binary_sensor.binary_sensor_schema(),
        "bypass_valve_open": binary_sensor.binary_sensor_schema(),
        "preheating_state": binary_sensor.binary_sensor_schema(),
        "summer_mode": binary_sensor.binary_sensor_schema(),
        "supply_fan_active": binary_sensor.binary_sensor_schema(),
    },
    "sensor": {
        "intake_fan_speed": sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "exhaust_fan_speed": sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "intake_fan_speed_rpm": sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_REVOLUTIONS_PER_MINUTE,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "exhaust_fan_speed_rpm": sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_REVOLUTIONS_PER_MINUTE,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "ventilation_level": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "outside_air_temperature": sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "supply_air_temperature": sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "return_air_temperature": sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "exhaust_air_temperature": sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "enthalpy_temperature": sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "ewt_temperature": sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "reheating_temperature": sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "kitchen_hood_temperature": sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "return_air_level": sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            unit_of_measurement=UNIT_CUBIC_METER,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "supply_air_level": sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            unit_of_measurement=UNIT_CUBIC_METER,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "bypass_factor": sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "bypass_step": sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "bypass_correction": sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "bypass_open_hours": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "motor_current_bypass": sensor.sensor_schema(
            device_class=DEVICE_CLASS_CURRENT,
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "motor_current_preheating": sensor.sensor_schema(
            device_class=DEVICE_CLASS_CURRENT,
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "preheating_hours": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "level0_hours": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "level1_hours": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "level2_hours": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "level3_hours": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "frost_protection_hours": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "frost_protection_minutes": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="min",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "filter_hours": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "bypass_valve": sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "bathroom_switch_on_delay_minutes": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "bathroom_switch_off_delay_minutes": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "l1_switch_off_delay_minutes": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "boost_ventilation_minutes": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "filter_warning_weeks": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="weeks",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "rf_high_time_short_minutes": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "rf_high_time_long_minutes": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        "extractor_hood_switch_off_delay_minutes": sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    },
    "button": {
        "filter_reset": button.button_schema(ComfoAirButton),
        "error_reset": button.button_schema(ComfoAirButton),
    },
}


CONFIG_SCHEMA = (
    climate.climate_schema(ComfoAirComponent)
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(
        {
            cv.Required(REQUIRED_KEY_NAME): cv.string,
        }
    )
    .extend(
        {
            cv.Optional("entities", default={}): {
                cv.Optional(kn, default={"id": kn}): vn
                for v in entities.values()
                for (kn, vn) in v.items()
            }
        }
    )
)


def to_code(config):
    """Generates code"""
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)
    yield climate.register_climate(var, config)

    for kind, values in entities.items():
        for k in values:
            sconf = config["entities"][k]
            sconf["internal"] = False

            if kind == "sensor":
                entity = yield sensor.new_sensor(sconf)
            elif kind == "binary_sensor":
                entity = yield binary_sensor.new_binary_sensor(sconf)
            elif kind == "text_sensor":
                entity = yield text_sensor.new_text_sensor(sconf)
            elif kind == "button":
                entity = yield button.new_button(sconf)
                cg.add(entity.set_parent(var))
                continue

            if entity is not None:
                cg.add(
                    AssignmentExpression(None, None, f"{config[CONF_ID]}->{k}", entity)
                )
