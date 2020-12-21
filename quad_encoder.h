#ifndef QUAD_ENCODER_H_
#define QUAD_ENCODER_H_

#include "mbed.h"

namespace quad_encoder
{
class QuadEncoder
{
 public:
  QuadEncoder(PinName pin_a, PinMode mode_a, PinName pin_b, PinMode mode_b);
  int32_t getPulses(void);

 private:
  InterruptIn interrupt_pin_a_;
  InterruptIn interrupt_pin_b_;
  volatile int32_t pulses_;
  volatile uint8_t enc_val_ = 0;

  void pinChangeISR(void);
};

}  // namespace quad_encoder

#endif  // QUAD_ENCODER_H_
