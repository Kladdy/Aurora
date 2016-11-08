#pragma once
#include "stdafx.h"
#include "RoundedRectangleShape.hpp"

using namespace std;

class MainWindow {
public:
	void updateWindow(sf::RenderWindow& window, sf::Vector2i mousePos);
	void mouseClicked(sf::Vector2i mousePos, int buttonClicked);
	void mouseScrolled(sf::Vector2i mousePos, int scrollDelta);

private:
	struct Button;

	void initializeMain();
	void newFont(const void* data, int sizeInBytes, string label);
	void newText(sf::Vector2f position, string text, string font, int size, sf::Color color = sf::Color::White);
	void newTexture(const void* data, int sizeInBytes, string label);
	void newSprite(sf::Vector2f position, string textureLabel, sf::Vector2f scale = sf::Vector2f(1.f, 1.f));
	void newRoundRectangle(sf::Vector2f position, sf::Vector2f size, int radius = 10, sf::Color color = sf::Color::White, int thickness = -3);
	void newCircleShape(sf::Vector2f position, int radius, int corners, sf::Color color = sf::Color::White, int rotation = 0);
	void newRectangle(sf::Vector2f position, sf::Vector2f size, sf::Color color = sf::Color::White, int thickness = 0, sf::Color outlineColor = sf::Color::White, int rotation = 0);

	void addMode(string texture, string name);
	void updateButtons(sf::Vector2i mousePos);
	void updateRenderTextures(int lM);
	void updateRotations(sf::Vector3i rt);
};