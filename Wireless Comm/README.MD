Platform: Raspberry Pi, C, C++, OpenCV, Magick++

Contributor: Tsa Chun Liu

The two main library we use is OpenCV and Magick++.

The functions are capture, gray-scaled the video stream into pixels. Then transmit the data stream with laser on to photovoltaic chip. Then restore and display the pixelated gray scale video.

Every folder contains code for transmitter and receiver.

Since Raspberr Pi does not have ADC built in, so we use mcp3008 and control with Spi, hence the Spi on Raspberry Pi needs tobe enabled. The connections ar <br /> e as followed:

Raspberry Pi -- mcp3008

Vdd          -- 5V

Vref         -- 5V

AGND         -- GND

CLK          -- SCLK

Dout         -- MISO

Din          -- MISI

CS/SHDN      -- CSO (GPIO8)

DGND         -- GND

Currently using CH0, change with data[1] in the code


There are 4 different attempts.
  1. Image processing using Magick++
  2. Image processing using OpenCV
      - sending 0, 1 repeatly
      - sending one byte of information counting from decimal 0 to 255 (0 t FF in hex).
      - sending one pixel from the camera
