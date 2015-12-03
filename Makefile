UNAME := $(shell uname)

## MAC
ifeq ($(UNAME), Darwin)
CC = clang++
CC_FLAGS = -std=c++11 -Wall
CC_LINK = -shared -undefined dynamic_lookup
INCLUDE_SRC = -Iinclude -I/usr/local/include/freetype2/ -I/usr/local/include/
LINKER_FLAG = -lpng -laxLibCore -lfreetype -laxExec

else ifeq ($(UNAME), FreeBSD)
CC = clang++
CC_FLAGS = -std=c++11 -fPIC -DaxCoreX11 -DGL_GLEXT_PROTOTYPES=1 -fpermissive
CC_LINK = -shared
INCLUDE_SRC = -Iinclude/ -I/usr/local/include/freetype2/ -I/usr/local/include/

INCLUDE_LINKER =  -L/usr/local/lib/  -Llib/
LINKER_FLAG = -lpng -laxLibCore -lfreetype -lpng -lGL -leos -lutil
else 
#ifeq ($(UNAME), Linux)

CC = g++-4.9
CC_FLAGS = -std=c++14 -DANDROID -DGL_GLEXT_PROTOTYPES=1 -fpermissive
CC_LINK = -shared
INCLUDE_SRC = -Iinclude/ -I/usr/include/freetype2/ -I/usr/local/include/ -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I /opt/vc/include/interface/vmcs_host/linux -I/usr/include/

INCLUDE_LINKER =  -L/usr/local/lib/ -L/opt/vc/lib/ -L/usr/lib/arm-linux-gnueabihf/ -Llib/
#-lGLESv1_CM
LINKER_FLAG = -lpng -laxLibCore -lfreetype -lEGL -lGLESv2 -lpthread -lopenmaxil -lbcm_host -ldl -lutil -lsqlite3 -leos
endif

OBJ_DIR = build
SRC_DIR = source
LIB_DIR = lib

CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(addprefix $(OBJ_DIR)/,$(notdir $(CPP_FILES:.cpp=.o)))

all: create_dir $(OBJ_FILES)
	$(CC) $(CC_LINK) -o $(LIB_DIR)/libeos.so $(OBJ_FILES)
	$(CC) $(CC_FLAGS) $(INCLUDE_SRC) $(INCLUDE_LINKER) -lxdo `Magick++-config --cppflags --cxxflags --ldflags --libs` main.cpp  $(LINKER_FLAG) -o eos_main

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CC_FLAGS) $(INCLUDE_SRC) -c -o $@ $<

create_dir:
	@mkdir -p build app lib


MODULES_DIR = $(shell ls modules)

.PHONY: app $(MODULES_DIR)

app: $(MODULES_DIR)

$(MODULES_DIR):
	make -C modules/$@

start:
	cp lib/libeos.so /usr/local/lib/
	startx $(shell pwd)/eos_main
clean:
	rm -f build/*.o lib/*.so
