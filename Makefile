EXECUTABLE = parallax
BINARYDIR = ./bin
INCLUDEDIR = ./include
BUILDDIR = ./build
SRCDIR = ./src
SHADERSDIR = ./res/shaders/vulkan

CFLAGS = -std=c++17 -Wall -Wextra -iquote $(INCLUDEDIR)
LDFLAGS = -lglfw -ldl

SRC = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/**/*.cpp)
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
SRC-C = $(wildcard $(SRCDIR)/*.c)
OBJ-C = $(SRC-C:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

.PHONY: clean release

# TODO: Update shader compilation for both vulkan/openGL eventually
# Compile the .o files into a debuggable executable
${BINARYDIR}/$(EXECUTABLE): $(OBJ) $(BUILDDIR)/glad.o
	mkdir -p ${BINARYDIR}/shaders/vulkan

	glslangValidator -V -o ${BINARYDIR}/shaders/vulkan/shaders.frag.spv $(SHADERSDIR)/shaders.frag
	glslangValidator -V -o ${BINARYDIR}/shaders/vulkan/shaders.vert.spv $(SHADERSDIR)/shaders.vert

	g++ -g -o ${BINARYDIR}/${EXECUTABLE} $(LDFLAGS) $^

$(OBJ-C): $(BUILDDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(@D)

	g++ $(CFLAGS) -c -g -o $@ $<

# Compile .cpp files into .o
$(OBJ): $(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)

	g++ $(CFLAGS) -c -g -o $@ $<

release:
	mkdir -p ${BINARYDIR}/shaders/vulkan
	
	glslangValidator -V -o ${BINARYDIR}/shaders/vulkan/shaders.frag.spv $(SHADERSDIR)/shaders.frag
	glslangValidator -V -o ${BINARYDIR}/shaders/vulkan/shaders.vert.spv $(SHADERSDIR)/shaders.vert
	g++ $(CFLAGS) -O3 -D NDEBUG -o ${BINARYDIR}/${EXECUTABLE} $(LDFLAGS) $(SRC) $(SRC-C)

clean:
	rm -rf $(BINARYDIR)
	rm -rf $(BUILDDIR)
