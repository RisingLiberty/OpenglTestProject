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

//The color to output to the fragment shader
"out vec3 Color;\n"

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
"Color = color;"
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

"out vec4 outColor;\n"

"void main()\n"
"{\n"
"outColor = vec4(Color, 1.0f);\n"
"}\n";

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
		0.0f,  0.5f,  1.0f, 0.0f, 0.0f, //Vertex 1(x,y,r,g,b)
		0.5f, -0.5f,  0.0f, 1.0f, 0.0f, //Vertex 2(x,y,r,g,b)
	   -0.5f, -0.5f,  0.0f, 0.0f, 1.0f //Vertex 3(x,y,r,g,b)	
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
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	
	//Last but not least, the vertexattribarray should be enabled.
	glEnableVertexAttribArray(posAttrib);

	//The same for colorAttrib
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");

	//The fifth paramter is set to 5*sizeof(float) now, because each vertex consists of 5 floating point atttribute values.
	//The offset of 2*sizeof(float) for the color attribute is there because each vertex starts with 2 floating point
	//values for the position that it has to skip over.
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(colAttrib);

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
			
			glDrawArrays(GL_TRIANGLES, 0, 3);
		
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
