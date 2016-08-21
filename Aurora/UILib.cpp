#include "stdafx.h"
#include "UILib.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "RoundedRectangleShape.hpp"
#include <stdlib.h>     
#include <time.h>       

using namespace std;

bool updateSetup();

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

std::vector<sf::Sprite> loadedSprites;
int amountSprites = 0;

std::vector<sf::Font> textFont;
std::vector<string> fontLabel;
int amountTextFont = 0;

std::vector<sf::Texture> loadedTextures;
std::vector<string> textureLabel;
sf::Texture t;
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

std::vector<sf::Color> goodColors;
std::vector<sf::Color> evilColors;
int lastColor = -1;

int amountSupportedStrips;
int selectedModel;

bool IL::libSetup(int amountStrips, string path){
	
	amountSupportedStrips = amountStrips;

	if (path != "c") {
		fontPath = path;
		cout << "Path to font files defined as " + fontPath << endl;
	}

	goodColors.push_back(sf::Color(11, 176, 63, 180)); //Green
	goodColors.push_back(sf::Color(19, 161, 237, 180)); //Blue
	goodColors.push_back(sf::Color(202, 50, 240, 180)); //Purple
	goodColors.push_back(sf::Color(219, 192, 18, 180)); //Lime

	evilColors.push_back(sf::Color(176, 11, 124, 180)); //Purple
	evilColors.push_back(sf::Color(224, 90, 18, 180)); //Orange
	evilColors.push_back(sf::Color(69, 184, 40, 180)); //Dark Green
	evilColors.push_back(sf::Color(48, 40, 199, 180)); //Dark Blue

	return true;
}
bool IL::render(sf::RenderWindow& window){
	amountCheckboxText = 0;
	for (int i = 0; i < amountSprites; i++) {
		if (i == 0) {
			if (setupProgress == 0)
				window.draw(loadedSprites[i]);
		} else {
			window.draw(loadedSprites[i]);
		}
	}
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
		if (i > 1 && i < amountSupportedStrips + 2) {
			if (setupProgress == 1)
				window.draw(roundedRectangle[i]);
		} else {
			window.draw(roundedRectangle[i]);
			if (roundedRectangleDrawable[i] != NULL) {
				window.draw(*roundedRectangleDrawable[i]);
			}
		}
		
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
					if (setupProgress < 20){
						doFade[0] = true;
						fadeDir[0] = true;
						increaseProgress = true;
					}
				}
				else if (i > 1 && i < amountSupportedStrips + 2) {
					selectedModel = i;
					roundedRectangle[i].setOutlineColor(sf::Color(19, 161, 237, 220));
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
	
	texture.setSmooth(true);
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
int getTexture(string texture) {
	for (int i = 0; i < textureLabel.size(); i++) {
		if (textureLabel[i] == texture) {
			return i;
		}
		else if (i == (textureLabel.size() - 1)) {
			cout << "A texture with the label " + texture + " has not been initialized" << endl;
			return 0;
		}
	}
	return 0;
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
		updateSetup();
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
bool IL::newSprite(sf::Vector2f position, string textureLabel, sf::Vector2f scale) {
	sf::Sprite sprite;
	sprite.setTexture(loadedTextures[getTexture(textureLabel)]);
	sprite.setOrigin(sf::Vector2f(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2));
	sprite.setScale(scale);
	sprite.setPosition(position);

	loadedSprites.push_back(sprite);
	amountSprites++;
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

bool IL::updateSetup() {
	srand(time(NULL));

	int c;
	do { c = rand() % goodColors.size();
	} while (c == lastColor);
	
	lastColor = c;

	if (setupProgress != 0) {
		roundedRectangle[0].setOutlineColor(evilColors[c]);
		if (roundedRectangleDrawable[0] != NULL) {((sf::CircleShape*)roundedRectangleDrawable[0])->setFillColor(evilColors[c]);}
	} else if (setupProgress == 0) {
		sf::Color color = sf::Color(60, 60, 60, 200);
		roundedRectangle[0].setOutlineColor(color);
		if (roundedRectangleDrawable[0] != NULL) {((sf::CircleShape*)roundedRectangleDrawable[0])->setFillColor(color);}}

	roundedRectangle[1].setOutlineColor(goodColors[c]);
	if (roundedRectangleDrawable[1] != NULL) {((sf::CircleShape*)roundedRectangleDrawable[1])->setFillColor(goodColors[c]);}
	
	switch (setupProgress)
	{
	case 0: 
		textLabel[0].setString("Aurora - Setup: Introduction");
		textLabel[1].setString("Greetings! I will guide you through how to properly set up Aurora. If this is your first time using the program, \nplease read the contents of this walkthrough carefully."\
			" You can re-run the setup-process by *insert feature \nto repeat setup here*. Use the arrows below in order to maneuver your way through the different options."\
			"\n\nIf you are unsure about which settings to opt for, or you believe that something is missing or not working as \nintended, don't hesitate to contact me. *Insert methods of contacting me here*");
		break;

	case 1:
		textLabel[0].setString("Aurora - Setup: RGB Strip Model");
		textLabel[1].setString("To use Aurora, you need to know what kind of RGB strip that you are using. There are plenty of different \nmodels out there, and the currently supported ones are displayed below."\
			" If you have any RGB strip that is \nnot yet supported, throw me a message and I will add it for you. Selecting the proper RGB strip model is \nessential for making sure Aurora can do its magic with it.");
		break;

	case 2:
		textLabel[0].setString("Aurora - Setup: USB COM-port");
		textLabel[1].setString("");
		break;

	case 3:
		textLabel[0].setString("Aurora - Setup: Baud-rate & LED amount");
		textLabel[1].setString("");
		break;

	case 4:
		textLabel[0].setString("Aurora - Setup: Arduino configuration");
		textLabel[1].setString("");
		break;

	case 5:
		textLabel[0].setString("Aurora - Setup: Miscellaneous");
		textLabel[1].setString("");
		break;


	default:
		textLabel[0].setString("");
		textLabel[1].setString("");
		break;
	}
	

	return true;
}
bool IL::highlightRoundbox(sf::Vector2i mousePos) {
	if (setupProgress == 1) {
		for (int r = 2; r < amountSupportedStrips + 2; r++) {
			if (r != selectedModel) 
				roundedRectangle[r].setOutlineColor(sf::Color(120, 120, 120, 180));
		}
		for (int i = 2; i < amountSupportedStrips + 2; i++) {			
			if (mousePos.x >= roundButtonArea[i].x1 && mousePos.x <= roundButtonArea[i].x2 && mousePos.y >= roundButtonArea[i].y1 && mousePos.y <= roundButtonArea[i].y2) {		
				if (i != selectedModel)
					roundedRectangle[i].setOutlineColor(sf::Color(242, 242, 242, 210));
			}
		}
	}
	


	return true;
}