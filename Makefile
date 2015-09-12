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
CC_FLAGS = -std=c++14 -DANDROID -DGL_GLEXT_PROTOTYPES=1 -fpermissive
CC_LINK = -shared
INCLUDE_SRC = -Iinclude/ -I/usr/include/freetype2/ -I/usr/local/include/ -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I /opt/vc/include/interface/vmcs_host/linux -I/usr/include/

INCLUDE_LINKER =  -L/usr/local/lib/ -L/opt/vc/lib/ -L/usr/lib/arm-linux-gnueabihf/ -Lapp/
LINKER_FLAG = -lpng -laxLibCore -lfreetype -lEGL -lGLESv1_CM -lpthread -lopenmaxil -lbcm_host -ldl -lutil -lsqlite3 -leos
endif


OBJ_DIR = build
MAIN_SRC = source/eosAppViewer.cpp source/eosCoreTracer.cpp source/eosFrame.cpp source/eosStatusBar.cpp source/eosCoreUser.cpp source/eosHome.cpp source/eosAlert.cpp source/eosCoreManager.cpp source/eosDesktop.cpp source/eosAppLoader.cpp source/eosCoreSystem.cpp source/eosDock.cpp source/eosNotification.cpp

MAIN_OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(MAIN_SRC:.cpp=.o)))

TERM_SRC = source/eosTerminal.cpp source/axOSTerminal.cpp source/terminal_main.cpp
TERM_OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(TERM_SRC:.cpp=.o)))

VIEWER_SRC = source/eosImageViewer.cpp source/main_viewer.cpp
VIEWER_OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(VIEWER_SRC:.cpp=.o)))

all: create_dir $(MAIN_OBJ)
	$(CC) $(CC_LINK) -o app/libeos.so $(MAIN_OBJ)
	$(CC) $(CC_FLAGS) $(INCLUDE_SRC) $(INCLUDE_LINKER) source/main.cpp  $(LINKER_FLAG) -o main

build/%.o: source/%.cpp
	$(CC) $(CC_FLAGS) $(INCLUDE_SRC) -c -o $@ $<

browser:
	$(CC) -c -fPIC $(CC_FLAGS) $(INCLUDE_SRC) source/eosBrowser.cpp source/app.cpp 
	$(CC) $(CC_LINK) -o app/browser.so app.o eosBrowser.o

txtedit:
	$(CC) -c -fPIC $(CC_FLAGS) $(INCLUDE_SRC) source/eosTextEditor.cpp source/txt_edit_main.cpp
	$(CC) $(CC_LINK) -o app/text_editor.so txt_edit_main.o eosTextEditor.o

term: $(TERM_OBJ)
	$(CC) $(CC_LINK) -o app/terminal.so $(TERM_OBJ)

viewer: $(VIEWER_OBJ)
	$(CC) $(CC_LINK)  -o app/image_viewer.so $(VIEWER_OBJ)

calc:
	$(CC) -c -fPIC $(CC_FLAGS) $(INCLUDE_SRC) source/main_calc.cpp source/eosFrame.cpp 
	$(CC) $(CC_LINK) -o app/calculator.so main_calc.o eosFrame.o

create_dir:
	@mkdir -p build app

clean:
	rm -f build/*.o
