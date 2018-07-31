# Specify extensions of files to delete when cleaning
CPP_COMPILER	= clang++
CPP_FLAGS		= -Wall -g -std=c++11
# Specify the target file and the install directory
OUTPUTFILE 		= Nima-Cpp-MathTest
BUILD_DIR 		= Build
OBJ_DIR			= $(BUILD_DIR)/obj
CPP_SOURCES		= Main.cpp
CPP_OBJECTS		= $(CPP_SOURCES:%.cpp=$(OBJ_DIR)/%.o)
CPP_INCLUDES	= -I/usr/local/include -I../Build/include 
LFLAGS			= -Wl,-rpath,./ -L/usr/local/lib -L../Build/lib
LIBS			= -lnima-math

# Default target
all: dirs $(OUTPUTFILE)

clean:
	$(RM) -fR $(OBJ_DIR)/Main.o

dirs:
	mkdir -p $(OBJ_DIR)

# Build sources
$(OUTPUTFILE): $(CPP_OBJECTS)
	$(CPP_COMPILER) $(CPP_FLAGS) $(CPP_INCLUDES) -o $(BUILD_DIR)/$(OUTPUTFILE) $(CPP_OBJECTS) $(LFLAGS) $(LIBS)

$(OBJ_DIR)/%.o: %.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) $(CPP_INCLUDES) -c $< -o $@