#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>

int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 900), "Clock");
	window.setFramerateLimit(60.0f);

	sf::RectangleShape background(sf::Vector2f(window.getSize().x,window.getSize().y));
	background.setFillColor(sf::Color(128, 128, 128));

	sf::Texture clocktexture;
	std::string clockpath = "images\\hdclock.png";
	if (!clocktexture.loadFromFile(clockpath))
	{
		std::cout << "Failed to load image\n";
	}

	clocktexture.setSmooth(1);

	sf::Texture secondtexture;
	std::string secondpath = "images\\SecondHandle.png";
	if (!secondtexture.loadFromFile(secondpath))
	{
		std::cout << "Failed to load image\n";
	}
	secondtexture.setSmooth(1);

	sf::Texture minutetexture;
	std::string minutepath = "images\\MinuteHandle.png";
	if (!minutetexture.loadFromFile(minutepath))
	{
		std::cout << "Failed to load image\n";
	}
	//minutetexture.setSmooth(1);

	sf::Texture hourtexture;
	std::string hourpath = "images\\HourHandle.png";
	if (!hourtexture.loadFromFile(hourpath))
	{
		std::cout << "Failed to load image\n";
	}
	//hourtexture.setSmooth(1);

	sf::Sprite clock;
	clock.setTexture(clocktexture);
	clock.setOrigin(clocktexture.getSize().x / 2, clocktexture.getSize().y / 2);
	clock.setPosition(sf::Vector2f(window.getSize().x/2,window.getSize().y/2));
	clock.setScale((window.getSize().x - (window.getSize().x / 10))  / clock.getLocalBounds().width, (window.getSize().y - window.getSize().y/10) / clock.getLocalBounds().height);

	sf::Sprite second;
	second.setTexture(secondtexture);
	second.setOrigin(secondtexture.getSize().x / 2, secondtexture.getSize().y);
	second.setPosition(sf::Vector2f(clock.getPosition().x,clock.getPosition().y));
	second.setScale(1.0f * clock.getScale().x, 2.3f * clock.getScale().y);

	sf::Sprite minute;
	minute.setTexture(minutetexture);
	minute.setOrigin(6,142);
	minute.setPosition(sf::Vector2f(clock.getPosition().x, clock.getPosition().y));
	minute.setScale(1.0f * clock.getScale().x, 2.8f * clock.getScale().y);

	sf::Sprite hour;
	hour.setTexture(hourtexture);
	hour.setOrigin(7, 94);
	hour.setPosition(sf::Vector2f(clock.getPosition().x, clock.getPosition().y));
	hour.setScale(1.0f * clock.getScale().x, 2.6f * clock.getScale().y);

	time_t t;
	t = time(NULL);
	char* tm = ctime(&t);
	
	struct tm* tmp = localtime(&t);

	const double secondphase = (double)360.0 / 60.0 / 60.0;
	const double minutephase = secondphase / 60.0;
	const double hourphase = minutephase / 12.0;

	double secRot = tmp->tm_sec * 6.0;
	double minRot = tmp->tm_min * 6.0 + ((tmp->tm_sec * 6.0) / 60.0);
	double hourRot = (tmp->tm_hour % 12) * 30.0 + tmp->tm_min * 30.0 / 60.0;

	sf::Clock updateTime;
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
			{
				int smallwinsize = window.getSize().x < window.getSize().y ? window.getSize().x : window.getSize().y;
				std::cout << "Resized\n";

				sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
				window.setView(view);

				clock.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));

				second.setPosition(sf::Vector2f(clock.getPosition().x, clock.getPosition().y));
				minute.setPosition(sf::Vector2f(clock.getPosition().x, clock.getPosition().y));
				hour.setPosition(sf::Vector2f(clock.getPosition().x, clock.getPosition().y));
				background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));

				clock.setScale((smallwinsize - (smallwinsize / 10)) / clock.getLocalBounds().width, (smallwinsize - smallwinsize / 10) / clock.getLocalBounds().height);
				second.setScale(1.0f * clock.getScale().x, 2.3f * clock.getScale().y);
				minute.setScale(1.0f * clock.getScale().x, 2.8f * clock.getScale().y);
				hour.setScale(1.0f * clock.getScale().x, 2.6f * clock.getScale().y);
			}
		}
		
		if (updateTime.getElapsedTime().asSeconds() > 5)
		{
			time_t t;
			t = time(NULL);
			char* tm = ctime(&t);

			struct tm* tmp = localtime(&t);

			secRot = tmp->tm_sec * 6.0;
			minRot = tmp->tm_min * 6.0 + ((tmp->tm_sec * 6.0) / 60.0);
			hourRot = (tmp->tm_hour % 12) * 30.0 + tmp->tm_min * 30.0 / 60.0;
			//reupdate times

			updateTime.restart();
		}

		second.setRotation(secRot += secondphase);
		minute.setRotation(minRot += minutephase);
		hour.setRotation(hourRot += hourphase);
		//update stuff

		//window display
		window.clear();

		window.draw(background);
		window.draw(clock);
		window.draw(minute);
		window.draw(second);
		window.draw(hour);
		window.display();
	}

	return 0;
}