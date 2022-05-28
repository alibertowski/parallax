EXECUTABLE = parallax
BINARYDIR = ./bin
INCLUDEDIR = ./include
BUILDDIR = ./build
SRCDIR = ./src
SHADERSDIR = ./res/shaders/vulkan

CFLAGS = -Wall -Wextra -iquote $(INCLUDEDIR)
CFLAGS += -I lib/glad/include -I lib/stb
LDFLAGS = -lglfw -ldl

SRC = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/**/*.cpp)
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

SRC_C = lib/glad/src/glad.c
OBJ_C = $(SRC_C:%.c=$(BUILDDIR)/%.o)

SRC_TEST = $(wildcard ./test/*.cpp)

.PHONY: clean release test

# TODO: Update shader compilation for both vulkan/openGL eventually
# TODO: Update to compile library files easier
# TODO: Make a cleaner Makefile

# Compile the .o files into a debuggable executable
$(BINARYDIR)/$(EXECUTABLE): $(OBJ) $(OBJ_C)
	mkdir -p $(BINARYDIR)/shaders/vulkan

	glslangValidator -V -o $(BINARYDIR)/shaders/vulkan/shaders.frag.spv $(SHADERSDIR)/shaders.frag
	glslangValidator -V -o $(BINARYDIR)/shaders/vulkan/shaders.vert.spv $(SHADERSDIR)/shaders.vert

	g++ -g -o $(BINARYDIR)/$(EXECUTABLE) $(LDFLAGS) $^

# Compile .cpp files into .o
$(OBJ): $(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)

	g++ $(CFLAGS) -std=c++17 -c -g -o $@ $<

# Compile .c files into .o
$(OBJ_C): $(BUILDDIR)/%.o: %.c
	mkdir -p $(@D)

	gcc $(CFLAGS) -std=c17 -c -g -o $@ $<

release:
	make clean
	mkdir -p $(BINARYDIR)/shaders/vulkan
	
	glslangValidator -V -o $(BINARYDIR)/shaders/vulkan/shaders.frag.spv $(SHADERSDIR)/shaders.frag
	glslangValidator -V -o $(BINARYDIR)/shaders/vulkan/shaders.vert.spv $(SHADERSDIR)/shaders.vert
	g++ $(CFLAGS) -std=c++17 -O3 -D NDEBUG -o $(BINARYDIR)/$(EXECUTABLE) $(LDFLAGS) $(SRC) $(SRC_C)

clean:
	rm -rf $(BINARYDIR)
	rm -rf $(BUILDDIR)
	rm -rf ./test/parallax-tests

test:
	mkdir -p ./test
	g++ $(CFLAGS) -lgtest -std=c++17 -O3 -D NDEBUG -o ./test/parallax-tests $(SRC_TEST)
