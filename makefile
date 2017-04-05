CC       = g++
CFLAGS   = -g -Wall
# Include Freetype for Xft
INCLUDES = -I/usr/include/freetype2

# Link to X11, Xft
LINKER = -lX11 -lXft -lstdc++

SOURCE_DIR = src
OUTPUT_DIR = bin
OBJECT_DIR = $(OUTPUT_DIR)/o

SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(SOURCES:$(SOURCE_DIR)/%.cpp=$(OBJECT_DIR)/%.o)

EXECUTABLE_NAME = clicky

all: clean build
	bin/clicky

build: $(OUTPUT_DIR)/$(EXECUTABLE_NAME)

$(OUTPUT_DIR)/$(EXECUTABLE_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LINKER) -o $(OUTPUT_DIR)/$(EXECUTABLE_NAME)

$(OBJECTS): $(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LINKER) -c $< -o $@

clean:
	@rm -f -r $(OBJECT_DIR)
	@mkdir $(OBJECT_DIR)
	@rm -f $(OUTPUT_DIR)/$(EXECUTABLE_NAME)
