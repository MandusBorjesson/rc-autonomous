# rc-autonomous
Modified RC car capable of driving autonomously using and array of sensors and some clever 1's and 0's.


# Hardware usage

|       |         distance_sensor |         main |         motor |         servo |         spi |         uart |
|-------|:-----------------------:|:------------:|:-------------:|:-------------:|:-----------:|:------------:|
| ADC   | Left sensor <br> <code>CH8</code> |    |               |               |             |              |
| DAC1  |                         |              | Current limit <br> <code>CH1</code> | |     |              |
| DMA1  |                         |              |               |               | SPI1 <br> <code>CH3</code> ||
| SPI1  |                         |              |               |               | Led data interface ||
| TIM1  |                         | Loop timings<br><code>CCR1</code> <code>CCR2</code> <code>CCR3</code> <code>CCR4</code>| | | | |
| TIM2  |                         |              | Wheel tick counter |          |             |              |
| TIM3  |                         |              | PWM <br> <code>CCR1</code> <code>CCR2</code> | | |         |
| TIM6  |                         |              |               |               |             |              |
| TIM7  |                         |              |               |               |             |              |
| TIM14 |                         |              |               | PWM <br> <code>CCR1</code>| |              |
| TIM15 |                         |              |               |               |             |              |
| TIM16 |                         |              |               |               |             |              |
| TIM17 |                         |              |               |               |             |              |
| USART |                         |              |               |               |             |          CLI |
