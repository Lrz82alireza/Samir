#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>

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
const string NOT_CHANGING_CHAR = "-";
const float PERSENT = 100.00;

class Employee;
int team_total_working_hours(vector<Employee> team_members);

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

enum team_info_order
{
    TEAM_ID,
    TEAM_HEAD_ID,
    MEMBER_IDS,
    BONUS_MIN_WORKING_HOURS,
    BONUS_WORKING_HOURS_MAX_VARIANCE,
};

enum salary_config_info_order
{
    LEVEL,
    BASE_SALARY,
    SALARY_PER_HOUR,
    SALARY_PER_EXTRA_HOUR,
    OFFICIAL_WORKING_HOURS,
    TAX_PERCENTAGE,
};

enum employee_info_order
{
    ID,
    NAME,
    AGE,
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

    bool is_working_interval_in_day(int first_time, int second_time)
    {
        for (auto working_hour : working_interval)
        {
            if (working_hour.first <= first_time && working_hour.second >= second_time)
                return true;
        }
        return false;
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
        level = input[LEVEL];
        base_salary = stoi(input[BASE_SALARY]);
        salary_per_hour = stoi(input[SALARY_PER_HOUR]);
        salary_per_extra_hour = stoi(input[SALARY_PER_EXTRA_HOUR]);
        official_working_hours = stoi(input[OFFICIAL_WORKING_HOURS]);
        tax_percentage = stoi(input[TAX_PERCENTAGE]);
    }
    void update_fields(vector<string> new_info)
    {
        set_field(base_salary, new_info[BASE_SALARY]);
        set_field(salary_per_hour, new_info[SALARY_PER_HOUR]);
        set_field(salary_per_extra_hour, new_info[SALARY_PER_EXTRA_HOUR]);
        set_field(official_working_hours, new_info[OFFICIAL_WORKING_HOURS]);
        set_field(tax_percentage, new_info[TAX_PERCENTAGE]);
    }

    void show()
    {
        cout << level << " "
             << base_salary << " "
             << tax_percentage << " ";
    }

    void fill_salary_report(map<string, int> &report)
    {
        report["Base Salary"] = base_salary;
        report["Salary Per Hour"] = salary_per_hour;
        report["Salary Per Extra Hour"] = salary_per_extra_hour;
        report["Official Working Hours"] = official_working_hours;
        report["Tax"] = tax_percentage;
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

    void set_field(int &field, string new_info)
    {
        if (new_info != NOT_CHANGING_CHAR)
            field = stoi(new_info);
    }
};

class Team
{
public:
    void set_fields(vector<string> team_info)
    {
        team_id = stoi(team_info[TEAM_ID]);
        team_head_id = stoi(team_info[TEAM_HEAD_ID]);
        vector<string> temp = seperate_words(team_info[MEMBER_IDS], SEPRATE_MEMBERS_CHAR);
        for (auto s : temp)
            member_ids.push_back(stoi(s));
        bonus_min_working_hours = stoi(team_info[BONUS_MIN_WORKING_HOURS]);
        bonus_working_hours_max_variance = stof(team_info[BONUS_WORKING_HOURS_MAX_VARIANCE]);
    }
    bool update_bonus(int bonus_percentage)
    {
        if (bonus_percentage > 100 || bonus_percentage < 0)
            return false;

        bonus = bonus_percentage;
        return true;
    }

    void fill_team_info_map(map<string, string> &team_report)
    {
        team_report["ID"] = to_string(team_id);
        team_report["Head ID"] = to_string(team_head_id);
        team_report["Bonus"] = to_string(bonus);
    }

    void show()
    {
        cout << team_id << " "
             << bonus_working_hours_max_variance << " ";
    }

    int get_team_id() { return team_id; }
    vector<int> get_member_ids() { return member_ids; }
    int get_bonus_min_working_hours() { return bonus_working_hours_max_variance; }
    int get_head_id() { return team_head_id; }
    int get_bonus() { return bonus; }

private:
    int team_id;
    int team_head_id;
    vector<int> member_ids;
    int bonus_min_working_hours;
    float bonus_working_hours_max_variance;
    int bonus = 0;
};

class Employee
{
public:
    void set_fields(vector<string> input, Salary_Configs *salary_address);
    void set_team_pointer(Team *init_team) { team = init_team; }
    void set_new_day(vector<string> day_info);

    int calculate_absent_days() { return (DAYS_OF_MOUNTH - days.size()); }
    int caculate_total_earning() { return (calculate_salary() + calculate_bonus() - calculate_tax()); }
    int calculate_total_hours();
    int calculate_salary();
    int calculate_bonus();
    int calculate_tax();

    void fill_employee_info_map(map<string, string> &report);
    void fill_all_employees_info_map(map<string, string> &report)
    {
        report["ID"] = to_string(id);
        report["Name"] = name;
        report["Total Working Hours"] = to_string(calculate_total_hours());
        report["Total Earning"] = to_string(caculate_total_earning());
    }

    int get_id() { return id; }
    string get_name() { return name; }
    int get_age() { return age; }

    int calculate_work_hour_in_day(int init_day)
    {
        int sum_work_hour = 0;
        for (auto day : days)
        {
            if (day.get_day_num() == init_day)
                for (auto working_interval : day.get_working_interval())
                    sum_work_hour += (working_interval.second - working_interval.first);
        }
        return sum_work_hour;
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

    int number_of_days_include_working_interval(int start_time, int end_time)
    {
        int counter = 0;
        for (auto day : days)
        {
            if (day.is_working_interval_in_day(start_time, end_time))
                counter++;
        }
        return counter;
    }

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

//*********************** Employee methods ****************************
int Employee::calculate_total_hours()
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
int Employee::calculate_salary()
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
int Employee::calculate_bonus()
{
    if (team != NULL)
        return round((calculate_salary() * team->get_bonus()) / PERSENT);
    return 0;
}
int Employee::calculate_tax()
{
    int earning = calculate_salary() + calculate_bonus();
    return round((earning * level->get_tax_percentage()) / PERSENT);
}

void Employee::fill_employee_info_map(map<string, string> &report)
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

void Employee::set_fields(vector<string> input, Salary_Configs *salary_address)
{
    id = stoi(input[ID]);
    name = input[NAME];
    age = stoi(input[AGE]);
    level = salary_address;
}
void Employee::set_new_day(vector<string> day_info)
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
//*********************************************************************

class Data_Base
{
public:
    map<string, string> report_employee_salary(int id);
    vector<map<string, string>> report_salaries()
    {
        vector<map<string, string>> all_reports;
        int id = 1;
        Employee *target_employee;
        map<string, string> report;
        for (auto employee : employees)
        {
            target_employee = find_employee_by_id(id);
            if (target_employee != NULL)
            {
                target_employee->fill_all_employees_info_map(report);
                all_reports.push_back(report);
            }
            id++;
        }
        return all_reports;
    }
    map<int, int> report_total_hours_per_day(int first, int second)
    {
        int total_hour;
        vector<int> days = split(first, second);
        map<int, int> schedule;
        for (auto day : days)
        {
            total_hour = 0;
            for (auto employee : employees)
                total_hour += employee.calculate_work_hour_in_day(day);

            schedule[day] = total_hour;
        }
        return schedule;
    }
    map<string, int> report_salary_config(string level)
    {
        map<string, int> report;
        Salary_Configs *target_salary_config = find_salary_configs_by_level(level);
        if (target_salary_config == NULL)
        {
            report["Error"] = true;
            return report;
        }
        target_salary_config->fill_salary_report(report);
        return (report);
    }

    vector<int> split(int firs_number, int second_number)
    {
        vector<int> numbers;
        int first = firs_number;
        int second = second_number;
        for (int number = first; number <= second_number; number++)
        {
            numbers.push_back(number);
        }
        return numbers;
    }

    vector<map<string, string>> report_team_salary(int team_id)
    {
        vector<map<string, string>> team_reports;
        Team *target_team = find_team_by_id(team_id);
        map<string, string> temp_map;

        if (target_team == NULL)
        {
            temp_map["Error"] = "found";
            team_reports.push_back(temp_map);
            return team_reports;
        }

        target_team->fill_team_info_map(temp_map);
        temp_map["Head Name"] = find_employee_by_id(target_team->get_head_id())->get_name();

        vector<Employee> team_members = find_employees_by_id(target_team->get_member_ids());
        temp_map["Team Total Working Hours"] = to_string(team_total_working_hours(team_members));

        float avg_total_earning = (stof(temp_map["Team Total Working Hours"]) / team_members.size());
        temp_map["Average Member Working Hour"] = to_string(avg_total_earning);
        team_reports.push_back(temp_map);

        for (auto team_member : team_members)
        {
            map<string, string> memeber_info;
            memeber_info["Member ID"] = to_string(team_member.get_id());
            memeber_info["Total Earning"] = to_string(team_member.caculate_total_earning());
            team_reports.push_back(memeber_info);
        }
        return team_reports;
    }

    Team *find_team_by_id(int team_id)
    {
        for (int i = 0; i < teams.size(); i++)
        {
            if (teams[i].get_team_id() == team_id)
                return &teams[i];
        }
        return NULL;
    }

    vector<Employee> find_employees_by_id(vector<int> member_ids)
    {
        vector<Employee> team_members;
        for (auto member_id : member_ids)
        {
            team_members.push_back(*find_employee_by_id(member_id));
        }
        return team_members;
    }

    vector<float> avg_employee_in_working_interval(int start_time, int end_time)
    {
        vector<float> output;
        vector<int> hours = split(start_time, end_time);
        float avr;
        int employees_num = employees.size();
        for (int i = 0, j = i + 1; j < hours.size(); i++, j++)
        {
            avr = number_of_employees_in_working_hour(hours[i], hours[j]);
            output.push_back(avr / employees_num);
        }
        return output;
    }

    void transfer_to_days(vector<vector<string>> employees_days);
    void transfer_to_salarys(vector<vector<string>> salarys_info);
    void transfer_to_employees(vector<vector<string>> employees_info);
    void transfer_to_teams(vector<vector<string>> teams_info);

    void set_team_pointers_for_employees(Team &team);

    Employee *find_employee_by_id(int id);
    Salary_Configs *find_salary_configs_by_level(string employees_info);

    void show_salary();
    void show_team();
    void show_employee();

    vector<Employee> get_employees() { return employees; }

private:
    vector<Employee> employees;
    vector<Team> teams;
    vector<Salary_Configs> salary_configs;
    void set_teams_pointers_for_employees();
    int number_of_employees_in_working_hour(int start_time, int end_time)
    {
        int number = 0;
        for (auto employee : employees)
            number += employee.number_of_days_include_working_interval(start_time, end_time);
        return number;
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

void Data_Base::transfer_to_days(vector<vector<string>> employees_days)
{
    for (auto employee_days : employees_days)
    {
        Employee *target_employee = find_employee_by_id(stoi(employee_days[0]));
        target_employee->set_new_day(employee_days);
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
void Data_Base::transfer_to_salarys(vector<vector<string>> salarys_info)
{
    for (auto salary_info : salarys_info)
    {
        Salary_Configs temp_salary_configs;
        temp_salary_configs.set_fields(salary_info);
        salary_configs.push_back(temp_salary_configs);
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
    vector<int> team_members = team.get_member_ids();
    for (int j = 0; j < team_members.size(); j++)
    {
        Employee *target_employee = find_employee_by_id(team_members[j]);
        target_employee->set_team_pointer(&team);
    }
}
void Data_Base::set_teams_pointers_for_employees()
{
    for (int i = 0; i < teams.size(); i++)
    {
        set_team_pointers_for_employees(teams[i]);
    }
}

map<string, string> Data_Base::report_employee_salary(int id)
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
//**********************************************************************

void update_salary_config(Data_Base &base, vector<string> new_info)
{
    Salary_Configs *salary_configs = base.find_salary_configs_by_level(new_info[0]);
    if (salary_configs == NULL)
    {
        cout << "INVALID_LEVEL" << endl;
        return;
    }
    salary_configs->update_fields(new_info);
    cout << "OK" << endl;
}

void update_team_bonus(Data_Base &base, int team_id, int bonus_percentage)
{
    Team *team = base.find_team_by_id(team_id);
    if (team == NULL)
    {
        cout << "TEAM_NOT_FOUND" << endl;
        return;
    }
    if (team->update_bonus(bonus_percentage))
    {
        cout << "OK" << endl;
        return;
    }
    cout << "INVALID_ARGUMENTS" << endl;
}

void print_report_team_salary(Data_Base &base, int team_id)
{
    vector<map<string, string>> team_reports = base.report_team_salary(team_id);
    if (team_reports[0]["Error"] == "found")
    {
        cout << "TEAM_NOT_FOUND" << endl;
        return;
    }

    cout << "ID: " + team_reports[0]["ID"] << endl
         << "Head ID: " + team_reports[0]["Head ID"] << endl
         << "Head Name: " + team_reports[0]["Head Name"] << endl
         << "Team Total Working Hours: " + team_reports[0]["Team Total Working Hours"] << endl
         << "Average Member Working Hour: " << fixed << setprecision(1) << stof(team_reports[0]["Average Member Working Hour"]) << endl
         << "Bonus: " + team_reports[0]["Bonus"] << endl
         << "---" << endl;

    for (int i = 1; i < team_reports.size(); i++)
    {
        cout << "Member ID: " + team_reports[i]["Member ID"] << endl
             << "Total Earning: " + team_reports[i]["Total Earning"] << endl
             << "---" << endl;
    }
}

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

void print_report_salaries(Data_Base &base)
{
    vector<map<string, string>> all_reports = base.report_salaries();
    for (auto report : all_reports)
    {
        cout << "ID: " << report["ID"] << endl
             << "Name: " << report["Name"] << endl
             << "Total Working Hours: " << report["Total Working Hours"] << endl
             << "Total Earning: " << report["Total Earning"] << endl;
        cout << "---" << endl;
    }
}

int find_max_element_of_map(map<int, int> &mymap)
{
    int max = 0;
    for (auto map : mymap)
        if (map.second >= max)
            max = map.second;
    return max;
}

int find_min_element_of_map(map<int, int> &mymap)
{
    int min = mymap.begin()->second;
    for (auto map : mymap)
        if (map.second <= min)
            min = map.second;
    return min;
}

void print_max_elements_of_map(map<int, int> &mymap)
{
    int max_value = find_max_element_of_map(mymap);
    for (auto map : mymap)
        if (map.second == max_value)
            cout << map.first << ' ';
    cout << endl;
}

void print_min_elements_of_map(map<int, int> &mymap)
{
    int min_value = find_min_element_of_map(mymap);
    for (auto map : mymap)
        if (map.second == min_value)
            cout << map.first << ' ';
    cout << endl;
}

void print_report_total_hours_per_day(Data_Base &base, int first_day, int last_day)
{
<<<<<<< HEAD
    if (first_day < 1 || last_day > DAYS_OF_MOUNTH)
=======
    if (first_day < 1 || last_day > DAYS_OF_MOUNTH || first_day > last_day)
>>>>>>> a6264ea570fdc06ee4e9f1896c22f51e1db6f31a
    {
        cout << "INVALID_ARGUMENTS" << endl;
        return;
    }
    map<int, int> schedule = base.report_total_hours_per_day(first_day, last_day);
    for (int day = first_day; day <= last_day; day++)
        cout << "Day #" + to_string(day) + ": " << schedule[day] << endl;
    cout << "---" << endl;

    cout << "Day(s) with Max Working Hours: ";
    print_max_elements_of_map(schedule);
    cout << "Day(s) with Min Working Hours: ";
    print_min_elements_of_map(schedule);
}

void show_salary_config(Data_Base &base, string level_name)
{
    map<string, int> report = base.report_salary_config(level_name);
    if (report["Error"])
    {
        cout << "INVALID_LEVEL" << endl;
        return;
    }
    cout << "Base Salary: " << report["Base Salary"] << endl
         << "Salary Per Hour: " << report["Salary Per Hour"] << endl
         << "Salary Per Extra Hour: " << report["Salary Per Extra Hour"] << endl
         << "Official Working Hours: " << report["Official Working Hours"] << endl
         << "Tax: " << report["Tax"] << '%' << endl;
}

void round_one_tenth(vector<float> &v)
{
    for (int i = 0; i < v.size(); i++)
        v[i] = round(v[i] * 10) / 10;
}

float find_max_element_of_vec(vector<float> const &v)
{
    float max = 0.0;
    for (auto e : v)
        if (e >= max)
            max = e;
    return max;
}

float find_min_element_of_vec(vector<float> const &v)
{
    float min = v.front();
    for (auto e : v)
        if (e <= min)
            min = e;
    return min;
}

void print_min_elements_of_vec(vector<float> const &v, int start_time)
{
    float min = find_min_element_of_vec(v);
    for (int i = 0; i < v.size(); i++)
        if (v[i] == min)
            cout << start_time + i << '-' << start_time + i + 1 << ' ';

    cout << endl;
}

void print_max_elements_of_vec(vector<float> const &v, int start_time)
{
    float max = find_max_element_of_vec(v);
    for (int i = 0; i < v.size(); i++)
        if (v[i] == max)
            cout << start_time + i << '-' << start_time + i + 1 << ' ';

    cout << endl;
}

void report_employee_per_hour(Data_Base &base, int start_time, int end_time)
{
    int element = 0;
    vector<float> avrs = base.avg_employee_in_working_interval(start_time, end_time);
    round_one_tenth(avrs);
    for (int first = start_time, second = first + 1; second <= end_time; first++, second++)
    {
        cout << first << '-' << second << ": " << avrs[element] << endl;
        element++;
    }
    cout << "---" << endl;
    cout << "Period(s) with Max Working Employees: ";
    print_max_elements_of_vec(avrs, start_time);
    cout << "Period(s) with Min Working Employees: ";
    print_min_elements_of_vec(avrs, start_time);
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

void command_manager(Data_Base &base)
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
    case REPORT_TOTAL_HOURS_PER_DAY:
    {
        int first, second;
        cin >> first;
        cin >> second;
        print_report_total_hours_per_day(base, first, second);
    }
    case SHOW_SALARY_CONFIG:
    {
        string level_name;
        cin >> level_name;
        show_salary_config(base, level_name);
    }
    }
}

int team_total_working_hours(vector<Employee> team_members)
{
    int sum = 0;
    for (auto team_member : team_members)
    {
        sum += team_member.calculate_total_hours();
    }
    return sum;
}

int main(int argc, char *argv[])
{
    string address = argv[1];
    Data_Base base;
    get_inputs_from_csv(base, address + '/');
<<<<<<< HEAD
    report_employee_per_hour(base, 0, 24);
    // print_report_team_salary(base, 1);
    // print_report_total_hours_per_day(base, 1, 30);
    //  show_salary_config(base , "fsdfsdf");
=======

    print_report_team_salary(base, 1);
    // print_report_total_hours_per_day(base , 18 , 18);
    // show_salary_config(base , "fsdfsdf");
>>>>>>> a6264ea570fdc06ee4e9f1896c22f51e1db6f31a
    //  command_manager(base);
}