# badge-blinky-bill

This challenge involves reading the blinking of LEDs on the badge.

## Steps - Official Walkthrough

<details>
<summary>Spoiler warning</summary>

## Solution

The flag ascii string is broken up into bits and is used to blink the LEDs on the badge.

The bits are moved one-by-one through the following GPIO pins (in order):
 - GPIO21
 - GPIO16
 - GPIO10

Video'ing with a phone (then using play/pause) should be enough to decode the flag.

</details>
