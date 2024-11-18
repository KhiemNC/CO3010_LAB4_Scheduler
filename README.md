
# MICROCONTROLLER - MICROPROCESSOR, LABORATORY 4

This is the fourth lab to get familiar with Cooperative Scheduler.


## 1. Schematic for this lab

I simulated the source code on an STM32F103C6 using Proteus 8. The schematic looks like this:

![Proteus Schematic](./images/Proteus_Schematic_LAB4.png)

The pin assignments are listed in the table below:

| PIN      | PIN LABEL      | PIN      | PIN LABEL      |
|:---------|:---------------|:---------|:---------------|
| PA0      | not in use          | PB0      | not in use     |
| PA1      | not in use          | PB1      | not in use     |
| PA2      | not in use          | PB2      | not in use     |
| PA3      | not in use          | PB3      | not in use     |
| PA4      | not in use          | PB4      | not in use      |
| PA5      | LED_RED_1           | PB5      | not in use      |
| PA6      | LED_RED_2           | PB6      | not in use      |
| PA7      | LED_RED_3           | PB7      | not in use      |
| PA8      | LED_RED_4           | PB8      | not in use      |
| PA9      | LED_RED_5           | PB9      | not in use      |
| PA10     | not in use          | PB10     | not in use     |
| PA11     | not in use          | PB11     | not in use     |
| PA12     | not in use          | PB12     | BTN_0          |
| PA13     | not in use          | PB13     | BTN_1          |
| PA14     | not in use          | PB14     | BTN_2          |
| PA15     | not in use          | PB15     | not in use     |

## 2. How to run the exercises source code?

1. Ensure that all hardware connections are correct as described above. The timer is configured with prescaler = 7999, counter = 9 so the TICK is 10 ms.

2. Clone this Project and Run.