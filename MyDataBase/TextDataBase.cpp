// Created by Yehor on 26.01.2021.

#include "TextDataBase.h"

/**
 *  Runs a database
 */
void TextDataBase::runDataBase() {
    ifstream reader(AVAILABLE_TABLES_FILE_META, ios_base::binary);
    // an object to work with
    TextDataBase* textDataBaseObj = nullptr;
    while (true) {
        if (textDataBaseObj != nullptr) {
            delete textDataBaseObj;
            textDataBaseObj = nullptr;
        }
        cout << "Enter \"stop\" to terminate.\n\r"
                "Select a table or create a new one: (new / select)." << endl;
        string tableChoice;
        cin >> tableChoice;
        cin.ignore(32767, '\n');

        if (strStartsWith(tableChoice, "new")) {
            // start creating a new table
            textDataBaseObj = &askInitQuestions();
        }
        else if (strStartsWith(tableChoice, "select")) {
            // checks if register contains at least one stored table
            if (!checkAnyTableIsAvailable(reader))
                cout << "There is no available tables. Try to create a new one." << endl;
            else {
                vector<string>& availableTables = printAvailableTables(reader);
                while (true) {
                    cout << "Enter \"leave\" to leave from the selection of table.\n\r"
                            "Select a table:" << endl;
                    string table;
                    getline(cin, table);
                    if (strStartsWith(table, "leave"))
                        break;
                    if (!vectorContainsStr(availableTables, table)) // todo changed
                        cout << "The database doesn't contain table \"" << table << "\"." << endl;
                    else {
                        string serialPathLocal = createSerialPathDB(DIR_SERIAL_PATH_META, table);
                        textDataBaseObj = deserializeTable(serialPathLocal);
                        break;
                    }
                }
                delete &availableTables;
            }
        } else if (strStartsWith(tableChoice, "stop")) {
            break;
        }
        else {
            cout << "Invalid command." << endl;
        }
        // terminate program if this flag is true
        bool fullStop = false;
        // work with the table
        if (textDataBaseObj != nullptr) {
            cout << "Enter \"stop\" to terminate.\n\r"
                    "Enter \"leave\" to select an another table or create a new one. "
                    "Commands: \n\r*) read [start] [end], "
                    "\n\r*) write {value}, {value}, ..."
                    "\n\r*) delete {line_index \\ all},"
                    "\n\r*) set {line_index} {column_name} {new_value},"
                    "\n\r*) readwhere {column_name} {=} {value}:" << endl;
            // runs available commands
            while (true) {
                string command;
                getline(cin, command);

                if (!strStartsWith(command, "stop") &&
                    !strStartsWith(command, "leave"))
                {
                    (*textDataBaseObj).inputHandler(command);
                    cout << "A command was done." << endl;
                } else {
                    // remove deleted lines
                    rewriteFile(*textDataBaseObj, false);
                    // serialize current object
                    (*textDataBaseObj).serializeTable((*textDataBaseObj).serialPathDB);
                    if (strStartsWith(command, "stop"))
                        fullStop = true;
                    break;
                }
            }
        }
        if (fullStop) break;
    }
    reader.close();
    // delete an object
    delete textDataBaseObj;
}

TextDataBase& TextDataBase::askInitQuestions() {
    // creates a table
    string tableName, names, types;

    cout << "Enter table name: " << endl;
    getline(cin, tableName);

    cout << "Enter names of columns:" << endl;
    getline(cin, names);
    auto& columnNamesLocal = parseToWords(names);

    cout << "Enter types of columns (string / int / double) :" << endl;
    getline(cin, types);
    auto& columnTypesLocal = parseToWords(types);

    int numOfCharsInString;
    cout << "Enter a number of characters in the string type:" << endl;
    cin >> numOfCharsInString;
    cin.ignore(32767, '\n');

    // check if sizes are equal
    if (columnNamesLocal.size() != columnTypesLocal.size()) {
        cout << "Error: \"Different number of types and names\"" << endl;
        throw exception();
    }

    // create an object
    TextDataBase* textDataBaseObj = new TextDataBase(
            numOfCharsInString, columnTypesLocal, columnNamesLocal,
            (int) columnNamesLocal.size(), tableName);

    // create a file
    ofstream writer(textDataBaseObj->pathDB, ios_base::binary);
    writer.close();

    // add info about a new table to the registry
    ofstream outputRegister(AVAILABLE_TABLES_FILE_META, ios_base::binary | ios_base::out | ios_base::app);
    strWriting(outputRegister, tableName, TABLE_NAME_LENGTH_META);
    outputRegister.close();

    return *textDataBaseObj;
}

/**
 *  Constructor
 */
TextDataBase::TextDataBase(int stringSize, vector<string>& columnTypes,
                           vector<string>& columnNames, int numColumns, string& tableName) :
        stringSize(stringSize), columnTypes(columnTypes),
        columnNames(columnNames), numColumns(numColumns), tableName(tableName)
{
    // create a path where data of DB will be stored
    this->pathDB = createPathDB(TABLE_DIR, tableName);
    // create a path where db will be serialized
    this->serialPathDB = createSerialPathDB(DIR_SERIAL_PATH_META, tableName);

    this->numRows = 0;
    this->rowSizeBytes = getNumOfBytesInRowBetween(0, (int) this->columnTypes.size(),
                                                   this->columnTypes, this->stringSize);
}

/**
 *  Destructor
 */
TextDataBase::~TextDataBase() {
    delete &this->columnTypes;
    delete &this->columnNames;
    delete this->bitDeletedLines;
}

/* Meta block start */

int TextDataBase::getNumOfAvailableLinesInFile(ifstream& reader, int lineLengthInBytes) {
    ifstream::pos_type begin = reader.tellg();
    reader.seekg(0, ios::end);
    ifstream::pos_type end = reader.tellg();

    // return a cursor to the begin of the file
    reader.seekg(0, ios::beg);
    return (int)(end - begin) / lineLengthInBytes;
}

bool TextDataBase::checkAnyTableIsAvailable(ifstream &reader) {
    return getNumOfAvailableLinesInFile(reader, TABLE_NAME_LENGTH_META) != 0;
}

vector<string>& TextDataBase::printAvailableTables(ifstream &reader) {
    vector<string>* availableTableNames = new vector<string>();

    for (int i = 0, files = getNumOfAvailableLinesInFile(reader, TABLE_NAME_LENGTH_META); i < files; ++i)
    {
        string tableNameLocal = strReading(reader, TABLE_NAME_LENGTH_META);
        // print table name
        cout << i << ") Table: " << tableNameLocal << endl;
        // add values to the vector
        (*availableTableNames).push_back(tableNameLocal);
    }
    // return a cursor to the begin of the file
    reader.seekg(0, ios::beg);
    return *availableTableNames;
}

TextDataBase* TextDataBase::deserializeTable(string& serialPath) {
    vector<string>* colNames = new vector<string>();
    vector<string>* colTypes = new vector<string>();

    // restored object
    TextDataBase* obj = new TextDataBase(*colNames, *colTypes);

    ifstream input(serialPath, ios_base::binary);
    obj->numRows = intReading(input);
    obj->numColumns = intReading(input);
    obj->rowSizeBytes = intReading(input);
    obj->stringSize = intReading(input);
    obj->tableName = strReading(input, TABLE_NAME_LENGTH_META);
    obj->pathDB = createPathDB(TABLE_DIR, obj->tableName);
    obj->serialPathDB = createSerialPathDB(DIR_SERIAL_PATH_META, obj->tableName);
    obj->numOfDelLines = 0;
    deserializeVectorHelper(input, obj->columnTypes, 30);
    deserializeVectorHelper(input, obj->columnNames, 30);
    deserializeVectorHelper((*obj->bitDeletedLines), obj->numRows);
    input.close();
    return obj;
}

void TextDataBase::deserializeVectorHelper(ifstream& input, vector<string>& vector, int oneDataBlockSize)
{
    int dataLength = intReading(input);
    for (int i = 0; i < dataLength; ++i) {
        string s = strReading(input, oneDataBlockSize);
        vector.push_back(s);
    }
}

void TextDataBase::deserializeVectorHelper(vector<bool>& vector, int numOfRows) {
    vector.resize(numOfRows, true);
}

void TextDataBase::serializeTable(string &serialPath) {
    ofstream output(serialPath, ios_base::binary | ios_base::trunc);
    // write data
    intWriting(output, this->numRows);
    intWriting(output, this->numColumns);
    intWriting(output, this->rowSizeBytes);
    intWriting(output, this->stringSize);
    strWriting(output, tableName, TABLE_NAME_LENGTH_META);
    serializeVectorHelper(output, this->columnTypes, 30);
    serializeVectorHelper(output, this->columnNames, 30);
    output.close();
}

void TextDataBase::serializeVectorHelper(ofstream &output, vector<string>& vector, int oneDataBlockSize)
{
    int dataLength = (int) vector.size();
    intWriting(output, dataLength);
    for (int i = 0; i < dataLength; ++i) {
        strWriting(output, vector[i], oneDataBlockSize);
    }
}
/* Meta block end */

/**
 *  Input Handler
 */
void TextDataBase::inputHandler(string& commandStr) {
    // separated words (commands)
    vector<string>& command = parseToWords(commandStr);
    if (command[0] == "readwhere")
        // readwhere {column_name} {=} {value}
        readWhereOperator(command);
    else if (command[0] == "read")
        // read [begin] [end]
        readOperator(command);
    else if (command[0] == "write")
        // write {value}, {value}, ...
        writeOperator(command);
    else if (command[0] == "delete")
        // delete {all / index}
        deleteOperator(command);
    else if (command[0] == "set")
        // set {lineIndex} {column_name} {new_value}
        setOperator(command);
    else if (command[0] == "sort")
        // sort {column_name} {asc / desc}
        sortOperator(command);
    else
        cout << "Error: \"Illegal command.\"" << endl;
    delete &command;
}

void TextDataBase::readOperator(vector<string> &command) {
    int readFrom = 0;
    int readEnd = numRows - numOfDelLines;
    if (command.size() > 1) {
        // check the string correctness
        if (command.size() >= 2 && stringIsNumber(command[1]))
            readFrom = stoi(command[1]);
        if (command.size() >= 3 && stringIsNumber(command[2]))
            readEnd = stoi(command[2]);
    }
    // check correctness of order
    if (readEnd < readFrom) {
        cout << "Error: \"Read from\" line have to be less "
                "than \"read to\" line" << endl;
        return;
    }
    // check correctness of bounds
    if (readFrom < 0 || readEnd > numRows - numOfDelLines) {
        cout << "Error: \"Illegal bounds of reading.\"" << endl;
        return;
    }
    // call read method
    read(readFrom, readEnd, false, nullptr);
}

void TextDataBase::writeOperator(vector<string> &command) {
    if (command.size() > 1) {
        // remove first word == "write"
        command.erase(command.cbegin());
        // check if number of words in the vector == number of columns
        if (command.size() != numColumns) {
            cout << "Error: \"Inappropriate number of words\"" << endl;
            return;
        }
        // perform deletion of deleted lines
        rewriteFile(*this, false);
        // call write method
        write(command);
    }
}

void TextDataBase::deleteOperator(vector<string> &command) {
    if (command.size() > 1) {
        if (stringIsNumber(command[1])) {
            int index = stoi(command[1]);
            if (index >= this->numRows - this->numOfDelLines)
                cout << "Error: deleted index is out of bounds" << endl;
            else
                deleteLine(index); // change number of deleted lines
        } else if (command[1] == "all") {
            rewriteFile(*this, true);
        } else {
            cout << "Error: unexpected word \"" << command[1] << "\"" << endl;
        }
    } else
        cout << "Error: the second param must be a number or \"all\"" << endl;
}

void TextDataBase::readWhereOperator(vector<string> &command) {
    // check command correctness
    if (command.size() < 4) {
        cout << "Error: invalid command" << endl;
        return;
    }
    if (command[2] != "=") {
        cout << "Error: \"" << command[2] << "\" is invalid filter sign" << endl;
        return;
    }
    // find index of columnName
    int columnIndex = findIndexOfColumnByName(this->columnNames, command[1]);

    // check if a columnName is correct
    if (columnIndex == -1) {
        cout << "Error: table \"" << this->tableName <<
             "\" doesn't contain column \"" << command[1] << "\"" << endl;
        return;
    }
    // input value as a filter
    string filterValue = command[3];
    // a type of current column
    Types reqType;
    // set a correct filter function
    if (columnTypes[columnIndex] == "string")
        reqType = STRING;
    else if (columnTypes[columnIndex] == "int")
        reqType = INT;
    else
        reqType = DOUBLE;
    // perform reading with a filter
    read(0, numRows, true,
         [&columnIndex, &filterValue, &reqType](int& curColumn, string& curValue, Types type) {
            // if true:  print
            // if false: don't print
            return !(type == reqType && curColumn == columnIndex) || (curValue == filterValue);
         });
}

void TextDataBase::setOperator(vector<string> &command) {
    // set [lineIndex] [column_name] [new_value]
    if (command.size() == 4) {
        int lineIndex;
        // check if the first argument is a number
        if (stringIsNumber(command[1])) {
            lineIndex = stoi(command[1]);
        } else {
            cout << "Error: first argument must be a number" << endl;
            return;
        }
        // check if line index is inbounds
        if (lineIndex >= numRows) {
            cout << "Error: line index is out of bounds" << endl;
            return;
        }
        // check if a columnName is correct
        bool containsColumn = checkIfColumnNameIsCorrect(this->columnNames, command[2],
                                                         this->tableName);
        if (!containsColumn) return;

        // remove deleted lines
        rewriteFile(*this, false);

        // get index of column
        int columnIndex = findIndexOfColumnByName(this->columnNames, command[2]);

        // check if a columnName is correct
        if (columnIndex == -1) {
            cout << "Error: table \"" << this->tableName <<
                 "\" doesn't contain column \"" << command[2] << "\"" << endl;
            return;
        }

        // number of bytes to skip
        int skipBytes = lineIndex * this->rowSizeBytes;
        skipBytes += getNumOfBytesInRowBetween(0, columnIndex, this->columnTypes, this->stringSize);

        FILE* file = fopen(this->pathDB.c_str(), "r+b");
        try {
            // set the new value
            if (this->columnTypes[columnIndex] == "int")
                setInt(skipBytes, stoi(command[3]), file);
            else if (this->columnTypes[columnIndex] == "string")
                setStr(skipBytes, command[3], file, this->stringSize);
            else
                setDouble(skipBytes, stod(command[3]), file);
        } catch (exception& e) {
            cout << "Error: type of new value is inappropriate" << endl;
        }
        fclose(file);
    } else
        cout << "Error: inappropriate number of params" << endl;
}

void TextDataBase::sortOperator(vector<string>& command) {
    // sort {column_name} {asc / desc}
    if (command.size() == 3) {
        int columnIndex = findIndexOfColumnByName(this->columnNames, command[1]);
        // check if a columnName is correct
        if (columnIndex == -1) {
            cout << "Error: table \"" << this->tableName <<
                 "\" doesn't contain column \"" << command[1] << "\"" << endl;
            return;
        }
        if (command[2] != "asc" && command[2] != "desc") {
            cout << "Error: \"" << command[2] <<
                 R"(" is inappropriate value. Should be "asc" or "desc".)" << endl;
        }
//        int occupiedFiles = sortFileAndStoreInDifFiles(columnIndex, command[2] == "asc");
    } else
        cout << "Error: inappropriate number of params" << endl;
}

//int TextDataBase::sortFileAndStoreInDifFiles(int sortIndexColumn, bool asc) {
//    // binary reader
//    ifstream input(pathDB, ios_base::binary);
//    int rowBytesSkip = getNumOfBytesInRowBetween(
//            0, sortIndexColumn, this->columnTypes, this->stringSize);
//
//}
//
//int TextDataBase::sortFileIntHelper(ifstream& input, int rowBytesSkip, bool asc) {
//    MyRedBlackTree<pair<int, string>> tree([](pair<int, string>& p1, pair<int, string>& p2) {
//        return (p1.first < p2.first) ? -1 : (p1.first > p2.first) ? 1 : 0;
//    });
//    int linesRead = 0;
//    while (linesRead < this->numRows) {
//        input.seekg(rowBytesSkip, ios_base::cur)
//        pair<int, string> pairAdd = {intReading(input), }
//        linesRead++;
//
//        if (linesRead % MAX_SORT_ROWS == 0)
//            continue;
//    }
//}
//
//int TextDataBase::sortFileDoubleHelper(ifstream& input, int rowBytesSkip, bool asc) {
//    MyRedBlackTree<pair<double, string>> tree([](pair<double, string>& p1, pair<double, string>& p2)
//    {
//        return (p1.first < p2.first) ? -1 : (p1.first > p2.first) ? 1 : 0;
//    });
//    int linesRead = 0;
//    while (linesRead < this->numRows) {
//
//        linesRead++;
//
//        if (linesRead % MAX_SORT_ROWS == 0)
//            continue;
//    }
//}
//
//int TextDataBase::sortFileStrHelper(ifstream& input, int rowBytesSkip, bool asc) {
//    MyRedBlackTree<pair<string, string>> tree([](pair<string, string>& p1, pair<string, string>& p2) {
//        return p1.first.compare(p2.first);
//    });
//    int linesRead = 0;
//    while (linesRead < this->numRows) {
//
//        linesRead++;
//
//        if (linesRead % MAX_SORT_ROWS == 0)
//            continue;
//    }
//}

/**
 *  Reader
 */
void TextDataBase::read(int startLine, int endLine, bool withFilter,
                        const function<bool(int& curColumn, string& curValue, Types type)>& filter)
{
    // binary reader
    ifstream input(pathDB, ios_base::binary);

    int startSkip = rowSizeBytes * startLine;
    int availLines = getNumOfAvailableLinesInFile(input, this->rowSizeBytes);
    input.seekg(startSkip);

    // print column names
    printTableColumnNames();

    for (int i = startLine, printCounter = 0; i < endLine && i < availLines; ++i) {
        if ((*this->bitDeletedLines)[i]) {
            // index of column
            int columnIndex = 0;
            // line which will be printed on screen
            string resultLine;
            // a flag that indicates if a line can be printed
            bool canPrintLine = true;
            // number of bytes that has been already read
            int hasAlreadyRead = 0;

            while (columnIndex < numColumns) {
                // a line that may be append if {@code filter} result if true
                string curValue;
                // type of current column
                Types type;
                // choose a correct type
                if (columnTypes[columnIndex] == "int") {
                    curValue = to_string(intReading(input));
                    type = INT;
                    hasAlreadyRead += 4; // sizeof(int)
                }
                else if (columnTypes[columnIndex] == "double") {
                    curValue = to_string(doubleReading(input));
                    type = DOUBLE;
                    hasAlreadyRead += 8; // sizeof(double)
                }
                else { // type == "string"
                    curValue = strReading(input, stringSize);
                    type = STRING;
                    hasAlreadyRead += this->stringSize;
                }
                // check if this line can be printed
                if (!withFilter || filter(columnIndex, curValue, type)) {
                    resultLine.append(curValue).append("  ");
                } else {
                    canPrintLine = false;
                    input.seekg(this->rowSizeBytes - hasAlreadyRead, ios_base::cur);
                    break;
                }
                columnIndex++;
            }
            if (canPrintLine)
                cout << (printCounter++) << ") " << resultLine << endl;
        } else {
            // skip one row
            input.seekg(this->rowSizeBytes, ios_base::cur);
            endLine++;
        }
    }
    input.close();
}

// Reading helper methods //
int TextDataBase::intReading(ifstream &input) {
    int readI = 0;
    input.read((char*)&readI, sizeof(readI));
    return readI;
}

double TextDataBase::doubleReading(ifstream &input) {
    double readD = 0;
    input.read((char*) &readD, sizeof(readD));
    return readD;
}

string TextDataBase::strReading(ifstream &input, int strSize) {
    char* readStr = new char[strSize + 1];
    input.read(readStr, strSize);
    readStr[strSize] = '\0';
    return string(readStr);
}

/**
 *  Writer
 */
void TextDataBase::write(vector<string> &words) {
    if (valuesCorrespondsToTypes(this->columnTypes, words)) {
        // binary writer
        ofstream output(pathDB, ios_base::binary | ios_base::out | ios_base::app);

        for (int i = 0, size = (int) words.size(); i < size; ++i) {
            // current data type
            string curType = columnTypes[i];

            if (curType == "int")
                intWriting(output, stoi(words[i]));
            else if (curType == "double")
                doubleWriting(output, stod(words[i]));
            else // curType == "string"
                strWriting(output, words[i], stringSize);
        }
        this->bitDeletedLines->push_back(true);
        this->numRows++;
        output.close();
    } else
        cout << "Error: invalid parameter types" << endl;
}

// Writing helper methods //
void TextDataBase::intWriting(ofstream &output, int value) {
    output.write((const char*)&value, sizeof(int));
}

void TextDataBase::doubleWriting(ofstream &output, double value) {
    output.write((const char*)&value, sizeof(double));
}

void TextDataBase::strWriting(ofstream &output, string &value, int strSize) {
    output.write(value.c_str(), strSize);
}

void TextDataBase::deleteLine(int lineIndex) {
    // compensate for displacement
    int realIndex = getRealLineIndex(*this->bitDeletedLines, lineIndex);
    // set as deleted
    (*this->bitDeletedLines)[realIndex] = false;
    this->numOfDelLines++;
}

void TextDataBase::setInt(int skipBytes, int value, FILE* file) {
    // binary writer
    fseek(file, skipBytes, SEEK_SET);
    fwrite(&value, sizeof(int), 1, file);
}

void TextDataBase::setDouble(int skipBytes, double value, FILE* file) {
    // binary writer
    fseek(file, skipBytes, SEEK_SET);
    fwrite(&value, sizeof(double ), 1, file);
}

void TextDataBase::setStr(int skipBytes, string &value, FILE* file, int stringSizeInTable) {
    // binary writer
    fseek(file, skipBytes, SEEK_SET);
    fwrite(value.c_str(), stringSizeInTable, 1, file);
}

void TextDataBase::printTableColumnNames() {
    cout << "-------------------------------------------" << endl;
    for (const string& s : this->columnNames)
        cout << "  |  " << s;
    cout << "\n\r-------------------------------------------" << endl;
}

void TextDataBase::rewriteFile(TextDataBase& obj, bool deleteContent) {
    if (deleteContent && obj.numRows > 0) {
        // delete the DB file
        remove(obj.pathDB.c_str());
        // create a new DB file
        ofstream output(obj.pathDB, ios_base::binary);
        output.close();
        // change field values
        obj.numRows = 0;
        obj.bitDeletedLines->clear();
        obj.numOfDelLines = 0;
    }
    else if (obj.numOfDelLines > 0) {
        srand(time(nullptr));
        // a random number for temporary file
        // a random number for temporary file
        int tempNum = rand();
        // current file
        ifstream input(obj.pathDB, ios_base::binary);
        // new file writer
        string tempFileName = TABLE_DIR + obj.tableName + to_string(tempNum) + ".bin";
        ofstream output(tempFileName, ios::binary);
        // a number of lines in the file
        for (int i = 0; i < obj.numRows; ++i)
        {
            if ((*obj.bitDeletedLines)[i]) {
                unsigned char* line = new unsigned char[obj.rowSizeBytes];
                input.read(reinterpret_cast<char*>(line), obj.rowSizeBytes);
                output.write(reinterpret_cast<const char*>(line), obj.rowSizeBytes);
                delete[] line;
            } else {
                // skip bytes
                input.seekg(obj.rowSizeBytes,  ios_base::cur); // SEEK_CUR
                (*obj.bitDeletedLines)[i] = true;
            }
        }
        // close streams
        input.close();
        output.close();

        // delete current file
        remove(obj.pathDB.c_str());
        // rename the temporary file to the original name
        rename(tempFileName.c_str(), obj.pathDB.c_str());

        // change filed values
        obj.numRows = obj.numRows - obj.numOfDelLines;
        obj.bitDeletedLines->resize(obj.numRows);
        obj.numOfDelLines = 0;
    }
}

int TextDataBase::getRealLineIndex(vector<bool>& bits, int notRealIndex) {
    // a real index
    int index = 0;
    // a special case
    if (notRealIndex == 0 && !bits[0]) {
        while (!bits[index]) index++;
        return index;
    }
    while (index <= notRealIndex) {
        if (!bits[index]) notRealIndex++;
        index++;
    }
    return index - 1;
}