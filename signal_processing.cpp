#include "signal_processing.h"
#include "pico/stdlib.h"
#include "dft_lookup_tables.h"
#include "math.h"
#include "global_defines.h"

LookupTables lt;

float SignalProcessing::ComputeDFTAtFreq(uint16_t buffer[], uint32_t frequency, uint32_t sample_rate) {
    const uint8_t number_of_dft = 2;

    float X_real = 0;
    float X_imag = 0;
    uint16_t frequency_bin = (frequency * (N / 2)) / (sample_rate / 2) + 1;
    
    for (int i = 0; i < number_of_dft; i++) { // run the DFT three times and take the sum, as the DFT is very precise but the other functions might not produce a frequency that lines up fully
        for (int n = 0; n < N; n++) {
            float b = (-2 * PI * n * (frequency_bin + i - (int)((float)number_of_dft / 2)) / this->N);
            float inputAtIndex = (float)(buffer[n] - 2048) / 4096.0f;
            X_real += inputAtIndex * lt.GetCos(b);
            X_imag += inputAtIndex * lt.GetSin(b);
        }
    }
    X_real /= number_of_dft;
    X_imag /= number_of_dft;
    return sqrt(X_real * X_real + X_imag * X_imag) * number_of_dft; // technically this should be divided by ((1/2) * DFT_ADC_SAMPLES), but for this purpose it does not really matter
}

uint16_t SignalProcessing::GetIndexWithMaxVal(float buffer[], uint16_t n) {
    float max_val = 0;
    uint16_t max_val_index;
    for (int i = 0; i < n; i++) {
        if (buffer[i] > max_val) {
            max_val = buffer[i];
            max_val_index = i;
        }
    }
    return max_val_index;
}

void SignalProcessing::ReduceNoise(float buffer[]) { // basically, if average of neighbors is more than 50% greater, make it the average.
    for (int i = 1; i < AC_SWEEP_POINTS - 1; i++) {
        float neighbors_average = (buffer[i - 1] + buffer[i + 1]) / 2;
        if (neighbors_average > (buffer[i] * 1.5f)) {
            buffer[i] = neighbors_average;
        }
    }
}

uint32_t SignalProcessing::GetScaledFreq(uint16_t frequency_bin) {
    double frequency = ((double) (END_FREQ - START_FREQ) / (AC_SWEEP_POINTS * AC_SWEEP_POINTS)) * (frequency_bin * frequency_bin) + START_FREQ;
    return (uint32_t) round(frequency);
}

float SignalProcessing::GetPeakValue(float buffer[]) {
    float maxVal = buffer[0];
    for (int i = 1; i < AC_SWEEP_POINTS; i++) {
        if (buffer[i] > maxVal) {
            maxVal = buffer[i];
        }
    }
    return maxVal;
}

float SignalProcessing::GetAverageValue(float buffer[]) {
    double accumulate = 0;
    for (int i = 0; i < AC_SWEEP_POINTS; i++) {
        accumulate += buffer[i];
    }
    return accumulate / AC_SWEEP_POINTS;
}