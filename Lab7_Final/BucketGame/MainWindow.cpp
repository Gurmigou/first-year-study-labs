#include "MainWindow.h"


void MainWindovoid MainWindow::moveCursorToXY(int x, int y) {
    COORD pos = {(short )x, (short )y};
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(handle, pos);
}

void MainWindow::setCursorVisible(bool isVisible) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(handle, &structCursorInfo);
    structCursorInfo.bVisible = isVisible;
    SetConsoleCursorInfo(handle, &structCursorInfo);
}
w::fillWindowBoundaries() {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
                this->windowArr[i][j] = BOUNDARIES_CHAR;
            else
                this->windowArr[i][j] = ' ';
        }
    }
}

void MainWindow::displayWindow() {
    for (auto & i : this->windowArr) {
        for (char j : i) {
            cout << j;
        }
        cout << "\n";
    }
}

void MainWindow::fillLeftPipe(int x, int y, int length) {
    for (int i = x; i < length; ++i) {
        this->windowArr[y][i] = '-';
        this->windowArr[y-1][i] = '-';
    }
}

void MainWindow::fillRightPipe(int x, int y, int length) {
    for (int i = x - length + 1; i < WIDTH - 1; ++i) {
        this->windowArr[y][i] = '-';
        this->windowArr[y-1][i] = '-';
    }
}

vector<Point> MainWindow::fillWindowPipes(Point& upperPipe, int upperLength,
                                 Point& downPipe,  int downLength)
{
    fillLeftPipe(upperPipe.x, upperPipe.y, upperLength);
    fillLeftPipe(downPipe.x, downPipe.y, downLength);

    Point upperOpposite = upperPipe.getOppositeYPoint(WIDTH);
    upperOpposite.y += 2;
    Point downOpposite = downPipe.getOppositeYPoint(WIDTH);
    downOpposite.y += 3;

    fillRightPipe(upperOpposite.x, upperOpposite.y, upperLength);
    fillRightPipe(downOpposite.x, downOpposite.y, downLength);

    vector<Point> eggsGeneratePoints;
    eggsGeneratePoints.push_back(Point {upperPipe.x + upperLength, upperPipe.y});
    eggsGeneratePoints.push_back(Point {downPipe.x + downLength, downPipe.y});
    eggsGeneratePoints.push_back(Point {upperOpposite.x - upperLength - 1, upperOpposite.y});
    eggsGeneratePoints.push_back(Point {downOpposite.x - downLength - 1, downOpposite.y});
    return eggsGeneratePoints;
}

int MainWindow::getHeight() {
    return HEIGHT;
}

int MainWindow::getWidth() {
    return WIDTH;
}

void MainWindow::addBucketToWindow(Bucket& bucket) {
    const Point* arr = bucket.getPositionArr();
    int numChars = Bucket::getCharsToDisplayBucket();
    this->windowArr[HEIGHT - 2 - arr[0].y][arr[0].x] = '\\';
    for (int i = 1; i < numChars - 1; ++i) {
        this->windowArr[HEIGHT - 2 - arr[i].y][arr[i].x] = '_';
    }
    this->windowArr[HEIGHT - 2 - arr[numChars - 1].y][arr[numChars - 1].x] = '/';
}

bool MainWindow::arrContains(const int *arr, int size, int valueCheck) {
    for (int i = 0; i < size; ++i) {
        if (arr[i] == valueCheck)
            return true;
    }
    return false;
}

void MainWindow::spawnEggs(vector<Point>& spawnPoints, MyLinkedList<Point>& eggsList) {
    int spawnEggs = rand() % MainWindow::MAX_EGGS;

    int index = 0;
    int alreadyUsedSpawnPoints[4];

    for (int i = 0; i < spawnEggs; ++i) {
        int spawnEggInPipe = rand() % MainWindow::MAX_EGGS; // MAX_EGGS == MAX_PIPES

        if (!arrContains(alreadyUsedSpawnPoints, index, spawnEggInPipe)) {
            alreadyUsedSpawnPoints[index++] = spawnEggInPipe;

            Point& spawnPoint = spawnPoints[spawnEggInPipe];
            this->windowArr[spawnPoint.y][spawnPoint.x] = EGG_CHAR;
            eggsList.addLast(spawnPoint);
        }
    }
}

void MainWindow::moveDownEggs(MyLinkedList<Point>& eggPoints) {
    auto iterator = eggPoints.iterator();
    while (iterator.hasNext()) {
        Point& point = iterator.getNext();
        this->windowArr[point.y][point.x] = ' ';
        this->windowArr[point.y + 1][point.x] = EGG_CHAR;
        point.y += 1;
    }
}

void MainWindow::moveBucketLeft(Bucket& bucket) {
    const Point* arr = bucket.getPositionArr();
    int numChars = Bucket::getCharsToDisplayBucket();
    for (int i = 0; i < numChars; ++i) {
        char toPut;
        if (i == 0)
            toPut = '\\';
        else if (i == numChars - 1)
            toPut = '/';
        else
            toPut = '_';
        // clear point
        this->windowArr[HEIGHT-2-arr[i].y][arr[i].x] = ' ';
        // fill point
        this->windowArr[HEIGHT-2-arr[i].y][arr[i].x - 1] = toPut;
    }
    bucket.moveLeft();
}

void MainWindow::moveBucketRight(Bucket& bucket) {
    const Point* arr = bucket.getPositionArr();
    int numChars = Bucket::getCharsToDisplayBucket();
    for (int i = numChars - 1; i >= 0; --i) {
        char toPut;
        if (i == 0)
            toPut = '\\';
        else if (i == numChars - 1)
            toPut = '/';
        else
            toPut = '_';
        // clear point
        this->windowArr[HEIGHT-2-arr[i].y][arr[i].x] = ' ';
        // fill point
        this->windowArr[HEIGHT-2-arr[i].y][arr[i].x + 1] = toPut;
    }
    bucket.moveRight();
}

void MainWindow::printEmpty(int height, int width) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            cout << ' ';
        }
        cout << "\n";
    }
}

bool MainWindow::nextLeftBucketMoveWillBeOutOfBounds(Bucket &bucket) {
    const Point* arr = bucket.getPositionArr();
    return arr[0].x == 1;
}

bool MainWindow::nextRightBucketMoveWillBeOutOfBounds(Bucket &bucket) {
    const Point* arr = bucket.getPositionArr();
    return arr[Bucket::getCharsToDisplayBucket() - 1].x == WIDTH - 2;
}

bool MainWindow::eggIsOutOfBounds(Point& eggPoint) {
    return eggPoint.y >= HEIGHT;
}

void MainWindow::updateDisplay() {
    MainWindow::moveCursorToXY(0, 0);
    this->displayWindow();
}

void MainWindow::performBucketMove(Bucket& bucket) {
    Bucket::Movement movement;
    if (GetAsyncKeyState('A'))
        movement = Bucket::LEFT;
    else if (GetAsyncKeyState('D'))
        movement = Bucket::RIGHT;

    if (movement == Bucket::LEFT && !nextLeftBucketMoveWillBeOutOfBounds(bucket)) {
        this->moveBucketLeft(bucket);
    } else if (movement == Bucket::RIGHT && !nextRightBucketMoveWillBeOutOfBounds(bucket)) {
        this->moveBucketRight(bucket);
    }
}

bool MainWindow::canBeCaughtByBucket(Bucket& bucket, Point& eggPoint) {
    const Point* arr = bucket.getPositionArr();
    return eggPoint.x >= arr[0].x - 1 && eggPoint.x <= arr[Bucket::getCharsToDisplayBucket() - 1].x + 1;
}

bool MainWindow::isGameOverOrRemoveEgg(Bucket& bucket, MyLinkedList<Point>& eggPoints) {
    auto iterator = eggPoints.iterator();

    while (iterator.hasNext()) {
        Point& point = iterator.getNext();
        if (point.y == HEIGHT - 2) {
            if (canBeCaughtByBucket(bucket, point)) {
                this->statistics.addCaughtEgg();
                iterator.remove();
            }
            else
                return true; // means that there is a game over
        }
    }
    return false;
}

Statistics MainWindow::getStatistics() {
    return statistics;
}

