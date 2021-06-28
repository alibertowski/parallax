EXECUTABLE = parallax
BINARYDIR = ./bin
INCLUDEDIR = ./include
BUILDDIR = ./build
SRCDIR = ./src
SHADERSDIR = ./shaders

CFLAGS = -std=c++17 -Wall -Wextra -iquote $(INCLUDEDIR)
LDFLAGS = -lglfw -lvulkan

SRC = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/**/*.cpp)
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
SHADERSRC = $(wildcard $(SHADERSDIR)/*.frag) $(wildcard $(SHADERSDIR)/*.vert)

.PHONY: clean release

# Compile the .o files into a debuggable executable
${BINARYDIR}/$(EXECUTABLE): $(OBJ)
	mkdir -p ${BINARYDIR}/shaders
	
	glslangValidator -V -o ${BINARYDIR}/shaders/shaders.frag.spv shaders/shaders.frag
	glslangValidator -V -o ${BINARYDIR}/shaders/shaders.vert.spv shaders/shaders.vert

	g++ -g -o ${BINARYDIR}/${EXECUTABLE} $(LDFLAGS) $^

# Compile .cpp files into .o
$(OBJ): $(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)

	g++ $(CFLAGS) -c -g -o $@ $<

release:
	mkdir -p ${BINARYDIR}/shaders
	
	glslangValidator -V -o ${BINARYDIR}/shaders/shaders.frag.spv shaders/shaders.frag
	glslangValidator -V -o ${BINARYDIR}/shaders/shaders.vert.spv shaders/shaders.vert
	g++ $(CFLAGS) -O3 -D NDEBUG -o ${BINARYDIR}/${EXECUTABLE} $(LDFLAGS) $(SRC)

clean:
	rm -rf $(BINARYDIR)
	rm -rf $(BUILDDIR)
