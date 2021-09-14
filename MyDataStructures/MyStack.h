// Created by Yehor on 07.03.2021.

#ifndef UNIVPROJECT_MYSTACK_H
#define UNIVPROJECT_MYSTACK_H

#include "MyLinkedList.h"

template<class T>
class MyStack {
private:
    MyLinkedList<T>* linkedList;

public:
    MyStack() {
        this->linkedList = new MyLinkedList<T>();
    }

    ~MyStack() {
        delete linkedList;
    }

    void push(T element) {
        linkedList->addFirst(element);
    }

    T pop() {
        return linkedList->removeFirst();
    }

    int getSize() {
        return linkedList->getSize();
    }
};

#endif //UNIVPROJECT_MYSTACK_H