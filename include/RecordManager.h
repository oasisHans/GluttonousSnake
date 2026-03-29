#pragma once
#include "Common.h"
#include "GameSettings.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class RecordManager
{
private:
    std::vector<std::vector<int>> Allrecords;
    std::vector<std::vector<int>> TopThree;

public:
    RecordManager();
    ~RecordManager();

    bool ifFileEmpty;
    int last_Goat;
    void loadRecord();
    void saveRecord(const GameSettings &settings, const int &score);
    void sortRecord();
    void draw() const;
    int getLastGoat() const;
    std::string speedtoSPEED(int speednum) const;
    std::string obstacletoOBSTACLE(int obsnum) const;
};