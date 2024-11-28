#include <iostream>
#include <vector>
#include <fstream>
#include <cctype>
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

void Parse_input_data::parse_head(string &name, int &size, set<int> &life_condition, set<int> &birth_condition)
{
    ifstream fin(get_input_file());

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
        life_condition.insert(tmp[i] - '0');
        ++i;
    }

    i += 2;

    while (isdigit(tmp[i]))
    {
        birth_condition.insert(tmp[i] - '0');
        ++i;
    }
}

Field_calculation::Field_calculation(int argc, char **argv)
{
    Parse_input_data parser(argc, argv);
    parser.parse_head(this->game_name, this->size, this->life_condition, this->birth_condition);

    cout << this->game_name << endl
         << this->size << endl;
    for (int x : life_condition)
        cout << x << ' ';
    cout << endl;
    for (int x : birth_condition)
        cout << x << ' ';
    cout << endl;

    this->field.reserve(size);
    for (vector<bool> &x : this->field)
        x.assign(size, false);
}

Field_calculation::~Field_calculation()
{
}

int main(int argc, char **argv)
{
    Field_calculation game(argc, argv);

    return 0;
}