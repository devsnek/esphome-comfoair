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
        ("Filter Status", "filter_status"): text_sensor.text_sensor_schema(),
        (
            "Frost Protection Level",
            "frost_protection_level",
        ): text_sensor.text_sensor_schema(),
        ("Preheating Valve", "preheating_valve"): text_sensor.text_sensor_schema(),
    },
    "binary_sensor": {
        (
            "Frost Protection Active",
            "frost_protection_active",
        ): binary_sensor.binary_sensor_schema(),
        (
            "Bypass Valve Open",
            "bypass_valve_open",
        ): binary_sensor.binary_sensor_schema(),
        ("Preheating State", "preheating_state"): binary_sensor.binary_sensor_schema(),
        ("Summer Mode", "summer_mode"): binary_sensor.binary_sensor_schema(),
        (
            "Supply Fan Active",
            "supply_fan_active",
        ): binary_sensor.binary_sensor_schema(),
    },
    "sensor": {
        ("Intake Fan Speed", "intake_fan_speed"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Exhaust Fan Speed", "exhaust_fan_speed"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Intake Fan Speed RPM", "intake_fan_speed_rpm"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_REVOLUTIONS_PER_MINUTE,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Exhaust Fan Speed RPM", "exhaust_fan_speed_rpm"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_REVOLUTIONS_PER_MINUTE,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Ventilation Level", "ventilation_level"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Outside Air Temperature", "outside_air_temperature"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Supply Air Temperature", "supply_air_temperature"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Return Air Temperature", "return_air_temperature"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Exhaust Air Temperature", "exhaust_air_temperature"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Enthalpy Temperature", "enthalpy_temperature"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("EWT Temperature", "ewt_temperature"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Reheating Temperature", "reheating_temperature"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Kitchen Hood Temperature", "kitchen_hood_temperature"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Return Air Level", "return_air_level"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            unit_of_measurement=UNIT_CUBIC_METER,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Supply Air Level", "supply_air_level"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            unit_of_measurement=UNIT_CUBIC_METER,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Bypass Factor", "bypass_factor"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Bypass Step", "bypass_step"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Bypass Correction", "bypass_correction"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Bypass Open Hours", "bypass_open_hours"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Motor Current Bypass", "motor_current_bypass"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_CURRENT,
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Motor Current Preheating", "motor_current_preheating"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_CURRENT,
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Preheating Hours", "preheating_hours"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Level 0 Hours", "level0_hours"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Level 1 Hours", "level1_hours"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Level 2 Hours", "level2_hours"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Level 3 Hours", "level3_hours"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Frost Protection Hours", "frost_protection_hours"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Frost Protection Minutes", "frost_protection_minutes"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="min",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Filter Hours", "filter_hours"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="h",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Bypass Valve", "bypass_valve"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        (
            "Bathroom Switch On Delay Minutes",
            "bathroom_switch_on_delay_minutes",
        ): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        (
            "Bathroom Switch Off Delay Minutes",
            "bathroom_switch_off_delay_minutes",
        ): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        (
            "L1 Switch Off Delay Minutes",
            "l1_switch_off_delay_minutes",
        ): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        (
            "Boost Ventilation Minutes",
            "boost_ventilation_minutes",
        ): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        ("Filter Warning Weeks", "filter_warning_weeks"): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement="weeks",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        (
            "RF High Time Short Minutes",
            "rf_high_time_short_minutes",
        ): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        (
            "RF High Time Long Minutes",
            "rf_high_time_long_minutes",
        ): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        (
            "Extractor Hood Switch Off Delay Minutes",
            "extractor_hood_switch_off_delay_minutes",
        ): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    },
    "button": {
        ("Filter Reset", "filter_reset"): button.button_schema(ComfoAirButton),
        ("Error Reset", "error_reset"): button.button_schema(ComfoAirButton),
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
                cv.Optional(kn, default={"id": kn, "name": dn}): vn
                for v in entities.values()
                for ((dn, kn), vn) in v.items()
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
        for d, k in values:
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
