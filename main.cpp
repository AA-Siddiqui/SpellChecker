#include <Windows.h>
#include <conio.h>
#include <windows.h>

#include <fstream>
#include <iomanip>
#include <iostream>

#include "amisc.h"
#include "form.h"
#include "menu.h"
#include "spellChecker.h"
using namespace std;

int findCharacterBeforeIndex(const string& str, char targetChar, int endIndex) {
  if (endIndex < 0 || endIndex >= str.length()) {
    return -1;
  }

  for (int i = endIndex - 1; i >= 0; i--) {
    if (str[i] == targetChar) {
      return i;
    }
  }

  return -1;
}

int findCharacterAfterIndex(const string& str, char targetChar, int endIndex) {
  if (endIndex < 0 || endIndex >= str.length()) {
    return -1;
  }

  for (int i = endIndex + 1; i < str.length(); i++) {
    if (str[i] == targetChar) {
      return i;
    }
  }

  return -1;
}

class TextEditor {
 private:
  int charCount;
  string content;
  SpellChecker* spellchecker;
  fstream* file;
  vector<int> lineBreaks;

  void arrowControl(int& lastChar) {
    int leftMargin;
    int startPoint;
    switch ((lastChar = getch())) {
      case LEFT_ARROW:
        if (charCount <= 0) break;
        charCount--;
        break;

      case RIGHT_ARROW:
        if (charCount + 1 > content.length()) break;
        charCount++;
        break;

      case UP_ARROW:
        leftMargin =
            charCount - findCharacterBeforeIndex(content, '\n', charCount);

        if (leftMargin > COLUMNS) {
          charCount -= COLUMNS;
          return;
        }

        if (content[charCount + 1] == '\n') {
          for (int i = 0; i < lineBreaks.size(); i++) {
            if (lineBreaks[i] >= (charCount - 1)) {
              charCount = max(lineBreaks[max(i - 1, 0)] - 1, 0);
              return;
            }
          }
        }
        startPoint =
            findCharacterBeforeIndex(content, '\n', charCount - leftMargin);

        charCount =
            max(min(startPoint + leftMargin, charCount - leftMargin), 0);
        break;

      case DOWN_ARROW:
        leftMargin =
            charCount - findCharacterBeforeIndex(content, '\n', charCount);
        startPoint = findCharacterAfterIndex(content, '\n', charCount);

        if (startPoint > COLUMNS) {
          charCount += COLUMNS;
          return;
        }

        if (content[charCount] == '\n') {
          charCount =
              min(min(charCount + leftMargin, (int)content.length() - 1),
                  findCharacterAfterIndex(content, '\n', charCount + 1));
          return;
        }
        charCount = min(min(startPoint + leftMargin, (int)content.length() - 1),
                        findCharacterAfterIndex(content, '\n', startPoint + 1));
        break;
    }
  }

  bool control() {
    int lastChar;
    switch ((lastChar = getch())) {
      case SCAN_CODE:
        arrowControl(lastChar);
        break;
      case BACKSPACE:
        if (charCount <= 0) break;
        if (content[charCount - 1] == '\n') {
          lineBreaks.erase(
              remove(lineBreaks.begin(), lineBreaks.end(), charCount - 1),
              lineBreaks.end());
        }
        content.erase(charCount - 1, 1);
        charCount--;
        break;
      case TAB:
        content.insert(charCount, 4, ' ');
        charCount += 4;
        break;
      case ENTER:
        content.insert(charCount, 1, '\n');
        lineBreaks.push_back(charCount);
        charCount++;
        break;
      case ESCAPE:
        return false;
        break;
      default:
        content.insert(charCount, 1, (char)lastChar);
        charCount++;
    }
    return true;
  }

  vector<string> getMisspelled() {
    int printedChars = 0;
    int currentColor = White;
    string delimiters = " \n\t";
    vector<string> items = tokenize(content, delimiters);
    vector<string> misspelledWords;
    for (int i = 0; i < items.size(); i++) {
      string x = tolower(items[i]);

      bool wordExists =
          (!spellchecker->searchWord(x)) && !existsIn(x, delimiters);
      if (wordExists) {
        misspelledWords.push_back(items[i]);
        currentColor = Red;
      }

      setColor(currentColor, Black);
      for (int j = 0; j < items[i].size(); j++) {
        cout << items[i][j];
        if (printedChars + 1 == charCount) {
          setColor(White, White);
          cout << "|";
          setColor(currentColor, Black);
        }
        printedChars++;
      }

      currentColor = White;
      setColor(currentColor, Black);
    }
    cout << endl << endl;

    return misspelledWords;
  }

  void print() {
    vector<string> misspelledWords = getMisspelled();
    for (int i = 0; i < misspelledWords.size(); i++) {
      string misspelledWord = misspelledWords[i];
      string x = tolower(misspelledWord);
      vector<string> suggestions = spellchecker->getSuggestions(x);
      if (!suggestions.empty()) {
        cout << "Suggestions for " << misspelledWord << ": ";
        for (int i = 0; i < suggestions.size(); i++) {
          cout << suggestions[i] << ", ";
        }
        cout << endl;
      }
    }

    cout << "Press ESC to save and exit";

    // cout << (int)content[charCount] << ", " << lineBreaks.size() << ", "
    //      << COLUMNS << "x" << ROWS;
  }

 public:
  TextEditor(string filePath) {
    spellchecker = new SpellChecker("dictionary.txt");
    file = new fstream(filePath.c_str(), ios::in);
    string s;
    lineBreaks.push_back(0);
    while (getline((*file), s)) {
      content += (s + '\n');
      lineBreaks.push_back(content.length() - 1);
    }
    charCount = content.length();
    file->close();
    delete file;

    file = new fstream(filePath.c_str(), ios::out);
  }
  ~TextEditor() {
    file->close();
    delete spellchecker;
    delete file;
  }

  void start() {
    bool loop;

    print();
    do {
      loop = control();
      // gotoxy(0, 0);
      // setColor(Black, Black);
      // for (int i = 0; i < ROWS*COLUMNS; i++) cout << " ";
      // setColor(White, Black);
      // gotoxy(0, 0);
      system("CLS");
      print();
    } while (loop);
    (*file) << content;
  }
};

vector<string> getFilesFromFolder(string folder) {
  vector<string> names;
  string search_path = folder + "/*.*";
  WIN32_FIND_DATA fd;
  HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      // read all (real) files in current folder
      // , delete '!' read other 2 default folder . and ..
      if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        names.push_back(fd.cFileName);
      }
    } while (::FindNextFile(hFind, &fd));
    ::FindClose(hFind);
  }
  return names;
}

void thing() {
  string s[12] = {
      "  ************        *********  ", " **************     *************",
      "*****      ****    *****       **", "****        ***   *****          ",
      "*****             ****           ", " ***********      ****           ",
      "  ***********     ****           ", "         *****    ****           ",
      "***        ****   *****          ", "****      *****    *****       **",
      "**************      *************", "  ***********         *********  "};

  for (int i = 0; i < 12; i++) {
    gotoxy((COLUMNS - s[0].length()) / 2, 4 + i);
    cout << s[i];
  }
}

int main() {
  initialize();
  system("CLS");

  do {
    Menu m;

    m.addOptions("Create new file");
    vector<string> n = getFilesFromFolder("files");
    for (int i = 0; i < n.size(); i++) {
      m.addOptions(n[i]);
    }
    m.addOptions("Exit");

    m.setHeight(3);
    m.setLineSpace(0);
    m.setX((COLUMNS - m.getMaxLength()) / 2);
    m.setY(20);

    system("CLS");
    int ms = m.selectOption(White, Black, Black, White, thing);
    system("CLS");

    if (ms == -1) {
      continue;
    }

    string fileName = "";
    n = m.getOptions();
    if (ms == 0) {
      Form f(1);
      f.addOptions("File Name");

      f.setX((COLUMNS - (f.getMaxLength() + 24 + 4)) / 2);
      f.setY(ROWS / 2);

      system("CLS");
      thing();
      if (f.draw(Black, White, defaultBackGround, defaultForeGround, 18) == -1)
        continue;

      fileName = (f.getData()[0]) + ".txt";
    } else if (ms == n.size() - 1) {
      return 0;
    } else {
      fileName = n[ms];
    }

    fileName = "files\\" + fileName;
    system("CLS");
    TextEditor tx(fileName);
    tx.start();
  } while (true);

  return 0;
}
