EXECUTABLE-NAME = parallax
BINARY-FOLDER = ./bin

CFLAGS = -std=c++17 -Wall -Wextra -iquote ./include
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

parallax: src/main.cpp
	mkdir -p ./bin
	
	g++ $(CFLAGS) -g -o ${BINARY-FOLDER}/${EXECUTABLE-NAME} $(LDFLAGS) ./src/*.cpp ./src/vulkan/*.cpp
	
.PHONY: test clean

release:
	mkdir -p ./bin

	g++ $(CFLAGS) -O3 -D NDEBUG -o ${BINARY-FOLDER}/${EXECUTABLE-NAME} $(LDFLAGS) ./src/*.cpp ./src/vulkan/*.cpp

shaders:
	mkdir -p ./bin/shaders

	glslangValidator -V -o ${BINARY-FOLDER}/shaders/shaders.frag.spv shaders/shaders.frag
	glslangValidator -V -o ${BINARY-FOLDER}/shaders/shaders.vert.spv shaders/shaders.vert
.PHONY: shaders

test: ${EXECUTABLE-NAME}
	${BINARY-FOLDER}/${EXECUTABLE-NAME}

clean:
	rm -rf ./bin/${EXECUTABLE-NAME}
	rm -rf ./bin/shaders/*