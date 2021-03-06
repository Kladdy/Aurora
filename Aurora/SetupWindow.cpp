#include "SetupWindow.h"
#include "RoundedRectangleShape.hpp"
#include "enumser.h"
#include "pictures.hpp"
#include "fonts.hpp"
#include <time.h>
#include <math.h> 

// functional arduino-code functions

using namespace std;

namespace setupWin {

	//Stips
	struct ledStrip {
		sf::RoundedRectangleShape roundRect;
		sf::Text voltageText, pinsText, addressableText;
		sf::Sprite image;
		sf::IntRect supportedStripsArea;
	};
	vector<ledStrip> supportedStrips;
	int selectedModel = -1;

	//COM-ports
	struct comButton {
		sf::RoundedRectangleShape roundRect;
		string namesCOM, portsCOM;
		sf::Text namesCOMText, portsCOMText;
		bool isArduino;
		sf::IntRect activeCOMPortsArea;
	};
	vector<comButton> activeCOMPorts;
	int selectedCOM = -1;

	//Baud-rate and LED amount
	int baudRates[12] = { 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200 };
	int selectedRate = 5;
	int amountLEDs = 1;

	//Logo glow on first page
	sf::Clock logoGlowClock;

	//Fading transition
	sf::Clock fadeClock;
	sf::RectangleShape fadeRectangle;
	int fadeWaitDuration;
	int fade;
	bool doFade;
	bool fadeDir;
	bool invertFade = false;
	int currentElapsed = 0;

	//Rotating 'Refresh' button
	sf::Clock rotateClock;
	bool doRotate = false;
	bool rotateDir = true;
	bool rotateChangeDir = false;
	int changedDirRotation;
	int rotation;

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

	//Checkboxes
	struct checkBox {
		sf::RectangleShape checkBoxFrame, checkBoxFill;
		sf::IntRect checkBoxArea;
		bool isChecked = false;
	};
	vector<checkBox> checkBoxes;

	//Setup variables
	int setupProgress = 0;
	bool increaseProgress = false;
	bool runSetup = true;

	//Transition button colors
	vector<sf::Color> goodColors;
	vector<sf::Color> evilColors;
	int lastColor = -1;

	//IntRects
	vector<sf::IntRect> mouseBox;
}

using namespace setupWin;

void SetupWindow::updateWindow(sf::RenderWindow& window, sf::Vector2i mousePos) {

	if (runSetup)
		initializeSetup();

	if (setupProgress == 1 || setupProgress == 2 || setupProgress == 4)
		highlightRoundbox(mousePos);

	for (int i = 6; i < loadedSprites.size(); i++) {
		window.draw(loadedSprites[i]);
	}
 	for (int i = 0; i < textLabels.size(); i++) {
 		if (!(i >= 2 && i <= 11))
 			window.draw(textLabels[i]);
 	}
	for (int i = 0; i < roundedRectangles.size(); i++) {
		if (!(i >= 2 && i <= 6)) {
			window.draw(roundedRectangles[i].roundedRectangle);
		}
	}
	for (int i = 0; i < circleShapes.size(); i++) {
		window.draw(circleShapes[i]);
	}
	if (setupProgress == 0) {
		if (logoGlowClock.getElapsedTime().asMilliseconds() < 3000) {
			sf::Color c = loadedSprites[1].getColor();
			c = sf::Color(c.r, c.g, c.g, 255 * logoGlowClock.getElapsedTime().asMilliseconds() / 3000);
			loadedSprites[1].setColor(c);
		}
		window.draw(loadedSprites[0]);
		window.draw(loadedSprites[1]);
	} else if (setupProgress == 1) {
		for (int i = 0; i < supportedStrips.size(); i++) {
			window.draw(supportedStrips[i].roundRect);
			window.draw(supportedStrips[i].voltageText);
			window.draw(supportedStrips[i].pinsText);
			window.draw(supportedStrips[i].addressableText);
			window.draw(supportedStrips[i].image);
		}
	} else if (setupProgress == 2) {
		for (int i = 0; i < activeCOMPorts.size(); i++) {
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
			}
			else {
				float x = rotateClock.getElapsedTime().asMilliseconds() / 200.f;
				if (rotateDir)
					rotation = (-100 * sin(x)) / (x + 1);
				else
					rotation = (100 * sin(x)) / (x + 1);
			}

			roundedRectangles[2].roundedRectangle.setRotation(rotation);
			textLabels[2].setRotation(rotation);
		}
		window.draw(roundedRectangles[2].roundedRectangle);
		window.draw(textLabels[2]);
	} else if (setupProgress == 3) {
		textLabels[4].setFillColor(sf::Color(255, 255, 255, 255));
		loadedSprites[3].setColor(sf::Color(255, 255, 255, 255));

		window.draw(textLabels[3]);
		loadedSprites[3].setRotation(0);
		loadedSprites[3].setPosition(120, 206);
		window.draw(loadedSprites[3]);
		loadedSprites[3].setRotation(180);
		loadedSprites[3].setPosition(193, 206);
		window.draw(loadedSprites[3]);

		if (selectedModel == -1 || supportedStrips[selectedModel].addressableText.getString() != "Addressable") {
			textLabels[4].setFillColor(sf::Color(120, 120, 120, 180));
			loadedSprites[3].setColor(sf::Color(120, 120, 120, 180));
		}

		window.draw(textLabels[4]);
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
	} else if (setupProgress == 4) {
		for (int i = 3; i < 7; i++) {
			window.draw(roundedRectangles[i].roundedRectangle);
		}
		for (int i = 5; i < 10; i++) {
			window.draw(textLabels[i]);
		}
		if (selectedModel != -1)
			window.draw(loadedSprites[4]);
	} else if (setupProgress == 5) {
		for (int i = 0; i < checkBoxes.size(); i++) {
			for (int i = 10; i < 12; i++) {
				window.draw(textLabels[i]);
			}
			window.draw(loadedSprites[5]);
			window.draw(checkBoxes[i].checkBoxFrame);
			if (checkBoxes[i].isChecked)
				window.draw(checkBoxes[i].checkBoxFill);
		}
	}

	if (doFade)
		updateFade();
	if (fade != 0)
		window.draw(fadeRectangle);
}
void SetupWindow::mouseClicked(sf::Vector2i mousePos, int buttonClicked) {
	if (buttonClicked == 1) {
		for (int i = 0; i < roundedRectangles.size(); i++) {
			if (roundedRectangles[i].roundButtonArea.contains(mousePos)) {
				if (i == 0) {
					if (setupProgress != 0 && !(doFade && fadeDir)) {
						if (doFade) {
							currentElapsed = fadeClock.getElapsedTime().asMilliseconds();
							invertFade = true;
						}
						doFade = true;
						fadeDir = true;
						increaseProgress = false;
						fadeClock.restart();
					}
					return;
				}
				else if (i == 1) {
					if (setupProgress < 20 && !(doFade && fadeDir)) {
						if (doFade) {
							currentElapsed = fadeClock.getElapsedTime().asMilliseconds();
							invertFade = true;
						}
						doFade = true;
						fadeDir = true;
						increaseProgress = true;
						fadeClock.restart();
					}
					return;
				}
				else if (i == 2 && setupProgress == 2) {
					if (doRotate) {
						rotateChangeDir = true;
						changedDirRotation = rotation;
					}
					else {
						selectedCOM = -1;
						getActiveCOM();
						doRotate = true;
						rotateClock.restart();
					}
					return;
				}
				else if (i == 3 && setupProgress == 4) {
					ShellExecute(0, 0, "http://www.google.com", 0, 0, SW_SHOW);
					return;
				}
				else if (i == 5 && setupProgress == 4) {

					return;
				}
				else if (i == 6 && setupProgress == 4) {

					return;
				}
			}
		}
		if (setupProgress == 1) {
			for (int i = 0; i < supportedStrips.size(); i++) {
				if (supportedStrips[i].supportedStripsArea.contains(mousePos)) {
					selectedModel = i;
					supportedStrips[i].roundRect.setOutlineColor(sf::Color(19, 161, 237, 220));
					return;
				}
			}
		} else if (setupProgress == 2) {
			for (int i = 0; i < activeCOMPorts.size(); i++) {
				if (activeCOMPorts[i].activeCOMPortsArea.contains(mousePos)) {
					selectedCOM = i;
					activeCOMPorts[i].roundRect.setOutlineColor(sf::Color(19, 161, 237, 220));
					return;
				}
			}
		} else if (setupProgress == 3) {
			if (mouseBox[0].contains(mousePos)) {
				selectedRate--;
				if (selectedRate < 0)
					selectedRate = 11;
				textLabels[3].setString("Baud-rate:     " + to_string(baudRates[selectedRate]));
				return;
			}
			else if (mouseBox[1].contains(mousePos)) {
				selectedRate++;
				if (selectedRate > 11)
					selectedRate = 0;
				textLabels[3].setString("Baud-rate:     " + to_string(baudRates[selectedRate]));
				return;
			}
			else if (selectedModel != -1 && supportedStrips[selectedModel].addressableText.getString() == "Addressable") {
				if (mouseBox[2].contains(mousePos)) {
					amountLEDs--;
					if (amountLEDs <= 0)
						amountLEDs = 1;
					textLabels[4].setString("LED amount:          " + to_string(amountLEDs));
					return;
				}
				else if (mouseBox[3].contains(mousePos)) {
					amountLEDs -= 5;
					if (amountLEDs <= 0)
						amountLEDs = 1;
					textLabels[4].setString("LED amount:          " + to_string(amountLEDs));
					return;
				}
				else if (mouseBox[4].contains(mousePos)) {
					amountLEDs += 5;
					if (amountLEDs >= 100)
						amountLEDs = 99;
					textLabels[4].setString("LED amount:          " + to_string(amountLEDs));
					return;
				}
				else if (mouseBox[5].contains(mousePos)) {
					amountLEDs++;
					if (amountLEDs >= 100)
						amountLEDs = 99;
					textLabels[4].setString("LED amount:          " + to_string(amountLEDs));
					return;
				}
			}
		} else if (setupProgress == 4) {
			if (mouseBox[6].contains(mousePos)) {
				ShellExecute(0, 0, "https://www.arduino.cc/en/Main/Software", 0, 0, SW_SHOW);
				return;
			}
		} else if (setupProgress == 5) {
			for (int i = 0; i < checkBoxes.size(); i++){
				if (checkBoxes[i].checkBoxArea.contains(mousePos)) {
					checkBoxes[i].isChecked = !checkBoxes[i].isChecked;
					return;
				}
			}
		}
	}
	cout << "X: " << mousePos.x << " Y: " << mousePos.y << " B: " << buttonClicked << endl;
	return;
}
void SetupWindow::initializeSetup() {

	//Colors for back/forth buttons
	goodColors.push_back(sf::Color(11, 176, 63, 180)); //Green
	goodColors.push_back(sf::Color(19, 161, 237, 180)); //Blue
	goodColors.push_back(sf::Color(202, 50, 240, 180)); //Purple
	goodColors.push_back(sf::Color(219, 192, 18, 180)); //Lime
	evilColors.push_back(sf::Color(176, 11, 124, 180)); //Purple
	evilColors.push_back(sf::Color(224, 90, 18, 180)); //Orange
	evilColors.push_back(sf::Color(69, 184, 40, 180)); //Dark Green
	evilColors.push_back(sf::Color(48, 40, 199, 180)); //Dark Blue

	//Fonts
	newFont((void*)ComfortaaLight, ComfortaaLight_Size, "comfortaa");

	//Text labels
	newText(10, 7, "Aurora - Setup: Introduction", "comfortaa", 30);
	newText(10, 45, "*Setup text*", "comfortaa", 18);
	newText(463, 349, "Refresh", "comfortaa", 26);
	newText(10, 195, "Baud-rate:     9600", "comfortaa", 18);
	newText(10, 265, "LED amount:          1", "comfortaa", 18);
	newText(20, 126, "Wiring your RGB strip", "comfortaa", 26);
	newText(179, 281, "", "comfortaa", 16);
	newText(522, 349, "Push to Arduino", "comfortaa", 26);
	newText(750, 349, "Save as .ino file", "comfortaa", 26);
	newText(639, 245, "www.arduino.cc", "comfortaa", 18);
	newText(10, 150, "Launch Aurora at the same time as Windows", "comfortaa", 18);
	newText(10, 180, "Turn off LEDs when Aurora quits", "comfortaa", 18);
	textLabels[2].setOrigin((int)(textLabels[2].getGlobalBounds().width / 2), 16);
	textLabels[2].move((int)(textLabels[2].getGlobalBounds().width / 2), 16);

	//Textures
	newTexture((void*)AuroraLogo, AuroraLogo_Size, "auroralogo");
	newTexture((void*)AuroraLogoGlow, AuroraLogoGlow_Size, "auroralogoglow");
	newTexture((void*)ArduinoLogo, ArduinoLogo_Size, "arduinologo");
	newTexture((void*)StripDIN5V, StripDIN5V_Size, "stripdin5v");
	newTexture((void*)StripRGB12V, StripRGB12V_Size, "striprgb12v");
	newTexture((void*)baudArrow, baudArrow_Size, "baudarrow");
	newTexture((void*)AuroraLogo40, AuroraLogo40_Size, "auroralogo40");
	newTexture((void*)StripIltrof, StripIltrof_Size, "stripiltrof");

	//Sprites
	newSprite(sf::Vector2f(512, 280), "auroralogo");
	newSprite(sf::Vector2f(512, 280), "auroralogoglow");
	newSprite(sf::Vector2f(-300, -300), "arduinologo");
	newSprite(sf::Vector2f(120, 206), "baudarrow");
	newSprite(sf::Vector2f(125, 333), "stripdin5v");
	newSprite(sf::Vector2f(989, 365), "auroralogo40");
	for (int i = 0; i < 6; i++) {
		loadedSprites[i].setOrigin((int)(loadedSprites[i].getGlobalBounds().width / 2), (int)(loadedSprites[i].getGlobalBounds().height / 2));}
	

	//Strips
	addStrip(5, "DIN, +5V & GND", "stripdin5v", true);
	addStrip(12, "R, G, B & +12V", "striprgb12v", false);
	addStrip(314, "5 fingers", "stripiltrof", true);
	
	//CircleShapes
	newCircleShape(sf::Vector2f(34, 367), 20, 3, sf::Color(255, 191, 54, 200), -90);
	newCircleShape(sf::Vector2f(990, 362), 20, 3, sf::Color(0, 219, 58, 200), 90);

	//Rounded rectangles
	newRoundRectangle(sf::Vector2f(35, 365), sf::Vector2f(50, 50));
	newRoundRectangle(sf::Vector2f(989, 365), sf::Vector2f(50, 50));
	newRoundRectangle(sf::Vector2f(512, 365), sf::Vector2f(120, 50), 10, sf::Color(31, 109, 140, 200));
	newRoundRectangle(sf::Vector2f(153, 143), sf::Vector2f(286, 50), 10, sf::Color(145, 43, 179, 200));
	newRoundRectangle(sf::Vector2f(200, 332), sf::Vector2f(264, 115), 10, sf::Color(120, 120, 120, 180));
	newRoundRectangle(sf::Vector2f(620, 365), sf::Vector2f(220, 50), 10, sf::Color(25, 151, 156, 220));
	newRoundRectangle(sf::Vector2f(847, 365), sf::Vector2f(220, 50), 10, sf::Color(24, 115, 171, 180));

	//Checkboxes
	newCheckBox(sf::Vector2f(450, 161), sf::Vector2f(20, 20), 1);
	newCheckBox(sf::Vector2f(450, 191), sf::Vector2f(20, 20), 1);

	//Transition fade
	transitionFade(sf::Vector2f(1024, 400), 600);
	
	//IntRects
	mouseBox.push_back(sf::IntRect(116, 196, 10, 19));
	mouseBox.push_back(sf::IntRect(189, 196, 10, 19));
	mouseBox.push_back(sf::IntRect(133, 266, 10, 19));
	mouseBox.push_back(sf::IntRect(151, 266, 17, 19));
	mouseBox.push_back(sf::IntRect(201, 266, 17, 19));
	mouseBox.push_back(sf::IntRect(226, 266, 10, 19));
	mouseBox.push_back(sf::IntRect(639, 247, 141, 17));

	updateSetup();
	runSetup = false;
}

void SetupWindow::updateSetup() {

	srand(time(NULL));

	int c;
	do {
		c = rand() % goodColors.size();
	} while (c == lastColor);

	lastColor = c;

	if (setupProgress != 0) {
		roundedRectangles[0].roundedRectangle.setOutlineColor(evilColors[c]);
		circleShapes[0].setFillColor(evilColors[c]);
	} else {
		sf::Color color = sf::Color(60, 60, 60, 200);
		roundedRectangles[0].roundedRectangle.setOutlineColor(color);
		circleShapes[0].setFillColor(color);
	}

	if (setupProgress != 5) {
		roundedRectangles[1].roundedRectangle.setOutlineColor(goodColors[c]);
		circleShapes[1].setFillColor(goodColors[c]);
	} else {
		roundedRectangles[1].roundedRectangle.setOutlineColor(sf::Color(34, 95, 102, 200));
		circleShapes[1].setFillColor(sf::Color(0, 0, 0, 0));
	}
	

	switch (setupProgress) {
	case 0:
		textLabels[0].setString("Aurora - Setup: Introduction");
		textLabels[1].setString("Greetings! I will guide you through how to properly set up Aurora. If this is your first time using the program, \nplease read the contents of this walkthrough carefully."\
			" You can re-run the setup-process by *insert feature \nto repeat setup here*. Use the arrows below in order to maneuver your way through the different options."\
			"\n\nIf you are unsure about which settings to opt for, or you believe that something is missing or not working as \nintended, don't hesitate to contact me. *Insert methods of contacting me here*");
		logoGlowClock.restart();
		break;

	case 1:
		textLabels[0].setString("Aurora - Setup: RGB Strip Model");
		textLabels[1].setString("To use Aurora, you need to know what kind of RGB strip that you are using. There are plenty of different \nmodels out there, and the currently supported ones are displayed below."\
			" If you have any RGB strip that is \nnot yet supported, throw me a message and I will add it for you. Selecting the proper RGB strip model is \nessential for making sure Aurora can do its magic with it.");
		break;

	case 2:
		textLabels[0].setString("Aurora - Setup: USB COM-port");
		textLabels[1].setString("Aurora talks to your Arduino via the USB-Serial interface. In order to be detected, the Arduino needs to be \nconnected with a USB cable to your computer. Aurora will autonomously attempt"\
			" to identify any feasible \ncandidate and, if it finds one, highlight its button below. Bluetooth support is planned for a future release, \nbut until then, selecting the right COM-port below is the way to go.");
		getActiveCOM();
		break;

	case 3:
		textLabels[0].setString("Aurora - Setup: Baud-rate & LED amount");
		textLabels[1].setString("Changing the Baud-rate of Aurora should only be done if you know what you are doing and you have a good \nreason for it. Leaving it at the default value of 9600 is sufficient, and is the only rate"\
			" that will work unless you \nmanually change the Arduino code. Setting the LED amount is only applicable to addressable strips and \nrequires you to have selected a compatible model in a previous settings page. Having a different"\
			" amount of \nLEDs on your strip to the amount set in Aurora will not enable you to take full control over the entire strip.");
		if (selectedModel == -1)
			textLabels[4].setString("LED amount:          N/             You have not yet selected a strip model. This setting is located on the second page.");
		else if (supportedStrips[selectedModel].addressableText.getString() != "Addressable")
			textLabels[4].setString("LED amount:          N/                 Your currently selected strip model is not addressable; this setting is unavailable.");
		else
			textLabels[4].setString("LED amount:          " + to_string(amountLEDs));
		break;

	case 4:
		textLabels[0].setString("Aurora - Setup: Arduino configuration");
		textLabels[1].setString("The most important factor in making Aurora work is having both the wiring and the programming done \ncorrectly. If you already selected a strip model, clicking the button below will bring you to a walkthrough on \nhow to wire up"\
			" that specific model. \n\n\n\n\nFortunately, Aurora can assist you in uploading a functional program to your Arduino. Confirm that the \nsettings below are correct and simply click the 'Push'-button to send the code to the Arduino. If you want to"\
			" \nmanually change something in the code, you can save it as a .ino file on your computer. Please note that you \nneed to have the Arduino IDE installed, which you can download from ");
		if (selectedModel == 0)
			loadedSprites[4].setTexture(loadedTextures["stripdin5v"]);
		else if(selectedModel == 1)
			loadedSprites[4].setTexture(loadedTextures["striprgb12v"]);
		else if(selectedModel == 2)
			loadedSprites[4].setTexture(loadedTextures["stripiltrof"]);

		textLabels[6].setString("");
		if (selectedModel != -1)
			textLabels[6].setString(textLabels[6].getString() + supportedStrips[selectedModel].voltageText.getString() + "\n" + supportedStrips[selectedModel].addressableText.getString() + "\n");
		else
			textLabels[6].setString(textLabels[6].getString() + "No strip model" + "\n");
		if (selectedModel != -1 && supportedStrips[selectedModel].addressableText.getString() == "Addressable")
			textLabels[6].setString(textLabels[6].getString() + "LED amount: " + to_string(amountLEDs) + "\n");
		if (selectedCOM != -1)
			textLabels[6].setString(textLabels[6].getString() + activeCOMPorts[selectedCOM].namesCOM + "\n");
		else
			textLabels[6].setString(textLabels[6].getString() + "No COM-port" + "\n");
		if (selectedCOM != -1)
			textLabels[6].setString(textLabels[6].getString() + "Baud-rate: " + to_string(baudRates[selectedRate]) + "\n");

		textLabels[9].setFillColor(sf::Color(35, 171, 176, 255));

		break;

	case 5:
		textLabels[0].setString("Aurora - Setup: Miscellaneous");
		textLabels[1].setString("For the final round of settings, Aurora needs to ask you for a few permissions. You can allow Aurora to \nautomatically open alongside Windows, which will launch Aurora in the application tray when you turn on \nyour computer. You can also"\
			" make the lights on your LED strip turn off if you quit Aurora. This will attempt \nto turn your LEDs off upon your machine turning off.\n\n\n\n\n\n\nWhen you are satisfied with the settings, click the Aurora logo to the right to launch the color control."\
			" \nThis will be your main window from now on, but remember than you can always go back into this setup \nvia the tray icon window located on your taskbar. Good luck!");
		break;

	default:
		textLabels[0].setString("");
		textLabels[1].setString("");
		break;
	}

}
void SetupWindow::updateFade() {

	if (fadeDir) {
		int i;

		if (invertFade)
			i = (256 * (fadeClock.getElapsedTime().asMilliseconds() + fadeWaitDuration - currentElapsed) / fadeWaitDuration);
		else
			i = (256 * fadeClock.getElapsedTime().asMilliseconds() / fadeWaitDuration);

		if (i < 256)
			fade = i;
		else {
			if (increaseProgress)
				setupProgress++;
			else
				setupProgress--;
			updateSetup();
			fadeDir = false;
			fade = 255;
			fadeClock.restart();
			currentElapsed = 0;
			invertFade = false;
		}
	} else {
		int i = 255 - (255 * fadeClock.getElapsedTime().asMilliseconds() / fadeWaitDuration);
		if (i > 0)
			fade = i;
		else {
			fade = 0;
			doFade = false;
			fadeDir = true;
		}
	}
	fadeRectangle.setFillColor(sf::Color(0, 0, 0, fade));
}
void SetupWindow::highlightRoundbox(sf::Vector2i mousePos) {

	if (setupProgress == 1) {
		for (int i = 0; i < supportedStrips.size(); i++) {
			if (i != selectedModel)
				supportedStrips[i].roundRect.setOutlineColor(sf::Color(120, 120, 120, 180));
			if (supportedStrips[i].supportedStripsArea.contains(mousePos))
				supportedStrips[i].roundRect.setOutlineColor(sf::Color(242, 242, 242, 210));
		}
		if (selectedModel >= 0)
			supportedStrips[selectedModel].roundRect.setOutlineColor(sf::Color(19, 161, 237, 220));
	}
	else if (setupProgress == 2) {
		for (int i = 0; i < activeCOMPorts.size(); i++) {
			if (i != selectedCOM) {
				activeCOMPorts[i].roundRect.setOutlineColor(sf::Color(120, 120, 120, 180));
				if (activeCOMPorts[i].isArduino)
					activeCOMPorts[i].roundRect.setOutlineColor(sf::Color(25, 151, 156, 220));
				if (activeCOMPorts[i].activeCOMPortsArea.contains(mousePos))
					activeCOMPorts[i].roundRect.setOutlineColor(sf::Color(242, 242, 242, 210));
			}
		}
		if (selectedCOM >= 0)
			activeCOMPorts[selectedCOM].roundRect.setOutlineColor(sf::Color(19, 161, 237, 220));
	}
	else if (setupProgress == 4) {
		roundedRectangles[3].roundedRectangle.setOutlineColor(sf::Color(145, 43, 179, 200));
		textLabels[9].setFillColor(sf::Color(35, 171, 176, 255));
		if (roundedRectangles[3].roundButtonArea.contains(mousePos))
			roundedRectangles[3].roundedRectangle.setOutlineColor(sf::Color(197, 90, 232, 210));
		if (mousePos.x >= 639 && mousePos.x <= 639 + textLabels[9].getGlobalBounds().width && mousePos.y >= 247 && mousePos.y <= 249 + textLabels[9].getGlobalBounds().height)
			textLabels[9].setFillColor(sf::Color(69, 231, 237, 255));
	}
}
void SetupWindow::getActiveCOM() {

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
	if (CEnumerateSerial::UsingSetupAPI2(ports, names)) {
		activeCOMPorts.clear();

#ifdef CENUMERATESERIAL_USE_STL
		int p = 0;
		if (selectedModel == 2)
			p = 2;
		for (i = 0; i < ports.size() + p; i++) {

			comButton b;

			b.roundRect = sf::RoundedRectangleShape(sf::Vector2f(190, 196), 10, 10);
			b.roundRect.setOutlineThickness(-3);
			b.roundRect.setPosition(sf::Vector2f(10 + 199 * activeCOMPorts.size(), 135));
			b.roundRect.setFillColor(sf::Color::Transparent);
			b.roundRect.setOutlineColor(sf::Color(120, 120, 120, 180));

			string q;
			string s;
			if (i < ports.size()) {
				q = string("COM" + to_string(ports[i]));
				CString cs(names[i].c_str());
				CT2CA pszConvertedAnsiString(cs);
				s = string(pszConvertedAnsiString);
			}
			else {
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
			b.namesCOMText.setFont(textFonts["comfortaa"]);
			if (b.namesCOMText.getGlobalBounds().width > 170) {
				do {
					b.namesCOMText.setCharacterSize(b.namesCOMText.getCharacterSize() - 1);
				} while (b.namesCOMText.getGlobalBounds().width > 170);
			}
			b.namesCOMText.setOrigin((int)(b.namesCOMText.getGlobalBounds().width / 2), 0);
			b.namesCOMText.setPosition(sf::Vector2f(105 + 199 * activeCOMPorts.size(), 279));

			b.portsCOMText.setString(q);
			b.portsCOMText.setCharacterSize(19);
			b.portsCOMText.setFont(textFonts["comfortaa"]);
			b.portsCOMText.setOrigin((int)(b.portsCOMText.getGlobalBounds().width / 2), 0);
			b.portsCOMText.setPosition(sf::Vector2f(105 + 199 * activeCOMPorts.size(), 300));

			size_t found = s.find("Arduino");
			if (found != std::string::npos)
				b.isArduino = true;
			else
				b.isArduino = false;

			b.activeCOMPortsArea = sf::IntRect(10 + 199 * activeCOMPorts.size(), 135, 190, 196);

			activeCOMPorts.push_back(b);
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

void SetupWindow::newFont(const void* data, int sizeInBytes, string label) {

	sf::Font font;
	if (!font.loadFromMemory(data, sizeInBytes)) {
		cout << "Failed to load font" << endl;
		return;
	}
	
	textFonts[label] = font;
}
void SetupWindow::newText(int x, int y, string text, string font, int size, sf::Color color) {

	sf::Text label;
	label.setPosition(sf::Vector2f(x, y));
	label.setString(text);
	label.setCharacterSize(size);
	label.setFillColor(color);
	label.setFont(textFonts["comfortaa"]);

	textLabels.push_back(label);
}
void SetupWindow::newTexture(const void* data, int sizeInBytes, string label) {

	sf::Texture texture;
	if (!texture.loadFromMemory(data, sizeInBytes)) {
		cout << "Failed to load texture" << endl;
		return;
	}
	texture.setSmooth(true);

	loadedTextures[label] = texture;
}
void SetupWindow::newSprite(sf::Vector2f position, string texture, sf::Vector2f scale) {

	sf::Sprite sprite;
	sprite.setTexture(loadedTextures[texture]);
	sprite.setScale(scale);
	sprite.setPosition(position);

	loadedSprites.push_back(sprite);
}
void SetupWindow::newRoundRectangle(sf::Vector2f position, sf::Vector2f size, int radius, sf::Color color) {
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
void SetupWindow::newCircleShape(sf::Vector2f position, int radius, int corners, sf::Color color, int rotation) {

	sf::CircleShape c(radius, corners);
	c.setFillColor(color);
	c.setOrigin((int)(c.getGlobalBounds().width / 2), (int)(c.getGlobalBounds().height / 2));
	c.rotate(rotation);
	c.setPosition(position);

	circleShapes.push_back(c);
}
void SetupWindow::newCheckBox(sf::Vector2f position, sf::Vector2f size, int sideDistance){
	checkBox c;

	c.checkBoxFrame.setSize(size);
	c.checkBoxFrame.setOrigin((int)(c.checkBoxFrame.getGlobalBounds().width / 2), (int)(c.checkBoxFrame.getGlobalBounds().height / 2));
	c.checkBoxFrame.setPosition(position);
	c.checkBoxFrame.setFillColor(sf::Color::Transparent);
	c.checkBoxFrame.setOutlineColor(sf::Color::White);
	c.checkBoxFrame.setOutlineThickness(-1);

	c.checkBoxFill.setSize(sf::Vector2f(size.x - sideDistance - 3, size.y - sideDistance - 3));
	c.checkBoxFill.setOrigin((int)(c.checkBoxFill.getGlobalBounds().width / 2), (int)(c.checkBoxFill.getGlobalBounds().height / 2));
	c.checkBoxFill.setPosition(position);
	c.checkBoxFill.setFillColor(sf::Color::White);

	int w = c.checkBoxFrame.getGlobalBounds().width / 2;
	int h = c.checkBoxFrame.getGlobalBounds().height / 2;

	c.checkBoxArea = sf::IntRect(position.x - w, position.y - h, size.x, size.y);
	c.isChecked = false;

	checkBoxes.push_back(c);
}

void SetupWindow::transitionFade(sf::Vector2f size, int duration) {

	sf::RectangleShape rect = sf::RectangleShape(size);
	rect.setFillColor(sf::Color(0, 0, 0, 0));
	fadeRectangle = rect;
	duration = duration / 2;

	fade = 0;
	doFade = false;
	fadeDir = true;
	fadeWaitDuration = duration;
}
void SetupWindow::addStrip(int voltage, string pins, string textureLabel, bool addressable) {

	ledStrip s;

	s.roundRect = sf::RoundedRectangleShape(sf::Vector2f(140, 196), 10, 10);
	s.roundRect.setOutlineThickness(-3);
	s.roundRect.setPosition(sf::Vector2f(10 + 149 * supportedStrips.size(), 135));
	s.roundRect.setFillColor(sf::Color::Transparent);
	s.roundRect.setOutlineColor(sf::Color(120, 120, 120, 180));

	s.voltageText.setString("Voltage: +" + to_string(voltage) + "V");
	s.voltageText.setCharacterSize(16);
	s.voltageText.setFont(textFonts["comfortaa"]);
	s.voltageText.setOrigin((int)(s.voltageText.getGlobalBounds().width / 2), 0);
	s.voltageText.setPosition(sf::Vector2f(80 + 149 * supportedStrips.size(), 254));

	s.pinsText.setString("Pins: " + pins);
	s.pinsText.setCharacterSize(15);
	s.pinsText.setFont(textFonts["comfortaa"]);
	int posOffSet = 0;
	if (s.pinsText.getGlobalBounds().width > 125) {
		do {
			s.pinsText.setCharacterSize(s.pinsText.getCharacterSize() - 1);
			posOffSet++;
		} while (s.pinsText.getGlobalBounds().width > 125);
	}
	s.pinsText.setOrigin((int)(s.pinsText.getGlobalBounds().width / 2), 0);
	s.pinsText.setPosition(sf::Vector2f(80 + 149 * supportedStrips.size(), 279 + posOffSet));

	s.addressableText.setCharacterSize(16);
	s.addressableText.setFont(textFonts["comfortaa"]);
	if (addressable)
		s.addressableText.setString("Addressable");
	else {
		s.addressableText.setString("Non-addressable");
		s.addressableText.setCharacterSize(15);
	}
	s.addressableText.setOrigin((int)(s.addressableText.getGlobalBounds().width / 2), 0);
	s.addressableText.setPosition(sf::Vector2f(80 + 149 * supportedStrips.size() - 1, 305 + !addressable));

	s.image.setTexture(loadedTextures[textureLabel]);
	s.image.setOrigin((int)(s.image.getGlobalBounds().width / 2), (int)(s.image.getGlobalBounds().height / 2));
	s.image.setPosition(sf::Vector2f(80 + 149 * supportedStrips.size(), 200));

	s.supportedStripsArea = sf::IntRect(10 + 149 * supportedStrips.size(), 135, 140, 196);

	supportedStrips.push_back(s);
}