// Created by Yehor on 25.04.2021.

#ifndef UNIVPROJECT_POINT_H
#define UNIVPROJECT_POINT_H

struct Point {
    int x;
    int y;

public:
    Point getOppositeYPoint(int maxX) {
        return Point{maxX - this->x, this->y};
    }
};

#endif //UNIVPROJECT_POINT_H
