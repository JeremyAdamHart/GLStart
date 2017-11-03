<h1>Perlin noise</h1>

Program designed to view and manipulate 2D and 3D perlin noise.

<h2>Building</h2>

Type 'make' in the root directory to build. If it fails, try to delete the headers and shaders folders and type 'make' again.

<h2>Instructions</h2>

By default the program will open in 3D mode. The mode can be specified explicitly as the first command line argument, either 3D or 2D. 

The patterns of the noise can be manipulated by modifying the shaders. Press the space key to recompile the shader.

For the 2D mode, the following parameters are useful to modify in shaders/pnoise2D.frag:

<p style="font-family: monospace;"> 
//Perlin noise values
uniform int octaveNum = 5;			//The number of octaves of noise ie: Number of layers of noise stacked ontop of each other
uniform float baseWidth = 0.1;		//The width of the lowest resolution grid
uniform float persistance = 0.5;	//Rate of falloff of perlin noise
uniform int seedValue = 71;			//Change to get new random values
uniform float turbulence = 0.5;		//Impact of noise on distoring base function

...

main()
...

float intensity = noise;
</p>

As well as changing the intensity function in main.

Corresponding parameters also exist in shaders/ambient.frag. Similarily changing this function to sinLines can modify the pattern.

<p style="font-family: monospace;">
	float intensity = radialSin(coord, noise*turbulence);
</p>



