#include <iostream>
#include <vector>
#include <fstream>
#include <cctype>
#include <unordered_map>
#include <tuple>
#include <forward_list>
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

void Parse_input_data::parse_head(string &name, int &size, set<int> &set_birth_condition, set<int> &set_survival_condition)
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
        set_birth_condition.insert(tmp[i] - '0');
        ++i;
    }

    i += 2;

    while (isdigit(tmp[i]))
    {
        set_survival_condition.insert(tmp[i] - '0');
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
    parser.parse_head(this->game_name, this->size, this->set_birth_condition, this->set_survival_condition);

    // cout << this->game_name << endl
    //      << this->size << endl;

    // for (int x : set_birth_condition)
    //     cout << x << ' ';
    // cout << endl;
    // for (int x : set_survival_condition)
    //     cout << x << ' ';
    // cout << endl
    //      << "--------" << endl;

    // this->field.reserve(size); //?????
    // for (vector<bool> &x : this->field)
    //     x.assign(size, false);

    parser.parse_field(this->map_field);

    // for (const auto &[key, value] : this->map_field)
    // {
    //     cout << "(" << key.first << ' ' << key.second << ")" << " - " << value << endl;
    // }

    // cout << "---\n";
}

Field_calculation::~Field_calculation()
{
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

void Field_calculation::calc_iter(auto &map_field)
{
    forward_list<pair<int, int>> cell_on_del;
    forward_list<pair<int, int>> cell_on_birth;

    unordered_map<pair<int, int>, int, pair_hash> cell_on_check_birth;

    check_del(map_field, cell_on_del, cell_on_check_birth);
    check_birth(map_field, cell_on_check_birth, cell_on_birth);

    cout << "To del:\n";
    for (const auto &cell : cell_on_del)
        cout << "(" << cell.first << " " << cell.second << ")" << endl;

    cout << "Mb birth\n";
    for (const auto &cell : cell_on_birth)
        cout << "(" << cell.first << " " << cell.second << ")" << endl;
}

int main(int argc, char **argv)
{
    Field_calculation game(argc, argv);
    game.calc_iter(game.map_field);

    return 0;
}