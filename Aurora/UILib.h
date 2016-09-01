#include <string>
#include "RoundedRectangleShape.hpp"
#include "pictures.hpp"
#include "fonts.hpp"

using namespace std;



class IL{

public:
	bool closeAurora = false;

	bool libSetup();
	sf::Font loadFontFromMemory(const void* data, int sizeInBytes);
	sf::Texture loadTextureFromMemory(const void* data, int sizeInBytes);
	sf::Sprite newSprite(sf::Vector2f position, sf::Texture textureLabel, sf::Vector2f scale = sf::Vector2f(1.f, 1.f));
	sf::Text newTextLabel(int x, int y, string text, sf::Font font, int size, sf::Color color = sf::Color::White);
	sf::RoundedRectangleShape newRoundButton(sf::Vector2f position, sf::Vector2f size, int radius = 10, sf::Color color = sf::Color::White);

private:

};