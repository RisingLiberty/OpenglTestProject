#include <iostream>

#include <SFML/Window.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#undef main

int main()
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
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
		}
		SDL_GL_SwapWindow(pWindow);
	}

	SDL_GL_DeleteContext(context);

	return 0;

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


	SDL_Quit();

	return 0;
}