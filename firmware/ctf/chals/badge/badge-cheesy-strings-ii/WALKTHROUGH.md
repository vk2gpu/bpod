# badge-cheesy-strings-ii

Find the encoded string store in the binary and extract the flag from it.

## Steps - Official Walkthrough

<details>
<summary>Spoiler warning</summary>

## Solution

Dump the firmware or unpack the firmware blob from the updater script.

Use a reversing tool (such as ghidra) to locate the use of printf.

There is a common pattern where a function is called before printf with 2 values.
These two values are the start and end positions of the string store.
Reverse engineer this function to discover that the start/end positions are used
to:
 - Generate a simple 16-byte key, IV and data for AES 128 CBC.
 - The data is encrypted (see above, AES 128 CBC) with the key and IV to produce a 16-byte XOR mask.
 - This key is then used to XOR the encoded string store bytes.

While this is enough to decode a string, you need to know the start and end of each string
in order to decode it. However, knowing that flags start with "cybears{" and the string store
is a small array, it can be brute forced to find the string.

</details>
