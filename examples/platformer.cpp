#include "RRE.hpp"

#include <array>

#include "raymath.h"

using namespace RRE;
using namespace std;

constexpr int MAP_HEIGHT = 22;
constexpr int MAP_WIDTH = 40;

class Map : public Drawable {
  public:
    STexture brick;

    Map() {
        brick = LoadTexture("./resources/Bricks.png");
        for (auto &row : map) {
            row.fill(false);
        }
    }

    void draw() {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                if (map[y][x]) {
                    DrawTexture(brick, x * 32, y * 32, WHITE);
                }
            }
        }
    }

    void togglePoint(int x, int y) {
        if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)
            map[y][x] = !map[y][x];
    }

    void drawLine(Vector2 start, Vector2 end) {
        vector<Vector2> points =
            Funcs::bresenhamLine(start, end, MAP_WIDTH, MAP_HEIGHT);
        for (Vector2 &point : points) {
            togglePoint(point.x, point.y);
        }
    }

    bool isSolid(int x, int y) const {
        if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
            return true; // Treat out-of-bounds as solid
        return map[y][x];
    }

  private:
    array<array<bool, MAP_WIDTH>, MAP_HEIGHT> map;
};

class Player : public PlayerPrototype {
  public:
    Map *mapRef;

    float velocityX = 0.0f;
    float velocityY = 0.0f;
    float gravity = 0.5f;
    float jumpStrength = -10.0f;
    float maxFallSpeed = 12.0f;
    float acceleration = 0.5f;
    float friction = 0.3f;

    Player(Vector2 pos, STexture *texture, Map *map)
        : PlayerPrototype(pos, texture), mapRef(map) {}

    void update() override {
        if (IsKeyDown(KEY_D))
            velocityX += acceleration;
        else if (IsKeyDown(KEY_A))
            velocityX -= acceleration;
        else
            velocityX *= (1.0f - friction);

        velocityX = Clamp(velocityX, -speed, speed);

        velocityY += gravity;
        if (velocityY > maxFallSpeed)
            velocityY = maxFallSpeed;

        moveX(velocityX);
        moveY(velocityY);

        if (IsKeyPressed(KEY_SPACE) && onGround())
            velocityY = jumpStrength;

        int screenWidth = GetScreenWidth();
        pos.x = Clamp(pos.x, 0.0f,
                      static_cast<float>(screenWidth) - texture->get().width);
    }

  private:
    void moveX(float dx) {
        pos.x += dx;
        Rectangle rect{pos.x, pos.y, (float)texture->get().width,
                       (float)texture->get().height};

        int startX = (int)(rect.x / 32.0f);
        int endX = (int)((rect.x + rect.width) / 32.0f);
        int startY = (int)(rect.y / 32.0f);
        int endY = (int)((rect.y + rect.height - 1) / 32.0f);

        for (int y = startY; y <= endY; y++) {
            for (int x = startX; x <= endX; x++) {
                if (mapRef->isSolid(x, y)) {
                    if (dx > 0)
                        pos.x = x * 32 - rect.width;
                    else if (dx < 0)
                        pos.x = (x + 1) * 32;
                    velocityX = 0;
                    return;
                }
            }
        }
    }

    void moveY(float dy) {
        pos.y += dy;
        Rectangle rect{pos.x, pos.y, (float)texture->get().width,
                       (float)texture->get().height};

        int startX = (int)(rect.x / 32.0f);
        int endX = (int)((rect.x + rect.width - 1) / 32.0f);
        int startY = (int)(rect.y / 32.0f);
        int endY = (int)((rect.y + rect.height) / 32.0f);

        for (int y = startY; y <= endY; y++) {
            for (int x = startX; x <= endX; x++) {
                if (mapRef->isSolid(x, y)) {
                    if (dy > 0)
                        pos.y = y * 32 - rect.height;
                    else if (dy < 0)
                        pos.y = (y + 1) * 32;
                    velocityY = 0;
                    return;
                }
            }
        }
    }

    bool onGround() {
        float footY = pos.y + texture->get().height;
        float tolerance = 2.0f;
        int startX = (int)(pos.x / 32.0f);
        int endX = (int)((pos.x + texture->get().width - 1) / 32.0f);
        int yStart = (int)(footY / 32.0f);
        int yEnd = (int)((footY + tolerance) / 32.0f);

        for (int y = yStart; y <= yEnd; y++) {
            for (int x = startX; x <= endX; x++) {
                if (mapRef->isSolid(x, y))
                    return true;
            }
        }
        return false;
    }
};

int main() {
    GamePrototype game;
    game.tickRate = 1.0f / 60.0f; // 60 ticks/s
    Map map;
    STexture t_maja("./resources/maja_nobg_small.png");
    Player player(Vector2{300, 100}, &t_maja, &map);

    map.drawLine(Vector2{0, MAP_HEIGHT - 1},
                 Vector2{MAP_WIDTH - 1, MAP_HEIGHT - 1});
    map.drawLine(Vector2{1, 1}, Vector2{10, 10});
    map.drawLine(Vector2{1, 1}, Vector2{10, 10});
    map.drawLine(Vector2{1, 1}, Vector2{10, 10});

    game.run();
}
