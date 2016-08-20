#include "stdafx.h"
#include "UILib.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "RoundedRectangleShape.hpp"

using namespace std;

sf::Clock fadeClock;

struct clickableCheckBox{
	int x1, y1, x2, y2;
};
struct clickableDropDown{
	int x1, y1, x2, y2, openHeight; bool open;
};
struct clickableRoundButton{
	int x1, y1, x2, y2;
};

std::vector<sf::Font> textFont;
std::vector<string> fontLabel;
int amountTextFont = 0;

std::vector<sf::Texture> loadedTextures;
std::vector<string> textureLabel;
int amountTextures;

std::vector<sf::RectangleShape> fadeRectangle;
std::vector<int> fadeWaitDuration;
std::vector<int> fade;
std::vector<bool> doFade;
std::vector<bool> fadeDir;
bool fadeWait(int i) { return (fadeClock.getElapsedTime().asMilliseconds() > fadeWaitDuration[i]); }
int amountFade = 0;

std::vector<sf::Text> textLabel;
int amountTextLabel = 0;

std::vector<sf::RectangleShape> checkboxFrame;
std::vector<sf::RectangleShape> checkboxFill;
std::vector<clickableCheckBox> checkboxArea;
std::vector<bool> checkedBox;
std::vector<bool> textedBox;
std::vector<sf::Text> checkboxText;
int amountCheckbox = 0;
int amountCheckboxText = 0;
int fillDistance = 3;

std::vector<sf::RectangleShape> dropDownBox;
std::vector<std::vector<sf::Text>> dropDownTexts;
std::vector<sf::CircleShape> downTria;
std::vector<clickableDropDown> dropDownArea;
int amountDropDown;
int borderDistance = 4;

std::vector<sf::RoundedRectangleShape> roundedRectangle;
std::vector<sf::Drawable*> roundedRectangleDrawable;
std::vector<clickableRoundButton> roundButtonArea;
int amountRoundedRectangle;

string fontPath = "";

int setupProgress = 0;
bool increaseProgress = false;

/*bool animateDrop(int dropDownNumber){

int dropRate = (dropDownArea[dropDownNumber].openHeight - (dropDownArea[dropDownNumber].y2 - dropDownArea[dropDownNumber].y1)) / 20;

if (dropDownArea[dropDownNumber].animate && dropDownBox[dropDownNumber].getGlobalBounds().height < dropDownArea[dropDownNumber].openHeight && dropDownArea[dropDownNumber].open){
dropDownBox[dropDownNumber].setSize(sf::Vector2f(dropDownBox[dropDownNumber].getGlobalBounds().width, dropDownBox[dropDownNumber].getGlobalBounds().height + dropRate));
downTria[dropDownNumber].rotate(-4.5);
}
else if (dropDownBox[dropDownNumber].getGlobalBounds().height >= dropDownArea[dropDownNumber].openHeight && dropDownArea[dropDownNumber].open){
dropDownBox[dropDownNumber].setSize(sf::Vector2f(dropDownBox[dropDownNumber].getGlobalBounds().width, dropDownArea[dropDownNumber].openHeight));
}
if (downTria[dropDownNumber].getRotation() < 180 && dropDownArea[dropDownNumber].open){
downTria[dropDownNumber].setRotation(180);
}

return true;
}*/
bool IL::libSetup(string path){
	fontPath = path;
	cout << "Path to font files defined as " + fontPath << endl;

	return true;
}
bool IL::render(sf::RenderWindow& window){
	amountCheckboxText = 0;
	for (int i = 0; i < amountTextLabel; i++){
		window.draw(textLabel[i]);
	}
	for (int i = 0; i < amountCheckbox; i++){
		window.draw(checkboxFrame[i]);
		if (checkedBox[i])
			window.draw(checkboxFill[i]);
		if (textedBox[i]){
			window.draw(checkboxText[amountCheckboxText]);
			amountCheckboxText++;
		}
	}
	for (int i = 0; i < amountDropDown; i++){
		window.draw(dropDownBox[i]);
		window.draw(downTria[i]);
		window.draw(dropDownTexts[i][0]);
		if (dropDownArea[i].open){
			for (int t = 1; t < dropDownTexts[i].size(); t++){
				window.draw(dropDownTexts[i][t]);
			}
		}
	}
	for (int i = 0; i < amountRoundedRectangle; i++){
		window.draw(roundedRectangle[i]);
		window.draw(*roundedRectangleDrawable[i]);
	}



	for (int i = 0; i < amountFade; i++){
		if (doFade[i])
			updateFade(i);
		
		if (fade[i] != 0){
			window.draw(fadeRectangle[i]);
		}
	}
	
	return true;
}
bool IL::mouseClicked(sf::Vector2i mousePos, int buttonClicked){
	if (buttonClicked == 1){
		for (int i = 0; i < checkboxFrame.size(); i++){
			if (mousePos.x >= checkboxArea[i].x1 && mousePos.x <= checkboxArea[i].x2 && mousePos.y >= checkboxArea[i].y1 && mousePos.y <= checkboxArea[i].y2){
				checkedBox[i] = !checkedBox[i];
				return true;
			}
		}
		for (int i = 0; i < dropDownBox.size(); i++){
			if (!dropDownArea[i].open && mousePos.x >= dropDownArea[i].x1 && mousePos.x <= dropDownArea[i].x2 && mousePos.y >= dropDownArea[i].y1 && mousePos.y <= dropDownArea[i].y2){
				for (int i = 0; i < dropDownBox.size(); i++){
					dropDownArea[i].open = false;
					dropDownBox[i].setSize(sf::Vector2f(dropDownBox[i].getGlobalBounds().width, dropDownArea[i].y2 - dropDownArea[i].y1));
					downTria[i].setRotation(270);
				}
				dropDownBox[i].setSize(sf::Vector2f(dropDownBox[i].getGlobalBounds().width, dropDownArea[i].openHeight));
				downTria[i].setRotation(180);
				dropDownArea[i].open = true;
				return true;
			}
			if (dropDownArea[i].open && mousePos.x >= dropDownArea[i].x1 && mousePos.x <= dropDownArea[i].x2 && mousePos.y >= dropDownArea[i].y1 && mousePos.y <= dropDownArea[i].openHeight + dropDownArea[i].y1){
				dropDownBox[i].setSize(sf::Vector2f(dropDownBox[i].getGlobalBounds().width, dropDownArea[i].y2 - dropDownArea[i].y1));
				downTria[i].setRotation(270);
				dropDownArea[i].open = false;

				int clickedElement = (mousePos.y - dropDownArea[i].y1) * dropDownTexts[i].size() / dropDownArea[i].openHeight;

				dropDownTexts[i][0].setString(dropDownTexts[i][clickedElement].getString());

				return true;
			}
			else if (dropDownArea[i].open && !(mousePos.x >= dropDownArea[i].x1 && mousePos.x <= dropDownArea[i].x2 && mousePos.y >= dropDownArea[i].y1 && mousePos.y <= dropDownArea[i].openHeight + dropDownArea[i].y1)){
				dropDownBox[i].setSize(sf::Vector2f(dropDownBox[i].getGlobalBounds().width, dropDownArea[i].y2 - dropDownArea[i].y1));
				dropDownArea[i].open = false;
				downTria[i].setRotation(270);
			}
		}
		for (int i = 0; i < amountRoundedRectangle; i++){
			if (mousePos.x >= roundButtonArea[i].x1 && mousePos.x <= roundButtonArea[i].x2 && mousePos.y >= roundButtonArea[i].y1 && mousePos.y <= roundButtonArea[i].y2){
				if (i == 0){
					if (setupProgress != 0){
						doFade[0] = true;
						fadeDir[0] = true;
						increaseProgress = false;
					}
				} else if (i == 1){
					if (setupProgress < 5){
						doFade[0] = true;
						fadeDir[0] = true;
						increaseProgress = true;
					}
				}
				return true;
			}
		}
	}
	cout << "X: " << mousePos.x << " Y: " << mousePos.y << " B: " << buttonClicked << endl;
	return false;
}
bool IL::loadFont(string fontName, string label){
	sf::Font font;

	if (!font.loadFromFile(fontPath + fontName)){
		cout << "Failed to load font " + fontName << endl;
		return false;
	}
	else{ cout << "Successfully loaded font " + fontName + " with the label " + label << endl; }

	textFont.push_back(font);
	fontLabel.push_back(label);
	amountTextFont++;
	return true;
}
bool IL::loadFontFromMemory(const void* data, int sizeInBytes, string label){
	sf::Font font;
	if (!font.loadFromMemory(data, sizeInBytes)){
		cout << "Failed to load font " + label << endl;
		return false;
	}
	else{ cout << "Successfully loaded font with the label " + label << endl; }

	textFont.push_back(font);
	fontLabel.push_back(label);
	amountTextFont++;
	return true;
}
bool IL::loadTextureFromMemory(const void* data, int sizeInBytes, string label){
	sf::Texture texture;
	if (!texture.loadFromMemory(data, sizeInBytes)){
		cout << "Failed to load texture " + label << endl;
		return false;
	}
	else{ cout << "Successfully loaded texture with the label " + label << endl; }
	
	loadedTextures.push_back(texture);
	textureLabel.push_back(label);
	amountTextures++;
	return true;
}
bool setFont(sf::Text& text, string font){
	for (int i = 0; i < fontLabel.size(); i++){
		if (fontLabel[i] == font){
			text.setFont(textFont[i]);
			break;
		}
		else if (i == (fontLabel.size() - 1)){
			cout << "A font with the label " + font + " has not been initialized" << endl;
			return false;
		}
	}
	return true;
}
bool IL::newFade(sf::Vector2f size, int speed){
	sf::RectangleShape rect = sf::RectangleShape(size);
	rect.setFillColor(sf::Color(0, 0, 0, 0));
	fadeRectangle.push_back(rect);
	fade.push_back(0);
	doFade.push_back(false);
	fadeDir.push_back(true);
	fadeWaitDuration.push_back(speed);
	amountFade++;

	cout << "Setup progress: " << setupProgress << endl;

	return true;
}
bool IL::updateFade(int ID){
	if (doFade[ID] && fadeDir[ID] && fade[ID] < 255 && fadeWait(ID)){
		fade[ID]++;
		fadeClock.restart();
	}
	else if (doFade[ID] && !fadeDir[ID] && fade[ID] > 0 && fadeWait(ID)){
		fade[ID]--;
		fadeClock.restart();
	}
	else if (doFade[ID] && fadeDir[ID] && fade[ID] == 255 && fadeWait(ID)){
		if (increaseProgress)
			setupProgress++;
		else
			setupProgress--;
		cout << "Setup progress: " << setupProgress << endl;

		fadeDir[ID] = false;
		fadeClock.restart();
	}
	else if (doFade[ID] && !fadeDir[ID] && fade[ID] == 0 && fadeWait(ID)){
		doFade[ID] = false;
		fadeDir[ID] = true;
	}

	fadeRectangle[ID].setFillColor(sf::Color(0, 0, 0, fade[ID]));

	return true;
}
bool IL::newTextLabel(int x, int y, string text, string font, int size, sf::Color color){
	sf::Text label;
	label.setPosition(sf::Vector2f(x, y));
	label.setString(text);
	label.setCharacterSize(size);
	label.setFillColor(color);

	setFont(label, font);

	textLabel.push_back(label);
	amountTextLabel++;
	return true;
}
bool IL::newCheckbox(int x, int y, int vertexLenght, sf::Color color){
	sf::RectangleShape boxFrame;
	boxFrame.setPosition(sf::Vector2f(x, y));
	boxFrame.setSize(sf::Vector2f(vertexLenght, vertexLenght));
	boxFrame.setFillColor(sf::Color::Transparent);
	boxFrame.setOutlineThickness(-1);
	boxFrame.setOutlineColor(color);

	sf::RectangleShape boxFill;
	boxFill.setPosition(sf::Vector2f(x + fillDistance, y + fillDistance));
	boxFill.setSize(sf::Vector2f(vertexLenght - (2 * fillDistance), vertexLenght - (2 * fillDistance)));
	boxFill.setFillColor(color);

	clickableCheckBox clickArea = {
		x, y, x + vertexLenght, y + vertexLenght
	};

	checkboxFrame.push_back(boxFrame);
	checkboxFill.push_back(boxFill);
	checkboxArea.push_back(clickArea);
	textedBox.push_back(false);
	checkedBox.push_back(false);
	amountCheckbox++;
	return true;
}
bool IL::newCheckbox(int x, int y, int vertexLenght, sf::Color color, string text, string font, int size, sf::Color colorText, int location){
	newCheckbox(x, y, vertexLenght, color);
	amountCheckbox--;
	textedBox[amountCheckbox] = true;

	sf::Text boxText;
	boxText.setString(text);
	boxText.setCharacterSize(size);
	boxText.setFillColor(color);

	setFont(boxText, font);

	switch (location){
	case 0:
		boxText.setPosition(sf::Vector2f(x, y - (size - boxText.getGlobalBounds().height)));
		x = x + 7 + boxText.getGlobalBounds().width;
		y = y + (boxText.getGlobalBounds().height / 2) - (vertexLenght / 2);
		checkboxFrame[amountCheckbox].setPosition(sf::Vector2f(x, y));
		checkboxFill[amountCheckbox].setPosition(sf::Vector2f(x + fillDistance, y + fillDistance));
		checkboxArea[amountCheckbox] = { x, y, x + vertexLenght, y + vertexLenght };
		break;
	case 1:
		boxText.setPosition(sf::Vector2f(x + vertexLenght + 7, y - (size - boxText.getGlobalBounds().height)));
		y = y + (boxText.getGlobalBounds().height / 2) - (vertexLenght / 2);
		checkboxFrame[amountCheckbox].setPosition(sf::Vector2f(x, y));
		checkboxFill[amountCheckbox].setPosition(sf::Vector2f(x + fillDistance, y + fillDistance));
		checkboxArea[amountCheckbox] = { x, y, x + vertexLenght, y + vertexLenght };
		break;
	};

	checkboxText.push_back(boxText);
	amountCheckboxText++;
	amountCheckbox++;
	return true;
}
bool IL::newDropDown(int x, int y, string defaultText, std::vector<string> elements, string font, int size, sf::Color colorText, sf::Color colorFill, sf::Color colorBorder){
	std::vector<sf::Text> textVector;
	dropDownTexts.push_back(textVector);

	sf::Text defText;
	defText.setString(defaultText);
	defText.setCharacterSize(size);
	defText.setFillColor(colorText);
	setFont(defText, font);

	dropDownTexts[amountDropDown].push_back(defText);

	for (int i = 0; i < elements.size(); i++){
		sf::Text dropText;
		dropText.setString(elements[i]);
		dropText.setCharacterSize(size);
		dropText.setFillColor(colorText);
		setFont(dropText, font);

		dropDownTexts[amountDropDown].push_back(dropText);
	}

	sf::Vector2i textWidth;
	for (int i = 0; i < dropDownTexts[amountDropDown].size(); i++){
		if (dropDownTexts[amountDropDown][i].getGlobalBounds().width > textWidth.x){
			textWidth.x = dropDownTexts[amountDropDown][i].getGlobalBounds().width;
			textWidth.y = i;
		}
	}

	sf::RectangleShape dropDown;

	dropDown.setPosition(sf::Vector2f(x, y));
	dropDown.setFillColor(colorFill);
	dropDown.setOutlineThickness(-1);
	dropDown.setOutlineColor(colorBorder);
	dropDown.setSize(sf::Vector2f(dropDownTexts[amountDropDown][textWidth.y].getGlobalBounds().width + 4 * borderDistance + dropDownTexts[amountDropDown][textWidth.y].getGlobalBounds().height, dropDownTexts[amountDropDown][textWidth.y].getGlobalBounds().height + 2 * borderDistance));

	sf::CircleShape triangle(dropDownTexts[amountDropDown][textWidth.y].getGlobalBounds().height / 4 + borderDistance, 3);
	triangle.setFillColor(colorBorder);
	triangle.setOrigin(triangle.getGlobalBounds().width / 2, triangle.getGlobalBounds().height / 2);
	triangle.setPosition(sf::Vector2f(x + dropDown.getGlobalBounds().width - borderDistance - triangle.getGlobalBounds().width / 2, y + dropDown.getGlobalBounds().height / 2));
	//triangle.setPosition(sf::Vector2f(x + dropDownTexts[amountDropDown][textWidth.y].getGlobalBounds().width + 2 * borderDistance, y + dropDown.getGlobalBounds().height / 2));
	triangle.setRotation(270);

	clickableDropDown clickArea = {
		x, y, x + dropDown.getGlobalBounds().width, y + dropDown.getGlobalBounds().height, dropDownTexts[amountDropDown].size() * size + borderDistance, false
	};

	float top = dropDownTexts[amountDropDown][0].getGlobalBounds().top;
	dropDownTexts[amountDropDown][0].setPosition(sf::Vector2f(x + borderDistance, y + dropDown.getGlobalBounds().height / 2 - dropDownTexts[amountDropDown][0].getGlobalBounds().height / 2 - top));


	for (int i = 1; i < dropDownTexts[amountDropDown].size(); i++){
		dropDownTexts[amountDropDown][i].setPosition(sf::Vector2f(x + borderDistance, y + dropDown.getGlobalBounds().height / 2 - dropDownTexts[amountDropDown][0].getGlobalBounds().height / 2 - top + i * size));
	}


	dropDownBox.push_back(dropDown);
	downTria.push_back(triangle);
	dropDownArea.push_back(clickArea);
	amountDropDown++;
	return true;
}
bool IL::newRoundButton(sf::Vector2f position, sf::Vector2f size, int radius, sf::Color color, sf::Drawable* d){
	sf::RoundedRectangleShape roundRect;

	roundRect = sf::RoundedRectangleShape(size, radius, 10);
	roundRect.setOutlineThickness(-3);
	roundRect.setPosition(position);
	roundRect.setFillColor(sf::Color::Transparent);
	roundRect.setOutlineColor(color);

	clickableRoundButton clickArea = {
		position.x, position.y, position.x + size.x, position.y + size.y
	};

	roundedRectangleDrawable.push_back(d); 
	roundedRectangle.push_back(roundRect);
	roundButtonArea.push_back(clickArea);
	amountRoundedRectangle++;
	return true;
}