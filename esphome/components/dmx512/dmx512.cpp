#include "dmx512.h"
#include "esphome/core/log.h"

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

  const uint32_t now = millis();
  // Correct timing here considering sent frame
      if (this->should_update_ == true
	  && now - this->last_dmx512_transmission_ > 20) {

	this->start_tx_pin_->digital_write(true);

	delayMicroseconds(1000);

	this->start_tx_pin_->digital_write(false);

	delayMicroseconds(100);

	device_values[0] = 0;

	this->write_array(device_values, 513);

	last_dmx512_transmission_ = millis();
	this->should_update_ = false;
      }
}

void DMX512::dump_config() {
  ESP_LOGCONFIG(TAG, "DMX512:");
  this->check_uart_settings(9600, 2);
}
float DMX512::get_setup_priority() const {
  // After UART bus
  return setup_priority::BUS - 1.0f;
}

void DMX512::setup() {
  this->start_tx_pin_->setup();
  this->start_tx_pin_->digital_write(false);

  device_values[0] = 0;
  for(int i = 1; i < 512; i++)
    device_values[i] = 0;
}

void DMX512::write_channel(uint16_t channel, uint8_t value) {
  device_values[channel] = value;
  should_update_ = true;
}

void DMX512Output::write_state(float state) {
  uint16_t value = state * 0xffff;

  if(this->double_channel_) {
    universe_->write_channel(this->channel_, value >> 8);
    universe_->write_channel(this->channel_+1, value & 0xff);
  }
  else
    universe_->write_channel(this->channel_, (value >> 8));
}

}  // namespace dmx512
}  // namespace esphome
