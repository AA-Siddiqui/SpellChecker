#include <windows.h>

#include <iostream>
#include <vector>

using namespace std;

#define ESCAPE 27
#define BACKSPACE 8
#define TAB 9
#define ENTER 13
#define UP_ARROW 72
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define DOWN_ARROW 80
#define SCAN_CODE 224

int COLUMNS;
int ROWS;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO csbi;

void updateSize() {
  COLUMNS = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  ROWS = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

BOOL WINAPI HandlerRoutine(DWORD eventCode) {
  switch (eventCode) {
    case CTRL_CLOSE_EVENT:
      return FALSE;
      break;
    case WM_EXITSIZEMOVE:
      updateSize();
      break;
  }

  return TRUE;
}

void initialize() {
  ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
  GetConsoleScreenBufferInfo(hConsole, &csbi);
  SetConsoleCtrlHandler(HandlerRoutine, TRUE);
  updateSize();
}

enum Color {
  Black,
  Dark_Blue,
  Dark_Green,
  Sky_Blue,
  Red,
  Purple,
  Gold,
  Light_Grey,
  Dark_Grey,
  Water_Blue,
  Parrot_Green,
  Torquise,
  Pink,
  Magenta,
  Off_White,
  White
};

int defaultForeGround = White;
int defaultBackGround = Black;

void setColor(int foregroundColor, int backgroundColor) {
  SetConsoleTextAttribute(hConsole,
                          foregroundColor + (backgroundColor * 16));
}

void gotoxy(int x, int y) {
  COORD pos = {x, y};
  SetConsoleCursorPosition(hConsole, pos);
}

string tolower(string str) {
  for (int i = 0; i < str.length(); i++) {
    str[i] = tolower(str[i]);
  }
  return str;
}

vector<string> tokenize(const string& input, const string& delimiters) {
  vector<string> tokens;
  size_t start = 0, end = 0;

  while ((end = input.find_first_of(delimiters, start)) != string::npos) {
    if (end != start) {
      tokens.push_back(input.substr(start, end - start));
    }

    tokens.push_back(input.substr(end, 1));
    start = end + 1;
  }

  if (start < input.length()) {
    tokens.push_back(input.substr(start));
  }

  return tokens;
}

bool existsIn(string word, string list) {
  for (int i = 0; i < list.size(); i++) {
    if (word[0] == list[i]) {
      return true;
    }
  }
  return false;
}
