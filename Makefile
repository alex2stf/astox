#debuggers: ASTX_DBG_ECMA, ASTX_DBG_SSH2
PLATFORM=$(shell uname)

COMPILER=g++
CCOMPILER=gcc

BUILD_DIR=build/
ASTOX_SRC=src/
CODE_SRC=./
TESTS_SRC=test/src/
BIN_DIR=bin/

#final linker flags
LIBS=-lpthread -lm
LIB_PATHS=
INCLUDES=-Iinclude
TEST_INCLUDES=-Itest/include
DEFINES=
OTHER_FLAGS=

#simulate nodejs enviroment with static cast:
#DEBUG_DEFS := -DASTX_value_STATIC_CAST


#for cygwin option -fPIC is not used by libs
ifneq (,$(findstring CYGWIN,$(shell uname)))
  OBJ_SETTINGS=$(DEFINES) $(INCLUDES) -c
else
  OBJ_SETTINGS=$(DEFINES) $(INCLUDES) -fPIC -c
endif


ASTOX_OBJS = $(BUILD_DIR)value.o\
			$(BUILD_DIR)string.o\
			$(BUILD_DIR)boolean.o\
			$(BUILD_DIR)number.o\
			$(BUILD_DIR)object.o\
			$(BUILD_DIR)array.o\
			$(BUILD_DIR)function.o\
			$(BUILD_DIR)error.o\
			$(BUILD_DIR)util.o\
			$(BUILD_DIR)date.o\
			$(BUILD_DIR)testing.o\
			$(BUILD_DIR)methods.o\
			$(BUILD_DIR)filesystem.o\
			$(BUILD_DIR)stack.o\
			$(BUILD_DIR)regexp.o\
			$(BUILD_DIR)serializers.o\
			$(BUILD_DIR)scriptengine.o\
			$(BUILD_DIR)thread.o\
			$(BUILD_DIR)lexic.o
			


ASTOX_MAKE_LIBS = Libs


all: prepareDirs Libs tests


socket-tests:
	$(COMPILER) $(TESTS_SRC)socket.cpp $(DEFINES) $(OTHER_FLAGS) -o $(BIN_DIR)socket-test
	
	
shared: Libs shared-lib

display-vars:
	@echo $(BIN_DIR) 
	@echo $(JAVA_HOME)
	

prepareDirs:
	mkdir -p $(BIN_DIR) $(BUILD_DIR)

ext-jni:
	$(COMPILER) -Wl,--add-stdcall-alias  -I"$(JAVA_HOME)\include" -I"$(JAVA_HOME)\include\win32" $(INCLUDES) -D__int64=int64_t -shared -o lib/astox-jni.dll $(ASTOX_SRC)ext-jni.cpp
	
	#/usr/bin/ld: build/value.o: relocation R_X86_64_32 against `_ZTVN5astox5ValueE' can not be used when making a shared object; recompile with -fPIC
	
ext-jni-linux:
	$(COMPILER) -DSTX_ENABLE_JNI -D__int64=int64_t -I$(JAVA_HOME)"/include" -I"$(JAVA_HOME)/include/linux" $(INCLUDES) -o bin/libastox-jni.so -fPIC -shared $(ASTOX_SRC)ext-jni.cpp $(ASTOX_OBJS)

ext-jni-cygwin:
	$(COMPILER) -DSTX_ENABLE_JNI -D__int64=int64_t -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/win32" $(INCLUDES) -o bin/astox-jni.dll -shared $(ASTOX_SRC)ext-jni.cpp $(ASTOX_OBJS)
	
shared-lib:
	$(COMPILER) -shared -o lib/astox.so $(ASTOX_OBJS) $(INCLUDES)

tests:
	$(COMPILER) $(TESTS_SRC)astox-tests.cpp $(DEFINES) $(INCLUDES) $(TEST_INCLUDES) $(ASTOX_OBJS) $(OTHER_FLAGS) $(LIB_PATHS) $(LIBS) -o $(BIN_DIR)astox-test

codoc:
	$(COMPILER) $(CODE_SRC)codoc.cpp $(DEFINES) $(INCLUDES) $(ASTOX_OBJS) $(OTHER_FLAGS) $(LIB_PATHS) $(LIBS) -o $(BIN_DIR)codoc


astxSelfConfigure:
	$(COMPILER) $(CODE_SRC)astxSelfConfigure.cpp  $(DEFINES) $(INCLUDES) $(ASTOX_OBJS) $(OTHER_FLAGS) $(LIB_PATHS) $(LIBS) -o $(BIN_DIR)astxSelfConfigure 

####### ASTOX_OBJS:
Libs: thread value string boolean number methods lexic serializers util regexp error stack function testing filesystem\
	object array date math scriptengine  


autoconf:
	$(COMPILER) $(ASTOX_SRC)autoconf.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)autoconf.o 
	
thread:
	$(COMPILER) $(ASTOX_SRC)thread.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)thread.o
	
lexic:
	$(COMPILER) $(ASTOX_SRC)lexic.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)lexic.o
	
serializers:
	$(COMPILER) $(ASTOX_SRC)serializers.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)serializers.o
	
value:
	$(COMPILER) $(ASTOX_SRC)value.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)value.o
	
string:
	$(COMPILER) $(ASTOX_SRC)string.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)string.o
	
boolean:
	$(COMPILER) $(ASTOX_SRC)boolean.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)boolean.o

number:
	$(COMPILER) $(ASTOX_SRC)number.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)number.o
	
object:
	$(COMPILER) $(ASTOX_SRC)object.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)object.o
	
array:
	$(COMPILER) $(ASTOX_SRC)array.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)array.o
	
function:
	$(COMPILER) $(ASTOX_SRC)function.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)function.o
	
date:
	$(COMPILER) $(ASTOX_SRC)date.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)date.o

error:
	$(COMPILER) $(ASTOX_SRC)error.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)error.o
	
regexp:
	$(COMPILER) $(ASTOX_SRC)regexp.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)regexp.o
	
math:
	$(COMPILER) $(ASTOX_SRC)math.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)math.o
	
methods:
	$(COMPILER) $(ASTOX_SRC)methods.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)methods.o
		
filesystem:
	$(COMPILER) $(ASTOX_SRC)filesystem.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)filesystem.o
	
util:
	$(COMPILER) $(ASTOX_SRC)util.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)util.o
	
testing:
	$(COMPILER) $(ASTOX_SRC)testing.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)testing.o
	
scriptengine:
	$(COMPILER) $(ASTOX_SRC)scriptengine.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)scriptengine.o
	
stack:
	$(COMPILER) $(ASTOX_SRC)stack.cpp $(OBJ_SETTINGS) -o $(BUILD_DIR)stack.o
	


	

clean:
	rm $(BUILD_DIR)* 
	rm $(BIN_DIR)*
	
install:
	cp $(BIN_DIR)astox /usr/local/bin/astox

	
