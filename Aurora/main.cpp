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
#include "fonts.h"
#include "RoundedRectangleShape.hpp"

using namespace std;

CTrayIcon trayIcon("Aurora", true, LoadIcon((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)));
sf::Vector2i getTaskbarPos();
sf::Vector2i mousePos;
sf::Clock timerClock;

sf::CircleShape backTriangle(20, 3);
sf::CircleShape forwardTriangle(20, 3);

IL UI; //Initialize user-interface library

const int TRAYWINDOW_WIDTH = 350;
const int TRAYWINDOW_HEIGHT = 275;
const int TRAYWINDOW_DISTANCE = 15;

const int MAINWINDOW_WIDTH = 1024;
const int MAINWINDOW_HEIGHT = 576;

const int SETUPWINDOW_WIDTH = 1024;
const int SETUPWINDOW_HEIGHT = 400;

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
		cout << "TrayIcon: Clicked" << endl;
		if (!trayWindow.isVisible() && timerClock.getElapsedTime().asMilliseconds() > 200){
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
		timerClock.restart();
	}
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			cout << "TrayWindow: Leftclick" << endl;
			mainWindow.inst.setVisible(true);
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
	}
	window.clear(sf::Color::Black);

	UI.render(window);

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

	UI.newTextLabel(10, 10, "Hello akbar!", "comfortaa", 30, sf::Color::Cyan);

	backTriangle.setFillColor(sf::Color(255, 191, 54, 200));
	backTriangle.setOrigin(backTriangle.getGlobalBounds().width / 2, backTriangle.getGlobalBounds().height / 2);
	backTriangle.rotate(-90);
	backTriangle.setPosition(sf::Vector2f(34, 367));
	UI.newRoundButton(sf::Vector2f(10, 340), sf::Vector2f(50, 50), 10, sf::Color(255, 191, 54, 200), &backTriangle);
	
	forwardTriangle.setFillColor(sf::Color(0, 219, 58, 200));
	forwardTriangle.setOrigin(forwardTriangle.getGlobalBounds().width / 2, forwardTriangle.getGlobalBounds().height / 2);
	forwardTriangle.rotate(90);
	forwardTriangle.setPosition(sf::Vector2f(990, 362));
	UI.newRoundButton(sf::Vector2f(964, 340), sf::Vector2f(50, 50), 10, sf::Color(0, 219, 58, 200), &forwardTriangle);

	runSetup = false;
}

int main(){

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

	UI.loadFontFromMemory((void*)ComfortaaLight, ComfortaaLight_Size, "comfortaa");
	

	mainWindow.inst.setPosition(mainWindowPos);

	MSG msg;
	while (true){
		if (runSetup)
			initializeSetup();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		if (mainWindow.isVisible())
			mainWindow.loop();

		if (trayWindow.isVisible())
			trayWindow.loop();
	}
	return 0;
}