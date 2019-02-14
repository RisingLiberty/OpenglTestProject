#include <iostream>

#include <SFML/Window.hpp>


int main()
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
}