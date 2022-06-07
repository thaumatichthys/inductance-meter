# inductance-meter
    inductance meter which works by performing a frequency sweep to find the resonant frequency


# Usage
    Connect the circuit according to the schematic diagram
    Note that C1 and C3 do not need to be exactly 2.2uF, anything within an order of magnitude of that should work
    The tank capacitor's value may be adjusted to a different value to ensure the resonant frequency is less than END_FREQ and greater than START_FREQ
    This program lacks a proper GUI at the moment, so just edit the print lines. If you are using the calculated inductance, make sure to change the 
    CAPACITANCE value in global_defines.h to match that of your tank capacitor, or else the inductance will be wrong