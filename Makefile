

CC=nvcc
CFLAGS=-g -G
OPENCV_INCLUDE=/usr/local/include
CUDA_INCLUDE=/usr/local/cuda-9.1/include
CUDA_LIB=/usr/local/cuda-9.1/lib64
OPENCV_LIB=/usr/local/lib

test:
	$(CC) -I$(OPENCV_INCLUDE) -I$(CUDA_INCLUDE) -L$(CUDA_LIB) \
	 -lcudart -lopencv_core -lopencv_highgui -lopencv_imgcodecs \
	 -lopencv_imgproc -o test test.cu


debug:
	$(CC) $(CFLAGS) -I$(OPENCV_INCLUDE) -I$(CUDA_INCLUDE) -L$(CUDA_LIB) \
	 -lcudart -lopencv_core -lopencv_highgui -lopencv_imgcodecs \
	 -lopencv_imgproc -o test test.cu

clean:
	rm test
