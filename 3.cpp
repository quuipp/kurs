﻿#include <iostream>
#include <vector>
#include <random>
#include <string>

// Перечисление для классов персонажей
enum class CharacterClass
{
    Defender = 1,
    Healer,
    MeleeFighter,
    RangedFighter
};

// Структура для характеристик персонажа
struct CharacterStats
{
    int health;
    int armor;
    int strength;
    int intelligence;
    int agility;
    int accuracy;
    int luck;
    int mastery;
};

// Базовый класс элементов экипировки
class Equipment
{
public:
    virtual ~Equipment() {}
    virtual void applyStats(CharacterStats& stats) const = 0;
};

// Класс для генерации случайной экипировки
class EquipmentGenerator
{
public:
    virtual ~EquipmentGenerator() {}
    virtual Equipment* generateEquipment() = 0;
};

// Пример элемента экипировки - оружие
class Weapon : public Equipment
{
private:
    int damage;
public:
    Weapon(int damage) : damage(damage) {}

    void applyStats(CharacterStats& stats) const override
    {
        stats.strength += damage;
    }
};

// Пример элемента экипировки - броня
class Armor : public Equipment
{
private:
    int armorValue;
public:
    Armor(int armorValue) : armorValue(armorValue) {}

    void applyStats(CharacterStats& stats) const override
    {
        stats.armor += armorValue;
    }
};

// Генератор случайного оружия
class WeaponGenerator : public EquipmentGenerator
{
public:
    Equipment* generateEquipment() override
    {
        // Генерация случайного урона для оружия
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 10); // Пример диапазона урона
        int damage = dis(gen);
        return new Weapon(damage);
    }
};

// Генератор случайной брони
class ArmorGenerator : public EquipmentGenerator
{
public:
    Equipment* generateEquipment() override
    {
        // Генерация случайного значения брони
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 20); // Пример диапазона брони
        int armorValue = dis(gen);
        return new Armor(armorValue);
    }
};

// Класс для игрового персонажа
class Character
{
private:
    CharacterClass characterClass;
    CharacterStats stats;
    std::vector<Equipment*> equipment;
public:

    Character(CharacterClass characterClass) : characterClass(characterClass)
    {
        // Инициализация характеристик в зависимости от класса персонажа
        switch (characterClass)
        {
        case CharacterClass::Defender:
            stats.health = 100;
            stats.armor = 50;
            stats.strength = 10;
            stats.intelligence = 10;
            stats.agility = 5;
            stats.mastery = 15;
            stats.luck = 10;
            break;
        case CharacterClass::Healer:
            stats.health = 50;
            stats.armor = 50;
            stats.strength = 10;
            stats.intelligence = 100;
            stats.agility = 5;
            stats.mastery = 15;
            stats.luck = 20;
            break;
        case CharacterClass::MeleeFighter:
            stats.health = 50;
            stats.armor = 50;
            stats.strength = 100;
            stats.intelligence = 10;
            stats.agility = 50;
            stats.mastery = 15;
            stats.luck = 30;
            break;
        case CharacterClass::RangedFighter:
            stats.health = 50;
            stats.armor = 50;
            stats.strength = 10;
            stats.intelligence = 100;
            stats.agility = 5;
            stats.mastery = 15;
            stats.luck = 40;
            break;
        }
    }

    // Функция для добавления элемента экипировки
    void equip(Equipment* item)
    {
        equipment.push_back(item);
        item->applyStats(stats);
    }

    int calculateDamage() const
    {
        return stats.strength + stats.mastery;
    }

    int calculateProtection() const
    {
        return stats.health + stats.luck;
    }

    int calculateTherapy() const
    {
        return stats.intelligence + stats.luck;
    }

    int calculateEndurance() const
    {
        return stats.mastery + stats.luck;
    }

    int calculateRange() const
    {
        return stats.agility + stats.luck;
    }
};

int main()
{
    system("color F0");

    // Создание персонажа
    int character;

    std::cout << "Character:\n1)Defender\n2)Healer\n3)MeleeFighter\n4)RangedFighter\nTake character's class: ";
    std::cin >> character;

    CharacterClass charclass = CharacterClass(character);
    Character player(charclass);

    // Создание генератора для оружия и брони
    WeaponGenerator weaponGenerator;
    ArmorGenerator armorGenerator;

    // Генерация случайного оружия и брони
    Equipment* weapon = weaponGenerator.generateEquipment();
    Equipment* armor = armorGenerator.generateEquipment();

    // Экипировка персонажа
    player.equip(weapon);
    player.equip(armor);

    // Расчет урона
    int damage = player.calculateDamage();
    int protection = player.calculateProtection();
    int therapy = player.calculateTherapy();
    int endurance = player.calculateEndurance();
    int range = player.calculateRange();

    switch (character)
    {
    case 1:
        std::cout << "You have chosen the Defender class!\n";
        std::cout << "Damage: " << damage << std::endl;
        std::cout << "Protection: " << protection << std::endl;
        break;
    case 2:
        std::cout << "You have chosen the Healer class!\n";
        std::cout << "Damage: " << damage << std::endl;
        std::cout << "Protection: " << protection << std::endl;
        std::cout << "Therapy: " << therapy << std::endl;
        break;
    case 3:
        std::cout << "You have chosen the MeleeFighter class!\n";
        std::cout << "Damage: " << damage << std::endl;
        std::cout << "Protection: " << protection << std::endl;
        std::cout << "Endurance: " << endurance << std::endl;
        break;
    case 4:
        std::cout << "You have chosen the RangedFighter class!\n";
        std::cout << "Damage: " << damage << std::endl;
        std::cout << "Protection: " << protection << std::endl;
        std::cout << "Range: " << range << std::endl;
        break;
    default:
        break;
    }
    // Очистка памяти
    delete weapon;
    delete armor;

    return 0;
}