#include "MainWindow.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "pictures.hpp"
#include "fonts.hpp"

using namespace std;

namespace mainWin {
	//A clickable rectangle struct
	struct clickableArea {
		sf::Vector2i topLeft, bottomRight;
	};

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
		clickableArea roundButtonArea;
	};
	vector<roundRectangle> roundedRectangles;

	//CircleShapes
	vector<sf::CircleShape> circleShapes;
}

using namespace mainWin;

void MainWindow::updateWindow(sf::RenderWindow& window, sf::Vector2i mousePos) {
	for (int i = 5; i < loadedSprites.size(); i++) {
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
}
void MainWindow::mouseClicked(sf::Vector2i mousePos, int buttonClicked) {
	if (buttonClicked == 1) {
		for (int i = 0; i < roundedRectangles.size(); i++) {
			if (mousePos.x >= roundedRectangles[i].roundButtonArea.topLeft.x && mousePos.x <= roundedRectangles[i].roundButtonArea.bottomRight.x && mousePos.y >= roundedRectangles[i].roundButtonArea.topLeft.y && mousePos.y <= roundedRectangles[i].roundButtonArea.bottomRight.y) {
				return;
			}
		}
	}
	cout << "X: " << mousePos.x << " Y: " << mousePos.y << " B: " << buttonClicked << endl;
	return;
}

void MainWindow::newFont(const void* data, int sizeInBytes, string label) {

	sf::Font font;
	if (!font.loadFromMemory(data, sizeInBytes)) {
		cout << "Failed to load font" << endl;
		return;
	}
	else {
		cout << "successfully made font" << endl;
	}

	textFonts.push_back({ font, label });
}
void MainWindow::newText(int x, int y, string text, sf::Font font, int size, sf::Color color) {

	sf::Text label;
	label.setPosition(sf::Vector2f(x, y));
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
void MainWindow::newSprite(sf::Vector2f position, sf::Texture texture, sf::Vector2f scale) {

	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setScale(scale);
	sprite.setPosition(position);

	loadedSprites.push_back(sprite);
}
void MainWindow::newRoundRectangle(sf::Vector2f position, sf::Vector2f size, int radius, sf::Color color) {

	roundRectangle r;

	r.roundedRectangle = sf::RoundedRectangleShape(size, radius, 10);
	r.roundedRectangle.setOutlineThickness(-3);
	r.roundedRectangle.setPosition(position);
	r.roundedRectangle.setFillColor(sf::Color::Transparent);
	r.roundedRectangle.setOutlineColor(color);

	r.roundButtonArea = {
		sf::Vector2i(position.x, position.y), sf::Vector2i(position.x + size.x, position.y + size.y)
	};

	roundedRectangles.push_back(r);
}
void MainWindow::newCircleShape(sf::Vector2f position, int radius, int corners, sf::Color color, int rotation) {

	sf::CircleShape c(radius, corners);
	c.setFillColor(color);
	c.setOrigin(c.getGlobalBounds().width / 2, c.getGlobalBounds().height / 2);
	c.rotate(rotation);
	c.setPosition(position);

	circleShapes.push_back(c);
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

