//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "concurrency-mt-unsafe"
//#include "Lab7_Final/BucketGame/MainWindow.h"
//#include "Lab7_Final/ResultsStorage/SimpleResultBD.h"
//
//using namespace std;
//
//const pair<int, int> light{100, 18};
//const pair<int, int> middle{90, 16};
//const pair<int, int> hard{75, 16};
//
//int gameRunner(pair<int, int> difficulty) {
//    MainWindow mainWindow{};
//    mainWindow.fillWindowBoundaries();
//
//    Point p1 {1, 3};
//    Point p2 {1, 6};
//    int length = 22;
//
//    vector<Point> eggsSpawnPointsVector = mainWindow.fillWindowPipes(
//            p1, length + 8,p2, length);
//
//    Bucket bucket{};
//    bucket.initPosition(MainWindow::getWidth());
//    mainWindow.addBucketToWindow(bucket);
//    MainWindow::setCursorVisible(false);
//
//    MyLinkedList<Point> eggPointsList;
//
//    int eggMover = 0;
//    while (true) {
//        if (eggMover % difficulty.first == 0) {
//            mainWindow.spawnEggs(eggsSpawnPointsVector, eggPointsList);
//        }
//        if (eggMover == 10000) {
//            eggMover = 0;
//        }
//        if (eggMover % difficulty.second == 0) {
//            mainWindow.moveDownEggs(eggPointsList);
//            bool gameOver = mainWindow.isGameOverOrRemoveEgg(bucket, eggPointsList);
//            if (gameOver) break;
//        }
//        mainWindow.performBucketMove(bucket);
//        mainWindow.updateDisplay();
//        eggMover++;
//    }
//    return mainWindow.getStatistics().getEggsCaught();
//}
//
//pair<int, int> difficultySelection() {
//    cout << "Choose the difficulty level: " << endl;
//    cout << "(1) Light" << endl;
//    cout << "(2) Middle" << endl;
//    cout << "(3) Hard" << endl;
//    int level;
//    cin >> level;
//    if (level == 1)
//        return light;
//    if (level == 2)
//        return middle;
//    if (level == 3)
//        return hard;
//    else
//        return middle;
//}
//
//int play() {
//    pair<int, int> level = difficultySelection();
//    return gameRunner(level);
//}
//
//int main() {
//
//    Statistics s{};
//
//    while (true) {
//        system("cls");
//
//        cout << "(1) Play" << endl;
//        cout << "(2) Select score this game" << endl;
//        cout << "(3) Select score of previous games" << endl;
//        cout << "(4) Exit" << endl;
//
//        int choice;
//        cin >> choice;
//
//        SimpleResultBD resultBd;
//
//        if (choice == 1) {
//            s.clearStatistics();
//            s.addCaughtEgg(play());
//            SimpleResultBD::write(s);
//        }
//        else if (choice == 2) {
//            cout << "Your score: " << s.getEggsCaught() << endl;
//            system("pause");
//        }
//        else if (choice == 3) {
//            resultBd.read();
//            while (true) {
//                cout << "Sort: " << endl;
//                cout << "(1) By asc" << endl;
//                cout << "(2) By desc" << endl;
//                cout << "(3) Return to the main menu" << endl;
//                int choiceLocal;
//                cin >> choiceLocal;
//                if (choiceLocal == 1)
//                    resultBd.printSorted(true);
//                else if (choiceLocal == 2)
//                    resultBd.printSorted(false);
//                else if (choiceLocal == 3)
//                    break;
//                else {
//                    cout << "Invalid command, please try again" << endl;
//                    system("pause");
//                }
//            }
//        }
//        else if (choice == 4)
//            break;
//        else {
//            cout << "Invalid command, please try again" << endl;
//            system("pause");
//        }
//    }
//
//    return 0;
//}
//
//#pragma clang diagnostic pop