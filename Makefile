EXECUTABLE-NAME = parallax
CFLAGS = -std=c++17 -g -Wall -Wextra -iquote ./include
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

RELEASE-CFLAGS = -std=c++17 -O3 -Wall -Wextra -iquote ./include

parallax: src/main.cpp
	g++ $(CFLAGS) -o ${EXECUTABLE-NAME} $(LDFLAGS) ./src/*.cpp ./src/vulkan/*.cpp
	glslangValidator -V -o shaders/shaders.frag.spv shaders/shaders.frag
	glslangValidator -V -o shaders/shaders.vert.spv shaders/shaders.vert
	
.PHONY: test clean

release:
	g++ $(RELEASE-CFLAGS) -D NDEBUG -o ${EXECUTABLE-NAME} $(LDFLAGS) ./src/*.cpp ./src/vulkan/*.cpp
	glslangValidator -V -o shaders/shaders.frag.spv shaders/shaders.frag
	glslangValidator -V -o shaders/shaders.vert.spv shaders/shaders.vert

test: ${EXECUTABLE-NAME}
	./${EXECUTABLE-NAME}

clean:
	rm -f ${EXECUTABLE-NAME}
	rm -f ./*.o
	rm -f ./shaders/*.spv
