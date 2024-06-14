#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

#define GPIOF_DIR          (*((volatile uint32_t *)0x4005D400U)) // GPIO Direction
#define GPIOF_DEN          (*((volatile uint32_t *)0x4005D51CU)) // GPIO Digital Enable
#define GPIOF_AFSEL        (*((volatile uint32_t *)0x4005D420U)) // GPIO Alternate Function Select
#define GPIOF_PCTL         (*((volatile uint32_t *)0x4005D52CU)) // GPIO Port Control

void GPIO_F_Configure()
{
    // Initialize PF0 as digital output pins
    GPIOF_DIR |= 0x01;  // Set PF0  digital output pins
    GPIOF_DEN |= 0x01;  // Enable PF0 as digital pins

    // Configure GPIO Port F for PWM output
    GPIOF_DIR = 0x01U;       // Set PF0 as output
    GPIOF_AFSEL = 0x01U;     // Enable alternate function on PF0
    GPIOF_PCTL = 0x06U;      // Configure PF0 for PWM output
    GPIOF_DEN = 0x01U;       // Enable digital I/O on PF0
}
