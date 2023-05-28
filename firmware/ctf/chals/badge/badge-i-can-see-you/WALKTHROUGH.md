# badge-i-can-see-you

This challenge involves decoding i2c messages being sent on the badge's i2c bus.

## Steps - Official Walkthrough

<details>
<summary>Spoiler warning</summary>

## Solution

The badge sends out the ascii flag on the default ESP32-S2 I2C bus/pins.

Using a second badge, you can use its diagnostic feature 'i2csniff' to view the I2C bytes being sent.
To do this, follow the wiring diagram and notes shown on the Badge's screen.

You can also use a logic analyzer (but not everyone will have those :P)

</details>
