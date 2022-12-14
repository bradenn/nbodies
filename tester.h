/* *
 * @project nbodies
 * @file tester.h
 *
 * @author Braden Nicholson
 * @date 3/28/21, 5/17/22
 */



#ifndef BARNESHUT_TESTER_H
#define BARNESHUT_TESTER_H

#include<vector>
#include<string>
#include<filesystem>
#include "body.h"

namespace fs = std::filesystem;
using std::vector, std::string;

struct Test {
    string path;
    string name;
};

class TestManager {
private:
    vector<Test *> tests;
    bool pathExists{};
    Test *currentTest;
    Test *selected;
    vector<Body *> *bodies;
    void findTests();

public:
    vector<Test *> getTests();

    explicit TestManager(vector<Body *> *bodies);

    void selectTest();
    void nextTest();
    void lastTest();
    Test *getTest();
    Test *getSelected();
    void getBodies();

};


#endif //BARNESHUT_TESTER_H
