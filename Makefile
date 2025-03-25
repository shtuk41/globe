CC=/usr/local/cuda/bin/nvcc 
NVCCFLAGS=-arch=compute_61 -code=sm_61
CFLAGS=-g -G
OPENCV_INCLUDE=/usr/include/opencv4
CUDA_INCLUDE=/usr/local/cuda/include
CUDA_LIB=/usr/local/cuda/lib64
OPENCV_LIB=/usr/lib/x86_64-linux-gnu

all:	color bw

all_debug: debug_color debug_bw
	

color:
	$(CC) $(NVCCFLAGS) -I$(OPENCV_INCLUDE) -I$(CUDA_INCLUDE) -L$(CUDA_LIB) \
	 -lcudart -lopencv_core -lopencv_highgui -lopencv_imgcodecs \
	 -lopencv_imgproc -o color color.cu
	 
bw:
	$(CC) $(NVCCFLAGS) -I$(OPENCV_INCLUDE) -I$(CUDA_INCLUDE) -L$(CUDA_LIB) \
	 -lcudart -lopencv_core -lopencv_highgui -lopencv_imgcodecs \
	 -lopencv_imgproc -o bw bw.cu	 

debug_color:
	$(CC) $(NVCCFLAGS) $(CFLAGS) -I$(OPENCV_INCLUDE) -I$(CUDA_INCLUDE) -L$(CUDA_LIB) \
	 -lcudart -lopencv_core -lopencv_highgui -lopencv_imgcodecs \
	 -lopencv_imgproc -o color color.cu
	 
debug_bw:
	$(CC) $(NVCCFLAGS) $(CFLAGS) -I$(OPENCV_INCLUDE) -I$(CUDA_INCLUDE) -L$(CUDA_LIB) \
	 -lcudart -lopencv_core -lopencv_highgui -lopencv_imgcodecs \
	 -lopencv_imgproc -o bw bw.cu	 

clean:
	@rm -f color debug_color bw debug_bw *.o *.png *.xml
