CC = g++-4.9
CC_FLAGS = -std=c++14 -DANDROID -fpermissive
INCLUDE_SRC = -Iinclude/ -I/usr/include/freetype2/ -I/usr/local/include/ -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I /opt/vc/include/interface/vmcs_host/linux

INCLUDE_LINKER =  -L/usr/local/lib/ -L/opt/vc/lib/

LINKER_FLAG = -lportaudio -lsndfile -lpng -laxLibCore -lfreetype -lEGL -lGLESv1_CM -lpthread -lopenmaxil -lbcm_host -ldl

all:
	$(CC) $(CC_FLAGS) $(INCLUDE_SRC) $(INCLUDE_LINKER) source/main.cpp source/eosStatusBar.cpp $(LINKER_FLAG) -o main 


browser:
	$(CC) -c -fPIC $(CC_FLAGS) $(INCLUDE_SRC) source/eosBrowser.cpp source/app.cpp source/eosFrame.cpp 
	$(CC) -shared -o app/browser.so app.o eosBrowser.o eosFrame.o


calc:
	$(CC) -c -fPIC $(CC_FLAGS) $(INCLUDE_SRC) source/main_calc.cpp source/eosFrame.cpp 
	$(CC) -shared -o app/calculator.so main_calc.o eosFrame.o
