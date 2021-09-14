// Created by Yehor on 19.04.2021.

#ifndef UNIVPROJECT_UTILITYDB_H
#define UNIVPROJECT_UTILITYDB_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;


/**
* Checks if a {@code vector} contains {@code str} value
* @return {@code true} if contains. Otherwise, {@code false}
*/
bool vectorContainsStr(vector<string>& vector, string& check);

/**
 * Check {@code columnNames} vector contains passed param {@columnCheck}
 * @param columnNames - a vector which contains names of columns in the table
 * @param columnCheck - a name which will be checked if {@code columnNames} contains it
 * @param tableName - a name of current table
 * @return {@code true} if contains. Otherwise, false
 */
bool checkIfColumnNameIsCorrect(vector<string>& columnNames, string& columnCheck, string& tableName);

/**
 * @param dirDB - a path where a corresponding table's binary files is stored
 * @param tableName - a name of the current table
 * @return a path where data of DB is stored
 */
string createPathDB(const string& dirDB, string& tableName);

/**
 * @param dirSerialPathMeta - path to file directory where are files which
 *                            contain info of each available table
 * @param tableName - name of the current table
 * @return a path where DB object is serialized
 */
string createSerialPathDB(const string& dirSerialPathMeta, string& tableName);

/**
 * @return {@true} if all the {@code values} corresponds to the types
 *         of {@code this->columnTypes}. Otherwise, {@code false}
 */
bool valuesCorrespondsToTypes(vector<string>& columnTypes, vector<string>& values);

/**
 * Checks if a string {@code str} is a number
 * @return {@code true} if {@code str} is a number. Otherwise, false;
 */
bool stringIsNumber(const string& str);

/**
 * Tries to find an index of {@code columnName}
 * @param columnNames
 * @param columnName
 * @return index which corresponds to {@code columnName} in the table.
 *         If there is no column with such index, then returns -1.
 */
int findIndexOfColumnByName(vector<string>& columnNames, string& columnName);

/**
 * Checks if a string {@code check} starts with a substring {@code another}
 * @return @return {@code true} if {@code check} starts with a substring
 *                 {@code another} is {@code str} is a number. Otherwise, false;
 */
bool strStartsWith(string& check, const string& another);

/**
* Parses a string to a vector of words
* @param s - string which will be parsed
* @return vector of words
*/
vector<string>& parseToWords(string s);

int getNumOfBytesInRowBetween(int from, int to, vector<string>& columnTypes, int bytesInString);

#endif //UNIVPROJECT_UTILITYDB_H