#include "pico/stdlib.h"

#define AC_SWEEP_POINTS 400             // this directly affects the resolution
#define DFT_ADC_SAMPLES 35              // this can be quite a small value
#define START_FREQ      3000            // PWM default min frequency is approx 2.9 kHz
#define END_FREQ        200000          // slightly less than 1/2 max ADC sample rate
#define CAPACITANCE     0.00000100      // (use a low loss capacitor)
#define PI              3.14159f

#define ADC_PIN         26
#define PWM_PIN         20

#define E6              1000000         // a million
#define E9              1000000000      // a billion