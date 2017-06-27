Platform: Raspberry Pi, C, C++, OpenCV, Magick++

Contributor: Tsa Chun Liu

The two main library we use is OpenCV and Magick++.

The functions are capture, gray-scaled the video stream into pixels. Then transmit the data stream with laser on to photovoltaic chip. Then restore and display the pixelated gray scale video.

There are 4 different attempts.
  1. Image processing using Magick++
  2. Image processing using OpenCV
    - sending 0, 1 repeatly
    - sending one byte of information counting from decimal 0 to 255 (0 t FF in hex).
    - sending one pixel from the camera
