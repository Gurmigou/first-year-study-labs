#include "SimpleResultBD.h"

int SimpleResultBD::getNumOfDataInFile(ifstream& in) {
    in.seekg (0, std::ifstream::end);
    int length = (int) in.tellg();
    in.seekg (0, std::ifstream::beg);
    return length / (int) sizeof (int);
}

void SimpleResultBD::read() {
    ifstream input(path, ios_base::binary);

    int length = getNumOfDataInFile(input);

    for (int i = 0; i < length; ++i) {
        int readScore = 0;
        input.read((char*)&readScore, sizeof(int));
        this->list->addLast(readScore);
    }
}

void SimpleResultBD::write(Statistics& statistics) {
    ofstream output(path, ios_base::binary | ios_base::out | ios_base::app);
    int value = statistics.getEggsCaught();
    output.write((const char*)&value, sizeof(int));
    output.close();
}

void SimpleResultBD::sort() {
    if (!isSorted) {
        this->list->mergeSort();
        this->isSorted = true;
    }
}

void SimpleResultBD::printSorted(bool asc) {
    if (!isSorted)
        sort();
    if (asc) {
        auto iterator = this->list->iterator();
        int index = 1;
        while (iterator.hasNext()) {
            cout << index <<  ") score: " << iterator.getNext() << endl;
            index++;
        }
    } else {
        auto tail = this->list->getTail();
        int index = 1;
        while (tail != nullptr) {
            cout << index <<  ") score: " << tail->value << endl;
            index++;
            tail = tail->prev;
        }
    }
}
