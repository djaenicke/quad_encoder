
#include "quad_encoder.h"
#include <cstdint>

namespace quad_encoder {

static const int8_t lookup_table[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

QuadEncoder::QuadEncoder(PinName pin_a, PinMode mode_a, PinName pin_b, PinMode mode_b):
  interrupt_pin_a_(pin_a, mode_a),
  interrupt_pin_b_(pin_b, mode_b) {
  pulses_ = 0;

  // Attach the PinChangeISR to both channels rise and fall to get 4x resolution
  interrupt_pin_a_.rise(Callback<void()>(this, &QuadEncoder::PinChangeISR));
  interrupt_pin_a_.fall(Callback<void()>(this, &QuadEncoder::PinChangeISR));
  
  interrupt_pin_b_.rise(Callback<void()>(this, &QuadEncoder::PinChangeISR));
  interrupt_pin_b_.fall(Callback<void()>(this, &QuadEncoder::PinChangeISR));

  interrupt_pin_a_.enable_irq();
  interrupt_pin_b_.enable_irq();
}

void QuadEncoder::PinChangeISR(void) {  
  enc_val_ = enc_val_ << 2;
  enc_val_ = enc_val_ | (interrupt_pin_a_ << 1) | interrupt_pin_b_;
  pulses_ += lookup_table[enc_val_ & 0b1111];
}

int32_t QuadEncoder::GetPulses(void) {
  interrupt_pin_a_.disable_irq();
  interrupt_pin_b_.disable_irq();
  const int32_t temp = pulses_;
  pulses_ = 0;
  interrupt_pin_a_.enable_irq();
  interrupt_pin_b_.enable_irq(); 

  return temp;
}

}  // namespace quad_encoder
