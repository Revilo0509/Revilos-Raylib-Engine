#include "RRE.hpp"
#include <algorithm>

namespace RRE {

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

// --- Global handlers for Updatable and Drawable instances ---

static InstanceManager<Updatable> UpdatableManager;
inline Updatable::Updatable() { UpdatableManager.add(this); }
inline Updatable::~Updatable() { UpdatableManager.remove(this); }

static InstanceManager<Drawable> DrawableManager;
inline Drawable::Drawable() { DrawableManager.add(this); }
inline Drawable::~Drawable() { DrawableManager.remove(this); }

// --- GamePrototype implementation ---

GamePrototype::GamePrototype(int WINDOW_WIDTH, int WINDOW_HEIGHT,
                             std::string WINDOW_TITLE,
                             unsigned int i_ConfigFlags, int i_monitor) {
    SetConfigFlags(i_ConfigFlags);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str());
    SetWindowMonitor(i_monitor);
    
}

GamePrototype::~GamePrototype() { CloseWindow(); }

void GamePrototype::run() {
    float accumulator = 0.0f;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        accumulator += deltaTime;

        while (accumulator >= tickRate) {
            this->update();
            UpdatableManager.run();
            accumulator -= tickRate;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawableManager.run();
        this->draw();

        EndDrawing();
    }
}

void GamePrototype::draw() {}
void GamePrototype::update() {}

// --- Object implementation ---

Object::Object(int x, int y, Texture *i_texture)
    : Drawable(), pos{static_cast<float>(x), static_cast<float>(y)},
      texture(i_texture) {}

Object::Object(Vector2 i_pos, Texture *i_texture)
    : Drawable(), pos(i_pos), texture(i_texture) {}

void Object::draw() { DrawTextureV(*texture, pos, WHITE); }

} // namespace RRE
