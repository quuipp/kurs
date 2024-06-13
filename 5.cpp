﻿#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;

struct Contract
{
    string number;
    string startDate;
    string endDate;
    string work;
    int cost;
};

struct Employee
{
    string lastName;
    string firstName;
    string middleName;
    vector<Contract> contracts;
};

map<string, Employee> employees; // Ассоциативный контейнер для хранения информации о сотрудниках 

void parseAndStoreData(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Failed to open the file." << endl;
        return;
    }

    string line;
    Employee currentEmployee;

    while (getline(file, line))
    {
        if (line.empty()) continue;
        if (line.back() == '}')
        {
            // Сохранение данных о сотруднике
            string key = currentEmployee.lastName + " " + currentEmployee.firstName + " " + currentEmployee.middleName;
            employees[key] = currentEmployee;
            key = "";
            currentEmployee = Employee(); // Сброс данных для следующего сотрудника 
        }
        else if (line.back() == ' ')
        {
            // Имя сотрудника
            stringstream ss(line);
            ss >> currentEmployee.lastName >> currentEmployee.firstName >> currentEmployee.middleName;
            getline(file, line);
        }
        else
        {
            // Данные о договоре
            Contract contract;
            stringstream ss(line);
            string temp;
            ss >> temp; // "Договор"
            ss >> contract.number;
            ss >> temp; // "нач."
            ss >> contract.startDate;
            ss >> temp; // "кон."
            ss >> contract.endDate;
            ss >> temp; // "Работа"            
            ss >> contract.work;
            ss >> temp; // "Стоимость"           
            ss >> contract.cost;
            currentEmployee.contracts.push_back(contract);
            //getline(file, line); 
        }
    }
    file.close();
}

// Функция для подсчета стоимости всех договоров сотрудника
int totalCostOfContracts(const Employee& employee)
{
    int totalCost = 0;
    for (const auto& contract : employee.contracts)
    {
        totalCost += contract.cost;
    }
    return totalCost;
}

// Функция для получения списка договоров сотрудника
vector<Contract> contractsOfWork(const Employee& employee)
{
    return employee.contracts;
}

// Функция для нахождения самого продолжительного договора сотрудника
Contract longestContract(const Employee& employee)
{
    auto longest = max_element(employee.contracts.begin(), employee.contracts.end(),
        [](const Contract& a, const Contract& b)
        {
            return a.endDate < b.endDate;
        });
    return *longest;
}

// Функция для нахождения самого дорогого договора сотрудника
Contract mostExpensiveContract(const Employee& employee)
{
    auto mostExpensive = max_element(employee.contracts.begin(), employee.contracts.end(),
        [](const Contract& a, const Contract& b)
        {
            return a.cost < b.cost;
        });
    return *mostExpensive;
}

// Функция для удаления информации о сотруднике
void removeEmployee(const string& lastName, const string& firstName, const string& middleName)
{
    string key = lastName + firstName + middleName;
    employees.erase(key);
}

int main()
{
    system("color F0");
    // Пример чтения данных из файла
    parseAndStoreData("data.txt");

    // Пример использования функций
    for (const auto& pair : employees)
    {
        cout << "Employee: " << pair.first << endl;
        cout << "Total cost of contracts: " << totalCostOfContracts(pair.second) << endl;

        cout << "Contracts:" << endl;
        for (const auto& contract : contractsOfWork(pair.second))
        {
            cout << contract.number << " - " << contract.cost << endl;
        }

        cout << "Longest contract: " << longestContract(pair.second).number << endl;
        cout << "Most expensive contract: " << mostExpensiveContract(pair.second).number << endl;

        cout << endl;
    }

    //removeEmployee("Antipkina", "Polina", "Konstantinovna");

    return 0;
}