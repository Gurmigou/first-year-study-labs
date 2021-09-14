// Created by Yehor on 26.01.2021.

#ifndef UNIVPROJECT_TEXTDATABASE_H
#define UNIVPROJECT_TEXTDATABASE_H

#include <ctime>
#include <functional>
#include "UtilityMethods/UtilityDB.h"
#include "Constants/Constants.h"
#include "C:\Users\Yehor\CLionProjects\UnivProject\Lab6_RedBlackTree\MyRedBlackTree.h"

using namespace std;

class TextDataBase {
private:
    enum Types {
        STRING,
        INT,
        DOUBLE
    };

    int numRows;
    int numColumns;
    int rowSizeBytes; // size of row in bytes
    int stringSize;   // number of chars (bytes) used in each string

    // the name of the table
    string tableName;
    // path to the table
    string pathDB;
    // path to the file of serialization
    string serialPathDB;

    // may contain: string, int, double types
    vector<string>& columnTypes; // each element 10 bytes
    vector<string>& columnNames; // each element 30 bytes

    // a sequence of 0 and 1; 0 means that line was deleted; 1 mean that line is present
    vector<bool>* bitDeletedLines = new vector<bool>();
    int numOfDelLines = 0;

    /**
     *  Processes incoming commands.
     *  Available commands:
     *      - readD
     *      - write
     */
    void inputHandler(string& command);

    /**
     * Reads and prints rows of database from
     * {@code start} inclusively to {@code end} exclusively.
     * @param start - the line to start reading
     * @param end - the line to end reading
     */
    void read(int startLine, int endLine, bool withFilter,
              const function<bool(int& curColumn, string& curValue, Types type)>& lambda);

    /**
     * Regulates all aspects of "read" command
     * @param command - a vector which contains user's commands
     */
    void readOperator(vector<string>& command);

    /**
     * Regulates all aspects of "write" command
     * @param command - a vector which contains user's commands
     */
    void writeOperator(vector<string>& command);

    /**
     * Regulates all aspects of "delete" command
     * @param command - a vector which contains user's commands
     */
    void deleteOperator(vector<string>& command);

    /**
     * Regulates all aspects of "readwhere" command
     * @param command - a vector which contains user's commands
     */
    void readWhereOperator(vector<string>& command);

    /**
     * Regulates all aspects of "set" command
     * @param command - a vector which contains user's commands
     */
    void setOperator(vector<string>& command);

    /**
     * Regulates all aspects of "sort" command
     * @param command - a vector which contains user's commands
     */
    void sortOperator(vector<string>& command);

    int sortFileAndStoreInDifFiles(int sortIndex, bool asc);

    int sortFileIntHelper(ifstream& input, int rowBytesSkip, bool asc);
    int sortFileDoubleHelper(ifstream& input, int rowBytesSkip, bool asc);
    int sortFileStrHelper(ifstream& input, int rowBytesSkip, bool asc);

    /**
     * Writes the input words into the database
     * @param words - words to write
     */
    void write(vector<string>& words);

    /**
     * Changes some parameters in order to perform a correct deletion in the future.
     * Actually, deletion performs {@code rewriteFile()} method
     * @param lineIndex - index of line that will be deleted in the future
     */
    void deleteLine(int lineIndex);

    /**
     * Sets a value of types: {@code int}, {@code double}, {@code string}
     * @param skipBytes - where to set a new value
     * @param value - value to be set
     */
    static void setInt(int skipBytes, int value, FILE* file);
    static void setDouble(int skipBytes, double value, FILE* file);
    static void setStr(int skipBytes, string& value, FILE* file, int stringSizeInTable);

    // Helper methods for reading operation //
    static int intReading(ifstream& input);
    static double doubleReading(ifstream& input);
    static string strReading(ifstream& input, int strSize);

    // Helper methods for writing operation //
    static void intWriting(ofstream& output, int value);
    static void doubleWriting(ofstream& output, double value);
    static void strWriting(ofstream& output, string& value, int strSize);

    /**
     * Prints names of columns of the current table
     */
    void printTableColumnNames();

    /**
     * Creates a new database table
     * @return {@code TextDataBase} object
     */
    static TextDataBase& askInitQuestions();

    /* Meta methods */
    /**
     * Prints available tables. In other words, it prints
     * names of tables which have been already created
     * @param reader - an input stream
     * @return {@code vector} which contains file names
     */
    static vector<string>& printAvailableTables(ifstream& reader);

    /**
     * @param reader - an input stream
     * @return {@code true} is there is at least one table available. Otherwise {@code false}
     */
    static bool checkAnyTableIsAvailable(ifstream& reader);

    /**
     * @param reader - an input object (corresponds to a particular file)
     * @return a number of lines which are stored in the file which is associated with a {@code reader}
     */
    static int getNumOfAvailableLinesInFile(ifstream& reader, int lineLengthInBytes);

    /**
     *  Serialization:
     *  Serializes current object into the file {@code serialPathDB}
     *
     *  Instruction:
     *  1) save [numRows]                4 bytes
     *  2) save [numColumns]             4 bytes
     *  3) save [rowSizeBytes]           4 bytes
     *  4) save [stringSize]             4 bytes
     *  5) save [tableName]              30 bytes
     *  6) save [numOfDelLines]          4 bytes
     *
     *  *) save columnTypes.size()       integer
     *  7) save [columnTypes]            7 bytes  * elements.size()
     *
     *  *) save columnNames.size()       integer
     *  8) save [columnNames]            30 bytes * elements.size()
     *
     *  *) save bitDeletedLines.size()   integer
     *  9) save [bitDeletedLines]        1 byte * elements.size()
     *
     * @param serialPath - a path where data will be stored
     */
    void serializeTable(string& serialPath);

    // This method helps to serialize vectors
    static void serializeVectorHelper(ofstream& output, vector<string>& vector,
                                        int oneDataBlockSize);

    /**
     * Creates an object of data base using existing data
     * @param serialPath - a path where serial data of the current table is stored
     * @return {@code TextDataBase} object
     */
    static TextDataBase* deserializeTable(string& serialPath);

    // This method helps to deserialize vectors
    static void deserializeVectorHelper(ifstream& input, vector<string>& vector,
                                        int oneDataBlockSize);
    static void deserializeVectorHelper(vector<bool>& vector, int numOfRows);

    /**
     * Rewrite file according to the given conditions.
     * @param deleteContent - if is {@code true}, the file will be rewrite
     *                        as an empty file.
     */
    static void rewriteFile(TextDataBase& obj, bool deleteContent);

    static int getRealLineIndex(vector<bool>& bits, int notRealIndex);

    // destructor
    ~TextDataBase();

    // constructor
    TextDataBase(int stringSize, vector<string>& columnTypes,
                 vector<string>& columnNames, int numColumns,  string& dbFileName);

    // default constructor
    TextDataBase(vector<string>& columnNames,
                 vector<string>& columnTypes)
                 : columnNames(columnNames), columnTypes(columnTypes) {}
public:
    /**
     *  Runs a database
     */
    static void runDataBase();
};

#endif //UNIVPROJECT_TEXTDATABASE_H