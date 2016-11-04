#include "MainWindow.h"
#include "pictures.hpp"
#include "fonts.hpp"
#include <cmath>

using namespace std;

namespace mainWin {
	
	//Lighting modes
	struct LightingModeButton
	{
		void update()
		{
			if(destinationY != currentPosition.y)
			{
				currentPosition.y += (destinationY - currentPosition.y) / 6.f;
				if(abs(destinationY - currentPosition.y) < 1)
					currentPosition.y = destinationY;
			}

			sprite.setPosition(currentPosition);

			if (hovered)
				textOpacity = min(textOpacity + 10, 255); 
			else
				textOpacity = 0;
		}

		sf::FloatRect getRect() { return sprite.getGlobalBounds(); }

		bool hovered = false;
		float destinationY;
		sf::Uint8 textOpacity = 0;
		string name;

		sf::Vector2f startPosition;
		sf::Vector2f currentPosition;
		sf::Sprite sprite;
	};
	vector<LightingModeButton> lightingModes;
	sf::RenderTexture renderTextures[4]; //Increase if adding a new lighting mode
	vector<sf::Vector2f> rTPosition;
	int hoveredMode = 0;

	//Fonts
	map<string, sf::Font> textFonts;

	//Text labels
	vector<sf::Text> textLabels;

	//Textures
	map<string, sf::Texture> loadedTextures;
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

	//RectangleShapes
	vector<sf::RectangleShape> rectangleShapes;

	//Static color
	sf::Color pickedColor = sf::Color(0, 255, 255);
	sf::Color previewColor = sf::Color(0, 255, 255);
	sf::VertexArray hueBar(sf::Quads, 24);

	//IntRects
	vector<sf::IntRect> mouseBox;

	bool runMain = true;
	int lightingMode = -1;
	int textFade = 255;

	//Rainbow settings
	bool directionRainbow = false;
	int speedRainbow = 1;

}

using namespace mainWin;

void MainWindow::updateWindow(sf::RenderWindow& window, sf::Vector2i mousePos) {
	
	if (runMain)
		initializeMain();

	updateButtons(mousePos);

	for (int i = 6; i < loadedSprites.size(); i++) {
		window.draw(loadedSprites[i]);
	}
	for (int i = 0; i < textLabels.size(); i++) {
		if (!(i >= 1 && i <= 2) && !(i >= 5 && i <= 9))
			window.draw(textLabels[i]);
	}
	for (int i = 2; i < roundedRectangles.size(); i++) {
		window.draw(roundedRectangles[i].roundedRectangle);
	}
	for (int i = 1; i < circleShapes.size(); i++) {
		window.draw(circleShapes[i]);
	}
// 	for (int i = 4; i < rectangleShapes.size(); i++) {
// 		window.draw(rectangleShapes[i]);
// 	}
	for (int i = 0; i < lightingModes.size(); i++) {
		window.draw(lightingModes[i].sprite);
		if(hoveredMode == i)
			window.draw(loadedSprites[0]);
	}
	if (lightingMode == 0 && sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect(204, 140, 120, 320).contains(mousePos)) {
		if (mousePos.y >= 140 && mousePos.y <= 200)
			pickedColor.r = 255;
		else if (mousePos.y >= 200 && mousePos.y <= 250)
			pickedColor.r = 255 - ((mousePos.y - 200) * 5.1);
		else if (mousePos.y >= 350 && mousePos.y <= 400)
			pickedColor.r = ((mousePos.y - 350) * 5.1);
		else if (mousePos.y >= 400 && mousePos.y <= 460)
			pickedColor.r = 255;
		else
			pickedColor.r = 0;


		if (mousePos.y >= 150 && mousePos.y <= 200)
			pickedColor.g = ((mousePos.y - 150) * 5.1);
		else if (mousePos.y >= 200 && mousePos.y <= 300)
			pickedColor.g = 255;
		else if (mousePos.y >= 300 && mousePos.y <= 350)
			pickedColor.g = 255 - ((mousePos.y - 300) * 5.1);
		else
			pickedColor.g = 0;


		if (mousePos.y >= 300 && mousePos.y <= 400)
			pickedColor.b = 255;
		else if (mousePos.y >= 250 && mousePos.y <= 300)
			pickedColor.b = ((mousePos.y - 250) * 5.1);
		else if (mousePos.y >= 400 && mousePos.y <= 450)
			pickedColor.b = 255 - ((mousePos.y - 400) * 5.1);
		else
			pickedColor.b = 0;

		if (mousePos.y <= 153)
			rectangleShapes[5].setPosition(104, 152);
		else if (mousePos.y >= 447)
			rectangleShapes[5].setPosition(104, 444);
		else
			rectangleShapes[5].setPosition(104, mousePos.y - 2);

		previewColor = pickedColor;
	}

	for (int i = 0; i < lightingModes.size(); i++) {
		renderTextures[i].clear(sf::Color::Black);

		updateRenderTextures(i);
		renderTextures[i].draw(textLabels[1]);
		renderTextures[i].draw(textLabels[2]);

		if (i == 0) { //Static
			rectangleShapes[0].setFillColor(pickedColor);
			rectangleShapes[4].setFillColor(previewColor);
			textLabels[5].setString("Red:     " + to_string(previewColor.r) + "\nGreen: " + to_string(previewColor.g) + "\nBlue:     " + to_string(previewColor.b));
			renderTextures[i].draw(textLabels[5]);
			renderTextures[i].draw(hueBar);
			for (int j = 0; j < 6; j++) {
				renderTextures[i].draw(rectangleShapes[j]);
			}
			renderTextures[i].draw(roundedRectangles[0].roundedRectangle);
			renderTextures[i].draw(roundedRectangles[1].roundedRectangle);
			for (int j = 2; j < 6; j++)
				renderTextures[i].draw(loadedSprites[j]);
			renderTextures[i].draw(circleShapes[0]);
		} else if (i == 1) { //Rainbow
			for (int j = 6; j < 10; j++) {
				renderTextures[i].draw(textLabels[j]);
			}
			loadedSprites[1].setPosition(111, 196);
			renderTextures[i].draw(loadedSprites[1]);
			loadedSprites[1].setPosition(111, 236);
			renderTextures[i].draw(loadedSprites[1]);
			loadedSprites[1].setRotation(180);
			loadedSprites[1].setPosition(284, 226);
			renderTextures[i].draw(loadedSprites[1]);
			loadedSprites[1].setPosition(236, 266);
			renderTextures[i].draw(loadedSprites[1]);
			loadedSprites[1].setRotation(0);

		} else if (i == 2) { //Fade

		} else if (i == 3) { //Cycle

		}

		renderTextures[i].display();

		if (lightingMode != -1) {
			sf::Sprite s;
			s.setTexture(renderTextures[i].getTexture());

			if (rTPosition[i].x != rTPosition[i].y) {
				rTPosition[i].x += (rTPosition[i].y - rTPosition[i].x) / 10.f; //tweak this for different transition speed
				if (abs(rTPosition[i].y - rTPosition[i].x) < 1)
					rTPosition[i].x = rTPosition[i].y;
			}

			s.setPosition(100, (int)rTPosition[i].x);
			window.draw(s);
		}	
	}
	if (lightingMode != -1 && textFade != 0) {
		textFade -= (20 + 20 * lightingMode);
		if (textFade < 0)
			textFade = 0;
		textLabels[3].setFillColor(sf::Color(255, 255, 255, textFade));
		textLabels[4].setFillColor(sf::Color(255, 255, 255, textFade));
	}
}
void MainWindow::mouseClicked(sf::Vector2i mousePos, int buttonClicked) {
	if (buttonClicked == 1) {
		for (int i = 2; i < roundedRectangles.size(); i++) {
			if (roundedRectangles[i].roundButtonArea.contains(mousePos)) {
				return;
			}
		}
		if (hoveredMode != lightingModes.size()) {
			lightingMode = hoveredMode;
			for (int i = 0; i < lightingModes.size(); i++) {
				rTPosition[i].y = (576 * (i - lightingMode));
			}
			return;
		}
		if (lightingMode == 0) {
			if (sf::IntRect(134, 150, 60, 300).contains(mousePos)) {
				if (mousePos.y <= 250)
					previewColor = pickedColor;
				else if (mousePos.y <= 350)
					previewColor = sf::Color::White;
				else if (mousePos.y <= 450)
					previewColor = sf::Color::Black;
				return;
			}
		} else if (lightingMode == 1) {
			if (mouseBox[0].contains(mousePos) || mouseBox[1].contains(mousePos)) {
				directionRainbow = !directionRainbow;
				if (directionRainbow) 
					textLabels[8].setString("Upstream");
				else
					textLabels[8].setString("Downstream");
				return;
			} else if (mouseBox[2].contains(mousePos)) {
				speedRainbow--;
				if (speedRainbow <= -1)
					speedRainbow = 2;
				if(speedRainbow == 0) textLabels[9].setString("Slow");
				else if (speedRainbow == 1) textLabels[9].setString("Normal");
				else if (speedRainbow == 2) textLabels[9].setString("Fast");
				return;
			} else if (mouseBox[3].contains(mousePos)) {
				speedRainbow++;
				if (speedRainbow >= 3)
					speedRainbow = 0;
				if (speedRainbow == 0) textLabels[9].setString("Slow");
				else if (speedRainbow == 1) textLabels[9].setString("Normal");
				else if (speedRainbow == 2) textLabels[9].setString("Fast");
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
	newText(sf::Vector2f(0, 10), "Mode title", "comfortaa", 30);
	newText(sf::Vector2f(0, 48), "Mode description", "comfortaa", 18);
	newText(sf::Vector2f(100, 10), "Aurora", "comfortaa", 30);
	newText(sf::Vector2f(100, 48), "Once you have properly set up Aurora as well as your Arduino, you are ready let the colors run \nfree! To the left, select a mode for your lighting. This will then bring up"\
		" additional parameters for \nyou to configure exactly how you want your strip to act. Once you're happy, push the lighting to \nyour Arduino, which in turn will make your RGB strip dazzle with colors."\
		" If something goes wrong, \nmake sure that you completed the setup, accessable from the tray window.", "comfortaa", 18);
	newText(sf::Vector2f(234, 260), "Red:\nGreen:\nBlue:", "comfortaa", 21);
	newText(sf::Vector2f(29, 200), "Direction:", "comfortaa", 18);
	newText(sf::Vector2f(29, 240), "Speed:", "comfortaa", 18);
	newText(sf::Vector2f(140, 200), "Downstream", "comfortaa", 18);
	newText(sf::Vector2f(140 , 240), "Normal", "comfortaa", 18);
	textLabels[0].setOrigin((int)(textLabels[0].getGlobalBounds().width / 2), 0);

	//Textures
	newTexture((void*)StaticIcon, StaticIcon_Size, "staticicon");
	newTexture((void*)RainbowIcon, RainbowIcon_Size, "rainbowicon");
	newTexture((void*)FadeIcon, FadeIcon_Size, "fadeicon");
	newTexture((void*)CycleIcon, CycleIcon_Size, "cycleicon");
	newTexture((void*)IconFrame, IconFrame_Size, "iconframe");
	newTexture((void*)baudArrow, baudArrow_Size, "arrow");
	newTexture((void*)colorWheel, colorWheel_Size, "colorwheel");
	newTexture((void*)cwRed, cwRed_Size, "cwred");
	newTexture((void*)cwGreen, cwGreen_Size, "cwgreen");
	newTexture((void*)cwBlue, cwBlue_Size, "cwblue");

	//Sprites
	newSprite(sf::Vector2f(10, 10), "iconframe");
	newSprite(sf::Vector2f(111, 196), "arrow");
	newSprite(sf::Vector2f(600, 300), "cwred");
	newSprite(sf::Vector2f(600, 300), "cwgreen");
	newSprite(sf::Vector2f(600, 300), "cwblue");
	newSprite(sf::Vector2f(600, 300), "colorwheel");
	for (int i = 2; i < 6; i++)
		loadedSprites[i].setOrigin(loadedSprites[i].getGlobalBounds().width / 2, loadedSprites[i].getGlobalBounds().height / 2);

	//Lighting modes
	addMode("staticicon", "Static");
	addMode("rainbowicon", "Rainbow");
	addMode("fadeicon", "Fade");
	addMode("cycleicon", "Cycle");

	//CircleShapes
	newCircleShape(sf::Vector2f(600, 300), 53, -1, sf::Color(32, 28, 218));
	circleShapes[0].setOrigin(53, 53);

	//Rounded rectangles
	newRoundRectangle(sf::Vector2f(34, 150), sf::Vector2f(60, 300), 10, sf::Color(145, 145, 145), -2);
	newRoundRectangle(sf::Vector2f(34, 150), sf::Vector2f(60, 300), 10, sf::Color::Black, 4);

	//RectangleShapes
	newRectangle(sf::Vector2f(34, 150), sf::Vector2f(60, 100), sf::Color::Red);
	newRectangle(sf::Vector2f(34, 250), sf::Vector2f(60, 100), sf::Color::White);
	newRectangle(sf::Vector2f(34, 350), sf::Vector2f(60, 100), sf::Color::Black);
	newRectangle(sf::Vector2f(104, 150), sf::Vector2f(120, 300), sf::Color::Transparent, -2, sf::Color(145, 145, 145));
	newRectangle(sf::Vector2f(234, 150), sf::Vector2f(100, 100), sf::Color::Red, -2, sf::Color(145, 145, 145));
	newRectangle(sf::Vector2f(104, 298), sf::Vector2f(120, 4), sf::Color::Transparent, 2, sf::Color(100, 100, 100));

	//Huebar
	for (int i = 0; i < 24; i++) {
		sf::Vector2f pos;
		sf::Color color;
		if (i % 4 == 1 || i % 4 == 2) pos.x = 224;
		else pos.x = 104;
		if (i <= 1) { pos.y = 150; color = sf::Color::Red;}
		else if (i <= 5) { pos.y = 200; color = sf::Color::Yellow; }
		else if (i <= 9) { pos.y = 250; color = sf::Color::Green; }
		else if (i <= 13) { pos.y = 300; color = sf::Color::Cyan; }
		else if (i <= 17) { pos.y = 350; color = sf::Color::Blue; }
		else if (i <= 21) { pos.y = 400; color = sf::Color::Magenta; }
		else if (i <= 23) { pos.y = 450; color = sf::Color::Red; }

		hueBar[i].position = pos;
		hueBar[i].color = color;
	}

	//IntRects
	mouseBox.push_back(sf::IntRect(221, 196, 10, 19));
	mouseBox.push_back(sf::IntRect(364, 196, 10, 19));
	mouseBox.push_back(sf::IntRect(221, 236, 10, 19));
	mouseBox.push_back(sf::IntRect(316, 236, 10, 19));

	runMain = false;
}

void MainWindow::newFont(const void* data, int sizeInBytes, string label) {

	sf::Font font;
	if (!font.loadFromMemory(data, sizeInBytes)) {
		cout << "Failed to load font" << endl;
		return;
	}

	textFonts[label] = font;
}
void MainWindow::newText(sf::Vector2f position, string text, string font, int size, sf::Color color) {

	sf::Text label;
	label.setPosition(position);
	label.setString(text);
	label.setCharacterSize(size);
	label.setFillColor(color);
	label.setFont(textFonts["comfortaa"]);

	textLabels.push_back(label);
}
void MainWindow::newTexture(const void* data, int sizeInBytes, string label) {

	sf::Texture texture;
	if (!texture.loadFromMemory(data, sizeInBytes)) {
		cout << "Failed to load texture" << endl;
		return;
	}
	texture.setSmooth(true);

	loadedTextures[label] = texture;
}
void MainWindow::newSprite(sf::Vector2f position, string texture, sf::Vector2f scale) {

	sf::Sprite sprite;
	sprite.setTexture(loadedTextures[texture]);
	sprite.setScale(scale);
	sprite.setPosition(position);

	loadedSprites.push_back(sprite);
}
void MainWindow::newRoundRectangle(sf::Vector2f position, sf::Vector2f size, int radius, sf::Color color, int thickness) {
	roundRectangle r;

	r.roundedRectangle = sf::RoundedRectangleShape(size, radius, 15);
	r.roundedRectangle.setOutlineThickness(thickness);
	//r.roundedRectangle.setOrigin((int)(r.roundedRectangle.getGlobalBounds().width / 2), (int)(r.roundedRectangle.getGlobalBounds().height / 2));
	r.roundedRectangle.setPosition(position);
	r.roundedRectangle.setFillColor(sf::Color::Transparent);
	r.roundedRectangle.setOutlineColor(color);

	int w = r.roundedRectangle.getGlobalBounds().width / 2;
	int h = r.roundedRectangle.getGlobalBounds().height / 2;

	r.roundButtonArea = sf::IntRect(position.x - w, position.y - h, size.x, size.y);

	roundedRectangles.push_back(r);
}
void MainWindow::newCircleShape(sf::Vector2f position, int radius, int corners, sf::Color color, int rotation) {

	sf::CircleShape c(radius);
	if (!(corners < 0))
		c.setPointCount(corners);
	c.setFillColor(color);
	c.setOrigin((int)(c.getGlobalBounds().width / 2), (int)(c.getGlobalBounds().height / 2));
	c.rotate(rotation);
	c.setPosition(position);

	circleShapes.push_back(c);
}
void MainWindow::newRectangle(sf::Vector2f position, sf::Vector2f size, sf::Color color, int thickness, sf::Color outlineColor, int rotation) {
	
	sf::RectangleShape rect(size);
	rect.setFillColor(color);
	rect.rotate(rotation);
	rect.setPosition(position);
	rect.setOutlineThickness(thickness);
	rect.setOutlineColor(outlineColor);

	rectangleShapes.push_back(rect);
}

void MainWindow::addMode(string texture, string name) {
	
	sf::Sprite s(loadedTextures[texture]);
	s.setPosition(10, 10 + 90 * lightingModes.size());

	renderTextures[lightingModes.size()].create(924, 576);

	LightingModeButton newButton;
	newButton.startPosition = newButton.currentPosition = sf::Vector2f(10, 10 + 90 * lightingModes.size());
	newButton.destinationY = newButton.startPosition.y;
	newButton.name = name;
	newButton.sprite = s;
	rTPosition.push_back(sf::Vector2f(576 * (lightingModes.size() + 1), 576 * lightingModes.size()));
	lightingModes.push_back(newButton);
}
void MainWindow::updateButtons(sf::Vector2i mousePos) {
	hoveredMode = lightingModes.size();
	for(int i = 0; i < lightingModes.size(); i++)
	{
		lightingModes[i].update();
		sf::FloatRect hoverArea = lightingModes[i].getRect();

		if(i < lightingModes.size() - 1)
			hoverArea.height = lightingModes[i+1].getRect().top - hoverArea.top;
		else
			hoverArea.height = 117; 

		if(hoverArea.contains(sf::Vector2f(mousePos)))
		{
			lightingModes[i].hovered = true;
			loadedSprites[0].setPosition(lightingModes[i].currentPosition);
			hoveredMode = i;
		}
		else
			lightingModes[i].hovered = false;
	}

	for(int i = 0; i < lightingModes.size(); i++)
	{
		if(i > hoveredMode)
			lightingModes[i].destinationY = lightingModes[i].startPosition.y + 40;
		else
			lightingModes[i].destinationY = lightingModes[i].startPosition.y;
	}

	if(hoveredMode == lightingModes.size())
		textLabels[0].setFillColor({ 0,0,0,0 });
	else
	{
		textLabels[0].setString(lightingModes[hoveredMode].name);
		textLabels[0].setOrigin((int)(textLabels[0].getGlobalBounds().width / 2), 0);
		textLabels[0].setPosition(50, 100 + 90 * hoveredMode);
		if(hoveredMode == 1)
			textLabels[0].setPosition(52, 100 + 90 * hoveredMode);
		textLabels[0].setFillColor({255,255,255, lightingModes[hoveredMode].textOpacity});
	}
}
void MainWindow::updateRenderTextures(int lM) {
	switch (lM) {

	case 0: 
		textLabels[1].setString("Static");
		textLabels[2].setString("This mode will apply a static lighting to your strip, meaning the color will remain the same unless \na new color or mode is applied. Applying black or"\
			" white will completely turn the strip off or on \nrespectively. Note that the power draw may be a lot higher when the strip is fully white. ");
		break;

	case 1:
		textLabels[1].setString("Rainbow");
		textLabels[2].setString("This mode will apply a rainbow-like lighting effect to your strip, making all LEDs on the strip shine \nin different colors, flowing in either one direction"\
			" or the other. Changing the speed will make the \nwave go faster or slower respectively, and setting which way the wave goes should be done to \nbest fit your configuration. If you have set"\
			" a different LED-amount in the setup compared to what \nyou actually have, the wave will be extended to only show a few different hues instead of the entire \nspectrum. If the amount was"\
			" lower, the spectrum will be repeated multiple times to fill up the strip.");
		break;

	case 2:
		textLabels[1].setString("Fade");
		textLabels[2].setString("This mode will apply a fading effect to your strip, making it fade in and out repeatedly. You can \nset which colors you wish your strip to fade into, chosing between"\
			" using a single color or cycling \neither between the entire RGB spectrum or a random color each wave. The speed of the fade \ncan also be set, making the transitions quicker.");
		break;

	case 3:
		textLabels[1].setString("Cycle");
		textLabels[2].setString("This mode will apply a lighting effect that will cycle through different colors. You can ");
		break;

	default:
		textLabels[1].setString("Aurora");
		textLabels[2].setString("");
		break;
	}
}