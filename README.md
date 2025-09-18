# Neondra Project
This project truly put all of my technical and coding knowledge to its MAX limits. It took me a full year just deciding what I wanted Neondra to be. I've searched so many articles, tutorials, answers, and now is the time.

## Suit's Features:
This script is used for the whole suit, if the sections are attached.
Otherwise uses just the helmet and allows to control it.

When speaking, the mouth is animated to look like it opens.

Yes, some of the texts the helmet displays are actual YouTube Comments
made by real human beings I have saved for some reason
and some are obscure references or forgotten inside-jokes.

People should probably help by coming up with clever and
funny lines that can then be displayed on the visor.

### The Modes:
- <i>Default</i>: Everything is in default smug state.
- <i>Fury</i>: Matrices are red, forehead has an animated yellow warning sign.
- <i>Love</i>: Matrices are pink, forehead has an animated heart.
- <i>Sad</i>: Matrices are blue, mouth is a frown.
- <i>Datetime</i>: The helmet displays date on the headband headband_matrix, and time on the mouth headband_matrix.

## The Neondra Suit's Parts:
### The Helmet:
- Helmet Rings: Original script is "BT_RGB".

### The Torso:
Torso has 3 speakers, 1 sub bass speaker, 2 normal ones.

### The Arms:
The Left Arm & The Right Arm

### The Legs:
The Left Leg & The Right Leg

### The Wings:
Wings' Brightness:
Left Wing and Right Wing have individual brightness controls for .

Wings' Motor Controls:
Left Wing and Right Wing individual controls UP/DOWN [1-100]. Controlled based on duration between moving difference.

### The Tail:
The tail is held straight by fishlines and can wiggle from left to right and spring lets the tail jolt up and down.

The Tail's Brightness:
The tail has brightness control for [1-100].

## The Code:
You can play any .mp3, .acc, . PlayAudio(String file), ie. PlayAudio("test-file-01.mp3");
You can display text calling DisplayText(String text), ie. DisplayText("Neondra:Project");

### UV Meter:
Here's a link to a YouTube video showcasing the UV Meter V2 library:
https://youtu.be/YVuYO014h0M
Found here:
https://forum.arduino.cc/t/ws2812-vu-meter/274775/2

### MATRIX DECLARATION:
Parameter 1 = width of NeoPixel matrix
Parameter 2 = height of matrix
Parameter 3 = pin number (most are valid)
Parameter 4 = matrix layout flags, add together as needed:
  NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
    Position of the FIRST LED in the matrix; pick two, e.g.
    NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
  NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
    rows or in vertical columns, respectively; pick one or the other.
  NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
    in the same order, or alternate lines reverse direction; pick one.
  See example below for these values in action.
Parameter 5 = pixel type flags, add together as needed:
  NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
  NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
  NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
  NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

## Control Panel:
Initialises a webserver which allows for remote controlling the suit and sword.