// Created by Yehor on 25.04.2021.

#ifndef UNIVPROJECT_MAINWINDOW_H
#define UNIVPROJECT_MAINWINDOW_H

#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>
#include "Point.h"
#include "Bucket.h"
#include "Statistics.h"
#include "../../MyDataStructures/MyLinkedList.h"

using namespace std;

class MainWindow {
private:
    static const int HEIGHT = 29;
    static const int WIDTH  = 70;
    static const char BOUNDARIES_CHAR = (char)219;
    static const char EGG_CHAR = 'O';
    static const int MAX_EGGS = 4;

    char windowArr[HEIGHT][WIDTH]{};
    Statistics statistics;

    void fillLeftPipe(int x, int y, int length);
    void fillRightPipe(int x, int y, int length);

    //Функции курсора
    static bool arrContains(const int* arr, int size, int valueCheck);

    static bool canBeCaughtByBucket(Bucket& bucket, Point& eggPoint);

public:

    MainWindow() {
        srand(time(nullptr));
    }

    Statistics getStatistics();

    static int getHeight();

    static int getWidth();

    static void moveCursorToXY(int x, int y);

    static void printEmpty(int height, int width);

    // fills an array with boundaries characters
    void fillWindowBoundaries();
    
    // prints an array
    void displayWindow();

    // fills an array with pipes
    // return: vector of points where eggs will be spawned
    vector<Point> fillWindowPipes(Point& upperPipe, int upperLength, Point& downPipe, int downLength);

    // adds the bucket coordinated to the window array
    void addBucketToWindow(Bucket& bucket);

    // spawns eggs
    // return: vector of points which corresponds to these eggs
    void spawnEggs(vector<Point>& spawnPoints, MyLinkedList<Point>& eggsList);

    void moveDownEggs(MyLinkedList<Point>& eggPoints);

    void moveBucketLeft(Bucket& bucket);

    void moveBucketRight(Bucket& bucket);

    void updateDisplay();

    void performBucketMove(Bucket& bucket);

    bool isGameOverOrRemoveEgg(Bucket& bucket, MyLinkedList<Point>& eggPoints);

    static bool nextLeftBucketMoveWillBeOutOfBounds(Bucket& bucket);
    static bool nextRightBucketMoveWillBeOutOfBounds(Bucket& bucket);

    static bool eggIsOutOfBounds(Point& eggPoint);

    static void setCursorVisible(bool isVisible);
};


#endif //UNIVPROJECT_MAINWINDOW_H

#pragma clang diagnostic pop