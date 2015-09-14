UNAME := $(shell uname)
## MAC
ifeq ($(UNAME), Darwin)
CC = clang++
CC_FLAGS = -std=c++11 -Wall
CC_LINK = -shared -undefined dynamic_lookup
INCLUDE_SRC = -Iinclude -I/usr/local/include/freetype2/ -I/usr/local/include/
LINKER_FLAG = -lpng -laxLibCore -lfreetype -laxExec

else 
#ifeq ($(UNAME), Linux)

CC = g++-4.9
CC_FLAGS = -std=c++14 -DANDROID -DGL_GLEXT_PROTOTYPES=1 -fpermissive -fPIC
CC_LINK = -shared
INCLUDE_SRC = -Iinclude/ -I../../include/ -I/usr/include/freetype2/ -I/usr/local/include/ -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I /opt/vc/include/interface/vmcs_host/linux -I/usr/include/

endif

APP_NAME = text_editor
OBJ_DIR = build
SRC_DIR = source
INC_DIR = include

CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(addprefix $(OBJ_DIR)/,$(notdir $(CPP_FILES:.cpp=.o)))


all: create_dir $(OBJ_FILES)
	$(CC) $(CC_LINK) -o ../../app/$(APP_NAME).so $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CC_FLAGS) $(INCLUDE_SRC) -c -o $@ $<

create_dir:
	@mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o
