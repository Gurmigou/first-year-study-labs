// Created by Yehor on 19.04.2021.

#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-raw-string-literal"
#pragma ide diagnostic ignored "cert-err58-cpp"

#ifndef UNIVPROJECT_CONSTANTS_H
#define UNIVPROJECT_CONSTANTS_H

#include <string>

using namespace std;

/**
 * File contains names of existing tables
 */
static const string AVAILABLE_TABLES_FILE_META =
        "C:\\Users\\Yehor\\CLionProjects\\UnivProject\\"
        "MyDataBase\\AvailableTables\\NameIdAllTables.bin";

/**
 * Path to file directory where are files which contain info of each available table
 * To get an access to the serial file should concat this path and table id;
 * for ex. SERIAL_PATH = DIR_PATH + ID_PATH
 */
static const string DIR_SERIAL_PATH_META =
        "C:\\Users\\Yehor\\CLionProjects\\UnivProject\\MyDataBase\\SerializedTables\\";

/**
 * A directory where binary files which correspond to tables are stored
 */
static const string TABLE_DIR = "C:\\Users\\Yehor\\CLionProjects\\"
                                   "UnivProject\\MyDataBase\\DB_Tables\\";

/**
 * A directory where temporary file are stored during sorting
 */
static const string SORT_TEMP_DIR = "C:\\Users\\Yehor\\CLionProjects\\UnivProject\\MyDataBase\\SortTemp\\";


// Parameters of stored files
static const int TABLE_NAME_LENGTH_META = 30; // bytes
static const int MAX_SORT_ROWS = 1024; // a number of rows

#endif //UNIVPROJECT_CONSTANTS_H

#pragma clang diagnostic pop