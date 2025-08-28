#pragma once

#include <string>
#include <vector>

#include "raylib.h"

#ifdef RRE_GUI
#include "RRE_GUI.hpp"
#endif

namespace RRE {

class STexture {
  public:
    STexture();
    explicit STexture(const std::string &filename);
    explicit STexture(Image &image);

    STexture(const Texture &texture);
    STexture(Texture &&texture) noexcept;

    ~STexture();

    STexture(const STexture &other) = delete;
    STexture &operator=(const STexture &other) = delete;

    STexture(STexture &&other) noexcept;
    STexture &operator=(STexture &&other) noexcept;

    operator Texture() const;
    Texture get() const;

  private:
    Texture tex{0};
    void unload();
};

class GamePrototype {
  public:
    float tickRate = 1.0f / 128.0f;
    Color backgroundColor = BLACK;

  public:
    GamePrototype(int WINDOW_WIDTH = 1280, int WINDOW_HEIGHT = 720,
                  std::string WINDOW_TITLE = "RRE Default Title",
                  unsigned int ConfigFlags = FLAG_VSYNC_HINT |
                                             FLAG_WINDOW_HIGHDPI,
                  int monitor = 0);
    GamePrototype(std::string WINDOW_TITLE);
    ~GamePrototype();

    void setBackgroundColor(Color color);

    void run();            // Main game loop
    virtual void update(); // Logic updated at fixed tick rate
    virtual void draw();   // Rendering runs as fast as possible

    virtual void postUpdate(); // Runs once after all updates are done
    virtual void preDraw();    // Runs right after CLEARBACKGROUND() is called
};

class Updatable {
  public:
    Updatable();
    virtual ~Updatable();

    virtual void update() = 0;
    void run() { update(); }
};

class Drawable {
  public:
    Drawable();
    virtual ~Drawable();

    virtual void draw() = 0;
    void run() { draw(); }
};

class Object : public Drawable {
  public:
    Vector2 pos;
    STexture *texture;

  public:
    Object(Vector2 pos, STexture *texture);
    Object(int x, int y, STexture *texture);
    Object(float x, float y, STexture *texture);
    void draw() override;
};

class PlayerPrototype : public Object, Updatable {
  public:
    float speed = 3.0f;

  public:
    using Object::Object;

    void update() override;
};

// Generic manager for instances
template <typename T> class InstanceManager {
  public:
    void add(T *instance);
    void remove(T *instance);
    virtual void run();

  protected:
    std::vector<T *> instances;
};

// Specialized InstanceManagers
class UpdatableManager : public InstanceManager<Updatable> {
  public:
    void run() override;
};
class DrawableManager : public InstanceManager<Drawable> {
  public:
    void run() override;
};

using Velocity = Vector2;

namespace Funcs {

Velocity getTopDownPlayerMovement();
Velocity getSidePlayerMovement();

std::vector<Vector2>
bresenhamLine(Vector2 start, Vector2 end, int mapWidth,
              int mapHeight); // Returns a list of all intersected points within
                              // the mapBounds

} // namespace Funcs

} // namespace RRE