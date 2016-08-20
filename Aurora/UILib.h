#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class IL{

public:
	bool libSetup(string path);
	bool render(sf::RenderWindow& window);
	bool mouseClicked(sf::Vector2i mousePos);
	bool loadFont(string fontName, string fontLabel);
	bool loadFontFromMemory(const void* data, int sizeInBytes, string fontLabel);
	bool loadTextureFromMemory(const void* data, int sizeInBytes, string textureLabel);
	bool newTextLabel(int x, int y, string text, string font, int size, sf::Color color = sf::Color::White);
	bool newCheckbox(int x, int y, int vertexLenght, sf::Color color = sf::Color::White);
	bool newCheckbox(int x, int y, int vertexLenght, sf::Color color, string text, string font, int size, sf::Color colorText = sf::Color::White, int location = 0);
	bool newDropDown(int x, int y, string defaultText, std::vector<string> elements, string font, int size, sf::Color colorText = sf::Color::White, sf::Color colorFill = sf::Color::Black, sf::Color colorBorder = sf::Color::White);
	bool newRoundButton(sf::Vector2f position, sf::Vector2f size, int radius, sf::Color color, sf::Drawable* d = NULL);
private:

};