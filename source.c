#include "source.h"
#include "hardware/gpio.h"

// Registros Glitch Detector
#define GLITCH_BASE 0x40158000
#define GLITCH_ARM (*(volatile uint32_t*)(GLITCH_BASE + 0x00))
#define GLITCH_DISARM (*(volatile uint32_t*)(GLITCH_BASE + 0x04))
#define GLITCH_SENS (*(volatile uint32_t*)(GLITCH_BASE + 0x08))

// Configuración de sensibilidad
#define SENS_MIN   0xDE00FF00  // 00 en todos los detectores
#define SENS_MED   0xDE00AA55  // 01 en todos los detectores
#define SENS_MAX   0xDE0000FF  // 11 en todos los detectores

void configurar_glitch_detector() {
    // Configurar pines de control
    const uint8_t pines_config[] = {26, 27, 28};
    for(int i = 0; i < 3; i++) {
        gpio_init(pines_config[i]);
        gpio_set_dir(pines_config[i], GPIO_IN);
        gpio_pull_up(pines_config[i]);
    }

    // Leer estado de los pines
    bool pin26 = gpio_get(26);
    bool pin27 = gpio_get(27);
    bool pin28 = gpio_get(28);

    // Lógica de configuración
    if(!pin28) {  // Si GPIO28 a GND
        GLITCH_DISARM = 0xDCAF;  // Desactivar completamente
    } else {
        GLITCH_ARM = 0x0000;  // Activar detector

        if(!pin27) {         // Máxima sensibilidad
            GLITCH_SENS = SENS_MAX;
        }
        else if(!pin26) {    // Sensibilidad media
            GLITCH_SENS = SENS_MED;
        }
        else {               // Sensibilidad mínima (default)
            GLITCH_SENS = SENS_MIN;
        }
    }
}

int main() {
    stdio_init_all();
    configurar_glitch_detector();
    // Configuración LED
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Lógica principal
    uint32_t ciclo = 10000000;
    int estado = 0;

    while(true) {
        estado ^= 1;
        gpio_put(LED_PIN, estado);

        // Verificación redundante
        volatile int c1 = 0, c2 = 0, c3 = 0;
        for(uint32_t i = 0; i < ciclo; i++) {
            c1++; c2++; c3++;
        }

        if(c1 != ciclo || c2 != ciclo || c3 != ciclo) {
            ciclo = ciclo / 4;  // Acelerar parpadeo
        }
    }

    return 0;
}
