#include <string>
#include <iostream>
#include <windows.h>

#include "stdafx.h"

using namespace std;

string getClip()
{
	HANDLE clip;
	string text;

	if (OpenClipboard(NULL)) {
		text = (char*)GetClipboardData(CF_TEXT);
		CloseClipboard();

		return text;
	}
}

bool isValid(string s)
{
	return s.size() >= 6 && s.find("aim_strength:") != -1
}

void setClip (double d)
{
	string s = to_string(d).substr(0,6);

	s.insert(0, "aim_strength:");
	s.insert(s.length(), ";");

	OpenClipboard(0);
	EmptyClipboard();

	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size());
	if (!hg) {
		CloseClipboard();
		return;
	}

	memcpy(GlobalLock(hg), s.c_str(), s.size());
	GlobalUnlock(hg);

	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();

	GlobalFree(hg);
}

void pressInsert()
{
	INPUT ip;

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	ip.ki.wVk = 0x2D;
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));

	return;
}

double getStrength(string s)
{
	return atof(s.substr(s.find("aim_strength") + 13, 5).c_str());
}

void adjustStrength()
{
	double secondary = 0.35;	

	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN)) {
		bool up = GetAsyncKeyState(VK_UP);
	
		if (validString(getClip())) {
			double s = getStrength(getClip()) + (up ? 0.2 : -0.2);
			cout << "new strength:   " << s << endl;
			setClip(s);
			pressINS();
		} else {
			secondary += (up ? 0.2 : -0.2);
			setClip(secondary);
			pressINS();
			cout << "invalid string in clipboard, ";
			cout << "using default, new strength: ";
			cout << secondary << endl;
		}
	}
}

int main()
{
	while (1) {
		adjustStrength();

		Sleep(200);
	}

	getchar();
}