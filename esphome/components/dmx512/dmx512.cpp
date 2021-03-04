#include "dmx512.h"
#include "uart_register.h"
#include "esphome/core/log.h"

/* UART for DMX output */
#define SEROUT_UART 1

/* DMX minimum timings per E1.11 */
#define DMX_BREAK 92
#define DMX_MAB 12

namespace esphome {
namespace dmx512 {

static const char *TAG = "dmx512";

enum DMX512TransmissionState {
  PACKET_IDLE = 0,
  PACKET_START_LOW,
  PACKET_START_HIGH,
  PACKET_SENDING_FRAMES
};

void DMX512::loop() {
  static enum DMX512TransmissionState state = PACKET_IDLE;
    this->sendBreak();
	device_values[0] = 0;
	this->write_array(device_values, 513);
}

void DMX512::sendBreak() {
   SET_PERI_REG_MASK(UART_CONF0(SEROUT_UART), UART_TXD_BRK);
   delayMicroseconds(DMX_BREAK);
   CLEAR_PERI_REG_MASK(UART_CONF0(SEROUT_UART), UART_TXD_BRK);
   delayMicroseconds(DMX_MAB);
}

void DMX512::dump_config() {
  ESP_LOGCONFIG(TAG, "DMX512:");
  this->check_uart_settings(250000, 2);
}

float DMX512::get_setup_priority() const {
  // After UART bus
  return setup_priority::BUS - 1.0f;
}

void DMX512::setup() {
  device_values[0] = 0;
  for(int i = 1; i <= 512; i++)
    device_values[i] = 0;
}

void DMX512::write_channel(uint16_t channel, uint8_t value) {
  device_values[channel] = value;
  should_update_ = true;
}

void DMX512Output::write_state(float state) {
  uint16_t value = state * 0xffff;
  universe_->write_channel(this->channel_, (value >> 8));
}

}  // namespace dmx512
}  // namespace esphome
