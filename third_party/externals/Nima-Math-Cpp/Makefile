# Specify extensions of files to delete when cleaning
CPP_COMPILER	= clang++
CPP_FLAGS		= -Wall -Werror -g -std=c++11 -I./ $(CFLAGS)
DEFINES			=
# Wildcard selector.
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
# Specify the target file and the install directory
OUTPUTFILE 		= libnima-math.a
BUILD_DIR 		= Build
LIB_DIR			= $(BUILD_DIR)/lib
OBJ_DIR			= $(BUILD_DIR)/obj
INCLUDE_DIR		= $(BUILD_DIR)/include/nima
CPP_SOURCES		= $(call rwildcard,Source/,*.cpp)
CPP_OBJECTS		= $(CPP_SOURCES:%.cpp=$(OBJ_DIR)/%.o)
CPP_FOLDERS		= $(sort $(dir $(CPP_OBJECTS)))
HEADERS			= $(call rwildcard,Source/,*.hpp)
# Default target
#all: dirs $(OUTPUTFILE)
#	cd Example && make

all: dirs $(OUTPUTFILE) headers

headers:
# Copy header files to include dir.
	$(foreach header,$(HEADERS),$(shell mkdir -p $(INCLUDE_DIR)/$(dir $(subst Source/,,$(header))) && cp $(header) $(INCLUDE_DIR)/$(subst Source/,,$(header))))

clean:
	@$(RM) -fR $(BUILD_DIR)

dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(INCLUDE_DIR)
	@$(foreach folder,$(CPP_FOLDERS),$(shell mkdir -p $(folder)))
	
install:
	cp -r Build/include/ /usr/local/include
	cp -r Build/lib/ /usr/local/lib

# Build sources
$(OUTPUTFILE): $(CPP_OBJECTS)
	@ar ru $(LIB_DIR)/$@ $^
	@ranlib $(LIB_DIR)/$@

$(OBJ_DIR)/%.o: %.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) $(DEFINES) -c $< -o $@