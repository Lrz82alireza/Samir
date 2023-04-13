#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <algorithm>

using namespace std;

const string file_salary = "salary_configs.csv";
const string file_employee = "employees.csv";
const string file_team = "teams.csv";

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
    void set_fields(vector<string> input)
    {
    }

    void set_day(int init_day)
    {
        day = init_day;
    }
    pair<int, int> read_interval_working_from_csv(string init_times)
    {
        vector<string> times_s = seperate_words(init_times, "-");
        pair<int, int> times_i = {stoi(times_s[0]), stoi(times_s[1])};
        return times_i;
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
        return true;
    }
};

class Salary_Configs
{
public:
    void set_fields(vector<string> input)
    {
        level = input[0];
        base_salary = stoi(input[1]);
        salary_per_hour = stoi(input[2]);
        salary_per_extra_hour = stoi(input[3]);
        official_working_hours = stoi(input[4]);
        tax_percentage = stoi(input[5]);
    }
    void show()
    {
        cout << level << " "
             << base_salary << " "
             << tax_percentage << " ";
    }
    string get_level() { return level; }

private:
    string level;
    int base_salary;
    int salary_per_hour;
    int salary_per_extra_hour;
    int official_working_hours;
    int tax_percentage;
};

class Team
{
public:
    void set_fields(vector<string> team_info)
    {
        team_id = stoi(team_info[0]);
        team_head_id = stoi(team_info[1]);
        vector<string> temp = seperate_words(team_info[2], "$");
        for (auto s : temp)
            member_ids.push_back(stoi(s));
        bonus_min_working_hours = stoi(team_info[3]);
        bonus_working_hours_max_variance = stof(team_info[4]);
    }

    vector<int> get_team_ids() { return member_ids; }

private:
    int team_id;
    int team_head_id;
    vector<int> member_ids;
    int bonus_min_working_hours;
    float bonus_working_hours_max_variance;
};

class Employee
{
public:
    void set_fields(vector<string> input, Salary_Configs *salary_address)
    {
        id = stoi(input[0]);
        name = input[1];
        age = stoi(input[2]);
        level = salary_address;
    }

    void show()
    {
        cout << id << " "
             << name << " "
             << age << " "
             << level->get_level() << " ";
    }

    void set_team_pointer(Team *init_team) { team = init_team; }

    int get_id() { return id; }

private:
    int id;
    string name;
    int age;
    vector<Day *> days;
    Salary_Configs *level;
    Team *team;
};

class Data_Base
{
public:
    void transfer_to_days(vector<vector<string>> days_info)
    {
    }

    void transfer_to_salarys(vector<vector<string>> salarys_info)
    {
        for (auto salary_info : salarys_info)
        {
            Salary_Configs temp_salary_configs;
            temp_salary_configs.set_fields(salary_info);
            salary_configs.push_back(temp_salary_configs);
        }
    }

    void set_team_pointers_for_employees(Team &team)
    {
        vector<int> team_members = team.get_team_ids();
        for (int i = 0; i < employees.size(); i++)
        {
            for (int j = 0; j < team_members.size(); j++)
            {
                if (employees[i].get_id() == team_members[j])
                    employees[i].set_team_pointer(&team);
            }
        }
    }

    void transfer_to_teams(vector<vector<string>> teams_info)
    {
        for (auto team_info : teams_info)
        {
            Team team_temp;
            team_temp.set_fields(team_info);
            teams.push_back(team_temp);
            set_team_pointers_for_employees(team_temp);
        }
    }

    Salary_Configs *find_salary_configs_by_level(string employees_info)
    {
        for (int i = 0; i < salary_configs.size(); i++)
        {
            if (employees_info == salary_configs[i].get_level())
                return &(salary_configs[i]);
        }
        return NULL;
    }

    void transfer_to_employees(vector<vector<string>> employees_info)
    {
        for (auto employee_info : employees_info)
        {
            Employee temp_employee;
            temp_employee.set_fields(employee_info, find_salary_configs_by_level(employee_info[3]));
            employees.push_back(temp_employee);
        }
    }

    void show_salary()
    {
        for (auto salary : salary_configs)
        {
            salary.show();
            cout << endl;
        }
    }

    void show_employee()
    {
        for (auto employee : employees)
        {
            employee.show();
            cout << endl;
        }
    }

private:
    vector<Employee> employees;
    vector<Team> teams;
    vector<Salary_Configs> salary_configs;
};

vector<vector<string>> get_info_from_csv(string file_name)
{
    vector<vector<string>> data;
    ifstream file(file_name);
    string line;
    int size = 1;
    getline(file, line);
    while (getline(file, line))
    {
        data.resize(size);
        vector<string> row = seperate_words(line, ",");
        for (auto x : row)
            data[size - 1].push_back(x);
        size++;
    }
    file.close();
    return data;
}

void get_inputs_from_csv(Data_Base &Base)
{
    Base.transfer_to_salarys(get_info_from_csv(file_salary));
    Base.transfer_to_employees(get_info_from_csv(file_employee));
    Base.transfer_to_teams(get_info_from_csv(file_team));
}

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
        return UPDATE_TEAM_BONUS;
    return -1;
}

int main()
{
    Data_Base base;
    get_inputs_from_csv(base);
    base.show_employee();
    base.show_salary();
}
