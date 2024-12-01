#include "fild_calc.hpp"

Meta_data::Meta_data()
{
}

Meta_data::~Meta_data()
{
}

void Meta_data::set_version(string game_version)
{
    this->game_version = game_version;
}

void Meta_data::set_name(string name)
{
    this->name = name;
}

void Meta_data::set_size(string size)
{
    this->size = size;
}

void Meta_data::set_conditions(string conditions)
{
    this->conditions = conditions;
}

void Meta_data::set_game_mode(int mode)
{
    this->game_mode = mode;
}

void Meta_data::set_out_file_name(string out_file_name)
{
    this->out_file_name = out_file_name;
}

void Meta_data::set_count_iter(int iter)
{
    this->count_iter = iter;
}

int Meta_data::get_game_mode()
{
    return this->game_mode;
}

string Meta_data::get_version()
{
    return this->game_version;
}

string Meta_data::get_name()
{
    return this->name;
}

string Meta_data::get_size()
{
    return this->size;
}

string Meta_data::get_conditions()
{
    return this->conditions;
}

string Meta_data::get_out_file_name()
{
    return this->out_file_name;
}

int Meta_data::get_count_iter()
{
    return this->count_iter;
}