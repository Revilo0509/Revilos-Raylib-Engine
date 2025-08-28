#include "RRE.hpp"
#include <algorithm>
#include <cmath>

namespace RRE {

STexture::STexture() = default;

STexture::STexture(const std::string &filename) {
    tex = LoadTexture(filename.c_str());
}

STexture::STexture(const Image &image) { tex = LoadTextureFromImage(image); }

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

GamePrototype::~GamePrototype() { CloseWindow(); }

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

// --- Object implementation ---

Object::Object(Vector2 pos, STexture *texture)
    : Drawable(), pos(pos), texture(texture) {}

Object::Object(int x, int y, STexture *texture)
    : Drawable(), pos{static_cast<float>(x), static_cast<float>(y)},
      texture(texture) {}

Object::Object(float x, float y, STexture *texture)
    : Drawable(), pos{x, y},
      texture(texture) {}

void Object::draw() { DrawTextureV(*texture, pos, WHITE); }

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

} // namespace Funcs

} // namespace RRE
