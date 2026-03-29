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
        int score, speed, obstacle;
        char comma1, comma2;

        if (ss >> score >> comma1 >> speed >> comma2 >> obstacle &&
            comma1 == ',' && comma2 == ',')
        {
            this->Allrecords.push_back({score, speed, obstacle});
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
    ofs << std::endl;

    this->Allrecords.push_back({score,
                                static_cast<int>(settings.getSpeedLevel()),
                                static_cast<int>(settings.getObstacleLevel())});

    ofs.close();
    this->ifFileEmpty = false;
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

void RecordManager::draw() const
{
    int count = std::min(5, static_cast<int>(this->Allrecords.size()));
    cleardevice();

    settextcolor(YELLOW);
    settextstyle(30, 0, _T("Arial"));
    outtextxy(WIDTH / 2 - 150, 30, "History Record");
    line(50, 80, WIDTH - 50, 80);

    settextstyle(20, 0, _T("Arial"));
    outtextxy(50, 100, "Record:");
    outtextxy(200, 100, "Speed Level:");
    outtextxy(350, 100, "Obstacle Level:");
    line(50, 120, 500, 120);

    int y = 150;
    for (int i = 0; i < count; i++)
    {
        std::string record = std::to_string(this->Allrecords[i][0]);
        std::string speed = speedtoSPEED(this->Allrecords[i][1]);
        std::string obstacle = obstacletoOBSTACLE(this->Allrecords[i][2]);

        // 显示记录数据
        outtextxy(50, y, record.c_str());
        outtextxy(200, y, speed.c_str());
        outtextxy(350, y, obstacle.c_str());

        line(50, y + 20, 500, y + 20);

        y += 30;
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
