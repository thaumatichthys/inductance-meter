#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "global_defines.h"

class PeripheralFunctions {
private:
    uint pwm_slice_num;
    uint dma_chan;
    uint adc_chan;
    dma_channel_config dma_cfg;
public:
    void InitPWM(uint pin);
    uint32_t ChangePWM(uint32_t frequency);
    void InitADC(uint pin, uint32_t sample_rate);
    void SampleADC(uint16_t buffer[], uint n);
    uint32_t ChangeSampleRate(uint32_t sample_rate);
};