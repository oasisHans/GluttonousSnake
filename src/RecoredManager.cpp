#include "RecordManager.h"
#include "GameSettings.h"
#include <sstream>
#include <fstream>
#include <algorithm>

RecordManager::RecordManager() { this->Allrecords.clear(); }
RecordManager::~RecordManager() {}

void RecordManager::loadRecord()
{
    this->Allrecords.clear();
    std::ifstream ifs("GluttonousSnake_score.csv", std::ios::in);
    if (!ifs.is_open())
    {
        this->ifFileEmpty = true;
        ifs.close();
        return;
    }

    std::string line;
    while (std::getline(ifs, line))
    {
        if (line.empty())
            continue;
        std::istringstream ss(line);
        int score = 0, speed = 0, obstacle = 0, iq = 0; // 分开声明并初始化
        char c1 = 0, c2 = 0, c3 = 0;

        if (ss >> score >> c1 >> speed >> c2 >> obstacle >> c3 >> iq &&
            c1 == ',' && c2 == ',' && c3 == ',')
        {
            std::vector<int> record = {score, speed, obstacle, iq}; // 显式构造
            this->Allrecords.push_back(record);
        }
    }
    ifs.close();
    this->ifFileEmpty = this->Allrecords.empty();

    this->sortRecord();
}

void RecordManager::saveRecord(const GameSettings &settings, const int &score)
{
    this->last_Goat = this->getLastGoat();
    std::ofstream ofs;
    ofs.open("GluttonousSnake_score.csv", std::ios::out | std::ios::app);

    ofs << score << ",";
    ofs << static_cast<int>(settings.getSpeedLevel()) << ",";
    ofs << static_cast<int>(settings.getObstacleLevel()) << ",";
    ofs << static_cast<int>(settings.getIQLevel()) << ",";
    ofs << std::endl;

    std::vector<int> record = {score, // 显式构造
                               static_cast<int>(settings.getSpeedLevel()),
                               static_cast<int>(settings.getObstacleLevel()),
                               static_cast<int>(settings.getIQLevel())};
    this->Allrecords.push_back(record);
    this->sortRecord();
    this->ifFileEmpty = false;
    ofs.close();
}

void RecordManager::sortRecord()
{
    if (this->Allrecords.empty())
    {
        return;
    }
    std::sort(Allrecords.begin(), Allrecords.end(), [](const std::vector<int> &a, const std::vector<int> &b)
              { return a[0] > b[0]; });
}

int RecordManager::getLastGoat() const
{
    if (Allrecords.empty())
    {
        return 0;
    }
    return this->Allrecords[0][0];
}

std::string RecordManager::speedtoSPEED(int speednum) const
{
    switch (speednum)
    {
    case 0:
        return "SLOW";
        break;
    case 1:
        return "NORMAL";
        break;
    case 2:
        return "FAST";
        break;
    default:
        return "UNKNOWN";
        break;
    }
}

std::string RecordManager::obstacletoOBSTACLE(int obsnum) const
{
    switch (obsnum)
    {
    case 0:
        return "LOW";
        break;
    case 1:
        return "NORMAL";
        break;
    case 2:
        return "HIGHT";
        break;
    default:
        return "UNKNOWN";
        break;
    }
}

std::string RecordManager::iqtoIQ(int iqnum) const
{
    switch (iqnum)
    {
    case 0:
        return "LOW";
    case 1:
        return "NORMAL";
    case 2:
        return "HIGH";
    default:
        return "UNKNOWN";
    }
}

void RecordManager::draw() const
{
    int count = std::min(5, static_cast<int>(this->Allrecords.size()));
    cleardevice();

    settextcolor(YELLOW);
    settextstyle(30, 0, _T("Arial"));
    outtextxy(WIDTH / 2 - 150, 20, _T("History Record"));
    line(30, 60, WIDTH - 30, 60);

    settextstyle(18, 0, _T("Consolas"));
    settextcolor(WHITE);
    outtextxy(30, 80, _T("Score"));
    outtextxy(150, 80, _T("Speed"));
    outtextxy(270, 80, _T("Obstacle"));
    outtextxy(420, 80, _T("Enemy IQ"));
    line(30, 102, WIDTH - 30, 102);

    int y = 115;
    for (int i = 0; i < count; i++)
    {
        if (i == 0)
            settextcolor(YELLOW);
        else
            settextcolor(WHITE);

        settextstyle(18, 0, _T("Consolas"));

        std::string score = std::to_string(this->Allrecords[i][0]);
        std::string speed = speedtoSPEED(this->Allrecords[i][1]);
        std::string obstacle = obstacletoOBSTACLE(this->Allrecords[i][2]);
        std::string iq = iqtoIQ(this->Allrecords[i][3]);

        outtextxy(30, y, score.c_str());
        outtextxy(150, y, speed.c_str());
        outtextxy(270, y, obstacle.c_str());
        outtextxy(420, y, iq.c_str());

        line(30, y + 22, WIDTH - 30, y + 22);
        y += 35;
    }

    // 没有记录
    if (this->Allrecords.empty())
    {
        settextstyle(20, 0, _T("Arial"));
        outtextxy(WIDTH / 2 - 100, HEIGHT / 2, "No records found");
    }

    settextcolor(WHITE);
    settextstyle(25, 0, _T("Consolas"));
    outtextxy(WIDTH / 4, HEIGHT / 2 + 100, _T("Press SPACE to SET"));

    settextcolor(WHITE);
    settextstyle(25, 0, _T("Consolas"));
    outtextxy(WIDTH / 4, HEIGHT / 2 + 120, _T("Press 0 to StartMenu"));
}
