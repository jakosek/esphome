import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins, automation
from esphome.components import uart
from esphome.const import CONF_ID, CONF_TX_PIN
from esphome.core import CORE, coroutine

DEPENDENCIES = ['uart']

dmx512_ns = cg.esphome_ns.namespace('dmx512')
DMX512 = dmx512_ns.class_('DMX512', cg.Component, uart.UARTDevice)
MULTI_CONF = True

CONF_DMX512_ID = 'dmx512_id'

def to_code(config):
    cg.add_global(dmx512_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    yield uart.register_uart_device(var, config)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DMX512),
}).extend(cv.COMPONENT_SCHEMA).extend(uart.UART_DEVICE_SCHEMA)
