# STM32 Bare-Metal EXTI Interrupt

A bare-metal implementation of external interrupts (EXTI) on the STM32F446RE using direct register manipulation. This project demonstrates interrupt-driven GPIO input by configuring the onboard user button (PC13) to generate an external interrupt that toggles the onboard LED (PA5).

Unlike polling-based applications, the CPU remains idle until a button press generates an interrupt request.

---

## Features

- Bare-metal GPIO programming
- External interrupt (EXTI) configuration
- Interrupt-driven button handling
- SYSCFG EXTI line mapping
- NVIC interrupt configuration
- Falling-edge interrupt detection
- Active-low button support
- LED toggle using an Interrupt Service Routine (ISR)
- Direct register manipulation using CMSIS

---

## Hardware

- STM32 Nucleo-F446RE
- ARM Cortex-M4
- Onboard LED (GPIOA Pin 5)
- User Push Button (GPIOC Pin 13)

---

## Software

- STM32CubeIDE
- CMSIS
- Embedded C

---

## Project Structure

```
Core/
│
├── Inc/
│   ├── button.h
│   └── main.h
│
├── Src/
│   ├── button.c
│   └── main.c
```

---

## How It Works

1. Configure PA5 as a GPIO output.
2. Configure PC13 as a GPIO input.
3. Enable the SYSCFG peripheral clock.
4. Map GPIOC Pin 13 to EXTI13.
5. Unmask EXTI13.
6. Enable falling-edge interrupt detection.
7. Enable the EXTI15_10 interrupt through the NVIC.
8. Wait in the main loop.
9. When the user button is pressed, the CPU executes the interrupt handler.
10. The interrupt handler clears the pending flag and toggles the onboard LED.

---

## Interrupt Flow

```
Button Press
      │
      ▼
GPIOC Pin 13
      │
      ▼
SYSCFG
      │
      ▼
EXTI13
      │
      ▼
Interrupt Mask Register
      │
      ▼
Pending Register
      │
      ▼
NVIC
      │
      ▼
CPU
      │
      ▼
EXTI15_10_IRQHandler()
      │
      ▼
Toggle LED
```

---

## Registers Used

| Register | Purpose |
|----------|---------|
| RCC->AHB1ENR | Enable GPIOA and GPIOC clocks |
| RCC->APB2ENR | Enable SYSCFG clock |
| GPIOA->MODER | Configure PA5 as output |
| GPIOA->ODR | Toggle onboard LED |
| GPIOC->MODER | Configure PC13 as input |
| SYSCFG->EXTICR[3] | Map PC13 to EXTI13 |
| EXTI->IMR | Unmask EXTI13 |
| EXTI->FTSR | Enable falling-edge detection |
| EXTI->RTSR | Disable rising-edge detection |
| EXTI->PR | Clear interrupt pending flag |
| NVIC->ISER[1] | Enable EXTI15_10 interrupt |

---

## Interrupt Handler

```c
void EXTI15_10_IRQHandler(void)
{
    if ((EXTI->PR & (1U << 13)) != 0U)
    {
        EXTI->PR = (1U << 13);

        GPIOA->ODR ^= (1U << 5);
    }
}
```

---

## Concepts Learned

- Memory-mapped I/O
- GPIO input/output configuration
- Peripheral clock control
- System Configuration Controller (SYSCFG)
- External Interrupt Controller (EXTI)
- Nested Vector Interrupt Controller (NVIC)
- Interrupt Service Routines (ISR)
- Interrupt masking
- Falling-edge detection
- Pending interrupt flags
- Active-low button logic
- Event-driven programming
- Bare-metal firmware architecture

---

## Current Limitations

- No button debouncing
- Mechanical switch bounce may generate multiple interrupts
- Uses default interrupt priority
- Main loop remains idle instead of entering a low-power sleep mode

---

## Future Improvements

- Software debouncing using SysTick
- Interrupt priorities
- Low-power sleep mode (WFI)
- Long-press detection
- Double-click detection
- Multiple external interrupt sources

---

![Demo](button.gif)

---

## Author

**Vasanthavel Jeeva Kumararaja**

GitHub: https://github.com/jek327
