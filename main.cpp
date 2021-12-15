#include <iostream>
#include <pthread.h>
#include <string>
#include "country.h"

// Сигнализирует, что одна из стран завершила военные действия.
bool war_is_on;

// Проверяет, что введённая строка состоит из цифр.
bool stringIsDigit(const std::string &input) {
    for (auto symb: input) {
        if (!isdigit(symb)) {
            return false;
        }
    }
    return true;
}

// Просит пользователя повторить ввод до тех пор, пока не будет получено корректное значение.
int getNumber(const std::string &message) {
    std::string number;
    std::cout << message;
    std::cin >> number;
    while (!stringIsDigit(number) || std::stoi(number) <= 0) {
        std::cout << "Получены некорректные данные. Пожалуйста, повторите ввод: ";
        std::cin >> number;
    }
    return std::stoi(number);
}

// Собирает данные о стоимости всех целей, расположенных в данном государстве.
void getAims(int *aims, const std::string &message, int aims_number) {
    std::cout << message;
    for (int i = 0; i < aims_number; ++i) {
        aims[i] = getNumber("Цель " + std::to_string(i + 1) + ": ");
    }
}

// Находит сумму всех значений массива.
int64_t getSum(const int *arr, int length) {
    int64_t total_sum = 0;
    for (int i = 0; i < length; ++i) {
        total_sum += arr[i];
    }
    return total_sum;
}

// Осуществляет действия по обстрелу противника.
void *fireAtOpponent(void *arg) {
    Country *opponent = (Country *) arg;
    int64_t money_on_missiles = 0;
    int64_t destroyed_cost = 0;
    int aim_number;
    while (money_on_missiles <= opponent->total_cost && destroyed_cost != opponent->total_cost
           && war_is_on) {
        aim_number = rand() % opponent->aims_number;
        money_on_missiles += opponent->missile_cost;
        destroyed_cost += opponent->aims[aim_number];
        opponent->aims[aim_number] = 0;
        ++(opponent->hits_suffered);
    }
    // Показывает, что страна завершила обстрел.
    war_is_on = false;
    return nullptr;
}

// Вывод итога войны для данного государства.
void printResults(Country *country) {
    std::cout << country->name << " получила " << country->hits_suffered << " удара(-ов).\n";
    int64_t losses = country->total_cost - getSum(country->aims, country->aims_number);
    std::cout << "Стоимость понесённого урона: " << losses << ".\n";
}

int main() {
    std::cout << "Все вводимые числа должны быть натуральными.\n";
    int aims_num_in_taranteria = getNumber("Введите количество целей в Тарантерии: ");
    int aims_num_in_anchuaria = getNumber("Введите количество целей в Анчуарии: ");
    int *taranteria_costs = new int[aims_num_in_taranteria];
    int *anchuaria_costs = new int[aims_num_in_anchuaria];

    getAims(taranteria_costs, "Введите стоимости целей в Тарантерии: ", aims_num_in_taranteria);
    getAims(anchuaria_costs, "Введите стоимости целей в Анчуарии: ", aims_num_in_anchuaria);
    // Подсчёт общей стоимости целей в государствах.
    int64_t total_taranteria_costs = getSum(taranteria_costs, aims_num_in_taranteria);
    int64_t total_anchuaria_costs = getSum(anchuaria_costs, aims_num_in_anchuaria);
    int missile_cost = getNumber("Введите стоимость одного снаряда: ");

    war_is_on = true;
    pthread_t taranteria_thread;
    pthread_t anchuaria_thread;
    // Сохранение необходимой информации о воюющих государствах.
    auto taranteria = new Country{taranteria_costs, aims_num_in_taranteria, total_taranteria_costs,
                                  missile_cost, "Тарантерия", 0};
    auto anchuaria = new Country{anchuaria_costs, aims_num_in_anchuaria, total_anchuaria_costs,
                                 missile_cost, "Анчуария", 0};

    // Запуск потоков, соответствующих воюющим государствам.
    pthread_create(&anchuaria_thread, nullptr, fireAtOpponent, (void *) taranteria);
    pthread_create(&taranteria_thread, nullptr, fireAtOpponent, (void *) anchuaria);
    //Ждём, пока потоки отработают.
    pthread_join(taranteria_thread, nullptr);
    pthread_join(anchuaria_thread, nullptr);
    // Вывод информации об итоге войны.
    printResults(taranteria);
    printResults(anchuaria);

    // Очистка памяти и зануление указателей.
    delete[] taranteria_costs;
    delete[] anchuaria_costs;
    delete taranteria;
    delete anchuaria;
    taranteria_costs = nullptr;
    anchuaria_costs = nullptr;
    taranteria = nullptr;
    anchuaria = nullptr;
    return 0;
}
