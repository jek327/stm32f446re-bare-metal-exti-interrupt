/**
---

* Project : STM32 Bare-Metal EXTI Button Interrupt
* File    : main.c
* Author  : Vasanthavel Jeeva Kumararaja
* Date    : August 4, 2026
*
* Description:
* Demonstrates interrupt-driven GPIO input handling on the STM32F446RE.
* The on-board user push button connected to GPIOC Pin 13 is configured
* as an external interrupt source through the SYSCFG, EXTI, and NVIC
* peripherals. Each button press toggles the on-board LED connected to
* GPIOA Pin 5.
*
* All GPIO, SYSCFG, EXTI, and NVIC configuration is performed through
* direct register manipulation without using the STM32 HAL APIs.
*
* The application uses falling-edge detection because the on-board user
* button is active-low. When the button is pressed, EXTI13 generates an
* interrupt request and the CPU executes the EXTI15_10 interrupt handler.
*
* Learning Objectives:
* * Configure GPIO pins as inputs and outputs using bare-metal programming
* * Enable GPIO and SYSCFG peripheral clocks through the RCC
* * Map GPIOC Pin 13 to EXTI13 using SYSCFG_EXTICR4
* * Configure falling-edge interrupt detection using EXTI
* * Un-mask an EXTI interrupt line using the Interrupt Mask Register
* * Enable the EXTI15_10 interrupt through the NVIC
* * Write and execute an Interrupt Service Routine
* * Check and clear the EXTI pending flag
* * Toggle a GPIO output in response to an external interrupt
* * Understand the difference between polling and interrupt-driven design
*
* Current Limitations:
* * No software or hardware button debouncing is implemented
* * Mechanical button bounce may generate multiple interrupts from one press
* * The interrupt priority remains at its default value
* * The CPU remains in an empty main loop rather than entering a low-power mode
*
* GitHub:
* github.com/jek327

---
*/


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "button.h"


/* Function Prototypes -------------------------------------------------------*/
void GPIO_init(unsigned int clockEnable, unsigned int ioModeClear, unsigned int ioMode);
void EXTI15_10_IRQHandler(void);
void EXTI_Button_init(unsigned int extiConfigClear, unsigned int extiLine,
		unsigned int extiPin, unsigned int irq);


int main(void)
{
	// initialize LED (LD2)
	GPIO_init(0x00000001, 0x00000C00, 0x00000400);

	// initialize User button
	Button_Init();

	// initialize EXTI for button interrupt
	EXTI_Button_init(0x000000F0, 0x00000020, 0x00002000, 0x00000100);

	/* Infinite loop */
	while (1)
	{
		// can remain empty
	}
}

/**
 * unsigned int clockEnable -- address to enable clock
 * unsigned int ioModeClear -- address to clear specified Mode register bits
 * unsigned int ioMode      -- address to set Mode register bits
**/
void GPIO_init(unsigned int clockEnable, unsigned int ioModeClear, unsigned int ioMode)
{
	 // Enables clock register for GPIOA peripheral
	  RCC->AHB1ENR |= clockEnable;

	  // Clears I/O direction mode register
	  GPIOA->MODER &= ~(ioModeClear);

	  // Configures I/O direction mode for Port
	  GPIOA->MODER |= ioMode;
}

/**
 * unsigned int extiConfigClear -- address to clear EXTI13 mapping field
 * unsigned int extiLine        -- address to map EXTI13 mapping field
 * unsigned int extiPin         -- address for EXTI13 pin number
 * unsigned int irq   			-- address for IRQ inside ISER
 */
void EXTI_Button_init(unsigned int extiConfigClear, unsigned int extiLine,
		unsigned int extiPin, unsigned int irq)
{

	// Enable SYSCFG clock
	RCC->APB2ENR |= 0x00004000;

	// Clear EXTI13 field before setting
	SYSCFG->EXTICR[3] &= ~(extiConfigClear);

	// Maps EXTI13 to Port C
	SYSCFG->EXTICR[3] |= extiLine;

	// Un-masks EXTI13
	EXTI->IMR |= extiPin;

	// Enables falling-edge detection
	EXTI->FTSR |= extiPin;

	// Disables rising-edge detection
	EXTI->RTSR &= ~(extiPin);

	// Clear the pending register by setting it to 1
	EXTI->PR |= extiPin;

	// Enable IRQ40 using NVIC Interrupt Set-Enable register
	NVIC->ISER[1] |= irq;
}

// Once interrupt occurs, it executes this function to toggle LED
void EXTI15_10_IRQHandler(void)
{
	// checks if interrupt came from EXTI13
	if ((EXTI->PR & 0x00002000) != 0)
	{
		// clears EXTI13 flag
		EXTI->PR = 0x00002000;

		// toggles the LED
		GPIOA->ODR ^= (0x00000020);
	}

}

