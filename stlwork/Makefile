

CC=g++
CFLAGS=-g
OPENCV_INCLUDE=/usr/local/include
OPENCV_LIB=/usr/local/lib

all:
	$(CC) -o stlwriter stlwriter.cpp -I$(OPENCV_INCLUDE) -L$(OPENCV_LIB) \
	 -lopencv_core -lopencv_highgui -lopencv_imgcodecs \
	 -lopencv_imgproc 


debug:
	$(CC) -o stlwriter stlwriter.cpp $(CFLAGS) -I$(OPENCV_INCLUDE) -L$(OPENCV_LIB) \
	 -lopencv_core -lopencv_highgui -lopencv_imgcodecs \
	 -lopencv_imgproc 

clean:
	rm stlwriter
