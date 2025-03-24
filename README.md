# Pico2_RP2350_EMFI_glitch_target

With this simple program, you can use a Raspberry Pi Pico2 (RP2350) as a target for EMFI (Electromagnetic Fault Injection) glicth test with a tool like the PicoEMP, FaultyCat, Faultier, etc etc.

Initially the Raspberry Pi Pico's LED will blink slowly.
    
You can then test your EMFI tool on the RP2350, e.g. in the "pin 1" corner that's marked with a dot.

* If fault injection is successful, the LED will blink faster.
* If fault injection causes the RP20240 to hang, the LED will permanently stay on or off.

After the test, just power cycle the Pi Pico.

---

To compile run:
  `cmake -S . -B build -DPICO_BOARD=pico2 && cd build && make`

Short video of the target being attacked by a PicoEMP: https://youtu.be/5uhnqSTVd94
