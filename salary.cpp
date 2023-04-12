#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <algorithm>


enum commands
{
    REPORT_SALARIES,
    REPORT_EMPLOYEE_SALARY,
    REPORT_TEAM_SALARY,
    REPORT_TOTAL_HOURS_PER_DAY,
    REPORT_EMPLOYEE_PER_HOUR,
    SHOW_SALARY_CONFIG,
    UPDATE_SALARY_CONFIG,
    ADD_WORKING_HOURS,
    DELETE_WORKING_HOURS,
    UPDATE_TEAM_BONUS,
};


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





int read_command_convert_to_int(string input)
{
    if (input == "report_salies")
        return REPORT_SALARIES;
    if (input == "report_employee_salary")
        return REPORT_EMPLOYEE_SALARY;
    if (input == "report_team_salary")
        return REPORT_TEAM_SALARY;
    if (input == "report_total_hours_per_day")
        return REPORT_TOTAL_HOURS_PER_DAY;
    if (input == "report_employee_per_hour")
        return REPORT_EMPLOYEE_PER_HOUR;
    if (input == "show_salary_config")
        SHOW_SALARY_CONFIG;
    if (input == "update_salary_config")
        UPDATE_SALARY_CONFIG;
    if (input == "add_working_hours")
        return ADD_WORKING_HOURS;
    if (input == "delete_working_hours")
        return DELETE_WORKING_HOURS;
    if (input == "update_team_bonus")
        UPDATE_TEAM_BONUS;
}

int main()
{

}
