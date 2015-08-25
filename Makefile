CC = g++-4.9
CC_FLAGS = -std=c++14 -DANDROID -fpermissive
INCLUDE_SRC = -I/usr/include/freetype2/ -I/usr/local/include/ -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I /opt/vc/include/interface/vmcs_host/linux

INCLUDE_LINKER =  -L/usr/local/lib/ -L/opt/vc/lib/

LINKER_FLAG = -lpng -laxLibCore -lfreetype -lEGL -lGLESv1_CM -lpthread -lopenmaxil -lbcm_host -ldl

all:
	$(CC) $(CC_FLAGS) $(INCLUDE_SRC) $(INCLUDE_LINKER) main.cpp $(LINKER_FLAG) -o main 


browser:
	$(CC) -c -fPIC $(CC_FLAGS) $(INCLUDE_SRC) eosBrowser.cpp 
