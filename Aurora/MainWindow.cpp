#include "MainWindow.h"
#include "pictures.hpp"
#include "fonts.hpp"
#include <cmath>

using namespace std;

namespace mainWin {

	//Ligtning modes
	vector<sf::Sprite> lightningModes;
	vector<string> modeNames;
	int slideDistance = 0;
	int hoveredMode = -1;
	int lastHovered = -1;
	bool retract = false;

	//Fonts
	struct font {
		sf::Font textFont;
		string fontLabel;
	};
	vector<font> textFonts;

	//Text labels
	vector<sf::Text> textLabels;

	//Textures
	struct texture {
		sf::Texture loadedTexture;
		string textureLabel;
	};
	vector<texture> loadedTextures;
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

	updateHighlight(mousePos);

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
		window.draw(lightningModes[i]);
		if(hoveredMode != -1)
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

	textFonts.push_back({ font, label });
}
void MainWindow::newText(sf::Vector2f position, string text, string font, int size, sf::Color color) {

	sf::Text label;
	label.setPosition(position);
	label.setString(text);
	label.setCharacterSize(size);
	label.setFillColor(color);
	label.setFont(textFonts[getFont("comfortaa")].textFont);

	textLabels.push_back(label);
}
void MainWindow::newTexture(const void* data, int sizeInBytes, string label) {

	sf::Texture texture;
	if (!texture.loadFromMemory(data, sizeInBytes)) {
		cout << "Failed to load texture" << endl;
		return;
	}
	texture.setSmooth(true);

	loadedTextures.push_back({ texture, label });
}
void MainWindow::newSprite(sf::Vector2f position, string texture, sf::Vector2f scale) {

	sf::Sprite sprite;
	sprite.setTexture(loadedTextures[getTexture(texture)].loadedTexture);
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
	
	sf::Sprite s;
	s.setTexture(loadedTextures[getTexture(texture)].loadedTexture);
	s.setPosition(10, 10 + 90 * lightningModes.size());

	lightningModes.push_back(s);
	modeNames.push_back(name);
}
void MainWindow::updateHighlight(sf::Vector2i mousePos) {

	if (!retract) {
		for (int i = 0; i < lightningModes.size(); i++) {
			int dx = mousePos.x - lightningModes[i].getPosition().x - 40;
			int dy = mousePos.y - lightningModes[i].getPosition().y - 40;
			int distance = sqrt(pow(dx, 2) + pow(dy, 2));
			if (distance <= 41) {
				hoveredMode = i;
				lastHovered = i;
				loadedSprites[0].setPosition(10, lightningModes[i].getPosition().y);
				textLabels[0].setString(modeNames[hoveredMode]);
				textLabels[0].setOrigin((int)(textLabels[0].getGlobalBounds().width / 2), 0);
				textLabels[0].setPosition(50, 100 + 90 * i);
				if (hoveredMode == 1)
					textLabels[0].setPosition(52, 100 + 90 * i);
				break;
			}
			if (i == lightningModes.size() - 1) {
				hoveredMode = -1;
				retract = true;
				break;
			}
		}
		if (!retract) {
			if (lightningModes[hoveredMode + 1].getPosition().y - 90 == lightningModes[hoveredMode + 2].getPosition().y) {
				retract = true;
				return;
			}

			slideDistance += 3;
			if (slideDistance > 40)
				slideDistance = 40;
			for (int i = hoveredMode + 1; i < lightningModes.size(); i++) {
				lightningModes[i].setPosition(10, 10 + 90 * i + slideDistance);
			}
		}
		for (int i = 0; i < lightningModes.size(); i++) {
			if (lightningModes[i].getPosition().y > 10 + 90 * i) {
				lightningModes[i].setPosition(10, 10 + 90 * i + slideDistance);
				if (i <= hoveredMode)
					retract = true;
			}
		}

		textLabels[0].setFillColor(sf::Color(255, 255, 255, (slideDistance / 40.f) * 255.f));
		return;
	} else {
		slideDistance -= 3;
		if (slideDistance < 0) {
			slideDistance = 0;
			retract = false;
		}

		for (int i = lastHovered + 1; i < lightningModes.size(); i++) {
			lightningModes[i].setPosition(10, 10 + 90 * i + slideDistance);
		}

		textLabels[0].setFillColor(sf::Color(255, 255, 255, (slideDistance / 40.f) * 255.f));
		return;
	}

}

int MainWindow::getFont(string font) {

	for (int i = 0; i < textFonts.size(); i++) {
		if (textFonts[i].fontLabel == font) {
			return i;
		}
		else if (i == (textFonts.size() - 1)) {
			cout << "A font with the label " + font + " has not been initialized" << endl;
			return -1;
		}
	}
}
int MainWindow::getTexture(string texture) {

	for (int i = 0; i < loadedTextures.size(); i++) {
		if (loadedTextures[i].textureLabel == texture) {
			return i;
		}
		else if (i == (loadedTextures.size() - 1)) {
			cout << "A texture with the label " + texture + " has not been initialized" << endl;
			return -1;
		}
	}
}

