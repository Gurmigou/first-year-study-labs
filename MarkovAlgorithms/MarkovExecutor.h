// Created by Yehor on 11.02.2021.

#ifndef UNIVPROJECT_MARKOVEXECUTOR_H
#define UNIVPROJECT_MARKOVEXECUTOR_H

#include <string>
#include <iostream>
#include <vector>
#include "C:\Users\Yehor\CLionProjects\UnivProject\MyDataStructures\MyLinkedList.h"

using namespace std;

class MarkovExecutor {
private:
    class MarkovRule {
    private:
        string oldValue;
        string newValue;
        bool isTerminatedRule;
    public:
        MarkovRule(string oldValue, string newValue, bool isTerminatedRule);
        string& getOldValue();
        string& getNewValue();
        bool end() const;
    };

    inline static const
        char* printRules = "Markov's algorithms allow transforming text according to specified rules."
                           "\n\rEx. of a rule: 'abc' -> 'da' means converting text from the left part"
                           " to the right part of the rule."
                           "\n\r'->' means convert and continue, '|->' means convert and stop.";

    inline static const
        char* cases = "(1) add rule '->'\n\r"
                      "(2) add interrupting rule '|->'\n\r"
                      "(3) start execution.";

    // contains recorded results
    vector<string>* records;

    /** Using my linked list */
    MyLinkedList<MarkovRule*>* rules;

    string* currentWord;

    // parses user input to rules
    void parseToRule();

    // executes Markov's algorithms
    void performMarkovRules();

    // applies a certain rule to the word
    bool applyRule(string& curWord, MarkovRule& rule);

    // records current words to print it as a result
    void recordCurWordState(string curWord);

    // finds the first position of substring s2 in string s1
    int getPosOfFirstSubstr(char* s1, char* s2);

    // performs left shift of the word
    void leftShift(string& s, int startPos, int oldSubSrtLength, int numOfPosMoveLeft);

    // performs right shift of the word
    void rightShift(string& s, int startPos, int oldSubSrtLength, int numOfPosMoveRight);

public:
    MarkovExecutor();

    ~MarkovExecutor();

    // starts execution
    void start();
};

#endif //UNIVPROJECT_MARKOVEXECUTOR_H
