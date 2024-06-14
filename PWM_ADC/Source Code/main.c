#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "GPIO_F_Configure.h"
#include "ADC0_Configure.h"


// Define memory-mapped register addresses for ADC, GPIO, and PWM
#define ADC0_SSFIFO3       (*((volatile uint32_t *)0x400380A8U)) // ADC Sample Sequence Result FIFO

#define SYSCTL_RCGCGPIO    (*((volatile uint32_t *)0x400FE608U)) // Enable clock for GPIO
#define SYSCTL_RCGCPWM     (*((volatile uint32_t *)0x400FE640U)) // Enable clock for PWM
#define PWM_CTL            (*((volatile uint32_t *)0x40028040U)) // PWM Control
#define PWM0_LOAD          (*((volatile uint32_t *)0x40028050U)) // PWM Load
#define PWM0_CMPA0         (*((volatile uint32_t *)0x40028058U)) // PWM Compare A
#define PWM_ENABLE         (*((volatile uint32_t *)0x40028008U)) // PWM Enable

// Global variables for PWM parameters
int load = 16000;  // PWM load value
int start = 0;     // Initial PWM start value

int main(void)
{
    uint32_t ui32SysClock;

    // Set system clock frequency
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 20000000);

    // Enable ADC0 and GPIO peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Enable clock for GPIO Port F and PWM module
    SYSCTL_RCGCGPIO = 0xFFFFU; // Enable clock for all GPIO ports
    SYSCTL_RCGCPWM = 0x01U;    // Enable clock for PWM module

    // Configure the GPIO_F Port Function
    GPIO_F_Configure();

    // Configure the ADC0 Function
    ADC0_Configure();

    while(1)  // Infinite Loop
    {

        // Trigger ADC conversion
        ADCProcessorTrigger(ADC0_BASE, 3);

        // Wait for ADC conversion to complete
        while(!ADCIntStatus(ADC0_BASE, 3, false));

        // Clear ADC interrupt
        ADCIntClear(ADC0_BASE, 3);

        // Read ADC result
        int result = ADC0_SSFIFO3;

        // Calculate PWM duty cycle based on ADC result
        start = (result * 3.9);

        // Configure PWM settings
        PWM_CTL = 0x00U;       // Disable PWM
        PWM0_GENA0 = 0x8C;     // Configure PWM Generator A
        PWM0_LOAD = load;      // Set PWM frequency
        PWM0_CMPA0 = start;    // Set PWM duty cycle
        PWM_CTL = 1;           // Enable PWM
        PWM_ENABLE = 0x01;     // Enable PWM output

        // Delay
        SysCtlDelay(ui32SysClock / 12);

    }
}
