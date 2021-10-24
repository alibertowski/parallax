EXECUTABLE = parallax
BINARYDIR = ./bin
INCLUDEDIR = ./include
BUILDDIR = ./build
SRCDIR = ./src
SHADERSDIR = ./res/shaders/vulkan

CFLAGS = -std=c++17 -Wall -Wextra -iquote $(INCLUDEDIR)
CFLAGS += -I lib/glad/include -I lib/stb
LDFLAGS = -lglfw -ldl

SRC = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/**/*.cpp)
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

SRC_LIB = lib/glad/src/glad.c
OBJ_LIB = $(SRC_LIB:lib/%.c=lib/%.o)

.PHONY: clean release

# TODO: Update shader compilation for both vulkan/openGL eventually
# TODO: Update to compile library files

# Compile the .o files into a debuggable executable
${BINARYDIR}/$(EXECUTABLE): $(OBJ) $(OBJ_LIB)
	mkdir -p ${BINARYDIR}/shaders/vulkan

	glslangValidator -V -o ${BINARYDIR}/shaders/vulkan/shaders.frag.spv $(SHADERSDIR)/shaders.frag
	glslangValidator -V -o ${BINARYDIR}/shaders/vulkan/shaders.vert.spv $(SHADERSDIR)/shaders.vert

	g++ -g -o ${BINARYDIR}/${EXECUTABLE} $(LDFLAGS) $^

# Compile .cpp files into .o
$(OBJ): $(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)

	g++ $(CFLAGS) -c -g -o $@ $<

release:
	make clean
	mkdir -p ${BINARYDIR}/shaders/vulkan
	
	glslangValidator -V -o ${BINARYDIR}/shaders/vulkan/shaders.frag.spv $(SHADERSDIR)/shaders.frag
	glslangValidator -V -o ${BINARYDIR}/shaders/vulkan/shaders.vert.spv $(SHADERSDIR)/shaders.vert
	g++ $(CFLAGS) -O3 -D NDEBUG -o ${BINARYDIR}/${EXECUTABLE} $(LDFLAGS) $(SRC) $(SRC_LIB)

clean:
	rm -rf $(BINARYDIR)
	rm -rf $(BUILDDIR)
	rm -f lib/glad/src/glad.o
