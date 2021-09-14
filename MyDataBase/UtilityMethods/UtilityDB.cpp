// Created by Yehor on 19.04.2021.

#include "UtilityDB.h"

bool vectorContainsStr(vector<string>& vector, string& check) {
    return any_of(vector.begin(), vector.end(), [check](string& s) { return s == check; });
}

bool checkIfColumnNameIsCorrect(vector<string>& columnNames, string& columnCheck, string& tableName)
{
    bool containsColumn = vectorContainsStr(columnNames, columnCheck);
    if (!containsColumn) {
        cout << "Error: table \"" << tableName << "\" doesn't contain column \""
             << columnCheck << "\"" << endl;
    }
    return containsColumn;
}

string createPathDB(const string& dirDB, string& tableName) {
    return dirDB + tableName + ".bin";
}

string createSerialPathDB(const string& dirSerialPathMeta, string& tableName) {
    return dirSerialPathMeta + tableName + ".bin";
}

bool valuesCorrespondsToTypes(vector<string>& columnTypes, vector<string>& values) {
    for (int i = 0, size = (int) columnTypes.size(); i < size; ++i) {
        string type = columnTypes[i];
        if (type == "int") {
            for (char ch : values[i])
                if (ch > '9' || ch < '0') return false;
        } else if (type == values[i]) {
            for (char ch : type)
                if (ch != ',' && (ch > '9' || ch < '0')) return false;
        }
    }
    return true;
}

bool stringIsNumber(const string &str) {
    return !str.empty() &&
            all_of(str.begin(), str.end(),[](char ch) { return ch >= '0' && ch <= '9'; });
}

int findIndexOfColumnByName(vector<string> &columnNames, string &columnName) {
    int index = 0;
    for (const string& name : columnNames) {
        if (name == columnName)
            return index;
        else
            index++;
    }
    return -1;
}

bool strStartsWith(string& check, const string &another) {
    size_t firstEntry = check.find(another);
    return firstEntry == 0;
}

vector<string>& parseToWords(string s) {
    vector<string>* words = new vector<string>();
    for (int i = 0, length = (int) s.length(); i < length ; ) {
        if (s[i] != ' ') {
            char* buffer = new char[30];
            char* pointer_buf = buffer;
            do {
                *(pointer_buf++) = s[i++];
            } while (i < length && s[i] != ' ');
            *(pointer_buf) = '\0';
            words->push_back(string(buffer));
        } else i++;
    }
    return *words;
}

int getNumOfBytesInRowBetween(int fromBound, int toBound, vector<string>& columnTypes, int bytesInString) {
    int bytes = 0;
    for (int i = fromBound; i < toBound; ++i) {
        if (columnTypes[i] == "string")
            bytes += bytesInString;
        else if (columnTypes[i] == "int")
            bytes += sizeof(int);
        else // double
            bytes += sizeof(double);
    }
    return bytes;
}
