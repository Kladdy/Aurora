#include "stdafx.h"
#include "UILib.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "RoundedRectangleShape.hpp" 
#include <stdlib.h>     
#include <time.h>
#include "enumser.h"
#include <math.h> 

using namespace std;

bool updateSetup();
bool updateFade(int ID);
void getActiveCOM();
bool highlightRoundbox(sf::Vector2i mousePos);

struct clickableCheckBox{
	int x1, y1, x2, y2;
};
struct clickableDropDown{
	int x1, y1, x2, y2, openHeight; bool open;
};
struct clickableRoundButton{
	int x1, y1, x2, y2;
};

struct ledStrip {
	sf::RoundedRectangleShape roundRect;
	sf::Text voltageText, pinsText, addressableText;
	sf::Sprite image; 
};
struct comButton {
	sf::RoundedRectangleShape roundRect;
	string namesCOM, portsCOM;
	sf::Text namesCOMText, portsCOMText;
	bool isArduino;
};

vector<ledStrip> supportedStrips;
vector<clickableRoundButton> supportedStripsArea;
int amountSupportedStrip;
int selectedModel = -1;

vector<comButton> activeCOMPorts;
vector<clickableRoundButton> activeCOMPortsArea;
int amountActiveCOM = 0;
int selectedCOM = -1;

sf::Clock logoFadeClock;
int logoFade = 0;

sf::Clock rotateClock;
bool doRotate = false;
bool rotateDir = true;
bool rotateChangeDir = false;
int changedDirRotation;
int rotation;

int baudRates[12] = { 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200 };
int selectedRate = 5;

int amountLEDs = 1;

vector<sf::Sprite> loadedSprites;
int amountSprite = 0;

vector<sf::Font> textFont;
vector<string> fontLabel;
int amountTextFont = 0;

vector<sf::Texture> loadedTextures;
vector<string> textureLabel;
sf::Texture t;
int amountTexture;

sf::Clock fadeClock;
vector<sf::RectangleShape> fadeRectangle;
vector<int> fadeWaitDuration;
vector<int> fade;
vector<bool> doFade;
vector<bool> fadeDir;
bool invertFade = false;
int currentElapsed = 0;
int amountFade = 0;

vector<sf::Text> textLabel;
int amountTextLabel = 0;

vector<sf::RectangleShape> checkboxFrame;
vector<sf::RectangleShape> checkboxFill;
vector<clickableCheckBox> checkboxArea;
vector<bool> checkedBox;
vector<bool> textedBox;
vector<sf::Text> checkboxText;
int amountCheckbox = 0;
int amountCheckboxText = 0;
int fillDistance = 3;

vector<sf::RectangleShape> dropDownBox;
vector<vector<sf::Text>> dropDownTexts;
vector<sf::CircleShape> downTria;
vector<clickableDropDown> dropDownArea;
int amountDropDown;
int borderDistance = 4;

vector<sf::RoundedRectangleShape> roundedRectangle;
vector<sf::Drawable*> roundedRectangleDrawable;
vector<clickableRoundButton> roundButtonArea;
int amountRoundedRectangle;

string fontPath = "";

int setupProgress = 0;
bool increaseProgress = false;

vector<sf::Color> goodColors;
vector<sf::Color> evilColors;
int lastColor = -1;


bool IL::libSetup(string path){

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
bool IL::render(sf::RenderWindow& window, sf::Vector2i mousePos) {
	amountCheckboxText = 0;

	if (setupProgress == 1 || setupProgress == 2 || setupProgress == 4)
		highlightRoundbox(mousePos);

	for (int i = 5; i < amountSprite; i++) {
		window.draw(loadedSprites[i]);
	}
	for (int i = 0; i < amountTextLabel; i++){
		if (!(i >= 2 && i <= 9))
			window.draw(textLabel[i]);
	}
	for (int i = 0; i < amountRoundedRectangle; i++){
		if (!(i >= 2 && i <= 6)) {
			window.draw(roundedRectangle[i]);
			if (roundedRectangleDrawable[i] != NULL)
				window.draw(*roundedRectangleDrawable[i]);
		}
	}
	if (setupProgress == 0) {
		if (logoFade < 255 && logoFadeClock.getElapsedTime().asMilliseconds() > 30) {
			logoFadeClock.restart();
			logoFade += 3;
			if (logoFade >= 255)
				logoFade = 255;
			sf::Color c = loadedSprites[1].getColor();
			c = sf::Color(c.r, c.g, c.g, logoFade);
			loadedSprites[1].setColor(c);
		}
		window.draw(loadedSprites[0]);
		window.draw(loadedSprites[1]);
	} else if (setupProgress == 1) {
		for (int i = 0; i < amountSupportedStrip; i++) {
			window.draw(supportedStrips[i].roundRect);
			window.draw(supportedStrips[i].voltageText);
			window.draw(supportedStrips[i].pinsText);
			window.draw(supportedStrips[i].addressableText);
			window.draw(supportedStrips[i].image);
		}
	} else if (setupProgress == 2) {
		for (int i = 0; i < amountActiveCOM; i++) {
			if (activeCOMPorts[i].isArduino) {
				loadedSprites[2].setPosition(sf::Vector2f(105 + 199 * i, 200)); 
					window.draw(loadedSprites[2]);
			}
			window.draw(activeCOMPorts[i].roundRect);
			window.draw(activeCOMPorts[i].namesCOMText);
			window.draw(activeCOMPorts[i].portsCOMText);
			window.draw(activeCOMPorts[i].roundRect);
			window.draw(activeCOMPorts[i].roundRect);
		}
		if (doRotate) {
			if (rotateChangeDir && ((changedDirRotation < 0 && rotation >= 0) || (changedDirRotation > 0 && rotation <= 0))) {
					selectedCOM = -1;
					getActiveCOM();
					if (changedDirRotation >= 0)
						rotateDir = true;
					if (changedDirRotation <= 0)
						rotateDir = false;
					rotateClock.restart();
					rotateChangeDir = false;
			}
			if (rotateClock.getElapsedTime().asMilliseconds() > 5027) {
				rotation = 0;
				doRotate = false;
			} else {
				float x = rotateClock.getElapsedTime().asMilliseconds() / 200.f;
				if (rotateDir)
					rotation = (-100 * sin(x)) / (x + 1);
				else
					rotation = (100 * sin(x)) / (x + 1);
			}
			

			roundedRectangle[2].setRotation(rotation);
			textLabel[2].setRotation(rotation);
		}
		window.draw(roundedRectangle[2]);
		window.draw(textLabel[2]);
	} else if (setupProgress == 3) {
		textLabel[4].setFillColor(sf::Color(255, 255, 255, 255));
		loadedSprites[3].setColor(sf::Color(255, 255, 255, 255));

		window.draw(textLabel[3]);
		loadedSprites[3].setRotation(0);
		loadedSprites[3].setPosition(120, 206);
		window.draw(loadedSprites[3]);
		loadedSprites[3].setRotation(180);
		loadedSprites[3].setPosition(193, 206);
		window.draw(loadedSprites[3]);

		if (selectedModel == -1 || supportedStrips[selectedModel].addressableText.getString() != "Addressable") {
			textLabel[4].setFillColor(sf::Color(120, 120, 120, 180));
			loadedSprites[3].setColor(sf::Color(120, 120, 120, 180));
		}

		window.draw(textLabel[4]);
		loadedSprites[3].setRotation(0);
		loadedSprites[3].setPosition(137, 276);
		window.draw(loadedSprites[3]);
		loadedSprites[3].setPosition(155, 276);
		window.draw(loadedSprites[3]);
		loadedSprites[3].setPosition(162, 276);
		window.draw(loadedSprites[3]);

		loadedSprites[3].setRotation(180);
		loadedSprites[3].setPosition(205, 276);
		window.draw(loadedSprites[3]);
		loadedSprites[3].setPosition(212, 276);
		window.draw(loadedSprites[3]);
		loadedSprites[3].setPosition(230, 276);
		window.draw(loadedSprites[3]);
	} else if (setupProgress == 4){
		window.draw(roundedRectangle[3]);
		window.draw(roundedRectangle[4]);
		window.draw(roundedRectangle[5]);
		window.draw(roundedRectangle[6]);
		window.draw(textLabel[5]);
		window.draw(textLabel[6]);
		window.draw(textLabel[7]);
		window.draw(textLabel[8]);
		window.draw(textLabel[9]);
		if (selectedModel != -1)
			window.draw(loadedSprites[4]);
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
					if (setupProgress != 0 && !(doFade[0] && fadeDir[0])){
						if (doFade[0]) {
							currentElapsed = fadeClock.getElapsedTime().asMilliseconds();
							invertFade = true;
						}
						doFade[0] = true;
						fadeDir[0] = true;
						increaseProgress = false;
						fadeClock.restart();
					}
					return true;
				} else if (i == 1){
					if (setupProgress < 20 && !(doFade[0] && fadeDir[0])){
						if (doFade[0]) {
							currentElapsed = fadeClock.getElapsedTime().asMilliseconds();
							invertFade = true;
						}
						doFade[0] = true;
						fadeDir[0] = true;
						increaseProgress = true;
						fadeClock.restart();
					}
					return true;
				} else if (i == 2 && setupProgress == 2) {
					if (doRotate) {
						rotateChangeDir = true; 
						changedDirRotation = rotation;
					} else {
						selectedCOM = -1;
						getActiveCOM();
						doRotate = true;
						rotateClock.restart();
					}
					return true;
				} else if (i == 3 && setupProgress == 4) {
					ShellExecute(0, 0, "http://www.google.com", 0, 0, SW_SHOW);
					return true;
				} else if (i == 5 && setupProgress == 4) {
	
					return true;
				} else if (i == 6 && setupProgress == 4) {

					return true;
				}
			}
		}
		if (setupProgress == 1) {
			for (int i = 0; i < amountSupportedStrip; i++) {
				if (mousePos.x >= supportedStripsArea[i].x1 && mousePos.x <= supportedStripsArea[i].x2 && mousePos.y >= supportedStripsArea[i].y1 && mousePos.y <= supportedStripsArea[i].y2) {
					selectedModel = i;
					supportedStrips[i].roundRect.setOutlineColor(sf::Color(19, 161, 237, 220));
					return true;
				}
			}
		} else if (setupProgress == 2) {
			for (int i = 0; i < amountActiveCOM; i++) {
				if (mousePos.x >= activeCOMPortsArea[i].x1 && mousePos.x <= activeCOMPortsArea[i].x2 && mousePos.y >= activeCOMPortsArea[i].y1 && mousePos.y <= activeCOMPortsArea[i].y2) {
					selectedCOM = i;
					activeCOMPorts[i].roundRect.setOutlineColor(sf::Color(19, 161, 237, 220));
					return true;
				}
			}
		} else if (setupProgress == 3) {
			if (mousePos.x >= 116 && mousePos.x <= 126 && mousePos.y >= 196 && mousePos.y <= 215) {
				selectedRate--;
				if (selectedRate < 0)
					selectedRate = 11;
				textLabel[3].setString("Baud-rate:     " + to_string(baudRates[selectedRate]));
				return true;
			} else if (mousePos.x >= 189 && mousePos.x <= 199 && mousePos.y >= 196 && mousePos.y <= 215) {
				selectedRate++;
				if (selectedRate > 11)
					selectedRate = 0;
				textLabel[3].setString("Baud-rate:     " + to_string(baudRates[selectedRate]));
				return true;
			}
			else if (selectedModel != -1 && supportedStrips[selectedModel].addressableText.getString() == "Addressable") {
				if (mousePos.x >= 133 && mousePos.x <= 143 && mousePos.y >= 266 && mousePos.y <= 285) {
					amountLEDs--;
					if (amountLEDs <= 0)
						amountLEDs = 1;
					textLabel[4].setString("LED amount:          " + to_string(amountLEDs));
					return true;
				}
				else if (mousePos.x >= 151 && mousePos.x <= 168 && mousePos.y >= 266 && mousePos.y <= 285) {
					amountLEDs -= 5;
					if (amountLEDs <= 0)
						amountLEDs = 1;
					textLabel[4].setString("LED amount:          " + to_string(amountLEDs));
					return true;
				}
				else if (mousePos.x >= 201 && mousePos.x <= 218 && mousePos.y >= 266 && mousePos.y <= 285) {
					amountLEDs += 5;
					if (amountLEDs >= 100)
						amountLEDs = 99;
					textLabel[4].setString("LED amount:          " + to_string(amountLEDs));
					return true;
				}
				else if (mousePos.x >= 226 && mousePos.x <= 236 && mousePos.y >= 266 && mousePos.y <= 285) {
					amountLEDs++;
					if (amountLEDs >= 100)
						amountLEDs = 99;
					textLabel[4].setString("LED amount:          " + to_string(amountLEDs));
					return true;
				}
			}
		}
		else if (setupProgress == 4){
			if (mousePos.x >= 639 && mousePos.x <= 639 + textLabel[9].getGlobalBounds().width && mousePos.y >= 247 && mousePos.y <= 249 + textLabel[9].getGlobalBounds().height) {
				ShellExecute(0, 0, "https://www.arduino.cc/en/Main/Software", 0, 0, SW_SHOW);
			return true;
			}
		}
	}
	cout << "X: " << mousePos.x << " Y: " << mousePos.y << " B: " << buttonClicked << endl;
	return false;
}
bool IL::loadFont(string fontName, string label) {
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
	amountTexture++;
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
bool IL::newFade(sf::Vector2f size, int duration){
	sf::RectangleShape rect = sf::RectangleShape(size);
	rect.setFillColor(sf::Color(0, 0, 0, 0));
	fadeRectangle.push_back(rect);
	duration = duration / 2;

	fade.push_back(0);
	doFade.push_back(false);
	fadeDir.push_back(true);
	fadeWaitDuration.push_back(duration);
	amountFade++;
	return true;
}
bool IL::updateFade(int ID) {
	if (fadeDir[ID]) {
		int i;

		if (invertFade) 
			i = (256 * (fadeClock.getElapsedTime().asMilliseconds() + fadeWaitDuration[ID] - currentElapsed) / fadeWaitDuration[ID]);
		else
			i = (256 * fadeClock.getElapsedTime().asMilliseconds() / fadeWaitDuration[ID]);

		if (i < 256)
			fade[ID] = i;
		else {
			if (increaseProgress)
				setupProgress++;
			else
				setupProgress--;
			updateSetup();
			fadeDir[ID] = false;
			fade[ID] = 255;
			fadeClock.restart();
			currentElapsed = 0;
			invertFade = false;
		}	
	} else {
		int i = 255 - (255 * fadeClock.getElapsedTime().asMilliseconds() / fadeWaitDuration[ID]);
		if (i > 0)
			fade[ID] = i;
		else {
			fade[ID] = 0;
			doFade[ID] = false;
			fadeDir[ID] = true;
		}
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
	amountSprite++;
	return true;
}
bool IL::addStrip(int voltage, string pins, string textureLabel, bool addressable) {
	
	ledStrip s;

	s.roundRect = sf::RoundedRectangleShape(sf::Vector2f(140, 196), 10, 10);
	s.roundRect.setOutlineThickness(-3);
	s.roundRect.setPosition(sf::Vector2f(10 + 149 * amountSupportedStrip, 135));
	s.roundRect.setFillColor(sf::Color::Transparent);
	s.roundRect.setOutlineColor(sf::Color(120, 120, 120, 180));

	s.voltageText.setString("Voltage: +" + to_string(voltage) + "V");
	s.voltageText.setCharacterSize(16);
	setFont(s.voltageText, "comfortaa");
	s.voltageText.setOrigin(s.voltageText.getGlobalBounds().width / 2, 0);
	s.voltageText.setPosition(sf::Vector2f(80 + 149 * amountSupportedStrip, 254));

	s.pinsText.setString("Pins: " + pins);
	s.pinsText.setCharacterSize(15);
	setFont(s.pinsText, "comfortaa");
	int posOffSet = 0;
	if (s.pinsText.getGlobalBounds().width > 125) {
		do {
			s.pinsText.setCharacterSize(s.pinsText.getCharacterSize() - 1);
			posOffSet++;
		} while (s.pinsText.getGlobalBounds().width > 125);
	}
	s.pinsText.setOrigin(s.pinsText.getGlobalBounds().width / 2, 0);
	s.pinsText.setPosition(sf::Vector2f(80 + 149 * amountSupportedStrip, 279 + posOffSet));

	s.addressableText.setCharacterSize(16);
	setFont(s.addressableText, "comfortaa");
	if (addressable)
		s.addressableText.setString("Addressable");	
	else {
		s.addressableText.setString("Non-addressable");
		s.addressableText.setCharacterSize(15);
	}
	s.addressableText.setOrigin(s.addressableText.getGlobalBounds().width / 2, 0);
	s.addressableText.setPosition(sf::Vector2f(80 + 149 * amountSupportedStrip - 1, 305 + !addressable));

	s.image.setTexture(loadedTextures[getTexture(textureLabel)]);
	s.image.setOrigin(sf::Vector2f(s.image.getGlobalBounds().width / 2, s.image.getGlobalBounds().height / 2));
	s.image.setPosition(sf::Vector2f(80 + 149 * amountSupportedStrip, 200));

	clickableRoundButton d = {
		(10 + 149 * amountSupportedStrip), 135, 140 + (10 + 149 * amountSupportedStrip), 331
	};

	supportedStripsArea.push_back(d);
	supportedStrips.push_back(s);
	amountSupportedStrip++;

	return true;
}
bool IL::newTextLabel(int x, int y, string text, string font, int size, sf::Color color){
	sf::Text label;
	label.setPosition(sf::Vector2f(x, y));
	label.setString(text);
	label.setCharacterSize(size);
	label.setFillColor(color);

	setFont(label, font);

	if (amountTextLabel == 2){
		label.setOrigin(label.getGlobalBounds().width / 2, 16);
		label.move(label.getGlobalBounds().width / 2, 16);
	}
	textLabel.push_back(label);
	amountTextLabel++;
	return true;
}
bool IL::newRoundButton(sf::Vector2f position, sf::Vector2f size, int radius, sf::Color color, sf::Drawable* d){
	sf::RoundedRectangleShape roundRect;

	roundRect = sf::RoundedRectangleShape(size, radius, 10);
	roundRect.setOutlineThickness(-3);
	roundRect.setPosition(position);
	roundRect.setFillColor(sf::Color::Transparent);
	roundRect.setOutlineColor(color);

	int w = roundRect.getGlobalBounds().width / 2;
	int h = roundRect.getGlobalBounds().height / 2;

	clickableRoundButton clickArea = {
		position.x - w, position.y - h, position.x - w + size.x, position.y - h + size.y
	};

	roundRect.setOrigin(w, h);
	roundedRectangle.push_back(roundRect);
	roundedRectangleDrawable.push_back(d);
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
	
	switch (setupProgress){
	case 0:
		textLabel[0].setString("Aurora - Setup: Introduction");
		textLabel[1].setString("Greetings! I will guide you through how to properly set up Aurora. If this is your first time using the program, \nplease read the contents of this walkthrough carefully."\
			" You can re-run the setup-process by *insert feature \nto repeat setup here*. Use the arrows below in order to maneuver your way through the different options."\
			"\n\nIf you are unsure about which settings to opt for, or you believe that something is missing or not working as \nintended, don't hesitate to contact me. *Insert methods of contacting me here*");
		logoFade = 0;
		break;

	case 1:
		textLabel[0].setString("Aurora - Setup: RGB Strip Model");
		textLabel[1].setString("To use Aurora, you need to know what kind of RGB strip that you are using. There are plenty of different \nmodels out there, and the currently supported ones are displayed below."\
			" If you have any RGB strip that is \nnot yet supported, throw me a message and I will add it for you. Selecting the proper RGB strip model is \nessential for making sure Aurora can do its magic with it.");
		break;

	case 2:
		textLabel[0].setString("Aurora - Setup: USB COM-port");
		textLabel[1].setString("Aurora talks to your Arduino via the USB-Serial interface. In order to be detected, the Arduino needs to be \nconnected with a USB cable to your computer. Aurora will autonomously attempt"\
			" to identify any feasible \ncandidate and, if it finds one, highlight its button below. Bluetooth support is planned for a future release, \nbut until then, selecting the right COM-port below is the way to go.");
		getActiveCOM();
		break;

	case 3:
		textLabel[0].setString("Aurora - Setup: Baud-rate & LED amount");
		textLabel[1].setString("Changing the Baud-rate of Aurora should only be done if you know what you are doing and you have a good \nreason for it. Leaving it at the default value of 9600 is sufficient, and is the only rate"\
			" that will work unless you \nmanually change the Arduino code. Setting the LED amount is only applicable to addressable strips and \nrequires you to have selected a compatible model in a previous settings page. Having a different"\
			" amount of \nLEDs on your strip to the amount set in Aurora will not enable you to take full control over the entire strip.");
		if (selectedModel == -1)
			textLabel[4].setString("LED amount:          N/             You have not yet selected a strip model. This setting is located on the second page.");
		else if (supportedStrips[selectedModel].addressableText.getString() != "Addressable")
			textLabel[4].setString("LED amount:          N/                 Your currently selected strip model is not addressable; this setting is unavailable.");
		else
			textLabel[4].setString("LED amount:          " + to_string(amountLEDs));
		break;

	case 4:
		textLabel[0].setString("Aurora - Setup: Arduino configuration");
		textLabel[1].setString("The most important factor in making Aurora work is having both the wiring and the programming done \ncorrectly. If you already selected a strip model, clicking the button below will bring you to a walkthrough on \nhow to wire up"\
			" that specific model. \n\n\n\n\nFortunately, Aurora can assist you in uploading a functional program to your Arduino. Confirm that the \nsettings below are correct and simply click the 'Push'-button to send the code to the Arduino. If you want to"\
			" \nmanually change something in the code, you can save it as a .ino file on your computer. Please note that you \nneed to have the Arduino IDE installed, which you can download from ");
		if (selectedModel == 0)
			loadedSprites[4].setTexture(loadedTextures[getTexture("stripdin5v")]);
		else if (selectedModel == 1)
			loadedSprites[4].setTexture(loadedTextures[getTexture("striprgb12v")]);
		else if (selectedModel == 2)
			loadedSprites[4].setTexture(loadedTextures[getTexture("stripiltrof")]);

		textLabel[6].setString("");
		if (selectedModel != -1) 
			textLabel[6].setString(textLabel[6].getString() + supportedStrips[selectedModel].voltageText.getString() + "\n" + supportedStrips[selectedModel].addressableText.getString() + "\n");
		else 
			textLabel[6].setString(textLabel[6].getString() + "No strip model" + "\n");
		if (selectedModel != -1 && supportedStrips[selectedModel].addressableText.getString() == "Addressable")
			textLabel[6].setString(textLabel[6].getString() + "LED amount: " + to_string(amountLEDs) + "\n");
		if (selectedCOM != -1)
			textLabel[6].setString(textLabel[6].getString() + activeCOMPorts[selectedCOM].namesCOM + "\n");
		else
			textLabel[6].setString(textLabel[6].getString() + "No COM-port" + "\n");
		if (selectedCOM != -1)
			textLabel[6].setString(textLabel[6].getString() + "Baud-rate: " + to_string(baudRates[selectedRate]) + "\n");

		textLabel[9].setFillColor(sf::Color(35, 171, 176, 255));

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
		for (int i = 0; i < amountSupportedStrip; i++) {
			if (i != selectedModel)
				supportedStrips[i].roundRect.setOutlineColor(sf::Color(120, 120, 120, 180));
				if (mousePos.x >= supportedStripsArea[i].x1 && mousePos.x <= supportedStripsArea[i].x2 && mousePos.y >= supportedStripsArea[i].y1 && mousePos.y <= supportedStripsArea[i].y2)
					supportedStrips[i].roundRect.setOutlineColor(sf::Color(242, 242, 242, 210));
		}
		if (selectedModel >= 0)
			supportedStrips[selectedModel].roundRect.setOutlineColor(sf::Color(19, 161, 237, 220));
	} else if (setupProgress == 2) {
		for (int i = 0; i < amountActiveCOM; i++) {
			if (i != selectedCOM) {
				activeCOMPorts[i].roundRect.setOutlineColor(sf::Color(120, 120, 120, 180));
				if (activeCOMPorts[i].isArduino)
					activeCOMPorts[i].roundRect.setOutlineColor(sf::Color(25, 151, 156, 220));
				if (mousePos.x >= activeCOMPortsArea[i].x1 && mousePos.x <= activeCOMPortsArea[i].x2 && mousePos.y >= activeCOMPortsArea[i].y1 && mousePos.y <= activeCOMPortsArea[i].y2)
					activeCOMPorts[i].roundRect.setOutlineColor(sf::Color(242, 242, 242, 210));
			}
		}
		if (selectedCOM >= 0)
			activeCOMPorts[selectedCOM].roundRect.setOutlineColor(sf::Color(19, 161, 237, 220));
	}
	else if (setupProgress == 4) {
		roundedRectangle[3].setOutlineColor(sf::Color(145, 43, 179, 200));
		textLabel[9].setFillColor(sf::Color(35, 171, 176, 255));
		if (mousePos.x >= roundButtonArea[3].x1 && mousePos.x <= roundButtonArea[3].x2 && mousePos.y >= roundButtonArea[3].y1 && mousePos.y <= roundButtonArea[3].y2)
			roundedRectangle[3].setOutlineColor(sf::Color(197, 90, 232, 210));
		if (mousePos.x >= 639 && mousePos.x <= 639 + textLabel[9].getGlobalBounds().width && mousePos.y >= 247 && mousePos.y <= 249 + textLabel[9].getGlobalBounds().height)
			textLabel[9].setFillColor(sf::Color(69, 231, 237, 255));
	}
	return true;
}
void getActiveCOM() {
	CEnumerateSerial::CPortsArray ports;
	CEnumerateSerial::CNamesArray names;
#ifdef CENUMERATESERIAL_USE_STL
	size_t i = 0;
	UNREFERENCED_PARAMETER(i);
#elif defined _AFX
	INT_PTR i = 0;
#else
	int i = 0;
#endif

#ifndef NO_CENUMERATESERIAL_USING_SETUPAPI2
	if (CEnumerateSerial::UsingSetupAPI2(ports, names)){
		activeCOMPortsArea.clear();
		activeCOMPorts.clear();
		amountActiveCOM = 0;

#ifdef CENUMERATESERIAL_USE_STL
		int p = 0;
		if (selectedModel == 2)
			p = 2;
		for (i = 0; i < ports.size() + p; i++) {

			comButton b;

			b.roundRect = sf::RoundedRectangleShape(sf::Vector2f(190, 196), 10, 10);
			b.roundRect.setOutlineThickness(-3);
			b.roundRect.setPosition(sf::Vector2f(10 + 199 * amountActiveCOM, 135));
			b.roundRect.setFillColor(sf::Color::Transparent);
			b.roundRect.setOutlineColor(sf::Color(120, 120, 120, 180));
			
			string q;
			string s;
			if (i < ports.size()){
				q = string("COM" + to_string(ports[i]));
				CString cs(names[i].c_str());
				CT2CA pszConvertedAnsiString(cs);
				s = string(pszConvertedAnsiString);
			} else {
				q = "COM" + to_string(i * 2);
				if (i - ports.size() == 0)
					s = "Du bist nicht";
				else
					s = "Arduino Russki";
			}
			b.namesCOM = q;
			b.portsCOM = s;
			
			b.namesCOMText.setString(s);
			b.namesCOMText.setCharacterSize(18);
			setFont(b.namesCOMText, "comfortaa");
			if (b.namesCOMText.getGlobalBounds().width > 170) {
				do {
					b.namesCOMText.setCharacterSize(b.namesCOMText.getCharacterSize() - 1);
				} while (b.namesCOMText.getGlobalBounds().width > 170);
			}
			b.namesCOMText.setOrigin(b.namesCOMText.getGlobalBounds().width / 2, 0);
			b.namesCOMText.setPosition(sf::Vector2f(105 + 199 * amountActiveCOM, 279));
			
			b.portsCOMText.setString(q);
			b.portsCOMText.setCharacterSize(19);
			setFont(b.portsCOMText, "comfortaa");
			b.portsCOMText.setOrigin(b.portsCOMText.getGlobalBounds().width / 2, 0);
			b.portsCOMText.setPosition(sf::Vector2f(105 + 199 * amountActiveCOM, 300));

			size_t found = s.find("Arduino");
			if (found != std::string::npos)
				b.isArduino = true;
			else
				b.isArduino = false;

			clickableRoundButton d = {
				(10 + 199 * amountActiveCOM), 135, (200 + 199 * amountActiveCOM), 331
			};

			activeCOMPortsArea.push_back(d);
			activeCOMPorts.push_back(b);
			amountActiveCOM++;
		}
/*#else
		for (i = 0; i < ports.GetSize(); i++) {
			string q("COM" + to_string(ports[i]) + " " + names[i].operator LPCTSTR());
			cout << q << endl;
			newRB(sf::Vector2f(10 + 149 * amountActiveCOMs, 135), sf::Vector2f(140, 196), 10, sf::Color(120, 120, 120, 180));
			namesCOM.push_back(q);
			amountActiveCOMs++;
		}*/
#endif //#ifdef CENUMERATESERIAL_USE_STL
	}
	else
		_tprintf(_T("Checking for active COM-ports failed, Error:%u\n"), GetLastError());
#endif
}