#include "fild_calc.hpp"

Field_calculation::Field_calculation(Meta_data &m_data, Parse_input_data &parser, int argc, char **argv)
{
    // Parse the game header to initialize game name, field size, and conditions live and birth
    parser.parse_head(m_data, this->game_name, this->size, this->set_birth_condition, this->set_survival_condition);

    // Parse the initial field configuration into map_field
    parser.parse_field(this->map_field);
}

Field_calculation::~Field_calculation()
{
}

int Field_calculation::get_size()
{
    return this->size;
}

// get all live cells of forward_list
forward_list<pair<int, int>> Field_calculation::get_cells()
{
    forward_list<pair<int, int>> cells;

    for (const auto &[cell, _] : this->map_field)
        cells.push_front(cell);

    return cells;
}

// Check a live cell and counting number neubors
void Field_calculation::check_neubors(auto &map_field, auto &cell_on_check_birth, pair<int, int> cell)
{
    pair<int, int> tmp_cell;

    // traversing the neighbors of one cell in a circle
    for (int i = -1; i <= 1; ++i)
    {
        tmp_cell.first = cell.first + i;

        for (int j = -1; j <= 1; j += (2 - (i && 1))) // (2 - (i && 1)) for skipping the center cell
        {
            tmp_cell.second = cell.second + j;

            // if such a cell exists, then the cell has found a new neighbor,
            //  if not, then the tmpcell has found a new neighbor
            if (map_field.count(tmp_cell))
                ++map_field[cell];
            else
                ++cell_on_check_birth[tmp_cell]; // list of cells that can be born
        }
    }
}

// Identify cells that need to be deleted based on survival conditions (Bx/Sy)
void Field_calculation::check_del(auto &map_field, forward_list<pair<int, int>> &cell_on_del, auto &cell_on_check_birth)
{
    for (const auto &[cell, count_n] : map_field)
    {
        check_neubors(map_field, cell_on_check_birth, cell);

        if (!(this->set_survival_condition.count(count_n)))
            cell_on_del.push_front(cell);
    }
}

// Identify cells that need to be born based on birth conditions
void Field_calculation::check_birth(auto &map_field, auto &cell_on_check_birth, forward_list<pair<int, int>> &cell_on_birth)
{
    for (const auto &[cell, count_n] : cell_on_check_birth)
    {
        if (this->set_birth_condition.count(count_n))
            cell_on_birth.push_front(cell);
    }
}

// Perform one iteration of the game logic
void Field_calculation::calc_iter()
{
    forward_list<pair<int, int>> cell_on_del;   // List of cells to be deleted
    forward_list<pair<int, int>> cell_on_birth; // List of cells to be born

    unordered_map<pair<int, int>, int, pair_hash> cell_on_check_birth; // List of cells that can be birth, depending on the condition (birth condition)

    // Identify cells to delete and potential birth candidates
    check_del(this->map_field, cell_on_del, cell_on_check_birth);

    // Identify cells to be born
    check_birth(this->map_field, cell_on_check_birth, cell_on_birth);

    // Remove cells marked for deletion from the map
    for (const auto &cell : cell_on_del)
        this->map_field.erase(cell);

    // Reset the neighbor count for all remaining cells
    for (const auto &[cell, count] : this->map_field)
        this->map_field[cell] = 0;

    // Add new cells to the map
    for (const auto &cell : cell_on_birth)
        this->map_field[cell] = 0;
}
