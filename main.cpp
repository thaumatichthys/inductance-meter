#include "stdio.h" // for printf
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/watchdog.h"
#include "math.h"

#include "peripheral_functions.h"
#include "signal_processing.h"
#include "dft_lookup_tables.h"
#include "global_defines.h"

uint16_t sample_buffer[DFT_ADC_SAMPLES];

float results_buffer[AC_SWEEP_POINTS];

PeripheralFunctions pf;
SignalProcessing sp;

uint32_t RunFrequencySweep() {
    const uint32_t freq_per_point = (uint32_t)((float)(END_FREQ - START_FREQ) / AC_SWEEP_POINTS);
    for (int i = 0; i < AC_SWEEP_POINTS; i++) {
        uint32_t freq = sp.GetScaledFreq(i); // Quadratic frequency sweep to even out the resolution across the sweep
        uint32_t sample_rate = round(freq * 2.5f);
        sample_rate = pf.ChangeSampleRate(sample_rate);
        freq = pf.ChangePWM(freq); // the pwm can't truly be any value, this compensates for that
        sleep_us(20);
        pf.SampleADC(sample_buffer, DFT_ADC_SAMPLES);
        results_buffer[i] = sp.ComputeDFTAtFreq(sample_buffer, freq, sample_rate);
        watchdog_update();
    }
    sp.ReduceNoise(results_buffer);
    sp.ReduceNoise(results_buffer);
    sp.ReduceNoise(results_buffer);
    sp.ReduceNoise(results_buffer);
    return round(sp.GetScaledFreq(sp.GetIndexWithMaxVal(results_buffer, AC_SWEEP_POINTS)));
}

double GetInductance() {
    // f = 1 / (2*pi*sqrt(L*C))
    // L = 1 / ((2*pi*f)^2 * C)
    uint32_t f = RunFrequencySweep();
    double L = 1.0f / ((2.0f * PI * f) * (2.0f * PI * f) * CAPACITANCE);
    printf("\nFr = %d\n", f);
    return L;
}

int main() {
    
    stdio_init_all(); // for printf

    watchdog_enable(500, true); // auto restart in case of a crash

    /*
        Inductance meter: Perform an AC sweep, and DFT the bin at each bin.
        Then find the peak. Pretty simple in theory.
    */

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    pf.InitADC(ADC_PIN, 500000);
    pf.InitPWM(PWM_PIN);

    while(1) { // the "GUI" needs some more work
        gpio_put(25, 1);
        sleep_ms(20);
        gpio_put(25, 0);

        watchdog_update();

        ///*
        // see the calculated inductance
        printf("Inductance: %fuH\n", (GetInductance() * E6));
        printf("Capacitance = %fnF\n", CAPACITANCE * E9);
        printf("DFT values: %f average, %f peak\n", sp.GetAverageValue(results_buffer), sp.GetPeakValue(results_buffer));
        //*/
        
        /*
        // use with arduino IDE's serial plotter or something similar to see the signal strength curve
        RunFrequencySweep();
        for (int i = 0; i < AC_SWEEP_POINTS; i++) {
            printf("%f\n", results_buffer[i] * 100);
        }
        printf("%d\n", -5);
        */
    }
}