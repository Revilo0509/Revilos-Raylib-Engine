#include "RRE.hpp"

#include <algorithm>
#include <cmath>

#include "raymath.h"

namespace RRE {

STexture::STexture() = default;

STexture::STexture(const std::string &filename) {
    tex = LoadTexture(filename.c_str());
}

STexture::STexture(Image &image) { tex = LoadTextureFromImage(image); }

STexture::STexture(const Texture &texture) : tex(texture) {}
STexture::STexture(Texture &&texture) noexcept : tex(texture) {}

STexture::~STexture() { unload(); }

STexture::STexture(STexture &&other) noexcept {
    tex = other.tex;
    other.tex = {0};
}

STexture &STexture::operator=(STexture &&other) noexcept {
    if (this != &other) {
        unload();
        tex = other.tex;
        other.tex = {0};
    }
    return *this;
}

STexture::operator Texture() const { return tex; }

Texture STexture::get() const { return tex; }

void STexture::unload() {
    if (tex.id != 0) {
        UnloadTexture(tex);
        tex = {0};
    }
}

// --- Implementation ---

template <typename T> void InstanceManager<T>::add(T *instance) {
    instances.push_back(instance);
}

template <typename T> void InstanceManager<T>::remove(T *instance) {
    instances.erase(std::remove(instances.begin(), instances.end(), instance),
                    instances.end());
}

template <typename T> void InstanceManager<T>::run() {
    for (auto instance : instances)
        instance->run();
}

// --- Special "InstanceManagers"

void DrawableManager::run() {
    for (auto instance : instances)
        instance->draw();
}
void UpdatableManager::run() {
    for (auto instance : instances)
        instance->update();
}

// --- Global handlers for Updatable and Drawable instances ---

static UpdatableManager updatableManager;
Updatable::Updatable() { updatableManager.add(this); }
Updatable::~Updatable() { updatableManager.remove(this); }

static DrawableManager drawableManager;
Drawable::Drawable() { drawableManager.add(this); }
Drawable::~Drawable() { drawableManager.remove(this); }

// --- GamePrototype implementation ---

GamePrototype::GamePrototype(int WINDOW_WIDTH, int WINDOW_HEIGHT,
                             std::string WINDOW_TITLE, unsigned int ConfigFlags,
                             int monitor) {
    SetConfigFlags(ConfigFlags);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str());
    SetWindowMonitor(monitor);
}

GamePrototype::GamePrototype(std::string WINDOW_TITLE) {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(1280, 720, WINDOW_TITLE.c_str());
    SetWindowMonitor(0);
}

GamePrototype::~GamePrototype() { CloseWindow(); }

void GamePrototype::setBackgroundColor(Color color) {
    backgroundColor = color;
};

void GamePrototype::run() {
    float accumulator = 0.0f;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        accumulator += deltaTime;

        while (accumulator >= tickRate) {
            this->update();
            updatableManager.run();
            accumulator -= tickRate;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        drawableManager.run();
        this->draw();

        EndDrawing();
    }
}

void GamePrototype::draw() {}
void GamePrototype::update() {}

void GamePrototype::postUpdate() {};
void GamePrototype::preDraw() {};

// --- Object implementation ---

Object::Object(Vector2 pos, STexture *texture)
    : Drawable(), pos(pos), texture(texture) {}

Object::Object(int x, int y, STexture *texture)
    : Drawable(), pos{static_cast<float>(x), static_cast<float>(y)},
      texture(texture) {}

Object::Object(float x, float y, STexture *texture)
    : Drawable(), pos{x, y}, texture(texture) {}

void Object::draw() { DrawTextureV(*texture, pos, WHITE); }

void PlayerPrototype::update() {
    Vector2 movement{0.0f, 0.0f};

    movement = Funcs::getTopDownPlayerMovement();
    movement.x *= speed;
    movement.y *= speed;

    pos.x += movement.x;
    pos.y += movement.y;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    Vector2 min{0.0f, 0.0f};
    Vector2 max{
        static_cast<float>(screenWidth) - texture->get().width,
        static_cast<float>(screenHeight) - texture->get().height //TODO: Fix ts
    };

    pos = Vector2Clamp(pos, min, max);
}

namespace Funcs {

Velocity getTopDownPlayerMovement() {
    Velocity vel{0, 0};

    if (IsKeyDown(KEY_W))
        vel.y -= 1;
    if (IsKeyDown(KEY_S))
        vel.y += 1;
    if (IsKeyDown(KEY_A))
        vel.x -= 1;
    if (IsKeyDown(KEY_D))
        vel.x += 1;

    float length = std::sqrt(vel.x * vel.x + vel.y * vel.y);
    if (length > 0) {
        vel.x /= length;
        vel.y /= length;
    }

    return vel;
}

Velocity getSidePlayerMovement() {
    Velocity vel{0, 0};

    if (IsKeyDown(KEY_A))
        vel.x -= 1;
    if (IsKeyDown(KEY_D))
        vel.x += 1;

    if (vel.x != 0) {
        vel.x = (vel.x > 0) ? 1 : -1;
    }

    if (IsKeyDown(KEY_SPACE)) {
        vel.y = -1;
    }

    return vel;
}

std::vector<Vector2> bresenhamLine(Vector2 start, Vector2 end, int mapWidth,
                                   int mapHeight) {
    std::vector<Vector2> points;

    int x0 = static_cast<int>(start.x);
    int y0 = static_cast<int>(start.y);
    int x1 = static_cast<int>(end.x);
    int y1 = static_cast<int>(end.y);

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        // Add point only if it is inside the map bounds
        if (x0 >= 0 && x0 < mapWidth && y0 >= 0 && y0 < mapHeight) {
            points.push_back({static_cast<float>(x0), static_cast<float>(y0)});
        }

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }

    return points;
}

} // namespace Funcs

} // namespace RRE
