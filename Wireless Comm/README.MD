Platform: Raspberry Pi, C, C++, OpenCV, Magick++

Contributor: Tsa Chun Liu

The two main library we use is OpenCV and Magick++.

The functions are capture, gray-scaled the video stream into pixels. Then transmit the data stream with laser on to photovoltaic chip. Then restore and display the pixelated gray scale video.

Every folder contains code for transmitter and receiver.

Since Raspberr Pi does not have ADC built in, so we use mcp3008 and control with Spi, hence the Spi on Raspberry Pi needs tobe enabled. The connections are as followed:

Raspberry Pi -- mcp3008<br />
Vdd          -- 5V<br />
Vref         -- 5V<br />
AGND         -- GND<br />
CLK          -- SCLK<br />
Dout         -- MISO<br />
Din          -- MISI<br />
CS/SHDN      -- CSO (GPIO8)<br />
DGND         -- GND<br />
Currently using CH0, change with data[1] in the code

There are 4 different attempts.
  1. Image processing using Magick++
  2. Image processing using OpenCV
      - sending 0, 1 repeatly
      - sending one byte of information counting from decimal 0 to 255 (0 t FF in hex).
      - sending one pixel from the camera
