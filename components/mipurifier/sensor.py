import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (CONF_HUMIDITY, CONF_ID, CONF_PM_2_5,
                           CONF_TEMPERATURE, DEVICE_CLASS_AQI,
                           DEVICE_CLASS_HUMIDITY, DEVICE_CLASS_PM25,
                           DEVICE_CLASS_TEMPERATURE, ICON_FLASH, ICON_GRAIN,
                           STATE_CLASS_MEASUREMENT, UNIT_CELSIUS, UNIT_EMPTY,
                           UNIT_MICROGRAMS_PER_CUBIC_METER, UNIT_PERCENT,
                           UNIT_WATT_HOURS)

from . import CONF_MIPURIFIER_ID, MiPurifier

DEPENDENCIES = ["mipurifier"]

CONF_FILTER_LIFE = "filter_life"

CONFIG_SCHEMA = cv.Schema(
        {
            cv.GenerateID(CONF_MIPURIFIER_ID): cv.use_id(MiPurifier),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_HUMIDITY,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_PM_2_5): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                icon=ICON_GRAIN,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_PM25,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_FILTER_LIFE): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                icon=ICON_FLASH,
                accuracy_decimals=0,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        },
    ).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
   
    var =  await cg.get_variable(config[CONF_MIPURIFIER_ID])
  
    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature_sensor(sens))
    if CONF_HUMIDITY in config:
        sens = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(var.set_humidity_sensor(sens))
    if CONF_PM_2_5 in config:
        sens = await sensor.new_sensor(config[CONF_PM_2_5])
        cg.add(var.set_airquality_sensor(sens))
    if CONF_FILTER_LIFE in config:
        sens = await sensor.new_sensor(config[CONF_FILTER_LIFE])
        cg.add(var.set_filter_life_sensor(sens))
    
