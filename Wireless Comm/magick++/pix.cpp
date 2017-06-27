#include <Magick++.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
using namespace Magick;
int main(int argc,char **argv){
	system("./stop_camd.sh");
	system("./start_camd.sh");
	usleep(1000000);
while(1){
	system("./do_caputure.sh");
	usleep(1000000);
	InitializeMagick(*argv);
	Image image;

  	image.read("tmp.jpg");
	image.resize("64x64");
	image.type( GrayscaleType );
	image.write("grey.jpg");
	int w=image.columns();
	int h=image.rows();

	Magick::Quantum* pixels = image.getPixels(0,0,w,h);

    cout << w << endl;
    cout << h << endl;
    cout << pixels[20] << endl;

Pixels view(image);
}
  	return 0;
}
