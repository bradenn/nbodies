/* *
 * @project nbodies
 * @file tester.cpp
 *
 * @author Braden Nicholson
 * @date 3/28/21, 5/17/22
 */

#include <string>
#include <fstream>
#include <dirent.h>
#include "tester.h"
#include "body.h"

namespace fs = std::filesystem;


TestManager::TestManager(vector<Body *> *bodies) : bodies(bodies) {
    findTests();
    if (pathExists) {
        selected = tests.front();
        currentTest = tests.front();
        getBodies();
    }
}

void TestManager::findTests() {

    struct dirent *entry;
    DIR *dir = opendir("./tests");

    if (dir == nullptr) {
        pathExists = false;
        return;
    }

    while ((entry = readdir(dir)) != nullptr) {
        string name = entry->d_name;
        name.length();
        if (name.rfind(".in") != std::string::npos) {
            tests.push_back(new Test{"./tests/" + string(entry->d_name), entry->d_name});
        }
    }

    struct {
        bool operator()(Test *a, Test *b) const { return a->name < b->name; }
    } sortName;

    std::sort(tests.begin(), tests.end(), sortName);

    pathExists = true;

    closedir(dir);

}

void TestManager::getBodies() {
    for (const auto &item: *bodies) {
        free(item);
    }
    bodies->clear();
    std::ifstream file(currentTest->path);
    if (file.is_open()) {
        while (!file.eof()) {
            float px = 0, py = 0, pz = 0, vx = 0, vy = 0, vz = 0, mass = 0;
            file >> px;
            file >> py;
            file >> pz;
            file >> vx;
            file >> vy;
            file >> vz;
            file >> mass;
            Body *b = new Body(px, py, pz, vx, vy, vz, 0, 0, 0, mass);
            bodies->push_back(b);
        }
    }
}

vector<Test *> TestManager::getTests() {
    return tests;
}

void TestManager::selectTest() {
    currentTest = selected;
    getBodies();
}

Test *TestManager::getTest() {
    return currentTest;
}

void TestManager::nextTest() {
    if (selected == tests.back()) {
        selected = tests.front();
        return;
    }
    auto it = std::find(tests.begin(), tests.end(), selected);
    auto nx = std::next(it, 1);
    selected = *nx;
}

Test *TestManager::getSelected() {
    return selected;
}

void TestManager::lastTest() {
    if (selected == tests.front()) {
        selected = tests.back();
        return;
    }
    auto it = std::find(tests.begin(), tests.end(), selected);
    auto nx = std::next(it, -1);
    selected = *nx;
}
