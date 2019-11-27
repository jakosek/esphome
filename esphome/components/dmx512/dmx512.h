#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/output/float_output.h"

namespace esphome {
namespace dmx512 {

class DMX512Output;

class DMX512 : public uart::UARTDevice, public Component {
 public:
  DMX512() = default;

  void setup();

  void loop() override;

  void dump_config() override;

  float get_setup_priority() const override;

  void set_start_tx_pin(GPIOPin *start_tx_pin) { this->start_tx_pin_ = start_tx_pin; }

  void write_channel(uint16_t channel, uint8_t value);

 protected:

  std::vector<uint8_t> rx_buffer_;
  uint32_t last_dmx512_transmission_{0};
  uint8_t device_values[513];
  GPIOPin *start_tx_pin_;
  bool should_update_{false};
  //ISRInternalGPIOPin *start_tx_pin_{nullptr};
};

class DMX512Output : public output::FloatOutput, public Component {
public:
  void set_universe(DMX512 *universe) { this->universe_ = universe; }
  void set_channel(uint16_t channel) { this->channel_ = channel; }
  void set_double_channel(bool double_channel) { this->double_channel_ = double_channel; }
  void write_state(float state);

protected:
  bool double_channel_{false};
  uint16_t channel_;
  DMX512 *universe_;
};

}  // namespace dmx512
}  // namespace esphome
