# badge-serial-hacker

This challenge involves decoding UART/SERIAL messages being sent on the badge's UART1 bus.

## Steps - Official Walkthrough

<details>
<summary>Spoiler warning</summary>

## Solution

The badge sends out the ascii flag on the UART1 bus/pins (RX1, TX1).

Using a second badge, you can use its diagnostic feature 'uartterm' to view the serial data being sent.
To do this, follow the wiring diagram and notes shown on the Badge's screen.

You can also use a logic analyzer or USB-to-serial device (but not everyone will have those :P).

</details>
