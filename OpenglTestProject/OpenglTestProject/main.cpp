#define INCLUDE_ALL

//#define GL_TEST
//#define SFML_TEST
//#define SDL_TEST

#include <iostream>
#include <thread>

#if defined GL_TEST || defined INCLUDE_ALL
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#endif

#if defined SFML_TEST || defined INCLUDE_ALL
#include <SFML/Window.hpp>
#endif

#if defined SDL_TEST || defined INCLUDE_ALL
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#undef main

#if defined GL_TEST
int GLFWTest()
{
	glfwInit();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	glfwTerminate();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* pWindow = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); //Windowed
	//GLFWwindow* pWindow = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); //Fullscreen

	glfwMakeContextCurrent(pWindow);

	glewExperimental = GL_TRUE;
	glewInit();

	//won't work without context
	auto version = glGetString(GL_VERSION);

	std::cout << version << std::endl;

	//GLuint vertexBuffer;
	//glGenBuffers(1, &vertexBuffer);

	//printf("%u\n", vertexBuffer);
	glfwTerminate();

	std::cin.get();
	
	return 0;
}

#elif defined SFML_TEST
int SFMLTest()
{
	sf::ContextSettings settings;

	//Equivalent to DXGO_FORMAT_D24_UNORM_S8_UNORM
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 2; //Optional

									// Request OpenGL version 3.2
	settings.majorVersion = 3;
	settings.minorVersion = 2;
	settings.attributeFlags = sf::ContextSettings::Core;

	//The basic constructor takes an sf::VideoMode strucute, a title for the window and a window style.
	//The sf::VideoMode structure specifies the widht, height and optionally the pixel depth of the window.
	//Finally, the requirement for a fixed size window is specified by overriding the default style of Style::Resize|Style::Close.
	//It is also possible to create a fullscreen window by passing Style::Fullscreen as window style.
	sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Close, settings);

	bool running = true;
	while (running)
	{
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			switch (windowEvent.type)
			{
			case sf::Event::Closed:
				running = false;
				break;
			case sf::Event::KeyPressed:
				if (windowEvent.key.code == sf::Keyboard::Escape)
					running = false;
				break;
			}
		}
	}

	return 0;
}

#elif defined SDL_TEST
int SdlTest()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//SDL_WINDOW_OPENGL: Create a window ready for OpenGL
	//SDL_WINDOW_RESIZABLE: create a resizable window.
	//SDL_WINDOW_FULLSCREEN: <Optional> Create a fullscreen window.
	SDL_Window* pWindow = SDL_CreateWindow("OpenGL", 100, 100, 800, 800, SDL_WINDOW_OPENGL);

	SDL_GLContext context = SDL_GL_CreateContext(pWindow);

	SDL_Event windowEvent;

	glewExperimental = GL_TRUE;
	glewInit();

	auto version = glGetString(GL_VERSION);

	while (true)
	{
		//The SDL_PollEvent function will check if there are any new events that have to be handled.
		//An event can be anything from a mouse click to the user moving the window.
		//Right now, the only event you need to respond to is the user pressing the little X button in the corner of the window
		//By breaking from the main loop, SDL_Quit is called and the window and graphics surface are destroyed.
		//SDL_GL_SwapWindow here takes care of swapping the front and back buffer after new things have been drawn by your application.
		//If you have a fullscreen window, it would be preferable to use the escape key as a means to close the window.
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
			else if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
			
		}
		SDL_GL_SwapWindow(pWindow);
	}

	SDL_GL_DeleteContext(context);

	SDL_Quit();

	return 0;
}

#endif

const char* vertexSource =
//from OpenGL version 3.3 shader version is equal to OpenGL version
//The #version preprocessor directive is used to indicate that the code that follows i GLSL 1.50 code
//using OpenGL's core profile.
"#version 150 core\n"

//Next we specify that there is only 1 attribute, the position
"in vec2 position;\n"
"in vec3 color;\n"
"in vec2 texcoord;\n"

//The color to output to the fragment shader
"out vec3 Color;\n"
"out vec2 Texcoord;"

//Apart from regular C types, GLSL has built-in vector and matrix types
//identified by vec* and mat* identifiers.
//the values within these constructs is always a float.
//The number after vec specifies the number of components(x,y,z,w) and
//the number after mat specifies the number of rows/columns.
//Since the position attribute consists of only an x and y coordinate, vec2 is perfect
"void main()\n"
"{\n"

//You can be quite creative when working with vertex types.
//In the example above a shortcuts was used to set the first two components of the vec4
//to those of vec2. the following 2 lines are equal
//gl_Position = vec(position, 0.0f, 1.0f);
//gl_Position = vec(position.x, position.y, 0.0f, 1.0f);
//When you're working with colors, you can also access the individual components with r, g, b and a
//instead of x, y, z and w. this makes no difference and can help with clarity.
//The final position of the vertex assigned to the special gl_Position variable,
//because the position is needed for primitive assembly and many other built-in processes.
//For these to function correctly, the last value w needs to have a value of 1.0f.
//Other than that, you're free to do anything you want with the attributes.
"gl_Position = vec4(position, 0.0f, 1.0f);\n"
"Color = color;\n"
"Texcoord = texcoord;\n"
"}\n";

const char* fragmentSource =
"#version 150 core\n"

//You'll immediately notice that we're not using some built-in variable for outputting the color, say gl_FragColor.
//This is because a fragment shader can in fact output multiple colors.
//The outColor variable uses the type vec4, because each color consists of a red, green, blue and alpha component.
//Colors in OpenGL are generally represented as floating point number between 0.0 and 1.0 instead of the common 0 and 255.

//Vertex attributes are not the only way to pass data to shader programs. There is another way to pass data to shaders called uniforms.
//These are essentially global variables, having the same value for all vertices and/or fragments.
"in vec3 Color;\n"
"in vec2 Texcoord;\n"
"out vec4 outColor;\n"

"uniform sampler2D texHalo;\n"
"uniform sampler2D texGoogle;\n"

"void main()\n"
"{\n"
"vec4 colHalo = texture(texHalo, Texcoord);// * vec4(Color, 1.0f);\n"
"vec4 colGoogle = texture(texGoogle, Texcoord);// * vec4(Color, 1.0f);\n"

//the mix function is a special GLSL function that linearly interpolates between 2 variables based on the third parameter.
//A value of 0.0 will result in the first value, a value of 1.0 will result in the second value and a value in between will
//result in a mixture of both.
"outColor = mix(colHalo, colGoogle, 0.5f);"
"}\n";

#include <stdio.h>

int main()
{
	int errResult = 0;
	
#pragma region Test
#ifdef SFML_TEST
	errResult = SFMLTest();
	if (errResult != 0) return errResult;
#endif // SFML_TEST

#ifdef SDL_TEST
	errResult = SdlTest();
	if (errResult != 0) return errResult;
#endif // SDL_TEST

#ifdef GL_TEST
	errResult = GLFWTest();
	if (errResult != 0) return errResult;
#else
#pragma endregion

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.majorVersion = 3;
	settings.minorVersion = 2;

	sf::Window window(sf::VideoMode(800, 600, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();


	//The array should simply be a list of all vertices with their attributes packed together.
	//The order in which the attributes appear doesn't matter, as long as it's the same for each vertex.
	//the order of the vertices doesn't have to be sequential(i.e. the order in which shapes are formed),
	//but this requires us to provide extra data in the form of an element buffer.
	float vertices[] =
	{
	//	Position	    	   Color			  UV
	   -0.5f,  0.5f,     1.0f, 0.0f, 0.0f,    0.0f, 0.0f, //Vertex 1(x,y,r,g,b)
		0.5f,  0.5f,     0.0f, 1.0f, 0.0f,    1.0f, 0.0f, //Vertex 2(x,y,r,g,b)
	    0.5f, -0.5f,     0.0f, 0.0f, 1.0f,	  1.0f, 1.0f, //Vertex 3(x,y,r,g,b)
	   -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,    0.0f, 1.0f  //Vertex 4(x,y,r,g,b)
	};

	GLuint indices[]
	{
		0,1,2,
		2,3,0
	};

	//You can imagine that real graphics programs use many different shaders and vertex layouts to take care of a wide variety of needs and special effects.
	//Changing the active shader program is easy enough with a call to glUseProgram, but it would be quite inconvenient if you had to set up all of the attributes again every time.

	//Luckily, OpenGL solves that problem with Vertex Array Objects (VAO). VAOs store all of the links between the attributes and your VBOs with raw vertex data
	//A VAO is created in the same way as a VBO.
	GLuint vao;
	glGenVertexArrays(1, &vao);

	//To start using is, simply bind it
	glBindVertexArray(vao);

	//The next step is to upload this vertex data to the graphics card. 
	//This is important because the memory on your graphics card is much faster and
	//you won't have to send the data again every time your scene needs to rendered.

	//This is done by creating a Vertex Buffer Object
	GLuint vbo;

	//The memory is managed by OpenGL, so instead of a pointer you get a positive number as a reference to it.
	//GLuint is simply a cross-platform substitute for unsigned int, just like GLint is one for int.
	//You will need this number to make the VBO active and to destroy it when you're done with it.
	glGenBuffers(1, &vbo);

	//To upload the actual data to it you first have to make it the active object by calling glBindBuffer
	//As hinted by the GL_ARRAY_BUFFER enum value, there are other types of buffers.
	//This statement makes the VBO we just created the active array buffer. 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//now that it's active we can copy the vertex data to it.
	//Notice that this function doesn't refer to the id of our VBO,
	//but instead to the active array buffer. The second parameter specifies the size in byes.
	//The final parameter is very important and its value depends on the usage of the vertex data.
	//Possible usages:
	//GL_STATIC_DRAW: The vertex data will be uploaded once and drawn many times (eg. the world)
	//GL_DYNAMIC_DRAW: The vertex data will be created once, changed from time to time, but drawn many times more than that.
	//GL_STREAM_DRAW: the vertex data will be uploaded once and drawn once.

	//This usage value will determine in what kind of memory the data is stored on your graphics card for the highest efficiency.
	//For example, VBOs with GL_STREAM_DRAW as type may store their data in memory that allows faster writing in favor of slightly slower drawing.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//An element array is filled with unsigned integers referring to vertices bound to GL_ARRAY_BUFFER.
	//They are loaded into video memory through a VBO just like the vertex data.
	GLuint ebo;
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	//The only thing that differs is the GL_ELEMENT_ARRAY_BUFFER.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Just like vertex buffers, creating a shade itself starts with creating a shader object and loading data into it
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Unlike VBOs, you can simply pass a reference to shader functions instead of making it active
	//or anything like that. The glShaderSource function can take multiple source string in an array,
	//but you'll usually have your source code in one char array. The last parameter can contain an array of source code string lengths.
	//passing NULL simply makes it stop at the null terminator.
	glShaderSource(vertexShader, 1, &vertexSource, NULL);

	//Compile the shader
	glCompileShader(vertexShader);

	GLint status;

	//get the compile status
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	//buffer for debug info
	char buffer[512];

	//get the debug info of the shader compilation
	glGetShaderInfoLog(vertexShader, 512, NULL, buffer);

	//Successful compilation?
	if (status == GL_FALSE)
	{
		std::cout << "Vertex shader compile error" << std::endl;
		std::cout << buffer << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);

	if (status == GL_FALSE)
	{
		std::cout << "Fragment shader compile error" << std::endl;
		std::cout << buffer << std::endl;
	}

	//Up until now the vertex and fragment shader have been 2 separate objects.
	//While they've been programmed to work together, they aren't actually connected yet.
	//This connection is made by creating a program out of these 2 shader.
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//Since a fragment shader is allowed to write to multiple buffers,
	//you need to explicitly specify which output is written to which buffer.
	//This needs to happen before linking the program. 
	//However, since this is 0 by default and there's only one output right now
	//the following line of code is not necessary
	glBindFragDataLocation(shaderProgram, 0, "outColor");

	//USe glDrawBuffers when rendering to multiple buffers, because only the first output will be enabled by default.

	//After attaching both the fragment and vertex shaders, the connection is made by linking the program.
	//It i s allowed to make changes to the shaders after they've been added to a program (or multiple programs),
	//but the actual result will not change until a program has been linked again. It is also possible to attach
	//multiple shaders for the same stage (e.g. fragment) if they're parts forming the whole shader together.
	//A shader object can be deleted with glDeleteShader, but it will not actually be removed before it has been
	//detached from all programs with glDetachShader.
	glLinkProgram(shaderProgram);

	//to actually start using the shaders in the program, you just have to call glUseProgram
	glUseProgram(shaderProgram);

	//Just like a vertex buffer, only one program can be active at a time.

	//Although we have our vertex data and shaders now, OpenGL still doesn't know how the attributes are formatted and ordered. 
	//You first need to retrieve a reference to the position input in the vertex shader
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");

	//The location is a number depending on the order of the input definitions.
	//The first and only input position in this example will always have location 0
	//With the reference to the input, you can specify how the data for that input is retrieved from the array.
	//The first parameter references the input.
	//The second parameter specifies the number of values for that input, which is the same as the number of components
	//The third parameter specifies what type the values are
	//The fourth parameter specifies whether the input values should be normalized between -1.0 and 1.0 or
	//0.0 and 1.0 depending on the format if they aren't floating point numbers.
	//the last 2 parameters are arguably the most important here as they specify how the attribute is laid out in the vertex array.
	//The first numbers specifies the stride, or how many bytes are between each position attribute in the array.
	//The value 0 means that there is no data in between. This is currently the case as the position of each vertex
	//is immediately followed by the position of the next vertex.
	//the last parameter specifies the offset, or how many bytes from the start of the array the attribute occurs.
	//Since there are no other attributes, this is 0 as well.

	//It is important to know that this function will store not only the stride and the offset, but also the VBO that is currently bound to GL_ARRAY_BUFFER.
	//that means that you don't have to explicitly bind the correct VBO when the actual drawing functions are called.
	//This also implies that you can use a different VBO for each attribute
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
	
	//Last but not least, the vertexattribarray should be enabled.
	glEnableVertexAttribArray(posAttrib);

	//The same for colorAttrib
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");

	//The fifth paramter is set to 5*sizeof(float) now, because each vertex consists of 5 floating point atttribute values.
	//The offset of 2*sizeof(float) for the color attribute is there because each vertex starts with 2 floating point
	//values for the position that it has to skip over.
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(colAttrib);

	GLint texAttrib = (glGetAttribLocation(shaderProgram, "texcoord"));
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));

	//As soon as you've bound a certain VAO, every time you call glVertexAttribPointer,
	//that information will be stored in that VAO. This makes switching between different
	//vertex data and vertex formats as easy as binding a different VAO!
	//Just remember that a VAO doesn't store any vertex data by itself, it just references the VBOs
	//you've created and how to retrieve the attribute values from them.

	//Since only call after binding a VAO stick to it, make sure that you've created and bound the VAO at the start
	//of your program. Any vertex buffers and element buffers bound before it will be ignored.

	//Now it's time to draw a triangle!
	//the VAO that was used to store the attribute information is already bound, so you don't have to
	//worry about that. All that's left is to simply call glDrawArrays in your main loop

	//The first parameter specifies the kind of primitive(commonly point, line or triangle),
	//the second parameter specifies how many vertices to skip at the beginning
	//the last parameter specifies the number of vertices to process.
	bool running = true;
	
	auto t_start = std::chrono::high_resolution_clock::now();

	//Textures are typically used for images to decorate 3D models, but in reality
	//they can be used to store many different kinds of data. It's possible to have 1D, 2D and even 3D textures, 
	//which can be used to store bulk data on the GPU. an example of another use for textures is storing certain
	//terrain information.
	GLuint textures[2];
	glGenTextures(2, textures);

	//Just like other objects, textures have to be bound to apply operations on them.
	//Since images are 2D arrays of pixels, it will be bound to the GL_TEXTURE_2D target.

	//The pixels in the texture will be addressed using texture coordinates during drawing operations.
	//These coordinates range from 0.0 to 1.0 where (0,0) is conventionally the bottom-left corner and (1,1) is the top right corner of the texture image.
	//The operation that uses these texture coordinates to retrieve color information from the pixels is called sampling.
	//There are different ways to approach this problem, each being appropriate for different scenarios.
	//OpenGL offers you many options to control how this sampling is done.

	//Black/White checkerboard
	float pixels [] = 
	{
		0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,	0.0f, 0.0f, 0.0f
	};


	//Without using the method glActiveTexture, the texture is automatically bound to GL_TEXTURE0
	//That's why the default value of 0 for the sampler in the shader works just fine.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	//The amount of texture units supported differs per graphics card, but it will be at least 48.
	//It is safe to say that you will never hit this limit in even the most extreme graphics applications

	int width, height, texChannels;
	unsigned char* image = stbi_load("../../Data/HaloInfinite.png", &width, &height, nullptr, STBI_rgb);

	//The first parameter after the texture target is the level of detail, where 0 is the base image.
	//This parameter can be used to load your own mipmap images.
	//The second parameter specifies the internal pixel format, the format in which pixels should be stored on the gpu.
	//many different formats are available, including compressed formats, so it's certainly worth taking a look at all of the options.
	//The third and fourth parameters specify the width and height of the image.
	//the fifth parameter should always have a value of 0 per specification.
	//The next 2 parameters describe the format of the pixels in the array that will be loaded.
	//The final parameters specifies the array itself.
	//the functions begins loading the image at coordinate (0,0) so pay attention to this.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//You can clean up the image data right after you've loaded into the texture.
	stbi_image_free(image);
	glUniform1i(glGetUniformLocation(shaderProgram, "texHalo"), 0);

	//The first thing you'll have to consider is how the texture should be sampled when a coordinate outside the range of 0 to1 is given. 
//OpenGL offers 4 ways of handling this.
//GL_REPEAT: The integer part of the coordinate will be ignored and a repeating pattern is formed.
//GL_MIRROERED_REPEAT: The texture will also be repeated, but it will be mirrored when the integer part of the coordinate is odd.
//GL_CLAMP_TO_EDGE: the coordinate will simply be clamped between 0 and 1.
//GL_CLAMP_TO_BORDER: the coordinates that fall outside the range will be given a specified border color.

	//As before, the i here indicates the type of value you want to specify. If you use GL_CLAMP_TO_BORDER 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//and you want to change the border color, you need to change the value of GL_TEXTURE_BORDER_COLOR bypassing an RGBA float array
	float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

	//Since texture coordinates are resolution independent, they won't always match a pixel exactly.
	//this happens when a texture image is stretched beyond its original size or when it's sized down. OpenGL offers various methods to decide
	//on the sampled color when this happens. This process is called filtering and the following methods are available.

	//GL_NEAREST: returns the pixel is closest to the coordinates.
	//GL_LINEAR: returns the weighted average of the 4 pixels surrounding the given coordinates.
	//GL_NEAREST_MIPMAP, GL_LINEAR_MIP_MAP_NEAREST,
	//GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR: Sample from mipmaps instead.

	//There is another way to filter textures: mipmaps
	//Mipmaps are smaller copies of your textures that have been sized down and filtered in advance.
	//It is recommended that you use them because they result in both a higher quality and higher performance.

	//To use mipmaps, select one of the four mipmap filtering methods.
	//GL_NEAREST_MIPMAP_NEAREST: Uses the mipmap that most  closely matches the size of the pixel being textured
	//and samples with nearest neighbor interpolation
	//GL_LINEAR_MIPMAP_NEAREST: Samples the closest mipmap with linear interpolation
	//GL_NEAREST_MIPMAP_LINEAR: Uses the 2 mipmaps that most closely match the size of the pixel being textures
	//and samples with nearest neighbor interpolation.
	//GL_LINEAR_MIPMAP_LINEAR: Samples closet two mipmaps with linear 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	image = stbi_load("../../Data/img.png", &width, &height, nullptr, STBI_rgb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	stbi_image_free(image);
	glUniform1i(glGetUniformLocation(shaderProgram, "texGoogle"), 1);

	//As before, the i here indicates the type of value you want to specify. If you use GL_CLAMP_TO_BORDER 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//As mentioned before, OpenGL expects the first pixel to be located in the bottom-left corner, 
	//which means that textures will be flipped when loaded with STB directly. To counteract that, the code 
	//will use flipped Y coordinates for texture coordinates from now on.
	//That means that (0,0) will be assumed to be the top0left corner instead of the bottom-left. This practice might
	//make texture coordinates more intuitive as a side-effect.

	//But how is the pixel array itself established? Textures in graphics applications will usually be a lot more sophisticated than simple patterns and
	//will be loaded from files. Best practice is to ave your files in a format that is natively supported by the hardware, but it may sometimes be more convenient to load
	//textures from common image formats like JPG and PNG. Unfortunately OpenGL doesn't offer any helper functions to load pixels from these image files, 
	//but that's where third-party libraries come in handy again!

	while (running)
	{
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			switch (windowEvent.type)
			{
			case sf::Event::Closed:
				running = false;
				break;
			case sf::Event::KeyPressed:
				if (windowEvent.key.code == sf::Keyboard::Escape)
					running = false;
				break;
			}

			//Changing the value of a uniform is just like setting vertex attributes, you first have to grab the location.
			GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");

			//The values of uniforms are changed with any of the glUnifromXY functions, where X is the number of
			//components and Y is the type. Common types are f(float), d(double) and i(integer)
			auto t_now = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

			float redValue = (sin(time) + 1.0f) * 0.5f;
			
			std::cout << redValue << std::endl;

			glUniform3f(uniColor, redValue, 0.0f, 0.0f);

			//Clear the screen to black
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			
			//The first parameter is the same as with glDrawArray, but the other ones all refer to the element buffer.
			//The second parameter specifies the number of indices to draw
			//the third parameter specifies the type of the element data
			//The last parameter specifies the offset.
			//The only real difference is that you're talking about indices instead of vertices now.
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
			//Swap buffers
			window.display();

		}
	}

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);

	window.close();

	return errResult;
#endif
}
