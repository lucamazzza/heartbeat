# HeartBeat

This codebase programs a [Diligent Basys MX3](https://digilent.com/reference/microprocessor/basys-mx3/start) board, equipped with a
[PIC32MX370F512L](https://www.microchip.com/en-us/product/PIC32MX370F512L) microcontroller, and a [KY-039](https://www.amazon.de/dp/B07869C43V/ref=as_li_ss_tl?s=gateway&keywords=ky-039&language=en_US&sr=8-2&linkCode=gs2&linkId=d53bb74e566c860a39817100a1a32cd6&tag=murraynet04-21) 
heart beat sensor to monitor cardiac pulsations.

The measured values are viewed on the `LCD` display embedded on the microcontroller and on a [graphic terminal](https://hackaday.io/project/5334-serialplot-realtime-plotting-software).

## Specifications

When the system is turned on, the `RGB` LED is lit green and the following menu appears.

### 1. HeartBeat

### 2. Max BPM

### 3. Reset

## Peripherals

During the development of the system, the following peripherals were used:

* `UART`
* `Output Compare`
* `PMP`
* `Timer`
* `SPI`
* `GPIO`
* `ADC`
