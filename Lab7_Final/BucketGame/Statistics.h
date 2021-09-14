// Created by Yehor on 26.04.2021.

#ifndef UNIVPROJECT_STATISTICS_H
#define UNIVPROJECT_STATISTICS_H

#include <string>

using namespace std;

struct Statistics {
private:
    int eggsCaught = 0;
public:
    void addCaughtEgg() {
        this->eggsCaught++;
    }

    void clearStatistics() {
        this->eggsCaught = 0;
    }

    void addCaughtEgg(int numOfEggs) {
        this->eggsCaught += numOfEggs;
    }

    int getEggsCaught() const {
        return eggsCaught;
    }
};

#endif //UNIVPROJECT_STATISTICS_H
