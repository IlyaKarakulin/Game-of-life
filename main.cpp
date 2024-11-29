#include <iostream>
#include <vector>
#include <fstream>
#include <cctype>
#include <unordered_map>
#include <tuple>
#include <list>
#include <array>
#include "fild_calc.hpp"
#include "parser.hpp"

using namespace std;

Parse_input_data::Parse_input_data(int args, char **argv)
{
    this->arg_count = args;
    this->arg_arr = argv;

    if (args == 1)
    {
        this->in_file = "./default_field.live";
        game_mode = 0;
    }
    else
    {
        this->in_file = this->arg_arr[1];
        if (args == 2)
            game_mode = 1;
        else
            game_mode = 2;
    }
}

Parse_input_data::~Parse_input_data()
{
}

string Parse_input_data::get_input_file()
{
    return this->in_file;
}

void Parse_input_data::parse_head(string &name, int &size, set<int> &birth_condition, set<int> &survival_condition)
{
    fin.open(get_input_file());

    string format;
    getline(fin, format); // TODO add processing exeptions. if format isn't 1.06

    getline(fin, name);
    name = name.substr(3);

    string tmp;
    getline(fin, tmp);
    size = stoi(tmp.substr(3));

    getline(fin, tmp);
    tmp = tmp.substr(4);
    int i = 0;

    while (isdigit(tmp[i]))
    {
        birth_condition.insert(tmp[i] - '0');
        ++i;
    }

    i += 2;

    while (isdigit(tmp[i]))
    {
        survival_condition.insert(tmp[i] - '0');
        ++i;
    }
}

void Parse_input_data::parse_field(auto &map_field)
{
    pair<int, int> key;

    for (int i = 0; i < 3; ++i) // TODO make condition to for
    {
        fin >> key.first;
        fin.ignore();
        fin >> key.second;

        if (map_field.count(key))
            cerr << "Warning! the cell (" << key.first << ' ' << key.second << ") has already been created before";
        else
            map_field[key] = 0;
    }
}

Field_calculation::Field_calculation(int argc, char **argv)
{
    Parse_input_data parser(argc, argv);
    parser.parse_head(this->game_name, this->size, this->birth_condition, this->survival_condition);

    cout << this->game_name << endl
         << this->size << endl;

    for (int x : birth_condition)
        cout << x << ' ';
    cout << endl;
    for (int x : survival_condition)
        cout << x << ' ';
    cout << endl
         << "--------" << endl;

    // this->field.reserve(size); //?????
    // for (vector<bool> &x : this->field)
    //     x.assign(size, false);

    parser.parse_field(this->map_field);

    for (const auto &[key, value] : this->map_field)
    {
        cout << "(" << key.first << ' ' << key.second << ")" << " - " << value << endl;
    }

    cout << "---\n";
}

Field_calculation::~Field_calculation()
{
}

void Field_calculation::check_neubors(auto &map_field, list<pair<int, int>> &cell_on_check_birth, pair<int, int> cell)
{
    pair<int, int> tmp_cell;

    for (int i = -1; i <= 1; ++i)
    {
        tmp_cell.first = cell.first + i;

        for (int j = -1; j <= 1; j += (2 - (i && 1))) // что бы обойти ситуацию, когда i == 0 and j == 0
        {
            tmp_cell.second = cell.second + j;

            if (map_field.count(tmp_cell))
            {
                ++map_field[cell];
            }
            else
            {
                cell_on_check_birth.push_back(tmp_cell); // TODO replace to un_map
                // ++map_field[tmp_cell];
            }
        }
    }
}

void Field_calculation::check_del(auto &map_field, list<pair<int, int>> &cell_on_del, list<pair<int, int>> &cell_on_check_birth)
{
    for (const auto &[cell, count] : map_field)
    {
        check_neubors(map_field, cell_on_check_birth, cell);

        cout << "(" << cell.first << " " << cell.second << ")" << " - " << count << endl;
    }

    // for(auto )
}

void Field_calculation::check_birth(auto &map_field, list<pair<int, int>> &cell_on_check_birth)
{
}

void Field_calculation::calc_iter(auto &map_field)
{
    list<pair<int, int>> cell_on_del;
    list<pair<int, int>> cell_on_check_birth;

    check_del(map_field, cell_on_del, cell_on_check_birth);
    // check_birth(map_field, cell_on_del, cell_on_check_birth);
}

int main(int argc, char **argv)
{
    Field_calculation game(argc, argv);
    game.calc_iter(game.map_field);

    return 0;
}