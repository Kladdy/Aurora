#include "MainWindow.h"
#include "pictures.hpp"
#include "fonts.hpp"
#include <cmath>

using namespace std;

namespace mainWin {
	struct LightningModeButton
	{
		void update()
		{
			if(destinationY != currentPosition.y)
			{
				currentPosition.y += (destinationY - currentPosition.y) / 6.f; //tweak this for different transition speed
				if(abs(destinationY - currentPosition.y) < 1)
					currentPosition.y = destinationY;
			}

			sprite.setPosition(currentPosition);

			if(hovered)
				textOpacity = min(textOpacity+10, 255); //tweak this for the text to appear quicker
			else
				textOpacity = max(textOpacity-10, 0); //and this for it to disappear quicker
		}

		sf::FloatRect getRect() { return sprite.getGlobalBounds(); }

		bool hovered = false;
		float destinationY;
		sf::Uint8 textOpacity = 0;
		std::string name;

		sf::Vector2f startPosition;
		sf::Vector2f currentPosition;
		sf::Sprite sprite;
	};

	std::vector<LightningModeButton> lightningModes;
	int hoveredMode = 0;

	//Fonts
	std::map<std::string, sf::Font> textFonts;

	//Text labels
	vector<sf::Text> textLabels;

	//Textures
	std::map<std::string, sf::Texture> loadedTextures;
	sf::Texture t;

	//Sprites
	vector<sf::Sprite> loadedSprites;

	//Rounded rectangles
	struct roundRectangle {
		sf::RoundedRectangleShape roundedRectangle;
		sf::IntRect roundButtonArea;
	};
	vector<roundRectangle> roundedRectangles;

	//CircleShapes
	vector<sf::CircleShape> circleShapes;

	bool runMain = true;
}

using namespace mainWin;

void MainWindow::updateWindow(sf::RenderWindow& window, sf::Vector2i mousePos) {
	
	if (runMain)
		initializeMain();

	updateButtons(mousePos);

	for (int i = 1; i < loadedSprites.size(); i++) {
		window.draw(loadedSprites[i]);
	}
	for (int i = 0; i < textLabels.size(); i++) {
		window.draw(textLabels[i]);
	}
	for (int i = 0; i < roundedRectangles.size(); i++) {
		window.draw(roundedRectangles[i].roundedRectangle);
	}
	for (int i = 0; i < circleShapes.size(); i++) {
		window.draw(circleShapes[i]);
	}
	for (int i = 0; i < lightningModes.size(); i++) {
		window.draw(lightningModes[i].sprite);
		if(hoveredMode == i)
			window.draw(loadedSprites[0]);
	}
}
void MainWindow::mouseClicked(sf::Vector2i mousePos, int buttonClicked) {
	if (buttonClicked == 1) {
		for (int i = 0; i < roundedRectangles.size(); i++) {
			if (roundedRectangles[i].roundButtonArea.contains(mousePos)) {
				return;
			}
		}
	}
	cout << "X: " << mousePos.x << " Y: " << mousePos.y << " B: " << buttonClicked << endl;
	return;
}
void MainWindow::initializeMain() {
	
	//Fonts
	newFont((void*)ComfortaaLight, ComfortaaLight_Size, "comfortaa");

	//Text labels
	newText(sf::Vector2f(50, 100), "Static", "comfortaa", 21);
	textLabels[0].setOrigin((int)(textLabels[0].getGlobalBounds().width / 2), 0);

	//Textures
	newTexture((void*)StaticIcon, StaticIcon_Size, "staticicon");
	newTexture((void*)RainbowIcon, RainbowIcon_Size, "rainbowicon");
	newTexture((void*)FadeIcon, FadeIcon_Size, "fadeicon");
	newTexture((void*)CycleIcon, CycleIcon_Size, "cycleicon");
	newTexture((void*)IconFrame, IconFrame_Size, "iconframe");

	//Sprites
	newSprite(sf::Vector2f(10, 10), "iconframe");
	
	//Lightning modes
	addMode("staticicon", "Static");
	addMode("rainbowicon", "Rainbow");
	addMode("fadeicon", "Fade");
	addMode("cycleicon", "Cycle");

	runMain = false;
}

void MainWindow::newFont(const void* data, int sizeInBytes, string label) {

	sf::Font font;
	if (!font.loadFromMemory(data, sizeInBytes)) {
		cout << "Failed to load font" << endl;
		return;
	}

	textFonts[label] = font;
}
void MainWindow::newText(sf::Vector2f position, string text, string font, int size, sf::Color color) {

	sf::Text label;
	label.setPosition(position);
	label.setString(text);
	label.setCharacterSize(size);
	label.setFillColor(color);
	label.setFont(textFonts["comfortaa"]);

	textLabels.push_back(label);
}
void MainWindow::newTexture(const void* data, int sizeInBytes, string label) {

	sf::Texture texture;
	if (!texture.loadFromMemory(data, sizeInBytes)) {
		cout << "Failed to load texture" << endl;
		return;
	}
	texture.setSmooth(true);

	loadedTextures[label] = texture;
}
void MainWindow::newSprite(sf::Vector2f position, string texture, sf::Vector2f scale) {

	sf::Sprite sprite;
	sprite.setTexture(loadedTextures[texture]);
	sprite.setScale(scale);
	sprite.setPosition(position);

	loadedSprites.push_back(sprite);
}
void MainWindow::newRoundRectangle(sf::Vector2f position, sf::Vector2f size, int radius, sf::Color color) {
	roundRectangle r;

	r.roundedRectangle = sf::RoundedRectangleShape(size, radius, 10);
	r.roundedRectangle.setOutlineThickness(-3);
	r.roundedRectangle.setOrigin((int)(r.roundedRectangle.getGlobalBounds().width / 2), (int)(r.roundedRectangle.getGlobalBounds().height / 2));
	r.roundedRectangle.setPosition(position);
	r.roundedRectangle.setFillColor(sf::Color::Transparent);
	r.roundedRectangle.setOutlineColor(color);

	int w = r.roundedRectangle.getGlobalBounds().width / 2;
	int h = r.roundedRectangle.getGlobalBounds().height / 2;

	r.roundButtonArea = sf::IntRect(position.x - w, position.y - h, size.x, size.y);

	roundedRectangles.push_back(r);
}
void MainWindow::newCircleShape(sf::Vector2f position, int radius, int corners, sf::Color color, int rotation) {

	sf::CircleShape c(radius, corners);
	c.setFillColor(color);
	c.setOrigin((int)(c.getGlobalBounds().width / 2), (int)(c.getGlobalBounds().height / 2));
	c.rotate(rotation);
	c.setPosition(position);

	circleShapes.push_back(c);
}

void MainWindow::addMode(string texture, string name) {
	
	sf::Sprite s(loadedTextures[texture]);
	s.setPosition(10, 10 + 90 * lightningModes.size());

	LightningModeButton newButton;
	newButton.startPosition = newButton.currentPosition = sf::Vector2f(10, 10 + 90 * lightningModes.size());
	newButton.destinationY = newButton.startPosition.y;
	newButton.name = name;
	newButton.sprite = s;
	lightningModes.push_back(newButton);
}
void MainWindow::updateButtons(sf::Vector2i mousePos) {
	hoveredMode = lightningModes.size();
	for(int i = 0; i<lightningModes.size(); i++)
	{
		lightningModes[i].update();
		sf::FloatRect hoverArea = lightningModes[i].getRect();

		if(i < lightningModes.size()-1)
			hoverArea.height = lightningModes[i+1].getRect().top - hoverArea.top;
		else
			hoverArea.height = 90; //adjust this to change the hover area of the bottommost button

		if(hoverArea.contains(sf::Vector2f(mousePos)))
		{
			lightningModes[i].hovered = true;
			loadedSprites[0].setPosition(lightningModes[i].currentPosition);
			hoveredMode = i;
		}
		else
			lightningModes[i].hovered = false;
	}

	for(int i = 0; i<lightningModes.size(); i++)
	{
		if(i > hoveredMode)
			lightningModes[i].destinationY = lightningModes[i].startPosition.y + 40;
		else
			lightningModes[i].destinationY = lightningModes[i].startPosition.y;
	}

	if(hoveredMode == lightningModes.size())
		textLabels[0].setFillColor({ 0,0,0,0 });
	else
	{
		textLabels[0].setString(lightningModes[hoveredMode].name);
		textLabels[0].setOrigin((int)(textLabels[0].getGlobalBounds().width / 2), 0);
		textLabels[0].setPosition(50, 100 + 90 * hoveredMode);
		if(hoveredMode == 1)
			textLabels[0].setPosition(52, 100 + 90 * hoveredMode);
		textLabels[0].setFillColor({255,255,255, lightningModes[hoveredMode].textOpacity});
	}
}
