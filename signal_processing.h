#include "pico/stdlib.h"
#include "global_defines.h"

class SignalProcessing {
private:
    uint16_t freq_bin;
    int32_t cos_b[DFT_ADC_SAMPLES];
    int32_t sin_b[DFT_ADC_SAMPLES];
    static const uint16_t N = DFT_ADC_SAMPLES;
public:
    float ComputeDFTAtFreq(uint16_t buffer[], uint32_t frequency, uint32_t sample_rate);
    uint16_t GetIndexWithMaxVal(float buffer[], uint16_t n);
    void ReduceNoise(float buffer[]);
    uint32_t GetScaledFreq(uint16_t frequency_bin);
    float GetPeakValue(float buffer[]);
    float GetAverageValue(float buffer[]);
};