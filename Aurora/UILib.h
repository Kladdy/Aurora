#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class IL{

public:
	bool closeAurora = false;

	bool libSetup(string path = "c");
	bool render(sf::RenderWindow& window, sf::Vector2i mousePos);
	bool mouseClicked(sf::Vector2i mousePos, int buttonClicked);
	bool loadFont(string fontName, string fontLabel);
	bool loadFontFromMemory(const void* data, int sizeInBytes, string fontLabel);
	bool loadTextureFromMemory(const void* data, int sizeInBytes, string textureLabel);
	bool newFade(sf::Vector2f size, int durationInMilliSeconds);
	bool newSprite(sf::Vector2f position, string textureLabel, sf::Vector2f scale = sf::Vector2f(1, 1));
	bool addStrip(int voltage, string pins, string textureLabel, bool addressable);
	bool newTextLabel(int x, int y, string text, string font, int size, sf::Color color = sf::Color::White);
	bool newCheckbox(int x, int y, int vertexLenght, sf::Color color = sf::Color::White);
	bool newCheckbox(int x, int y, int vertexLenght, sf::Color color, string text, string font, int size, sf::Color colorText = sf::Color::White, int location = 0);
	bool newDropDown(int x, int y, string defaultText, std::vector<string> elements, string font, int size, sf::Color colorText = sf::Color::White, sf::Color colorFill = sf::Color::Black, sf::Color colorBorder = sf::Color::White);
	bool newRoundButton(sf::Vector2f position, sf::Vector2f size, int radius, sf::Color color = sf::Color(255, 255, 255), sf::Drawable* d = NULL);
	bool highlightRoundbox(sf::Vector2i mousePos);
	bool updateSetup();
	bool updateFade(int ID);

private:

};