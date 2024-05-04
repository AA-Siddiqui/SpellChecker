string getStringWithBackspace(int inputForeground, int inputBackground,
                              bool password = false) {
  string input;
  char ch;

  do {
    ch = getche();
    if (input.length() <= 0 && ch == '\b') {
      setColor(defaultForeGround, defaultBackGround);
      cout << " ";
      setColor(inputForeground, inputBackground);
    }
    if (ch == 27) {
      return "-1";
    } else if (ch == '\r') {
      return input;
    }

    if (ch != '\b') {
      if (password) {
        cout << "\b*";
      }
      input.push_back(ch);
    } else {
      if (input.length() > 0) {
        cout << " \b";
        input = input.substr(0, input.length() - 1);
      }
    }
  } while (ch != '\r');

  return input;
}

class Form {
 private:
  int dataFields;
  int x, y;
  int maxLength;
  int lineSpace;
  vector<string> labels;
  vector<string> data;

  int control(int inputForeground, int inputBackground,
              int passwordField = -1) {
    string temp;
    data.clear();
    for (int i = 0; i < dataFields; i++) {
      setColor(inputForeground, inputBackground);
      gotoxy(x + maxLength + 4, y + i + (i * lineSpace));
      if (i == passwordField) {
        temp = getStringWithBackspace(inputForeground, inputBackground, true);
      } else {
        temp = getStringWithBackspace(inputForeground, inputBackground, false);
      }
      if (temp == "-1") return -1;
      // getline(cin, temp);
      data.push_back(temp);
      setColor(defaultForeGround, defaultBackGround);
    }
    return 0;
  }

 public:
  Form(int df = 1, int posX = 0, int posY = 0, int ls = 1) {
    dataFields = df;
    maxLength = 0;
    x = posX;
    y = posY;
    lineSpace = ls;
  }

  void addOptions(string label) {
    if (labels.size() < dataFields) {
      labels.push_back(label);
      if (label.length() > maxLength) {
        maxLength = label.length();
      }
    }
  }

  void addOptions(vector<string> label) {
    for (int i = 0; i < label.size(); i++) {
      addOptions(label[i]);
    }
  }

  int draw(int labelForeground, int labelBackground, int inputForeground,
           int inputBackground, int writingSpace = 20, int passwordField = -1) {
    if (labels.size() != dataFields) {
      return -1;
    }
    int internalY = y;
    for (int i = 0; i < dataFields; i++) {
      gotoxy(x, internalY);
      int margins = (maxLength - labels[i].length()) / 2;
      setColor(labelForeground, labelBackground);
      cout << " " << setw(margins) << "" << labels[i]
           << setw((maxLength - labels[i].length()) % 2 == 1 ? margins + 1
                                                             : margins)
           << ""
           << " : ";
      setColor(inputBackground, inputBackground);
      for (int j = 0; j < writingSpace; j++) cout << " ";
      internalY += lineSpace + 1;
      setColor(defaultForeGround, defaultBackGround);
    }
    int x = control(inputForeground, inputBackground, passwordField);
    system("CLS");
    setColor(defaultForeGround, defaultBackGround);
    return x;
  }

  vector<string> getData() { return data; }
  void setX(int xPos) { x = xPos; }
  void setY(int yPos) { y = yPos; }
  int getX() { return x; }
  int getY() { return y; }
  int getMaxLength() { return maxLength; }
};