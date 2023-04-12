#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <algorithm>

using namespace std;

vector<string> seperate_words(const string line, string separate_char)
{
    vector<string> words;
    string temp = line;
    size_t pos = 0;
    while ((pos = temp.find(separate_char)) != string::npos)
    {
        words.push_back(temp.substr(0, pos));
        temp.erase(0, pos + separate_char.length());
    }
    words.push_back(temp);
    return words;
}

class Day
{
public:
    void set_day(int init_day)
    {
        day = init_day;
    }
    pair<int, int> read_interval_working_from_csv(string init_times)
    {
        vector<string> times_s = seperate_words(init_times, "-");
        pair<int, int> times_i = {stoi(times_s[0]), stoi(times_s[1])};
    }
    void set_interval_working(pair<int, int> init_times)
    {
        working_interval = init_times;
    }

private:
    int day;
    pair<int, int> working_interval = {0, 0};

    bool check_interval_working(pair<int, int> hour)
    {
        if (hour.first >= hour.second)
            return false;
        if (hour.first > 24 || hour.first < 0 ||
            hour.second > 24 || hour.second < 0)
            return false;
        // hampooshani check shavad
        return true;
    }
};

class Salary_Configs
{
public:
private:
    string level;
    int base_salary;
    int salary_per_hour;
    int salary_per_extra_hour;
    int official_working_hours;
    int tax_percentage;
};

class Employee
{
public:
private:
    int id;
    string name;
    int age;
    Salary_Configs *level;
};

class Team
{
public:
private:
    int team_id;
    int team_head_id;
    string member_ids;
    int bonus_min_working_hours;
    float bonus_working_hours_max_variance;
};

int main()
{
}