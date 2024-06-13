#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <sstream>
#include <ctime>


struct Message
{
    std::string userName;
    std::string time;
    std::string text;

    Message(const std::string& user, const std::string& t, const std::string& msg)
        : userName(user), time(t), text(msg) {}

    friend std::ostream& operator<<(std::ostream& os, const Message& msg)
    {
        os << msg.userName << " " << msg.time << msg.text;
        return os;
    }
};


struct MessageComparator
{
    bool operator()(const Message* lhs, const Message* rhs) const
    {
        if (lhs->userName != rhs->userName)
        {
            return lhs->userName < rhs->userName;
        }
        return lhs->time < rhs->time;
    }
};


class MessageProcessor final
{
private:

    std::set<Message*, MessageComparator> messages;

public:

    MessageProcessor() = default;

    MessageProcessor(const MessageProcessor& other)
    {
        for (auto msg : other.messages)
        {
            messages.insert(new Message(*msg));
        }
    }
    MessageProcessor(MessageProcessor&& other) noexcept
        : messages(std::move(other.messages)) {}

    MessageProcessor& operator=(const MessageProcessor& other)
    {
        if (this != &other)
        {

            for (auto msg : messages)
            {
                delete msg;
            }
            messages.clear();


            for (auto msg : other.messages)
            {
                messages.insert(new Message(*msg));
            }
        }
        return *this;
    }

    MessageProcessor& operator=(MessageProcessor&& other) noexcept
    {
        if (this != &other)
        {
            for (auto msg : messages)
            {
                delete msg;
            }
            messages.clear();

            messages = std::move(other.messages);
        }
        return *this;
    }

    ~MessageProcessor()
    {
        for (auto msg : messages)
        {
            delete msg;
        }
        messages.clear();
    }
public:

    void addMessage(const std::string& user, const std::string& time, const std::string& text)
    {
        Message* msg = new Message(user, time, text);
        messages.insert(msg);
    }

    // Вывод всех сообщений заданного пользователя
    void printUserMessages(const std::string& user) const
    {
        for (auto msg : messages)
        {
            if (msg->userName == user)
            {
                std::cout << *msg << std::endl;
            }
        }
    }

    // Вывод сообщений заданного пользователя для заданного временного интервала
    void printUserMessagesInTimeInterval(const std::string& user, const std::string& startTime, const std::string& endTime) const
    {
        for (auto msg : messages)
        {
            if (msg->userName == user && msg->time >= startTime && msg->time <= endTime)
            {
                std::cout << *msg << std::endl;
            }
        }
    }

    // Вывод всех сообщений из заданного временного интервала
    void printMessagesInTimeInterval(const std::string& startTime, const std::string& endTime) const
    {
        for (auto msg : messages)
        {
            if (msg->time >= startTime && msg->time <= endTime)
            {
                std::cout << *msg << std::endl;
            }
        }
    }

    // Удаление сообщения
    void deleteMessage(const std::string& user, const std::string& time)
    {
        for (auto it = messages.begin(); it != messages.end(); ++it)
        {
            if ((*it)->userName == user && (*it)->time == time)
            {
                delete* it;
                messages.erase(it);
                return;
            }
        }
    }

    // Удаление всех сообщений заданного пользователя
    void deleteUserMessages(const std::string& user)
    {
        for (auto it = messages.begin(); it != messages.end();)
        {
            if ((*it)->userName == user)
            {
                delete* it;
                it = messages.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
};

int main()
{
    system("color F0");
    MessageProcessor processor;
    std::fstream inputFile("messages.txt");

    if (!inputFile.is_open())
    {
        std::cout << "Failed to open the file." << std::endl;
        return 1;
    }

    std::string line;
    while (getline(inputFile, line))
    {
        std::stringstream ss(line);
        std::string user, time, messageText;
        ss >> user;
        ss >> time;
        getline(ss, messageText); // Считываем оставшуюся часть строки как текст сообщения
        processor.addMessage(user, time, messageText);
    }
    inputFile.close();

    std::string name;
    std::string timestart;
    std::string timestop;

    std::cout << "Enter the name of the user whose messages you want to see:" << std::endl;
    std::cin >> name;

    // Пример использования методов класса MessageProcessor
    std::cout << "All messages of user '" << name << "':" << std::endl;
    processor.printUserMessages(name);

    std::cout << "\nEnter the name of the user whose messages you want to see in this time interval:" << std::endl;
    std::cin >> name;
    std::cout << "Enter the start time of the message, accurate to milliseconds: ";
    std::cin >> timestart;
    std::cout << "Enter the end time of the message, accurate to milliseconds: ";
    std::cin >> timestop;

    std::cout << "Messages of user '" << name << "' between " << timestart << " and " << timestop << std::endl;
    processor.printUserMessagesInTimeInterval(name, timestart, timestop);

    std::cout << "\nEnter the time interval within which you want to display messages from all users: ";
    std::cin >> timestart;
    std::cout << "and ";
    std::cin >> timestop;

    std::cout << "All messages between " << timestart << " and " << timestop << std::endl;
    processor.printMessagesInTimeInterval(timestart, timestop);

    std::cout << "\nEnter the username of the message whose message you want to delete: ";
    std::cin >> name;
    processor.deleteUserMessages(name);

    std::cout << "\nEnter the username and time of the message you want to delete: ";
    std::cin >> name >> timestart;
    processor.deleteMessage(name, timestart);

    return 0;
}