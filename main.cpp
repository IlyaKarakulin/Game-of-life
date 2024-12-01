#include <iostream>
#include <vector>
#include <fstream>
#include <cctype>
#include <unordered_map>
#include <tuple>
#include <forward_list>
#include <array>
#include <cstdlib>
// #include <boost/algorithm/string.hpp>
#include <string>
#include "fild_calc.hpp"

using namespace std;

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

void Meta_data::set_conditions(string conditional)
{
    this->conditions = conditions;
}

void Meta_data::set_game_mode(int mode)
{
    this->game_mode = mode;
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

Parse_input_data::Parse_input_data(Meta_data &m_data, int args, char **argv)
{
    this->arg_count = args;
    this->arg_arr = argv;

    if (args == 1)
    {
        this->in_file = "./default_field.live";
        m_data.set_game_mode(0);
    }
    else
    {
        this->in_file = this->arg_arr[1];
        if (args == 2)
            m_data.set_game_mode(1);
        else
            m_data.set_game_mode(2);
    }
}

Parse_input_data::~Parse_input_data()
{
}

string Parse_input_data::get_input_file()
{
    return this->in_file;
}

void Parse_input_data::parse_head(Meta_data &m_data, string &name, int &size, set<int> &set_birth_condition, set<int> &set_survival_condition)
{
    fin.open(get_input_file());

    string version;
    getline(fin, version); // TODO add processing exeptions. if version isn't 1.06
    m_data.set_version(version);

    getline(fin, name);
    m_data.set_name(name);
    name = name.substr(3);

    string tmp;
    getline(fin, tmp);
    m_data.set_size(tmp);
    size = stoi(tmp.substr(3));

    getline(fin, tmp);
    m_data.set_conditions(tmp);
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

    while (fin >> key.second)
    {
        fin.ignore();
        fin >> key.first;

        if (map_field.count(key))
            cerr << "Warning! the cell (" << key.first << ' ' << key.second << ") has already been created before";
        else
            map_field[key] = 0;
    }

    this->fin.close();
}

void Parse_input_data::normalize_command(string &command)
{
    while (!command.empty() && isspace(command.back()))
        command.pop_back();
}

Field_calculation::Field_calculation(Meta_data &m_data, Parse_input_data &parser, int argc, char **argv)
{
    parser.parse_head(m_data, this->game_name, this->size, this->set_birth_condition, this->set_survival_condition);

    // cout << this->game_name << endl
    //      << this->size << endl;

    // for (int x : set_birth_condition)
    //     cout << x << ' ';
    // cout << endl;
    // for (int x : set_survival_condition)
    //     cout << x << ' ';
    // cout << endl
    //      << "--------" << endl;

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

    // cout << "To del:\n";
    // for (const auto &cell : cell_on_del)
    //     cout << "(" << cell.first << " " << cell.second << ")" << endl;

    // cout << "Mb birth\n";
    // for (const auto &cell : cell_on_birth)
    //     cout << "(" << cell.first << " " << cell.second << ")" << endl;

    //* for (const auto &[key, value] : this->map_field)
    //*     cout << "(" << key.first << ' ' << key.second << ")" << " - " << value << endl;
}

Print_field::Print_field(Field_calculation &map_field)
{
    this->size = map_field.get_size();

    this->matrix_field.reserve(this->size);

    for (int i = 0; i < this->size; ++i)
        matrix_field.push_back(vector<bool>(this->size, false));
}

Print_field::~Print_field()
{
}

void Print_field::print(Field_calculation &map_field)
{
    for (const pair<int, int> cell : map_field.get_cells())
        matrix_field[cell.first][cell.second] = true;

    for (int i = 0; i < this->size; ++i)
    {
        for (int j = 0; j < this->size; ++j)
        {
            cout << (matrix_field[i][j] ? "# " : ". ");
            matrix_field[i][j] = false;
        }
        cout << endl;
    }
    cout << endl;
}

void Print_field::save(Field_calculation &map_field, Meta_data &m_data, string &file_name)
{
    ofstream fout(file_name);
    fout << m_data.get_version() << endl;
    fout << m_data.get_name() << endl;
    fout << m_data.get_size() << endl;
    fout << m_data.get_conditions() << endl;

    for (const auto &[x, y] : map_field.get_cells())
    {
        fout << y << " " << x << endl;
    }

    fout.close();
}

void Print_field::clear(int lines)
{
    for (int i = 0; i < lines; ++i)
        std::cout << "\033[F\033[K";
}

void Print_field::print_help()
{
    cout << "You can specify the name of the output file and the number of steps at startup:" << endl;
    cout << "./game <name input file> -i <count step> -o <name out file>" << endl
         << endl;
    cout << "Or play step by step. At startup, you can transfer a file that" << endl;
    cout << "describes the field in the live 1.06 format." << endl;
    cout << "if you do not transfer the file, the default field opens" << endl
         << endl;
    cout << "Commants to play step by step:" << endl;
    cout << " - dump <out file> (by default, it is saved in out.live)" << endl;
    cout << " - tick <n> (by default n = 1)" << endl;
    cout << " - exit" << endl;
}

Game_process::Game_process()
{
}

Game_process::~Game_process()
{
}

bool Game_process::mode0_1(Field_calculation &game, Print_field &printer, Meta_data &m_data, string command, string output_file_name)
{
    static int count_str_to_del = 0;
    int count_tick = 1;
    bool flag = true;

    if (command.starts_with("dump"))
    {
        printer.clear(1);

        if (command.length() == 4)
        {
            printer.save(game, m_data, output_file_name);
        }
        else if (!command.ends_with(".live") || (command.length() < 11)) // "dump ?.live" min correct command (len = 11)
        {
            cerr << "Error! Incorrect file format" << endl;
            ++count_str_to_del;
        }
        else
        {
            output_file_name = command.substr(5);
            printer.save(game, m_data, output_file_name);
        }
    }
    else if (command.starts_with("tick"))
    {
        printer.clear(game.get_size() + 2 + count_str_to_del);
        count_str_to_del = 0;

        if (command.length() > 5)
            count_tick = stoi(command.substr(5));

        for (int i = 0; i < count_tick; ++i)
            game.calc_iter();

        printer.print(game);
    }
    else if (command == "exit")
    {
        printer.clear(1 + count_str_to_del);
        count_str_to_del = 0;

        flag = false;
    }
    else if (command == "help")
    {
        printer.clear(1 + count_str_to_del);
        count_str_to_del = 0;

        printer.print_help();

        cout << endl
             << "press ENTER to continue...";
        // cin.ignore();
        cin.get();
        printer.clear(13);
    }
    else
    {
        printer.clear(count_str_to_del);
        count_str_to_del = 0;

        cout << "Error!" << endl
             << command << " not found" << endl;
        count_str_to_del += 3;
    }

    return flag;
}

void Game_process::start_game(int argc, char **argv)
{
    Meta_data m_data;
    Parse_input_data parser(m_data, argc, argv);
    Field_calculation game(m_data, parser, argc, argv);
    Print_field printer(game);
    string command;
    const string output_file_name = "out.live";
    int count_tick = 1;
    bool flag = true;

    printer.print(game);

    while (flag)
    {
        if (m_data.get_game_mode() != 2)
        {
            getline(cin, command);
            parser.normalize_command(command);
            flag = mode0_1(game, printer, m_data, command, output_file_name);
        }
    }
}

int main(int argc, char **argv)
{
    Game_process game;
    game.start_game(argc, argv);

    return 0;
}