// Created by Yehor on 26.04.2021.

#ifndef UNIVPROJECT_SIMPLERESULTBD_H
#define UNIVPROJECT_SIMPLERESULTBD_H

#include <iostream>
#include <fstream>
#include <string>
#include "../BucketGame/Statistics.h"
#include "../../MyDataStructures/MyLinkedList.h"

class SimpleResultBD {
private:
    inline static const string path = R"(C:\Users\Yehor\CLionProjects\UnivProject\Lab7_Final\ResultsStorage\SerializedStatistics.bin)";
    bool isSorted = false;

    void sort();

public:
    MyLinkedList<int>* list;

    SimpleResultBD() {
        this->list = new MyLinkedList<int>();
    }

    int getNumOfDataInFile(ifstream& in);

    void read();

    static void write(Statistics& statistics);

    void printSorted(bool asc);
};


#endif //UNIVPROJECT_SIMPLERESULTBD_H
