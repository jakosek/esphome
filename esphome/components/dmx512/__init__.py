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
CONF_START_TX_PIN = 'start_tx_pin'

def to_code(config):
    cg.add_global(dmx512_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    pin = yield cg.gpio_pin_expression(config[CONF_START_TX_PIN])
    cg.add(var.set_start_tx_pin(pin))

    yield uart.register_uart_device(var, config)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DMX512),
    cv.Required(CONF_START_TX_PIN): pins.gpio_output_pin_schema,
}).extend(cv.COMPONENT_SCHEMA).extend(uart.UART_DEVICE_SCHEMA)

def dmx512_device_schema(default_address):
    schema = {
        cv.GenerateID(CONF_DMX512_ID): cv.use_id(DMX512),
    }
    return cv.Schema(schema)


@coroutine
def register_dmx512_device(var, config):
    parent = yield cg.get_variable(config[CONF_DMX512_ID])
    cg.add(var.set_parent(parent))
    cg.add(parent.register_device(var))
