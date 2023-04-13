#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;

const int DAYS_OF_MOUNTH = 30;
const int DAY_LENGTH = 24;
const string FILE_SALARY = "salary_configs.csv";
const string FILE_EMPLOYEE = "employees.csv";
const string FILE_TEAM = "teams.csv";
const string FILE_WORKING_HOURS = "working_hours.csv";
const string SEPRATE_MEMBERS_CHAR = "$";
const string SPREAT_CSV_CHAR = ",";
const string SEPREAT_TIME_CHAR = "-";
const float PERSENT = 100.00;

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
    void set_fields(vector<string> day_info)
    {
        if (working_interval.size() == 0)
            day = stoi(day_info[1]);

        vector<string> working_hours = seperate_words(day_info[2], SEPREAT_TIME_CHAR);

        working_interval.push_back({stoi(working_hours[0]), stoi(working_hours[1])});
    }

    void show()
    {
        for (auto i : working_interval)
        {
            cout << day << " / "
                 << i.first << SEPREAT_TIME_CHAR
                 << i.second << endl;
        }
    }

    int get_day_num() { return day; }
    vector<pair<int, int>> get_working_interval() { return working_interval; }

private:
    int day;
    vector<pair<int, int>> working_interval;

    bool check_interval_working(pair<int, int> hour)
    {
        if (hour.first >= hour.second)
            return false;
        if (hour.first > DAY_LENGTH || hour.first < 0 ||
            hour.second > DAY_LENGTH || hour.second < 0)
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
    int get_salary_per_extra_hour() { return salary_per_hour; }
    int get_official_working_hours() { return official_working_hours; }
    int get_salary_per_hour() { return salary_per_hour; }
    int get_tax_percentage() { return tax_percentage; }
    int get_base_salary() { return base_salary; }

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
        vector<string> temp = seperate_words(team_info[2], SEPRATE_MEMBERS_CHAR);
        for (auto s : temp)
            member_ids.push_back(stoi(s));
        bonus_min_working_hours = stoi(team_info[3]);
        bonus_working_hours_max_variance = stof(team_info[4]);
    }

    void show()
    {
        cout << team_id << " "
             << bonus_working_hours_max_variance << " ";
    }

    int get_team_id() { return team_id; }
    vector<int> get_team_ids() { return member_ids; }
    int get_bonus_min_working_hours() { return bonus_working_hours_max_variance; }

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

    void make_new_day(vector<string> day_info)
    {
        Day *target_day = find_day_by_number(stoi(day_info[1]));
        if (target_day == NULL)
        {
            Day new_day;
            new_day.set_fields(day_info);
            days.push_back(new_day);
            return;
        }
        target_day->set_fields(day_info);
    }

    void show()
    {
        cout << id << " ";
        for (auto i : days)
        {
            i.show();
            cout << endl;
        }
    }

    int calculate_total_hours()
    {
        int total_hours = 0;
        for (auto day : days)
        {
            for (auto pair : day.get_working_interval())
            {
                total_hours += (pair.second - pair.first);
            }
        }
        return total_hours;
    }

    int calculate_absent_days() { return (DAYS_OF_MOUNTH - days.size()); }

    int calculate_salary()
    {
        int total_hour = calculate_total_hours();
        int base_salary = level->get_base_salary();

        if (total_hour <= level->get_official_working_hours())
            return base_salary + (total_hour * level->get_salary_per_hour());
        int official_hours = level->get_official_working_hours();
        int basic_salary = official_hours * level->get_salary_per_hour();
        int extra_salary = (total_hour - official_hours) * level->get_salary_per_extra_hour();
        return (basic_salary + extra_salary + base_salary);
    }

    int calculate_bonus()
    {
        if (team != NULL)
            return round((calculate_salary() * team->get_bonus_min_working_hours()) / PERSENT);
        return 0;
    }

    int caculate_total_earning() { return (calculate_salary() + calculate_bonus() - calculate_tax()); }

    int calculate_tax()
    {
        int earning = calculate_salary() + calculate_bonus();
        return round((earning * level->get_tax_percentage()) / PERSENT);
    }

    void set_team_pointer(Team *init_team) { team = init_team; }

    void fill_employee_info_map(map<string, string> &report)
    {
        report["ID"] = to_string(id);
        report["Name"] = name;
        report["Age"] = to_string(age);
        report["Level"] = level->get_level();
        if (team != NULL)
            report["Team ID"] = to_string(team->get_team_id());
        else
            report["Team ID"] = "N/A";
        report["Total Working Hours"] = to_string(calculate_total_hours());
        report["Absent Days"] = to_string(calculate_absent_days());
        report["Salary"] = to_string(calculate_salary());
        report["Bonus"] = to_string(calculate_bonus());
        report["Tax"] = to_string(calculate_tax());
        report["Total Earning"] = to_string(caculate_total_earning());
    }

    int get_id() { return id; }
    string get_name() { return name; }
    int get_age() { return age; }

private:
    int id;
    string name;
    int age;
    vector<Day> days;
    Salary_Configs *level;
    Team *team = NULL;

    Day *find_day_by_number(int number_of_day)
    {
        for (int i = 0; i < days.size(); i++)
        {
            if (days[i].get_day_num() == number_of_day)
                return &days[i];
        }
        return NULL;
    }
};

class Data_Base
{
public:
    map<string, string> report_employee_salary(int id)
    {
        map<string, string> report;
        Employee *target_employee = find_employee_by_id(id);
        if (target_employee == NULL)
        {
            report["Error"] = "found";
            return report;
        }

        target_employee->fill_employee_info_map(report);
        return report;
    }

    void transfer_to_days(vector<vector<string>> employees_days)
    {
        for (auto employee_days : employees_days)
        {
            Employee *target_employee = find_employee_by_id(stoi(employee_days[0]));
            target_employee->make_new_day(employee_days);
        }
    }

    vector<Employee> get_employees() { return employees; }

    void transfer_to_salarys(vector<vector<string>> salarys_info);
    void set_team_pointers_for_employees(Team &team);
    Employee *find_employee_by_id(int id);
    void transfer_to_teams(vector<vector<string>> teams_info);
    Salary_Configs *find_salary_configs_by_level(string employees_info);
    void transfer_to_employees(vector<vector<string>> employees_info);
    void show_salary();
    void show_team();
    void show_employee();

private:
    vector<Employee> employees;
    vector<Team> teams;
    vector<Salary_Configs> salary_configs;

    void set_teams_pointers_for_employees()
    {
        for (int i = 0; i < teams.size(); i++)
        {
            set_team_pointers_for_employees(teams[i]);
        }
    }
};

//*********************** Data_Base methods ****************************
void Data_Base::show_team()
{
    for (auto team : teams)
    {
        team.show();
        cout << endl;
    }
}

void Data_Base::show_employee()
{
    for (auto employee : employees)
    {
        employee.show();
        cout << endl;
    }
}

void Data_Base::show_salary()
{
    for (auto salary : salary_configs)
    {
        salary.show();
        cout << endl;
    }
}

void Data_Base::transfer_to_employees(vector<vector<string>> employees_info)
{
    for (auto employee_info : employees_info)
    {
        Employee temp_employee;
        temp_employee.set_fields(employee_info, find_salary_configs_by_level(employee_info[3]));
        employees.push_back(temp_employee);
    }
}

Salary_Configs *Data_Base::find_salary_configs_by_level(string employees_info)
{
    for (int i = 0; i < salary_configs.size(); i++)
    {
        if (employees_info == salary_configs[i].get_level())
            return &(salary_configs[i]);
    }
    return NULL;
}

void Data_Base::transfer_to_teams(vector<vector<string>> teams_info)
{
    for (auto team_info : teams_info)
    {
        Team team_temp;
        team_temp.set_fields(team_info);
        teams.push_back(team_temp);
    }
    set_teams_pointers_for_employees();
}

Employee *Data_Base::find_employee_by_id(int id)
{
    for (int i = 0; i < employees.size(); i++)
    {
        if (employees[i].get_id() == id)
            return &employees[i];
    }
    return NULL;
}

void Data_Base::set_team_pointers_for_employees(Team &team)
{
    vector<int> team_members = team.get_team_ids();
    for (int j = 0; j < team_members.size(); j++)
    {
        Employee *target_employee = find_employee_by_id(team_members[j]);
        target_employee->set_team_pointer(&team);
    }
}

void Data_Base::transfer_to_salarys(vector<vector<string>> salarys_info)
{
    for (auto salary_info : salarys_info)
    {
        Salary_Configs temp_salary_configs;
        temp_salary_configs.set_fields(salary_info);
        salary_configs.push_back(temp_salary_configs);
    }
}
//**********************************************************************

void print_report_of_employee_salary(Data_Base &base, int id)
{
    map<string, string> report = base.report_employee_salary(id);
    if (report["Error"] == "found")
    {
        cout << "EMPLOYEE_NOT_FOUND" << endl;
        return;
    }

    cout << "ID: " + report["ID"] << endl
         << "Name: " + report["Name"] << endl
         << "Age: " + report["Age"] << endl
         << "Level: " + report["Level"] << endl
         << "Team ID: " + report["Team ID"] << endl
         << "Total Working Hours: " + report["Total Working Hours"] << endl
         << "Absent Days: " + report["Absent Days"] << endl
         << "Salary: " + report["Salary"] << endl
         << "Bonus: " + report["Bonus"] << endl
         << "Tax: " + report["Tax"] << endl
         << "Total Earning: " + report["Total Earning"] << endl;
}

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
        vector<string> row = seperate_words(line, SPREAT_CSV_CHAR);
        for (auto x : row)
            data[size - 1].push_back(x);
        size++;
    }
    file.close();
    return data;
}

void get_inputs_from_csv(Data_Base &Base, string address)
{
    Base.transfer_to_salarys(get_info_from_csv(address + FILE_SALARY));
    Base.transfer_to_employees(get_info_from_csv(address + FILE_EMPLOYEE));
    Base.transfer_to_teams(get_info_from_csv(address + FILE_TEAM));
    Base.transfer_to_days(get_info_from_csv(address + FILE_WORKING_HOURS));
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

int command_manager(Data_Base base)
{
    string command;
    cin >> command;
    switch (read_command_convert_to_int(command))
    {
    case REPORT_EMPLOYEE_SALARY:
    {
        int id;
        cin >> id;
        print_report_of_employee_salary(base, id);
    }
    }
}

int main(int argc, char *argv[])
{
    string address = argv[1];
    Data_Base base;
    get_inputs_from_csv(base, address + '/');
    command_manager(base);
}
