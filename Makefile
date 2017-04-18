#debuggers: ASTX_DBG_ECMA, ASTX_DBG_SSH2
PLATFORM=$(shell uname)

COMPILER=g++
CCOMPILER=gcc

BUILD_DIR = build/
ASTOX_SRC=src/astox/
CODE_SRC=src/
BIN_DIR=bin/

#final linker flags
LIBS=-lpthread -lm
LIB_PATHS=
INCLUDES=-Isrc/astox/icc
DEFINES=
OTHER_FLAGS=


MINGUI_LIBS=
MINGUI_LIBPATH=
MINGUI_DEFS=
MINGUI_INCLUDES=
MINGUI_FLAGS=
MINGUI_FILE=MinGUI.cpp


#MINGUI_MAC
MINGUI_MAC_LIBS=
MINGUI_MAC_LIBPATH=

MINGUI_MAC_INCLUDES=
MINGUI_MAC_FLAGS=-framework Cocoa -x objective-c


#MINGUI_GTK
MINGUI_GTK_LIBS=`pkg-config gtk+-2.0 libnotify --libs`
MINGUI_GTK_LIBPATH=
MINGUI_GTK_DEFS=-DASTOX_ENABLE_GTK
MINGUI_GTK_INCLUDES=`pkg-config gtk+-2.0 libnotify --cflags-only-I`
MINGUI_GTK_FLAGS=

### ENABLE MINGUI SUPPORT ##########################################
ifeq ($(ASTX_ENABLE_MINGUI),true)

	ifeq ($(PLATFORM),Darwin)
	    MINGUI_FILE=MinGUI.mm
	   
	    MINGUI_LIBS=$(MINGUI_MAC_LIBS)
		MINGUI_LIBPATH=$(MINGUI_MAC_LIBPATH)
		MINGUI_DEFS=-DASTX_ENABLE_OBJECTIVECPP
		MINGUI_INCLUDES=$(MINGUI_MAC_INCLUDES)
		MINGUI_FLAGS=$(MINGUI_MAC_FLAGS)
		
	else
		MINGUI_FILE=MinGUI.cpp
		
		ifeq ($(PLATFORM),Linux)
		#	MINGUI_LIBS=$(MINGUI_GTK_LIBS)
		#	MINGUI_LIBPATH=$(MINGUI_GTK_LIBPATH)
		#	MINGUI_DEFS=$(MINGUI_GTK_DEFS)
		#	MINGUI_INCLUDES=$(MINGUI_GTK_INCLUDES)
		#	MINGUI_FLAGS=$(MINGUI_GTK_FLAGS)
		endif
	endif



	LIBS :=$(LIBS) $(MINGUI_LIBS)
	LIB_PATHS :=$(LIB_PATHS) $(MINGUI_LIBPATH)
	DEFINES :=$(DEFINES) $(MINGUI_DEFS)
	INCLUDES := $(INCLUDES) $(MINGUI_INCLUDES)
	OTHER_FLAGS := $(OTHER_FLAGS) $(MINGUI_FLAGS)
endif


ifeq ($(ASTX_ENABLE_CURL),true)
	#curl options
	CURL_LIBS=-lcurl
	CURL_LIBPATH=
	CURL_DEFS=-DASTX_ENABLE_CURL
	CURL_INCLUDES=
	CURL_FLAGS=
	
endif


ifeq ($(ASTX_ENABLE_SSH2),true)
	#ssh2 options
	#SSH2_LIBS=-lssh2
	#SSH2_LIBPATH=-L/opt/local/lib 
	#SSH2_DEFS=-DASTX_ENABLE_LIBSSH2
	#SSH2_INCLUDES=-I/opt/local/include
	#SSH2_FLAGS=
	
	
	LIBS :=$(LIBS) $(SSH2_LIBS)
	LIB_PATHS :=$(LIB_PATHS) $(SSH2_LIBPATH)
	DEFINES :=$(DEFINES) $(SSH2_DEFS)
	INCLUDES :=$(INCLUDES) $(SSH2_INCLUDES)
	OTHER_FLAGS :=$(OTHER_FLAGS) $(SSH2_FLAGS)
endif

#LIBS :=$(LIBS) -lssl
#OTHER_FLAGS :=$(OTHER_FLAGS) -framework CoreFoundation

ifeq ($(ASTX_ENABLE_BOOST),true)
	LIBS :=$(LIBS) -lboost_filesystem-mt -lboost_system-mt
	#LIB_PATHS :=$(LIB_PATHS) /opt/local/lib
	DEFINES :=$(DEFINES) -DASTX_ENABLE_BOOST
	#INCLUDES :=$(INCLUDES)
endif

ifeq ($(ASTX_ENABLE_SASS),true)
	#sass_options
	#SASS_LIBS=-lsass -ldl
	#SASS_LIBPATH=
	#SASS_DEFS=-DASTX_ENABLE_SASS
	#SASS_INCLUDES=
	#SASS_FLAGS=-Wall -O2 -std=c++0x -fPIC //merge pe mac
	#SASS_FLAGS=-Wall -O2 -std=c++0x -fPIC
	
	LIBS :=$(LIBS) $(SASS_LIBS)
	LIB_PATHS :=$(LIB_PATHS) $(SASS_LIBPATH)
	DEFINES :=$(DEFINES) $(SASS_DEFS)
	INCLUDES :=$(INCLUDES) $(SASS_INCLUDES)
	OTHER_FLAGS :=$(OTHER_FLAGS) $(SASS_FLAGS)
endif


ifeq ($(ASTX_ENABLE_FFMPEG),true)
#FFMPEG_FLAGS
FFMPEG_ROOT=/Users/alexstf/Desktop/ffmpeg/
FFMPEG_LIBDIRS=-L$(FFMPEG_ROOT)libavcodec -L$(FFMPEG_ROOT)libavdevice -L$(FFMPEG_ROOT)libavfilter -L$(FFMPEG_ROOT)libswscale
FFMPEG_LIBS=-L/opt/local/lib -ljpeg -lavformat -lavcodec -lavutil -lswscale -lmp3lame -lz
endif

ifeq ($(ASTX_USE_HIREDIS),true)
	DEFINES := $(DEFINES) -DASTX_ENABLE_HIREDIS
	OTHER_FLAGS := $(OTHER_FLAGS) -lhiredis
endif

ifeq ($(USE_BOOST_LIBS),true)
	LINKER_FLAGS=-lboost_filesystem -lboost_regex -lboost_thread
endif

DEBUG_DEFS=
#simulate nodejs enviroment with static cast:
#DEBUG_DEFS := -DASTX_VALUE_STATIC_CAST
# -DASTX_VALUE_STATIC_CAST
DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_SELF_CONFIGURE
DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_SSH2
DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_ECMA_METHODS
DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_ECMA_METHODS_EXTERNS
DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_ECMA_JS_PRINT
DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_EVENT_DISPATCHER

#DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_ECMA_EXEC_SCRIPT 
#DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_ECMA_DETECT_NODES
#DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_FS
#DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_FS_WATCHER
#DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_ECMA_GET_MEMBER
#DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_ECMA_OPERATE
#DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_ECMA_COMPARE
#DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_ECMA_DETECT_NODES
#DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_ECMA_CONTEXTS

#DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_ECMA_EXEC_SCRIPT
#DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_ECMA_SPLIT_COMMAND
#DEBUG_DEFS := $(DEBUG_DEFS) -DASTX_DBG_REGEX

DEFINES := $(DEFINES) $(DEBUG_DEFS)

OBJ_SETTINGS= $(DEFINES) $(INCLUDES) -c

ASTOX_OBJS = $(BUILD_DIR)Value.o\
			$(BUILD_DIR)String.o\
			$(BUILD_DIR)Boolean.o\
			$(BUILD_DIR)Number.o\
			$(BUILD_DIR)Object.o\
			$(BUILD_DIR)Array.o\
			$(BUILD_DIR)Function.o\
			$(BUILD_DIR)Lang.o\
			$(BUILD_DIR)Pins.o\
			$(BUILD_DIR)Math.o\
			$(BUILD_DIR)FileSystem.o\
			$(BUILD_DIR)Xhtml.o\
			$(BUILD_DIR)Query.o\
			$(BUILD_DIR)Musteen.o\
			$(BUILD_DIR)ExtApi.o\
			$(BUILD_DIR)ECMAScript.o\
			$(BUILD_DIR)Util.o\
			$(BUILD_DIR)Operators.o\
			$(BUILD_DIR)Date.o\
			$(BUILD_DIR)EcmaMethods.o\
			$(BUILD_DIR)HTTPServer.o\
			$(BUILD_DIR)Socket.o\
			$(BUILD_DIR)Thread.o\
			$(BUILD_DIR)CssLinter.o\
			$(BUILD_DIR)Autoconf.o\
			$(BUILD_DIR)Properties.o\
			$(BUILD_DIR)Mingui.o\
			$(BUILD_DIR)Assert.o\
			$(BUILD_DIR)EventDispatcher.o\
			$(BUILD_DIR)ScriptEngine.o
			


ASTOX_MAKE_LIBS = Libs


all: $(ASTOX_MAKE_LIBS) astox

viewVars:
	@echo "DEFINES: $(DEFINES)"
	@echo "INCLUDES: $(INCLUDES)"
	@echo "LIBS: $(LIBS)"
	@echo "LIB_PATHS: $(LIB_PATHS)"
	@echo "OTHER_FLAGS: $(OTHER_FLAGS)"
	@echo "DEBUG OPTIONS:\n ASTX_DBG_FS (file system debugging)\n ASTX_DBG_ECMA (astox ecma debugging)\n ASTX_DBG_SSH2(ssh2 debugging)"

prepareDirs:
	mkdir -p $(BIN_DIR) $(BUILD_DIR)

demuxing:
	gcc $(CODE_SRC)demuxing_decoding.c  $(DEFINES) $(INCLUDES) $(FFMPEG_LIBS) -o $(BIN_DIR)demuxing

ffipush: ffipushbin

ffipushbin:
	gcc $(ASTOX_SRC)CApi.c $(CODE_SRC)ffipush.c $(DEFINES) $(INCLUDES) $(FFMPEG_LIBS) -o $(BIN_DIR)ffipush

astox:
	$(COMPILER) $(CODE_SRC)main.cpp $(DEFINES) $(INCLUDES) $(ASTOX_OBJS) $(OTHER_FLAGS) $(LIB_PATHS) $(LIBS) -o $(BIN_DIR)astox 

astxUnitTests:
	$(COMPILER) $(CODE_SRC)astxUnitTests.cpp  $(DEFINES) $(INCLUDES) $(ASTOX_OBJS) $(OTHER_FLAGS) $(LIB_PATHS) $(LIBS) -o $(BIN_DIR)astxUnitTests 
	
astxSelfConfigure:
	$(COMPILER) $(CODE_SRC)astxSelfConfigure.cpp  $(DEFINES) $(INCLUDES) $(ASTOX_OBJS) $(OTHER_FLAGS) $(LIB_PATHS) $(LIBS) -o $(BIN_DIR)astxSelfConfigure 

####### ASTOX_OBJS:
Libs: EventDispatcher Mingui Assert Properties ExtApi Mingui Autoconf Util Value String Boolean Number\
	Object Array Function Date Lang Pins Math Xhtml FileSystem Query Musteen\
	CssLinter EcmaMethods HTTPServer Socket ECMAScript Operators Thread ScriptEngine

EventDispatcher:
	$(COMPILER) $(ASTOX_SRC)EventDispatcher.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)EventDispatcher.o 

Autoconf:
	$(COMPILER) $(ASTOX_SRC)Autoconf.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Autoconf.o 

Mingui:
	$(COMPILER) $(ASTOX_SRC)$(MINGUI_FILE) $(OBJ_SETTINGS) -o $(BUILD_DIR)Mingui.o 

ExtApi:
	$(COMPILER) $(ASTOX_SRC)ExtApi.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)ExtApi.o 

Properties:
	$(COMPILER) $(ASTOX_SRC)Properties.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Properties.o
	
Value:
	$(COMPILER) $(ASTOX_SRC)Value.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Value.o
	
String:
	$(COMPILER) $(ASTOX_SRC)String.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)String.o
	
Boolean:
	$(COMPILER) $(ASTOX_SRC)Boolean.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Boolean.o

Number:
	$(COMPILER) $(ASTOX_SRC)Number.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Number.o
	
Object:
	$(COMPILER) $(ASTOX_SRC)Object.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Object.o
	
Array:
	$(COMPILER) $(ASTOX_SRC)Array.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Array.o
	
Function:
	$(COMPILER) $(ASTOX_SRC)Function.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Function.o
	
Date:
	$(COMPILER) $(ASTOX_SRC)Date.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Date.o
	
Lang:
	$(COMPILER) $(ASTOX_SRC)Lang.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Lang.o

Pins:
	$(COMPILER) $(ASTOX_SRC)Pins.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Pins.o
	
Math:
	$(COMPILER) $(ASTOX_SRC)Math.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Math.o

Xhtml:
	$(COMPILER) $(ASTOX_SRC)Xhtml.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Xhtml.o
	
Query:
	$(COMPILER) $(ASTOX_SRC)Query.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Query.o
	
Musteen:
	$(COMPILER) $(ASTOX_SRC)Musteen.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Musteen.o
	
CssLinter:
	$(COMPILER) $(ASTOX_SRC)CssLinter.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)CssLinter.o
		
FileSystem:
	$(COMPILER) $(ASTOX_SRC)FileSystem.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)FileSystem.o
	
Util:
	$(COMPILER) $(ASTOX_SRC)Util.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Util.o
	
ECMAScript:
	$(COMPILER) $(ASTOX_SRC)ECMAScript.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)ECMAScript.o

EcmaMethods:
	$(COMPILER) $(ASTOX_SRC)EcmaMethods.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)EcmaMethods.o

Operators:
	$(COMPILER) $(ASTOX_SRC)Operators.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Operators.o

HTTPServer:
	$(COMPILER) $(ASTOX_SRC)HTTPServer.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)HTTPServer.o

Socket:
	$(COMPILER) $(ASTOX_SRC)Socket.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Socket.o

Thread:
	$(COMPILER) $(ASTOX_SRC)Thread.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Thread.o
	
Assert:
	$(COMPILER) $(ASTOX_SRC)Assert.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)Assert.o
	
ScriptEngine:
	$(COMPILER) $(ASTOX_SRC)ScriptEngine.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)ScriptEngine.o
	

	

clean:
	rm $(BUILD_DIR)* 
	#rm $(BIN_DIR)*
	
install:
	cp $(BIN_DIR)astox /usr/local/bin/astox

	