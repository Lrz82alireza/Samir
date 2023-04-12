#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <algorithm>

using namespace std;

class Day
{
public:
    void set_day(int init_day)
    {
        day = init_day;
    }
    void set_interval_working(int start_time, int end_time) //*********
    {
        string start = to_string(start_time);
        string end = to_string(end_time);
        interval_working = start + '-' + end;
    }

private:
    int day;
    string interval_working;
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