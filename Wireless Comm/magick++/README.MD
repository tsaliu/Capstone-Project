The external library used in this magick++ code is the RaspiFastCamD, developed by Userland to perform fast image capturing with RaspiCam.

https://bitbucket.org/niklas rother/rasperry-pi-userland/raw/master/host applications/linux/apps/raspicam/raspifastcamd scripts

To change title of the image - in "start_camh.sh". In line "output_file=$f1-/home/pi/Desktop/send/tmp.jpg", change "tmp.jpb" into desired title. Incremental title can be achieved by "tmp\_%d.jpg".

To run the bash script from C/C++ "sysem("./xxx.sh")".

To compile: sudo g++ $(Magick++-config --cxxflags --cppflags) -o test test.cpp $(Magick++-config --ldflags --libs)
