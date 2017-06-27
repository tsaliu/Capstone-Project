To compile:<br />
Receiver: "sudo g++ -Wall -o rec2 `pkg-config opencv --cflags` mcp3008Spi.cpp `pkg-config opencv --libs` rec2.cpp -lwiringPi" "sudo ./rec2"<br />
Transmitter: "g++ `pkg-config opencv --cflags` trans.cpp -o trans `pkg-config opencv --libs` -lwiringPi" "sudo ./trans"
