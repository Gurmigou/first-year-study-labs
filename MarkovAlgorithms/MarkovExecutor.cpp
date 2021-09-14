// Created by Yehor on 11.02.2021.

#include "MarkovExecutor.h"

MarkovExecutor::MarkovRule::MarkovRule(string oldValue, string newValue, bool isTerminatedRule) {
    this->oldValue = oldValue;
    this->newValue = newValue;
    this->isTerminatedRule = isTerminatedRule;
}
string& MarkovExecutor::MarkovRule::getOldValue() {
    return oldValue;
}
string& MarkovExecutor::MarkovRule::getNewValue() {
    return newValue;
}
bool MarkovExecutor::MarkovRule::end() const {
    return isTerminatedRule;
}

int MarkovExecutor::getPosOfFirstSubstr(char* s1, char* s2) {
    const char* start = s1;
    while (*s1) {
        if (*s1 == *s2) {
            char* p_s1 = s1;
            char* p_s2 = s2;
            while (*p_s1 && *p_s2 && *p_s1 == *p_s2) {
                p_s1++;
                p_s2++;
            }
            if (*p_s2 == '\0')
                return s1 - start;
        }
        s1++;
    }
    return -1;
}

MarkovExecutor::MarkovExecutor() {
    this->rules = new MyLinkedList<MarkovRule*>();
    this->records = new vector<string>();
}

MarkovExecutor::~MarkovExecutor() {
    delete records;
    for (int i = 0, size = rules->getSize(); i < size; ++i) {
        MarkovRule* removed = rules->removeFirst();
        delete removed;
    }
    delete currentWord;
}

bool MarkovExecutor::applyRule(string& curWord, MarkovRule& rule) {
    if (rule.getOldValue() == "") {
        rightShift(curWord, 0, 0, rule.getNewValue().length());
        for (int i = 0; i < rule.getNewValue().length(); ++i)
            curWord[i] = rule.getNewValue()[i];
        recordCurWordState(*this->currentWord);
        return false;
    }
    int startPos = getPosOfFirstSubstr(curWord.data(), rule.getOldValue().data());
    if (startPos == -1)
        return false;
    else {
        // the length of a new substring
        int ruleNewStrLength = rule.getNewValue().length();
        int ruleOldStrLength = rule.getOldValue().length();

        // possible cases
        if (ruleNewStrLength >= ruleOldStrLength)
            rightShift(curWord, startPos,
                       ruleOldStrLength, ruleNewStrLength - ruleOldStrLength);
        else
            leftShift(curWord, startPos,
                       ruleOldStrLength, ruleOldStrLength - ruleNewStrLength);
        for (int i = startPos, j = 0, charsChange =
                rule.getNewValue().length(); j < charsChange; ++i, ++j) {
            curWord[i] = rule.getNewValue()[j];
        }
        // check termination
        return true;
    }
}

void MarkovExecutor::rightShift(string& s, int startPos, int oldSubSrtLength, int numOfPosMoveRight) {
    int terminateShift = startPos + oldSubSrtLength;
    int left = s.length() - 1;
    for (int i = 0; i < numOfPosMoveRight; ++i) s.append(" ");
    int right = s.length() - 1;
    while (left >= terminateShift)
        s[right--] = s[left--];
}

void MarkovExecutor::leftShift(string& s, int startPos, int oldSubSrtLength, int numOfPosMoveLeft) {
    int right = startPos + oldSubSrtLength;
    int left = right - numOfPosMoveLeft;
    while (right < s.length())
        s[left++] = s[right++];
    s.resize(s.length() - numOfPosMoveLeft);
}

void MarkovExecutor::performMarkovRules() {
    MyLinkedList<MarkovRule*>::Node* head = this->rules->getHead();
    MyLinkedList<MarkovRule*>::Node* cur = head;
    recordCurWordState(*this->currentWord);
    while (true) {
        if (applyRule(*this->currentWord, *cur->getValue())) {
            cur = head;
            recordCurWordState(*this->currentWord);
        } else
            cur = cur->getNext();
        // check termination
        if (cur == nullptr || cur->getValue()->end())
            break;
    }
}

void MarkovExecutor::recordCurWordState(string curWord) {
    this->records->push_back(curWord);
}

void MarkovExecutor::parseToRule() {
    cout << printRules << endl;

    bool looping = true;
    while (looping) {
        cout << cases << endl;
        char selected;
        cin >> selected;
        cin.ignore(32767, '\n');

        string oldPart, newPart;
        // if an invalid symbol was input
        bool error = false;
        switch (selected) {
            case '1':
            case '2':
                cout << "enter a left part of the rule:";
                getline(cin, oldPart);
                cout << "enter a right part of the rule:";
                getline(cin, newPart);
                break;
            case '3':
                looping = false;
                break;
            default:
                cout << "Invalid operation." << endl;
                error = true;
        }
        if (!error && selected <= '2') {
            MarkovRule* rule = new MarkovRule(oldPart, newPart, selected == '2');
            this->rules->addLast(rule);
        }
    }
}

void MarkovExecutor::start() {
    // get Markov's rules entered by user
    parseToRule();

    // get initial word
    cout << "Enter initial word:";
    string* init = new string();
    getline(cin, *init);

    // initialize string parameters
    this->currentWord = init;

    // execute Markov's rules
    performMarkovRules();
    // result
    for (int i = 0; i < this->records->size(); ++i)
        cout << (i+1) << ") " << this->records->at(i) << endl;
}