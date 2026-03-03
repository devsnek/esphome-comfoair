"""Creates module ComfoAir"""

from pathlib import Path

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, climate
from esphome.const import CONF_ID
from esphome.core import CORE

comfoair_ns = cg.esphome_ns.namespace("comfoair")
ComfoAirComponent = comfoair_ns.class_(
    "ComfoAirComponent", climate.Climate, cg.Component, uart.UARTDevice
)

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["climate", "sensor", "binary_sensor", "text_sensor", "button"]
REQUIRED_KEY_NAME = "name"

CONFIG_SCHEMA = (
    climate.climate_schema(ComfoAirComponent)
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(
        {
            cv.Required(REQUIRED_KEY_NAME): cv.string,
        }
    )
)


def count_entities():
    inc = (Path(__file__).parent / "sensors.h").read_text()
    state = ""
    for line in inc.splitlines():
        s = line.strip()
        if s.startswith("#define"):
            state = s.split(" ")[1].split("_DEFINITIONS")[0].lower()

        if s.startswith("V"):
            CORE.platform_counts[state] += 1


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)
    yield climate.register_climate(var, config)

    count_entities()
