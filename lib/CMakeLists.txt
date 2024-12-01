cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(game_of_live)
add_library(game_of_live STATIC 
    meta_data.cpp         # Базовые метаданные
    parse_input_data.cpp  # Разбор входных данных
    game_process.cpp      # Логика процесса игры
    field_calculation.cpp # Вычисления на поле
    print_field.cpp       # Вывод данных
)
