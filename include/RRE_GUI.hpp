#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

namespace RRE::GUI {

class GUIObject {
  public:
    unsigned int menu = 0;

  public:
    GUIObject(unsigned int menu);
    virtual ~GUIObject();

    virtual void draw() = 0;
    void run() { draw(); }
};

class GUIManager {
  public:
    void add(GUIObject* instance);
    void remove(GUIObject* instance);
    virtual void run();

  protected:
    std::vector<GUIObject> instances;
};

enum class State { NONE, HOVERED, PRESSED, RELEASED };

class Button : public GUIObject {
  public:
    Rectangle bounds;
    std::string label = "";
    std::chrono::seconds holdDuration{3};

  public:
    Button(Rectangle bounds, std::string label,
           std::chrono::seconds holdDuration =
               static_cast<std::chrono::seconds>(3));
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

} // namespace Funcs

} // namespace RRE::GUI