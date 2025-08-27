#pragma once

#include "raygui.h"
#include <chrono>
#include <raylib.h>
#include <string>


namespace RRE::GUI {

class GUIObject {
  public:
    unsigned int menu = 0;

  public:
    GUIObject(unsigned int i_menu);
    virtual ~GUIObject();

    virtual void draw() = 0;
    void run() { draw(); }
};

class Button : public GUIObject {
  public:
    Rectangle rect;

    Color BgColor = WHITE;

    bool state = false;
    std::chrono::seconds holdDuration{3};

  public:
    bool getState();
    virtual void onPress();
    virtual void onRelease();
    virtual void onHold();

    void run();
};

class LabeledButton : public Button {
  public:
    std::string text = "";
    int fontSize = 12;
    Color textColor = BLACK;
};

} // namespace RRE::GUI