import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select

from .. import CONF_MIPURIFIER_ID, MiPurifier
from esphome.const import CONF_ID

DEPENDENCIES = ["mipurifier", "select"]


CONF_ROLE = "role"
CONF_SCREEN_BRIGHTNESS = "screen_brightness"
CONF_MODE = "mode"

OPTIONS = {
    CONF_SCREEN_BRIGHTNESS: ["off", "low", "high"],
    CONF_MODE: ["auto", "sleep", "favorite", "night", "low", "medium", "high"]
}

MiPurifierSelect = cg.global_ns.class_(
   "MiPurifierSelect", select.Select, cg.Component)

MiPurifierSelectRole = cg.global_ns.enum("MiPurifierSelectRole", True);

CONFIG_SCHEMA = select.select_schema(
    MiPurifierSelect
).extend(
    cv.Schema(
        {
            cv.GenerateID(CONF_MIPURIFIER_ID): cv.use_id(MiPurifier),
            cv.Required(CONF_ROLE): cv.enum({
                CONF_SCREEN_BRIGHTNESS: MiPurifierSelectRole.SCREEN_BRIGHTNESS,
                CONF_MODE: MiPurifierSelectRole.MODE,
            }),
        }
    )
)



async def to_code(config):
    mp_var =  await cg.get_variable(config[CONF_MIPURIFIER_ID])
    var = cg.new_Pvariable(config[CONF_ID], mp_var, config[CONF_ROLE])
    await cg.register_component(var, config)
    await select.register_select(var, config, options=OPTIONS[config[CONF_ROLE]])

    cg.add(mp_var.set_select(var))


