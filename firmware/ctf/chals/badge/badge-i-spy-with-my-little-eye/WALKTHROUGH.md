# badge-i-spy-with-my-little-eye

This challenge involves decoding SPI messages being sent on the badge's SPI bus.

## Steps - Official Walkthrough

<details>
<summary>Spoiler warning</summary>

## Solution

The badge sends out the ascii flag on the default ESP32-S2 SPI bus/pins.

Using a second badge, you can use its diagnostic feature 'spisniff' to view the SPI bytes being sent.
To do this, follow the wiring diagram and notes shown on the Badge's screen.

You can also use a logic analyzer (but not everyone will have those :P)

</details>
