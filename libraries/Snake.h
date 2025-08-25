#pragma once

enum Direction {
    up = -1,
    down = 1,
    left = -2,
    right = 2
};

class Snake {
public:
    static const int FIXED_LENGTH = 4; // Testa + 3 segmenti del corpo

    Snake();

    void initialize(int startY, int startX);
    void setDirection(Direction d);
    Direction getDirection() const;

    void move();
    bool isAt(int y, int x) const;

    int getHeadY() const;
    int getHeadX() const;
    int getBodyY(int index) const;
    int getBodyX(int index) const;
    void setHeadPosition(int y, int x);

private:
    int headY, headX;
    int bodyY[FIXED_LENGTH - 1]; // Solo i segmenti del corpo (esclusa testa)
    int bodyX[FIXED_LENGTH - 1];
    Direction cur_dir;
    Direction next_dir; // Per gestire i cambi di direzione
};