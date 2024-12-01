#include "fild_calc.hpp"

Parse_input_data::Parse_input_data(Meta_data &m_data, int argc, char **argv)
{
    this->args.assign(argv, argv + argc);

    if (args.size() == 1)
    {
        this->in_file = "./default_field.live";
        m_data.set_game_mode(0);
    }
    else
    {
        this->in_file = this->args.at(1);
        if (args.size() == 2)
            m_data.set_game_mode(1);
        else
            m_data.set_game_mode(2);
    }
}

bool Parse_input_data::parse_args_iter(Meta_data &m_data)
{
    int ind_iter = 0, count_iter = 0;

    while ((ind_iter < (this->args.size() - 1)) && !((this->args.at(ind_iter) == "-i") || (this->args.at(ind_iter).starts_with("--iterations="))))
        ++ind_iter;

    if (this->args.at(ind_iter) == "-i" && (ind_iter < (this->args.size() - 1)))
    {
        count_iter = stoi(this->args.at(ind_iter + 1));
    }
    else if (this->args.at(ind_iter).starts_with("--iterations=") && (this->args.at(ind_iter).length() > 13))
    {
        count_iter = stoi(this->args.at(ind_iter).substr(13));
    }
    else
    {
        cerr << "Error! incorrect input of the number of iterations" << endl;
        return true;
    }

    m_data.set_count_iter(count_iter);
    return false;
}

bool Parse_input_data::parse_args_outfile(Meta_data &m_data)
{
    int ind_iter = 0;
    string out_file;

    while ((ind_iter < (this->args.size() - 1)) && !((this->args.at(ind_iter) == "-o") || (this->args.at(ind_iter).starts_with("--output="))))
        ++ind_iter;

    if (this->args.at(ind_iter) == "-o" && (ind_iter < (this->args.size() - 1)))
    {
        out_file = this->args.at(ind_iter + 1);
    }
    else if (this->args.at(ind_iter).starts_with("--output=") && this->args.at(ind_iter).ends_with(".live") && (this->args.at(ind_iter).length() > 14))
    {
        out_file = this->args.at(ind_iter).substr(9);
    }
    else
    {
        cerr << "Error! incorrect input of the output file" << endl;
        return true;
    }

    m_data.set_out_file_name(out_file);
    return false;
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
    getline(fin, version);

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

void Parse_input_data::parse_field(unordered_map<pair<int, int>, int, pair_hash> &map_field)
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
