// Created by Yehor on 26.04.2021.

#ifndef UNIVPROJECT_BUCKET_H
#define UNIVPROJECT_BUCKET_H

#include "Point.h"

class Bucket {
private:
    static const int CHARS_DISPLAY_BUCKET = 9;
    Point position[CHARS_DISPLAY_BUCKET];

public:

    enum Movement {
        LEFT,
        RIGHT
    };

    // initialized bucket position
    void initPosition(int windowWidth);

    // moves bucket one point left
    void moveLeft();

    // moves bucket one point right
    void moveRight();

    const Point* getPositionArr() const;

    static int getCharsToDisplayBucket();
};

#endif //UNIVPROJECT_BUCKET_H