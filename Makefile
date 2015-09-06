UNAME := $(shell uname)

## MAC
ifeq ($(UNAME), Darwin)
CC = clang++
CC_FLAGS = -std=c++11 -Wall
INCLUDE_SRC = -Iinclude -I/usr/local/include/freetype2/ -I/usr/local/include/
LINKER_FLAG = -lpng -laxLibCore -lfreetype -laxExec

else 
#ifeq ($(UNAME), Linux)

CC = g++-4.9
CC_FLAGS = -std=c++14 -DANDROID -DGL_GLEXT_PROTOTYPES=1 -fpermissive
INCLUDE_SRC = -Iinclude/ -I/usr/include/freetype2/ -I/usr/local/include/ -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I /opt/vc/include/interface/vmcs_host/linux

INCLUDE_LINKER =  -L/usr/local/lib/ -L/opt/vc/lib/
LINKER_FLAG = -lportaudio -lsndfile -lpng -laxLibCore -lfreetype -lEGL -lGLESv1_CM -lpthread -lopenmaxil -lbcm_host -ldl -lutil
endif


OBJ_DIR = build
MAIN_SRC = source/main.cpp source/eosDock.cpp source/eosAppLoader.cpp source/eosStatusBar.cpp source/eosFrame.cpp
MAIN_OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(MAIN_SRC:.cpp=.o)))

TERM_SRC = source/eosTerminal.cpp source/axOSTerminal.cpp source/terminal_main.cpp
TERM_OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(TERM_SRC:.cpp=.o)))

VIEWER_SRC = source/eosImageViewer.cpp source/main_viewer.cpp
VIEWER_OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(VIEWER_SRC:.cpp=.o)))

all: $(MAIN_OBJ)
	$(CC) $(CC_FLAGS) $(INCLUDE_SRC) $(INCLUDE_LINKER) $(MAIN_OBJ) $(LINKER_FLAG) -o main

build/%.o: source/%.cpp
	$(CC) $(CC_FLAGS) $(INCLUDE_SRC) -c -o $@ $<

browser:
	$(CC) -c -fPIC $(CC_FLAGS) $(INCLUDE_SRC) source/eosBrowser.cpp source/app.cpp source/eosFrame.cpp 
	$(CC) -shared -undefined dynamic_lookup -o app/browser.so app.o eosBrowser.o eosFrame.o

txtedit:
	$(CC) -c -fPIC $(CC_FLAGS) $(INCLUDE_SRC) source/eosTextEditor.cpp source/txt_edit_main.cpp source/eosFrame.cpp 
	$(CC) -shared -undefined dynamic_lookup -o app/text_editor.so txt_edit_main.o eosTextEditor.o eosFrame.o

term: $(TERM_OBJ)
	$(CC) -shared -undefined dynamic_lookup -o app/terminal.so $(TERM_OBJ)

viewer: $(VIEWER_OBJ)
	$(CC) -shared -undefined dynamic_lookup -o app/image_viewer.so $(VIEWER_OBJ)

calc:
	$(CC) -c -fPIC $(CC_FLAGS) $(INCLUDE_SRC) source/main_calc.cpp source/eosFrame.cpp 
	$(CC) -shared -undefined dynamic_lookup -o app/calculator.so main_calc.o eosFrame.o

clean:
	rm -f build/*.o
