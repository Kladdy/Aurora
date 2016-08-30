#pragma once
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "RoundedRectangleShape.hpp"

using namespace std;

class SetupWindow {
public:
	void updateWindow(sf::RenderWindow& window, sf::Vector2i mousePos);
	void mouseClicked(sf::Vector2i mousePos, int buttonClicked);
	

private:
	void initializeSetup();
	void newFont(const void* data, int sizeInBytes, string label);
	void newText(int x, int y, string text, string font, int size, sf::Color color = sf::Color::White);
	void newTexture(const void* data, int sizeInBytes, string label);
	void newSprite(sf::Vector2f position, string textureLabel, sf::Vector2f scale = sf::Vector2f(1.f, 1.f));
	void newRoundRectangle(sf::Vector2f position, sf::Vector2f size, int radius = 10, sf::Color color = sf::Color::White);
	void newCircleShape(sf::Vector2f position, int radius, int corners, sf::Color color = sf::Color::White, int rotation = 0);

	void updateSetup();
	void updateFade();
	void highlightRoundbox(sf::Vector2i mousePos);
	void getActiveCOM();

	void transitionFade(sf::Vector2f size, int duration);
	void addStrip(int voltage, string pins, string textureLabel, bool addressable);

	int getFont(string font);
	int getTexture(string texture);
};