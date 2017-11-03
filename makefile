CC=g++


CFLAGS=-c -std=c++11 -O3 -Wall -g
LINKFLAGS=-O3

#debug = true
ifdef debug
	CFLAGS +=-g
	LINKFLAGS += -flto
endif

INCDIR= -I./include

LIBDIR=-L/usr/X11R6 -L/usr/local/lib

LIBS=



OS_NAME:=$(shell uname -s)

ifeq ($(OS_NAME),Darwin)
	LIBS += `pkg-config --static --libs glfw3 gl`
	LIBS += \
	 -lglfw3 \
	 -lGLEW \
	 -framework Cocoa \
	 -framework OpenGL \
	 -framework IOKit \
	-framework CoreVideo
endif
ifeq ($(OS_NAME),Linux)
	LIBS += `pkg-config --static --libs glfw3 gl`
endif

ENGINE_DIR=.

ENGINE_SOURCES=$(wildcard $(ENGINE_DIR)/*cpp)

ENGINE_HEADERS=-I$(ENGINE_DIR) 

HEADERS=-Iheaders -Iinclude

ENGINE_OBJS=$(addprefix $(ENGINE_DIR)/obj/,$(notdir $(ENGINE_SOURCES:.cpp=.o)))

.PHONY: default
default:  fetch buildDirectories $(ENGINE_DIR)/renderingengine.out
	echo "Build completed"
ifeq ($(OS_NAME),Darwin)
$(ENGINE_DIR)/renderingengine.out: librenderingcore.a librenderingextensions.a
	$(CC) $(LINK_FLAGS) $(ENGINE_OBJS) $(CORE_OBJS) $(EXTENSION_OBJS) -o $@ -L. $(LIBS)
endif
ifeq ($(OS_NAME),Linux)
$(ENGINE_DIR)/renderingengine.out: $(ENGINE_OBJS)
	$(CC) $(LINK_FLAGS) $(ENGINE_OBJS) -o $@ -L. -lrenderingextensions -lrenderingcore $(LIBS)
endif

all: $(ENGINE_DIR)/perlinNoise.out 

$(ENGINE_DIR)/obj/%.o: $(ENGINE_DIR)/%.cpp
	$(CC) -c $(CFLAGS) $(ENGINE_HEADERS) $(HEADERS) $(INCDIR) $< -o $@

.PHONY: fetch
fetch: 
	if [ ! -d "./shaders" ]; then \
		svn export https://github.com/JeremyAdamHart/RenderingEngineLibrary/trunk/RenderingEngine/shaders; \
	fi
	if [ ! -d "./headers" ]; then \
		svn export https://github.com/JeremyAdamHart/RenderingEngineLibrary/trunk/RenderingLibrary/headers && \
		svn export https://github.com/JeremyAdamHart/RenderingEngineLibrary/trunk/RenderingExtensions/headers/ --force ; \
	fi


.PHONY: buildDirectories
buildDirectories:
	mkdir -p $(ENGINE_DIR)/obj

.PHONY: clean
clean:
	rm *.a $(ENGINE_DIR)/*.out $(CORE_DIR)/obj/*.o $(EXTENSION_DIR)/obj/*.o $(ENGINE_DIR)/obj/*.o