CC = g++-4.9
CC_FLAGS = -std=c++14 -DANDROID -fpermissive
INCLUDE_SRC = -Iinclude/ -I/usr/include/freetype2/ -I/usr/local/include/ -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I /opt/vc/include/interface/vmcs_host/linux

INCLUDE_LINKER =  -L/usr/local/lib/ -L/opt/vc/lib/

LINKER_FLAG = -lportaudio -lsndfile -lpng -laxLibCore -lfreetype -lEGL -lGLESv1_CM -lpthread -lopenmaxil -lbcm_host -ldl

OBJ_DIR = build
MAIN_SRC = source/main.cpp source/eosDock.cpp source/eosAppLoader.cpp source/eosStatusBar.cpp source/eosFrame.cpp
MAIN_OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(MAIN_SRC:.cpp=.o)))

all: $(MAIN_OBJ)
	$(CC) $(CC_FLAGS) $(INCLUDE_SRC) $(INCLUDE_LINKER) $(MAIN_OBJ) $(LINKER_FLAG) -o main

build/%.o: source/%.cpp
	$(CC) $(CC_FLAGS) $(INCLUDE_SRC) -c -o $@ $<

browser:
	$(CC) -c -fPIC $(CC_FLAGS) $(INCLUDE_SRC) source/eosBrowser.cpp source/app.cpp source/eosFrame.cpp 
	$(CC) -shared -o app/browser.so app.o eosBrowser.o eosFrame.o


calc:
	$(CC) -c -fPIC $(CC_FLAGS) $(INCLUDE_SRC) source/main_calc.cpp source/eosFrame.cpp 
	$(CC) -shared -o app/calculator.so main_calc.o eosFrame.o
