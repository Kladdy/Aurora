//A q p
//   w
//Before release: Link sfml-main.lib and set subsystem under linker->system to Windows (/SUBSYSTEM:WINDOWS)

#include <functional>
#include <Windows.h>
#include "stdafx.h"
#include "SetupWindow.h"
#include "MainWindow.h"
#include "resource.h"

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

 //Initialize the windows
SetupWindow SW;
MainWindow MW;

bool focusMain = false;
bool focusSetup = false;
int MoS = 0;

struct Window {
	sf::RenderWindow inst;
	std::function<void()> loop;
	bool isVisible() { return IsWindowVisible(inst.getSystemHandle()); }
};

Window mainWindow;
Window setupWindow;
Window trayWindow;

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
sf::Vector2i getTaskbarPos() {

	APPBARDATA taskBar;
	taskBar.cbSize = sizeof(taskBar);
	SHAppBarMessage(ABM_GETTASKBARPOS, &taskBar);

	bool leftRight = false;
	if ((taskBar.rc.bottom - taskBar.rc.top) > (taskBar.rc.right - taskBar.rc.left)) {
		leftRight = true;
	}

	if (taskBar.rc.left == 0 && leftRight) { //Taskbar is on the left
		return sf::Vector2i(taskBar.rc.right + TRAYWINDOW_DISTANCE, taskBar.rc.bottom - TRAYWINDOW_DISTANCE - TRAYWINDOW_HEIGHT);
	}
	else if (taskBar.rc.left != 0 && leftRight) { //Taskbar is on the right
		return sf::Vector2i(taskBar.rc.left - TRAYWINDOW_DISTANCE - TRAYWINDOW_WIDTH, taskBar.rc.bottom - TRAYWINDOW_DISTANCE - TRAYWINDOW_HEIGHT);
	}
	if (taskBar.rc.top == 0 && !leftRight) { //Taskbar is on the top
		return sf::Vector2i(taskBar.rc.right - TRAYWINDOW_DISTANCE - TRAYWINDOW_WIDTH, taskBar.rc.bottom + TRAYWINDOW_DISTANCE);
	}
	else if (taskBar.rc.top != 0 && !leftRight) { //Taskbar is on the bottom
		return sf::Vector2i(taskBar.rc.right - TRAYWINDOW_DISTANCE - TRAYWINDOW_WIDTH, taskBar.rc.top - TRAYWINDOW_DISTANCE - TRAYWINDOW_HEIGHT);
	}
	return sf::Vector2i(0, 0);
}

void AuroraMain(sf::RenderWindow* w) {
	sf::RenderWindow& window = *w;
	mousePos = sf::Mouse::getPosition(window);

	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			mainWindow.inst.setVisible(false);
		if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			MW.mouseClicked(mousePos, 1);
		if (event.type == sf::Event::MouseWheelScrolled)
			MW.mouseScrolled(mousePos, event.mouseWheelScroll.delta);
	}

	window.clear(sf::Color::Black);

	MW.updateWindow(window, mousePos);

	window.display();
}
void AuroraSetup(sf::RenderWindow* w) {
	sf::RenderWindow& window = *w;
	mousePos = sf::Mouse::getPosition(window);

	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			setupWindow.inst.setVisible(false);
		if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
			SW.mouseClicked(mousePos, 1);
	}

	window.clear(sf::Color::Black);

	SW.updateWindow(window, mousePos);

	window.display();
}
void AuroraTray(sf::RenderWindow* w){

	sf::RenderWindow& window = *w;
	if (GetFocus() != window.getSystemHandle()){
		if (GetFocus() == mainWindow.inst.getSystemHandle())
			focusMain = true;
		else if (GetFocus() == setupWindow.inst.getSystemHandle())
			focusSetup = true;
		SetWindowPos(window.getSystemHandle(),
			HWND_NOTOPMOST, 0, 0, 0, 0,
			SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE);
		if (focusMain){
			SetForegroundWindow(mainWindow.inst.getSystemHandle());
			focusMain = false;
		} else if (focusSetup) {
			SetForegroundWindow(setupWindow.inst.getSystemHandle());
			focusSetup = false;
		}
		closeTrayClock.restart();
	}
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			if (MoS == 0) {
				mainWindow.inst.setVisible(true);
				SetForegroundWindow(mainWindow.inst.getSystemHandle());
				focusMain = true;
			} else if (MoS == 1) {
				setupWindow.inst.setVisible(true);
				SetForegroundWindow(setupWindow.inst.getSystemHandle());
				focusSetup = true;
			}
		}
	}
	window.clear(sf::Color::Black);
	window.display();
}

int main(){

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	//Initialize the main window
	mainWindow.inst.create(sf::VideoMode(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT), "Aurora", sf::Style::Close, settings);
	SetClassLongPtr(mainWindow.inst.getSystemHandle(), GCL_HICON, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)));
	mainWindow.loop = bind(AuroraMain, &mainWindow.inst);
	mainWindow.inst.setVisible(false);

	//Initialize the setup window
	setupWindow.inst.create(sf::VideoMode(SETUPWINDOW_WIDTH, SETUPWINDOW_HEIGHT), "Aurora Setup", sf::Style::Close, settings);
	SetClassLongPtr(setupWindow.inst.getSystemHandle(), GCL_HICON, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)));
	setupWindow.loop = bind(AuroraSetup, &setupWindow.inst);
	setupWindow.inst.setVisible(false);

	//Initialize the tray window
	trayWindow.inst.create(sf::VideoMode(TRAYWINDOW_WIDTH, TRAYWINDOW_HEIGHT), "Aurora Tray", sf::Style::None, settings);
	trayWindow.loop = bind(AuroraTray, &trayWindow.inst);
	trayWindow.inst.setVisible(false);
	trayIcon.SetListener(TrayIcon1_OnMessage);
	SetWindowLong(trayWindow.inst.getSystemHandle(), GWL_EXSTYLE, WS_EX_TOOLWINDOW);

	MSG msg;

	while (1) {

		if (limitClock.getElapsedTime().asMilliseconds() > 20) {
			limitClock.restart();

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
			
		if (mainLimitClock.getElapsedTime().asMilliseconds() > 10) {
			mainLimitClock.restart();

			if (mainWindow.isVisible())
				mainWindow.loop();

			if (setupWindow.isVisible())
				setupWindow.loop();

			if (trayWindow.isVisible())
				trayWindow.loop();
		}
		this_thread::sleep_for(chrono::milliseconds(2));
	}
	return 0;
}