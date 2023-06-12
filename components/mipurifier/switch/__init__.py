import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch

from .. import CONF_MIPURIFIER_ID, MiPurifier
from esphome.const import CONF_ID

DEPENDENCIES = ["mipurifier", "switch"]


CONF_ROLE = "role"
CONF_BEEPER = "beeper"
CONF_LOCK = "lock"
CONF_POWER = "power"

MiPurifierSwitch = cg.global_ns.class_(
   "MiPurifierSwitch", switch.Switch, cg.Component)

MiPurifierSwitchRole = cg.global_ns.enum("MiPurifierSwitchRole", True);

CONFIG_SCHEMA = switch.switch_schema(
    MiPurifierSwitch
).extend(
    cv.Schema(
        {
            cv.GenerateID(CONF_MIPURIFIER_ID): cv.use_id(MiPurifier),
            cv.Required(CONF_ROLE): cv.enum({
                CONF_BEEPER: MiPurifierSwitchRole.BEEPER,
                CONF_LOCK: MiPurifierSwitchRole.LOCK,
                CONF_POWER: MiPurifierSwitchRole.POWER,
            }),
        }
    )
)



async def to_code(config):
    print("CONFIG: ", config)
    mp_var =  await cg.get_variable(config[CONF_MIPURIFIER_ID])
    var = cg.new_Pvariable(config[CONF_ID], mp_var, config[CONF_ROLE])
    await cg.register_component(var, config)
    await switch.register_switch(var, config)

    cg.add(mp_var.set_switch(var))
