// RenderingEngine.cpp : Defines the entry point for the console application.
//
#include "GLWindow.h"
#include <cstring>

int main(int argc, char **argv)
{
	WindowManager wm;

	if(argc > 1){
		const char* mode = argv[1];
		if(std::strcmp(mode, "3D") == 0){
			printf("Starting in 3D mode\n");
			wm.mainLoop();
		}
		if(std::strcmp(mode, "2D") == 0){
			printf("Starting in 2D mode\n");
			wm.noiseLoop();
		}
	}
	else
		wm.mainLoop();
}

