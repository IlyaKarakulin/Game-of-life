#include "fild_calc.hpp"

Field_calculation::Field_calculation(Meta_data &m_data, Parse_input_data &parser, int argc, char **argv)
{
    parser.parse_head(m_data, this->game_name, this->size, this->set_birth_condition, this->set_survival_condition);
    parser.parse_field(this->map_field);
}

Field_calculation::~Field_calculation()
{
}

int Field_calculation::get_size()
{
    return this->size;
}

forward_list<pair<int, int>> Field_calculation::get_cells()
{
    forward_list<pair<int, int>> cells;

    for (const auto &[cell, _] : this->map_field)
        cells.push_front(cell);

    return cells;
}

void Field_calculation::check_neubors(auto &map_field, auto &cell_on_check_birth, pair<int, int> cell)
{
    pair<int, int> tmp_cell;

    for (int i = -1; i <= 1; ++i)
    {
        tmp_cell.first = cell.first + i;

        for (int j = -1; j <= 1; j += (2 - (i && 1))) // что бы обойти ситуацию, когда i == 0 and j == 0
        {
            tmp_cell.second = cell.second + j;

            if (map_field.count(tmp_cell))
                ++map_field[cell];
            else
                ++cell_on_check_birth[tmp_cell];
        }
    }
}

void Field_calculation::check_del(auto &map_field, forward_list<pair<int, int>> &cell_on_del, auto &cell_on_check_birth)
{
    for (const auto &[cell, count_n] : map_field)
    {
        check_neubors(map_field, cell_on_check_birth, cell);

        if (!(this->set_survival_condition.count(count_n)))
            cell_on_del.push_front(cell);
    }
}

void Field_calculation::check_birth(auto &map_field, auto &cell_on_check_birth, forward_list<pair<int, int>> &cell_on_birth)
{
    for (const auto &[cell, count_n] : cell_on_check_birth)
    {
        if (this->set_birth_condition.count(count_n))
            cell_on_birth.push_front(cell);
    }
}

void Field_calculation::calc_iter()
{
    forward_list<pair<int, int>> cell_on_del;
    forward_list<pair<int, int>> cell_on_birth;

    unordered_map<pair<int, int>, int, pair_hash> cell_on_check_birth;

    check_del(this->map_field, cell_on_del, cell_on_check_birth);
    check_birth(this->map_field, cell_on_check_birth, cell_on_birth);

    for (const auto &cell : cell_on_del)
        this->map_field.erase(cell);

    for (const auto &[cell, count] : this->map_field)
        this->map_field[cell] = 0;

    for (const auto &cell : cell_on_birth)
        this->map_field[cell] = 0;
}
