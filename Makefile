EXECUTABLE-NAME = parallax
BINARY-FOLDER = ./bin
INCLUDE = ./include

CFLAGS = -std=c++17 -Wall -Wextra -iquote $(INCLUDE)
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SRC = $(wildcard ./src/*.cpp) $(wildcard ./src/vulkan/*.cpp)

.PHONY: test clean release

# TODO: Add shader target possibly to have object files created as well for the C++ files
${BINARY-FOLDER}/$(EXECUTABLE-NAME): $(SRC)
	mkdir -p ${BINARY-FOLDER}/shaders
	
	glslangValidator -V -o ${BINARY-FOLDER}/shaders/shaders.frag.spv shaders/shaders.frag
	glslangValidator -V -o ${BINARY-FOLDER}/shaders/shaders.vert.spv shaders/shaders.vert
	g++ $(CFLAGS) -g -o ${BINARY-FOLDER}/${EXECUTABLE-NAME} $(LDFLAGS) $(SRC)

release:
	mkdir -p ${BINARY-FOLDER}/shaders
	
	glslangValidator -V -o ${BINARY-FOLDER}/shaders/shaders.frag.spv shaders/shaders.frag
	glslangValidator -V -o ${BINARY-FOLDER}/shaders/shaders.vert.spv shaders/shaders.vert
	g++ $(CFLAGS) -O3 -D NDEBUG -o ${BINARY-FOLDER}/${EXECUTABLE-NAME} $(LDFLAGS) $(SRC)

test: ${EXECUTABLE-NAME}
	${BINARY-FOLDER}/${EXECUTABLE-NAME}

clean:
	rm -rf $(BINARY-FOLDER)
