#include "MainWindow.h"
#include <SFML/Graphics.hpp>
#include "RoundedRectangleShape.hpp"
#include "pictures.hpp"
#include "fonts.hpp"
#include <iostream>
#include "UILib.h"

using namespace std;

//Initialize the UI library
IL UImain;

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
	textFonts.push_back({ UImain.loadFontFromMemory((void*)data, sizeInBytes), label });
}
void MainWindow::newText(int x, int y, string text, sf::Font font, int size, sf::Color color) {
	textLabels.push_back(UImain.newTextLabel(x, y, text, font, size, color));
}
void MainWindow::newTexture(const void* data, int sizeInBytes, string label) {
	loadedTextures.push_back({ UImain.loadTextureFromMemory((void*)data, sizeInBytes), label });
}
void MainWindow::newSprite(sf::Vector2f position, sf::Texture texture, sf::Vector2f scale) {
	loadedSprites.push_back(UImain.newSprite(position, texture, scale));
}
void MainWindow::newRoundRectangle(sf::Vector2f position, sf::Vector2f size, int radius, sf::Color color) {
	roundRectangle r;
	r.roundedRectangle = UImain.newRoundButton(position, size, radius, color);

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