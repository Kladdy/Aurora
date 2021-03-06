#include "stdafx.h"
#include "UILib.h"
#include <string>
#include "RoundedRectangleShape.hpp" 
#include <stdlib.h>     
#include <time.h>
#include <math.h> 

using namespace std;

bool IL::libSetup(){

	return true;
}
sf::Font IL::loadFontFromMemory(const void* data, int sizeInBytes){
	sf::Font font;
	if (!font.loadFromMemory(data, sizeInBytes)){
		cout << "Failed to load font" << endl;
	}

	return font;
}
sf::Texture IL::loadTextureFromMemory(const void* data, int sizeInBytes){
	sf::Texture texture;
	if (!texture.loadFromMemory(data, sizeInBytes)){
		cout << "Failed to load texture" << endl;
	}
	texture.setSmooth(true);
	return texture;
}
sf::Sprite IL::newSprite(sf::Vector2f position, sf::Texture texture, sf::Vector2f scale) {
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setScale(scale);
	sprite.setPosition(position);

	return sprite;
}
sf::Text IL::newTextLabel(int x, int y, string text, sf::Font font, int size, sf::Color color){
	sf::Text label;
	label.setPosition(sf::Vector2f(x, y));
	label.setString(text);
	label.setCharacterSize(size);
	label.setFillColor(color);
	label.setFont(font);

	return label;
}
sf::RoundedRectangleShape IL::newRoundButton(sf::Vector2f position, sf::Vector2f size, int radius, sf::Color color) {
	sf::RoundedRectangleShape r = sf::RoundedRectangleShape(size, radius, 10);
	r.setOutlineThickness(-3);
	r.setPosition(position);
	r.setFillColor(sf::Color::Transparent);
	r.setOutlineColor(color);

	return r;
}