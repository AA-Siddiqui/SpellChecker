#include <conio.h>
#include <windows.h>

#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

class Menu {
 private:
  vector<string> options;
  int selected;
  int maxLength;
  int x, y;
  int margins;
  int lineSpace;
  int height;

  void draw(int colorBackgroundSelected, int colorForegroundSelected,
            int colorBackground, int colorForeground,
            void (*funcptr)() = NULL) {
    if (funcptr != NULL) funcptr();
    gotoxy(x, y);

    int internalY = y;
    for (int i = 0; i < options.size(); i++) {
      if (i == selected) {
        setColor(colorForegroundSelected, colorBackgroundSelected);
      } else {
        setColor(colorForeground, colorBackground);
      }

      for (int j = 0; j < ((height - 1) / 2); j++) {
        gotoxy(x, internalY++);
        cout << setw(maxLength + margins * 2) << "";
      }

      gotoxy(x, internalY++);
      int gLeave = margins + (maxLength - options[i].length()) / 2;
      cout << setw(gLeave) << "" << options[i]
           << setw((maxLength - options[i].length()) & 1 ? gLeave + 1 : gLeave)
           << "";

      for (int j = 0; j < ((height - 1) / 2); j++) {
        gotoxy(x, internalY++);
        cout << setw(maxLength + margins * 2) << "";
      }

      internalY += lineSpace;
    }
    setColor(defaultForeGround, defaultBackGround);
  }

  void up() {
    if (selected > 0) selected--;
  }

  void down() {
    if (selected < options.size() - 1) selected++;
  }

 public:
  Menu(int posX = 0, int posY = 0, int marginSide = 2, int ls = 1, int h = 3) {
    x = posX;
    y = posY;
    margins = marginSide;
    selected = 0;
    maxLength = 0;
    lineSpace = ls;
    height = h;
    if (height % 2 == 0) {
      height++;
    }
  }

  void addOptions(string option) {
    options.push_back(option);
    if (option.length() > maxLength) {
      maxLength = option.length();
    }
  }

  void addOptions(vector<string> option) {
    for (int i = 0; i < option.size(); i++) {
      addOptions(option[i]);
    }
  }

  int selectOption(int colorBackgroundSelected, int colorForegroundSelected,
                   int colorBackground, int colorForeground,
                   void (*funcptr)() = NULL) {
    int select = 0;
    while (select != '\r') {
      system("CLS");

      draw(colorBackgroundSelected, colorForegroundSelected, colorBackground,
           colorForeground, funcptr);
      // select = getch();

      switch ((select = getch())) {
        case ENTER:
          return selected;
          break;
        case ESCAPE:
          return -1;
          break;
        case SCAN_CODE:
          switch ((select = getch())) {
            case UP_ARROW:
              up();
              break;
            case DOWN_ARROW:
              down();
              break;
          }
          break;
      }
      // if (select == 'w')
      //   up();
      // else if (select == 's')
      //   down();
      // else if (select == 27)
      //   return -1;

      setColor(defaultForeGround, defaultBackGround);
    }

    return selected;
  }

  vector<string> getOptions() { return options; }
  int getButtons() { return options.size(); }
  int getMaxLength() { return maxLength; }
  void setX(int xPos) { x = xPos; }
  void setY(int yPos) { y = yPos; }
  void setHeight(int h) { height = h; }
  void setLineSpace(int ls) { lineSpace = ls; }
  void clear() {
    options.clear();
    maxLength = 0;
    selected = 0;
  }
};
