#include "peripheral_functions.h"
#include "pico/stdlib.h"
#include "stdio.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/dma.h"
#include "global_defines.h"

#define SYSTEM_CLK 125000000U

void PeripheralFunctions::InitPWM(uint pin) {
    gpio_init(pin);
    gpio_set_function(pin, GPIO_FUNC_PWM);
    this->pwm_slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(this->pwm_slice_num, 499); // some arbitrary default value
    pwm_set_chan_level(this->pwm_slice_num, PWM_CHAN_A, 250);
    pwm_set_clkdiv(this->pwm_slice_num, 40.0f);
    //pwm_set_enabled(this->pwm_slice_num, true);
}

void PeripheralFunctions::ChangePWM(uint32_t frequency) {
    uint32_t pwm_wrap_value = SYSTEM_CLK / (frequency * 40.0f);
    pwm_set_enabled(this->pwm_slice_num, false);
    pwm_set_wrap(this->pwm_slice_num, pwm_wrap_value - 1);
    pwm_set_chan_level(this->pwm_slice_num, PWM_CHAN_A, pwm_wrap_value / 2);
    pwm_set_enabled(this->pwm_slice_num, true);
}

void PeripheralFunctions::InitADC(uint pin, uint32_t sample_rate) {
    if (pin < 26 || pin > 28) {
        while(1) {
            printf("bruhbruhbruh yer adc pin isn't!\n");
        }
    }
    this->adc_chan = pin - 26;
    adc_gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    adc_init();
    adc_select_input(this->adc_chan);
    adc_fifo_setup (
        true,
        true,
        1,
        false,
        false
    );
    adc_set_clkdiv((48000000U / sample_rate) - 1);
    this->dma_chan = dma_claim_unused_channel(true);
    this->dma_cfg = dma_channel_get_default_config(this->dma_chan);
    channel_config_set_transfer_data_size(&this->dma_cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&this->dma_cfg, false);
    channel_config_set_write_increment(&this->dma_cfg, true);
    channel_config_set_dreq(&this->dma_cfg, DREQ_ADC);
}

uint32_t PeripheralFunctions::ChangeSampleRate(uint32_t sample_rate) {
    uint16_t divider = (48000000U / sample_rate);
    adc_set_clkdiv(divider - 1);
    return 48000000U / divider;
}

void PeripheralFunctions::SampleADC(uint16_t buffer[], uint n) {
    adc_fifo_drain();
    adc_run(false);
    dma_channel_configure (
        this->dma_chan,
        &this->dma_cfg,
        buffer,
        &adc_hw->fifo,
        n,
        true
    );
    adc_run(true);
    dma_channel_wait_for_finish_blocking(this->dma_chan);
}
