#ifndef HW5_COUNTRY_H
#define HW5_COUNTRY_H

// Содержит неообходимую информацию о воюющей стране.
struct Country {
    // Стоимости целей, расопложенных в стране.
    int *aims;
    // Количество целей.
    int aims_number;
    // Общая стоимость целей.
    int64_t total_cost;
    // Стоимость одного снаряда.
    int missile_cost;
    // Название страны.
    std::string name;
    // Количество перенесённых ударов.
    int hits_suffered;
};

#endif //HW5_COUNTRY_H
