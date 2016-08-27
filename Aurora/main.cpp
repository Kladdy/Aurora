//A q p
//   w
//Before release: Link sfml-main.lib and set subsystem under linker->system to Windows (/SUBSYSTEM:WINDOWS)

#include <iostream>
#include <SFML/Graphics.hpp>
#include <functional>
#include <Windows.h>
#include "stdafx.h"
#include "resource.h"
#include "UILib.h"
#include "fonts.hpp"
#include "pictures.hpp"
#include "RoundedRectangleShape.hpp"

#include <chrono>
#include <thread>

using namespace std;

const int TRAYWINDOW_WIDTH = 350;
const int TRAYWINDOW_HEIGHT = 275;
const int TRAYWINDOW_DISTANCE = 15;

const int MAINWINDOW_WIDTH = 1024;
const int MAINWINDOW_HEIGHT = 576;

const int SETUPWINDOW_WIDTH = 1024;
const int SETUPWINDOW_HEIGHT = 400;

CTrayIcon trayIcon("Aurora", true, LoadIcon((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)));
sf::Vector2i getTaskbarPos();
sf::Vector2i mousePos;
sf::Clock closeTrayClock;
sf::Clock limitClock;
sf::Clock mainLimitClock;

sf::CircleShape backTriangle(20, 3);
sf::CircleShape forwardTriangle(20, 3);

IL UI; //Initialize user-interface library

bool runSetup = true;
bool focusMain = false;

struct Window
{
	sf::RenderWindow inst;
	std::function<void()> loop;
	bool isVisible() { return IsWindowVisible(inst.getSystemHandle()); }
};

Window trayWindow;
Window mainWindow;

void TrayIcon1_OnMessage(CTrayIcon* pTrayIcon, UINT uMsg){

	if (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP){
		if (!trayWindow.isVisible() && closeTrayClock.getElapsedTime().asMilliseconds() > 200){
			sf::Vector2i trayWindowPos = getTaskbarPos();
			SetWindowPos(trayWindow.inst.getSystemHandle(),
				HWND_TOPMOST,
				trayWindowPos.x,
				trayWindowPos.y,
				TRAYWINDOW_WIDTH,
				TRAYWINDOW_HEIGHT,
				SWP_SHOWWINDOW);
			SetForegroundWindow(trayWindow.inst.getSystemHandle());
			trayWindow.inst.setVisible(true);
			trayWindow.inst.display();
		}
		else {
			trayWindow.inst.setVisible(false);
		}
	}
}

void AuroraTray(sf::RenderWindow* w){

	sf::RenderWindow& window = *w;
	if (GetFocus() != window.getSystemHandle()){
		if (GetFocus() == mainWindow.inst.getSystemHandle())
			focusMain = true;
		SetWindowPos(window.getSystemHandle(),
			HWND_NOTOPMOST, 0, 0, 0, 0,
			SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE);
		if (focusMain){
			SetForegroundWindow(mainWindow.inst.getSystemHandle());
			focusMain = false;
		}
		closeTrayClock.restart();
	}
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			mainWindow.inst.setVisible(true);
			SetForegroundWindow(mainWindow.inst.getSystemHandle());
			focusMain = true;
		}
	}
	window.clear(sf::Color::Black);
	window.display();
}

void AuroraMain(sf::RenderWindow* w){
	sf::RenderWindow& window = *w;
	mousePos = sf::Mouse::getPosition(window);

	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mainWindow.inst.setVisible(false);
		if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			UI.mouseClicked(mousePos, 1);
		}
	}

	window.clear(sf::Color::Black);

	UI.render(window, mousePos);

	window.display();
}

sf::Vector2i getTaskbarPos(){

	APPBARDATA taskBar;
	taskBar.cbSize = sizeof(taskBar);
	SHAppBarMessage(ABM_GETTASKBARPOS, &taskBar);

	bool leftRight = false;
	if ((taskBar.rc.bottom - taskBar.rc.top) > (taskBar.rc.right - taskBar.rc.left)){
		leftRight = true;
	}

	if (taskBar.rc.left == 0 && leftRight){ //Taskbar is on the left
		return sf::Vector2i(taskBar.rc.right + TRAYWINDOW_DISTANCE, taskBar.rc.bottom - TRAYWINDOW_DISTANCE - TRAYWINDOW_HEIGHT);}
	else if (taskBar.rc.left != 0 && leftRight){ //Taskbar is on the right
		return sf::Vector2i(taskBar.rc.left - TRAYWINDOW_DISTANCE - TRAYWINDOW_WIDTH, taskBar.rc.bottom - TRAYWINDOW_DISTANCE - TRAYWINDOW_HEIGHT);}
	if (taskBar.rc.top == 0 && !leftRight){ //Taskbar is on the top
		return sf::Vector2i(taskBar.rc.right - TRAYWINDOW_DISTANCE - TRAYWINDOW_WIDTH, taskBar.rc.bottom + TRAYWINDOW_DISTANCE);}
	else if (taskBar.rc.top != 0 && !leftRight){ //Taskbar is on the bottom
		return sf::Vector2i(taskBar.rc.right - TRAYWINDOW_DISTANCE - TRAYWINDOW_WIDTH, taskBar.rc.top - TRAYWINDOW_DISTANCE - TRAYWINDOW_HEIGHT);}
	return sf::Vector2i(0, 0);
}

void initializeSetup(){

	UI.loadFontFromMemory((void*)ComfortaaLight, ComfortaaLight_Size, "comfortaa");

	UI.newTextLabel(10, 7, "Aurora - Setup: Introduction", "comfortaa", 30);
	UI.newTextLabel(10, 45, "Greetings! I will guide you through how to properly set up Aurora. If this is your first time using the program, \nplease read the contents of this walkthrough carefully."\
		"You can re-run the setup-process by *insert feature \nto repeat setup here*. Use the arrows below in order to maneuver your way through the different options."\
		"\n\nIf you are unsure about which settings to opt for, or you believe that something is missing or not working as \nintended, don't hesitate to contact me. *Insert methods of contacting me here*", "comfortaa", 18);
	UI.newTextLabel(463, 349, "Refresh", "comfortaa", 26);
	UI.newTextLabel(10, 195, "Baud-rate:     9600", "comfortaa", 18);
	UI.newTextLabel(10, 265, "LED amount:          1", "comfortaa", 18);
	UI.newTextLabel(20, 126, "Wiring your RGB strip", "comfortaa", 26);
	UI.newTextLabel(179, 281, "", "comfortaa", 16);
	UI.newTextLabel(522, 349, "Push to Arduino", "comfortaa", 26);
	UI.newTextLabel(750, 349, "Save as .ino file", "comfortaa", 26);
	UI.newTextLabel(639, 245, "www.arduino.cc", "comfortaa", 18);

	UI.loadTextureFromMemory((void*)AuroraLogo, AuroraLogo_Size, "auroralogo");
	UI.loadTextureFromMemory((void*)AuroraLogoGlow, AuroraLogoGlow_Size, "auroralogoglow");
	UI.loadTextureFromMemory((void*)ArduinoLogo, ArduinoLogo_Size, "arduinologo");
	UI.loadTextureFromMemory((void*)StripDIN5V, StripDIN5V_Size, "stripdin5v");
	UI.loadTextureFromMemory((void*)StripRGB12V, StripRGB12V_Size, "striprgb12v");
	UI.loadTextureFromMemory((void*)StripIltrof, StripIltrof_Size, "stripiltrof");
	UI.loadTextureFromMemory((void*)baudArrow, baudArrow_Size, "baudarrow");

	UI.addStrip(5, "DIN, +5V & GND", "stripdin5v", true);
	UI.addStrip(12, "R, G, B & +12V", "striprgb12v", false);
	UI.addStrip(314, "5 fingers", "stripiltrof", true);

	UI.newSprite(sf::Vector2f(512, 280), "auroralogo");
	UI.newSprite(sf::Vector2f(512, 280), "auroralogoglow");
	UI.newSprite(sf::Vector2f(-300, -300), "arduinologo");
	UI.newSprite(sf::Vector2f(120, 206), "baudarrow");
	UI.newSprite(sf::Vector2f(125, 333), "stripdin5v");

	backTriangle.setFillColor(sf::Color(255, 191, 54, 200));
	backTriangle.setOrigin(backTriangle.getGlobalBounds().width / 2, backTriangle.getGlobalBounds().height / 2);
	backTriangle.rotate(-90);
	backTriangle.setPosition(sf::Vector2f(34, 367));
	UI.newRoundButton(sf::Vector2f(35, 365), sf::Vector2f(50, 50), 10, sf::Color(0, 0, 0, 0), &backTriangle);
	
	forwardTriangle.setFillColor(sf::Color(0, 219, 58, 200));
	forwardTriangle.setOrigin(forwardTriangle.getGlobalBounds().width / 2, forwardTriangle.getGlobalBounds().height / 2);
	forwardTriangle.rotate(90);
	forwardTriangle.setPosition(sf::Vector2f(990, 362));
	UI.newRoundButton(sf::Vector2f(989, 365), sf::Vector2f(50, 50), 10, sf::Color(0, 0, 0, 0), &forwardTriangle);

	UI.newRoundButton(sf::Vector2f(512, 365), sf::Vector2f(120, 50), 10, sf::Color(31, 109, 140, 200));
	UI.newRoundButton(sf::Vector2f(153, 143), sf::Vector2f(286, 50), 10, sf::Color(145, 43, 179, 200));
	UI.newRoundButton(sf::Vector2f(200, 332), sf::Vector2f(264, 115), 10, sf::Color(120, 120, 120, 180));
	UI.newRoundButton(sf::Vector2f(620, 365), sf::Vector2f(220, 50), 10, sf::Color(25, 151, 156, 220));
	UI.newRoundButton(sf::Vector2f(847, 365), sf::Vector2f(220, 50), 10, sf::Color(24, 115, 171, 180));



	UI.newFade(sf::Vector2f(SETUPWINDOW_WIDTH, SETUPWINDOW_HEIGHT), 600);
	UI.updateSetup();

	runSetup = false;
}

int main(){

	UI.libSetup();

	//Initialize the main window
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	if (!runSetup)
		mainWindow.inst.create(sf::VideoMode(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT), "Aurora", sf::Style::Close, settings);
	else{
		mainWindow.inst.create(sf::VideoMode(SETUPWINDOW_WIDTH, SETUPWINDOW_HEIGHT), "Aurora Setup", sf::Style::Close, settings);
	}
	SetClassLongPtr(mainWindow.inst.getSystemHandle(), GCL_HICON, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)));
	sf::Vector2i mainWindowPos = mainWindow.inst.getPosition();
	mainWindow.inst.setPosition(sf::Vector2i(-32000, -32000));
	mainWindow.loop = bind(AuroraMain, &mainWindow.inst);
	mainWindow.inst.setVisible(false);

	//Initialize the tray window
	trayWindow.inst.create(sf::VideoMode(TRAYWINDOW_WIDTH, TRAYWINDOW_HEIGHT), "Aurora Tray", sf::Style::None, settings);
	trayWindow.loop = bind(AuroraTray, &trayWindow.inst);
	trayWindow.inst.setVisible(false);
	trayIcon.SetListener(TrayIcon1_OnMessage);
	SetWindowLong(trayWindow.inst.getSystemHandle(), GWL_EXSTYLE, WS_EX_TOOLWINDOW);

	mainWindow.inst.setPosition(mainWindowPos);

	MSG msg;

	while (1) {

		if (limitClock.getElapsedTime().asMilliseconds() > 20) {
			limitClock.restart();

			if (runSetup)
				initializeSetup();

			if (UI.closeAurora)
				return 0;

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
			
		if (mainLimitClock.getElapsedTime().asMilliseconds() > 10) {
			mainLimitClock.restart();

			if (mainWindow.isVisible())
				mainWindow.loop();

			if (trayWindow.isVisible())
				trayWindow.loop();
		}
		this_thread::sleep_for(chrono::milliseconds(2));
	}
	return 0;
}