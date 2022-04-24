#include "pico/stdlib.h"

#define AC_SWEEP_POINTS 300
#define DFT_ADC_SAMPLES 100
#define START_FREQ      200             // PWM default min frequency is approx 2.9 kHz
#define END_FREQ        200000          // slightly less than 1/2 max ADC sample rate
#define CAPACITANCE     0.0000003       // 400nF (use a low loss capacitor)
#define PI              3.14159f

#define ADC_PIN         26
#define PWM_PIN         20