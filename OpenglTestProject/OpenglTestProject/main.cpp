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

	glfwInit();

	//Let's create the window.
	//before we create the actual window, we first set some options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	//The GLFW_OPENGL_PROFILE option specifies that we want a context that only support the new core functionality.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//The first 2 parameters of glfwCreateWindow specify the width and height of the drawing surface.
	//the third parameter specifies the window title.
	//The fourth parameter should be set to NULL for windowed mode, and glfwGetPrimaryMonitor for fullscreen mode
	//the last parameter allows you to specify an existing OpenGL context to share resources like textures with.
	//The glfwWindowHint function is used to specify additional requirements for a window.
	GLFWwindow* pWindow = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); //Windowed
	//GLFWwindow* pWindow = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); //Fullscreen

	glfwMakeContextCurrent(pWindow);
	
	/*
	
	//Specify prototype of function
	typedef void (*GENBUFFERS) (GLsizei, GLuint*)
	
	//Load address of function and assign it to a function pointer
	GENBUFFERS glGenBuffers = (GENBUFFERS)wglGetProcAddress("glGenBuffers");

	//or Linux:
	GENBUFFERS glGenBuffers = (GENBUFFERS)glXGetProcAddres((const GLubyte *) "glGenBuffers");

	//or OSX
	GENBUFFERS glGenBuffers = (GENBUFFERS)NSGLGetProcAddress("glGenBuffers");

	//Call function as normal
	GLuint buffer;
	glGenBuffers(1, &buffer);
	
	*/
	
	while (!glfwWindowShouldClose(pWindow))
	{
		//Swap front and back buffer
		glfwSwapBuffers(pWindow);
		glfwPollEvents();

		//For fullscreen:
		if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(pWindow, GL_TRUE);
	}

	glfwTerminate();

	return errResult;
#endif
}
