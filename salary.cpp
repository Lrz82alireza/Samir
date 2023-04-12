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

class Day
{
public:
    void set_day(int init_day)
    {
        day = init_day;
    }

private:
    int day;
    pair<int , int> interval_working;
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
