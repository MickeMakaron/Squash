#include <iostream>

#include <SFML/Graphics.hpp>

int main()
{
	using namespace std;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Hello World!", sf::Style::Default);
	
	cout << "Hello World!" << endl;

	sf::CircleShape shape(60, 4);

	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
		}

		window.clear();

		window.draw(shape);

		window.display();

	}

	return 0;
}