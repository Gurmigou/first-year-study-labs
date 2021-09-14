#include "Bucket.h"

void Bucket::initPosition(int windowWidth) {
    int middle = windowWidth / 2;
    int y = 0;
    for (int i = middle - 4, index = 0; i < middle + 5; ++i, ++index) {
        this->position[index] = Point{i, y};
    }
}

void Bucket::moveLeft() {
    for (auto& point : this->position) {
        point.x -= 1;
    }
}

void Bucket::moveRight() {
    for (auto& point : this->position) {
        point.x += 1;
    }
}

const Point *Bucket::getPositionArr() const {
    return position;
}

int Bucket::getCharsToDisplayBucket() {
    return Bucket::CHARS_DISPLAY_BUCKET;
}
