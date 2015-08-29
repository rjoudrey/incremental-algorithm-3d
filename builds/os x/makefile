all:
	sudo ditto SDL.framework /Library/Frameworks/SDL.framework; \
	rm -f CG_IncrementalAlgorithm; \
	g++ \
	-I/Library/Frameworks/SDL.framework/Headers \
	-I/System/Library/Frameworks/OpenGL.framework/Headers \
	-F/Library/Frameworks \
	SDLmain.m -framework SDL -framework Cocoa -framework OpenGL \
	../../source/main.cpp \
	../../source/platform.cpp \
	../../source/render.cpp \
	../../source/system.cpp \
	../../source/camera.cpp \
	../../source/Math.cpp \
	../../source/Color.cpp \
	../../source/Light.cpp \
	../../source/shader.cpp \
	-o CG_IncrementalAlgorithm; ./CG_IncrementalAlgorithm
