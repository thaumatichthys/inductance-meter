#include "pico/stdlib.h"
#include "dft_lookup_tables.h"
#include "math.h"
#include "global_defines.h"

float LookupTables::GetCos(float x) {
    x = abs(x);
    x += PI / 2;
    uint32_t repetitions = (uint32_t) ((float)x / (PI * 2));
    x -= repetitions * 2 * PI;
    x /= PI * 2;
    x *= 8191; // Don't change this to 8192, it will be an off-by-one bug that renders the program entirely disfunctional.
    uint16_t index = (uint16_t) round(x);
    return sine_table[index];
}

float LookupTables::GetSin(float x) {
    bool positive = (x > 0);
    x = abs(x);
    uint32_t repetitions = (uint32_t) ((float)x / (PI * 2));
    x -= repetitions * 2 * PI;
    x /= PI * 2;
    x *= 8191;
    uint16_t index = (uint16_t) round(x);
    float result = sine_table[index];
    if (!positive) {
        result *= -1;
    }
    return result;
}