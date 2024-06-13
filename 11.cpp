﻿#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <random>
#include <ctime>

// Класс для представления билета
class Ticket
{
private:
    int number;
    double prize;

public:
    Ticket(int num) : number(num), prize(0) {}

    int getNumber() const { return number; }
    double getPrize() const { return prize; }

    void setPrize(double amount) { prize = amount; }
};

// Класс для представления лотереи
class Lottery
{
private:

    std::vector<Ticket> tickets;

public:
    // Генерация билетов для лотереи
    void generateTickets(int numTickets)
    {
        tickets.clear();
        for (int i = 1; i < numTickets; ++i)
        {
            tickets.emplace_back(i); // Нумерация билетов с 1
        }
    }
    // Получение случайного билета
    Ticket& getRandomTicket()
    {
        static std::mt19937 mt(std::time(nullptr)); // Генератор случайных чисел
        std::uniform_int_distribution<int> dist(0, tickets.size() - 1);
        return tickets[dist(mt)];
    }
};

// Класс для представления тиража
class LotteryDraw
{
private:

    std::list<Ticket*> winningTickets;

public:
    // Проведение розыгрыша
    void conductDraw(Lottery& lottery, int numWinners)
    {
        winningTickets.clear();
        static std::mt19937 mt1(std::time(nullptr)); // Генератор случайных чисел
        std::uniform_int_distribution<int> dist1(0, 1000);
        for (int i = 0; i < numWinners; ++i)
        {
            Ticket& ticket = lottery.getRandomTicket();
            ticket.setPrize(dist1(mt1)); // Пример выигрыша  
            winningTickets.push_back(&ticket);
        }
    }

    // Поиск билета по номеру
    Ticket* findTicketByNumber(int number)
    {
        auto it = std::find_if(winningTickets.begin(), winningTickets.end(),
            [number](Ticket* t) { return t->getNumber() == number; });
        return (it != winningTickets.end()) ? *it : nullptr;
    }

    // Поиск билетов по величине выигрыша
    std::vector<Ticket*> findTicketsByPrize(double prize)
    {
        std::vector<Ticket*> result;
        for (auto ticket : winningTickets)
        {
            if (ticket->getPrize() == prize)
            {
                result.push_back(ticket);
            }
        }
        return result;
    }
};

int main()
{
    system("color F0");

    unsigned long count_ticket;

    std::cout << "Enter count tickets: ";
    std::cin >> count_ticket;

    Lottery lottery;
    lottery.generateTickets(count_ticket);

    LotteryDraw draw;
    draw.conductDraw(lottery, 50);

    unsigned long find_ticket;

    std::cout << "Enter ticket's number: ";
    std::cin >> find_ticket;

    // Пример поиска билета по номеру и вывода выигрышей
    Ticket* ticket = draw.findTicketByNumber(find_ticket);
    if (ticket)
    {
        std::cout << "Ticket " << ticket->getNumber() << " won $" << ticket->getPrize() << std::endl;
    }
    else
    {
        std::cout << "Ticket not found." << std::endl;
    }

    return 0;
}