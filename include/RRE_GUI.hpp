#pragma once

#include <chrono>
#include <string>

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

namespace RRE::GUI {

class Padding {
public:
  int top;
  int right;
  int bottom;
  int left;

public:
  Padding(int top = 0, int right = 0, int bottom = 0, int left = 0);
};

class GUIObject {
public:
  unsigned int menu = 0;

public:
  GUIObject(unsigned int menu);
  virtual ~GUIObject();

  virtual void draw() = 0;
  void run() { draw(); }
};

enum class TextAlign {
  TOP_LEFT,
  TOP_CENTER,
  TOP_RIGHT,
  LEFT,
  CENTER,
  RIGHT,
  BOTTOM_LEFT,
  BOTTOM_CENTER,
  BOTTOM_RIGHT
};

class Text {
public:
  std::string text;
  TextAlign aligment;
};

enum class State { NONE, HOVERED, PRESSED, RELEASED };

class Button : public GUIObject {
public:
  Rectangle bounds;
  std::string label = "";
  std::chrono::seconds holdDuration{3};
  Padding padding;

public:
  Button(
      Rectangle bounds, std::string label,
      std::chrono::seconds holdDuration = static_cast<std::chrono::seconds>(3));
  bool getState();
  virtual void onPress();
  virtual void onRelease();
  virtual void onHold();
  virtual void onHover();

  void run();

protected:
  State state = State::NONE;
};

class Slider : public GUIObject {
public:
  Rectangle bounds;

  std::string textLeft = "";
  std::string textRight = "";

  float lowerBound = 0;
  float upperBound = 100;

public:
  Slider(Rectangle bounds, float lowerBound = 0, float upperBound = 100,
         float value = 50, std::string textLeft = "",
         std::string textRight = "");
  float getValue;

protected:
  float m_value;
};

namespace Funcs {

void drawTextInRect(Rectangle *rect, std::string &text, int fontSize,
                    Color color, float xOffset);

void drawTextCenterRect(Rectangle &rect, std::string &text, int fontSize,
                        Color color);
void drawTextCenterRect(Rectangle &rect, std::string &text, int fontsize,
                        float spacing, Color tint,
                        Font font = GetFontDefault());

} // namespace Funcs

} // namespace RRE::GUI