#include "fild_calc.hpp"

// Constructor: Initializes Parse_input_data with command-line arguments and determines game mode
Parse_input_data::Parse_input_data(Meta_data &m_data, int argc, char **argv)
{
    // Store command-line arguments in a vector
    this->args.assign(argv, argv + argc);

    if (args.size() == 1)
    {
        this->in_file = "./default_field.live";
        m_data.set_game_mode(0); // Set game mode to 0 (default field)
    }
    else
    {
        this->in_file = this->args.at(1);

        if (args.size() == 2)
            m_data.set_game_mode(1); // Game mode 1 (interactive on custom field)
        else
            m_data.set_game_mode(2); // Game mode 2 (Automatic mode on custom field)
    }
}

// Fine the number of iterations from the command-line arguments
bool Parse_input_data::parse_args_iter(Meta_data &m_data)
{
    int ind_iter = 0, count_iter = 0;

    // Find the argument specifying the number of iterations
    while ((ind_iter < (this->args.size() - 1)) &&
           !((this->args.at(ind_iter) == "-i") ||
             (this->args.at(ind_iter).starts_with("--iterations="))))
        ++ind_iter;

    // Handle short option (-i) with the following value
    if (this->args.at(ind_iter) == "-i" && (ind_iter < (this->args.size() - 1)))
    {
        count_iter = stoi(this->args.at(ind_iter + 1));
    }
    // Handle long option (--iterations=value)
    else if (this->args.at(ind_iter).starts_with("--iterations=") &&
             (this->args.at(ind_iter).length() > 13))
    {
        count_iter = stoi(this->args.at(ind_iter).substr(13));
    }
    else
    {
        cerr << "Error! incorrect input of the number of iterations" << endl;
        return true;
    }

    m_data.set_count_iter(count_iter); // Store the parsed value in Meta_data
    return false;
}

// Find the output file name from the command-line arguments
bool Parse_input_data::parse_args_outfile(Meta_data &m_data)
{
    int ind_iter = 0;
    string out_file;

    while ((ind_iter < (this->args.size() - 1)) &&
           !((this->args.at(ind_iter) == "-o") ||
             (this->args.at(ind_iter).starts_with("--output="))))
        ++ind_iter;

    // Handle short option (-o) with the following value
    if (this->args.at(ind_iter) == "-o" && (ind_iter < (this->args.size() - 1)))
    {
        out_file = this->args.at(ind_iter + 1);
    }
    // Handle long option (--output=filename)
    else if (this->args.at(ind_iter).starts_with("--output=") &&
             this->args.at(ind_iter).ends_with(".live") &&
             (this->args.at(ind_iter).length() > 14))
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

// Returns the name of the input file
string Parse_input_data::get_input_file()
{
    return this->in_file;
}

// Parses the head of the input file to initialize game metadata
void Parse_input_data::parse_head(Meta_data &m_data, string &name, int &size,
                                  set<int> &set_birth_condition, set<int> &set_survival_condition)
{
    fin.open(get_input_file());

    string version;
    getline(fin, version); // Read the game version
    m_data.set_version(version);

    getline(fin, name); // Read the game name
    m_data.set_name(name);
    name = name.substr(3); // Trim str (del "#N ")

    string tmp;
    getline(fin, tmp);
    m_data.set_size(tmp);
    size = stoi(tmp.substr(3));

    getline(fin, tmp); // Read the birth/survival conditions
    m_data.set_conditions(tmp);
    tmp = tmp.substr(4);
    int i = 0;

    // Parse birth conditions
    while (isdigit(tmp[i]))
    {
        set_birth_condition.insert(tmp[i] - '0');
        ++i;
    }

    i += 2; // Skip the separator "/"

    // Parse survival conditions
    while (isdigit(tmp[i]))
    {
        set_survival_condition.insert(tmp[i] - '0');
        ++i;
    }
}

// Parses the field (initial state of the cells) from the input file
void Parse_input_data::parse_field(unordered_map<pair<int, int>, int, pair_hash> &map_field)
{
    pair<int, int> key;

    // Read cell coordinates from the input file
    // <x coordinate, y coordinates> - number neubors

    while (fin >> key.second)
    {
        fin.ignore(); // Skip the separator " "
        fin >> key.first;

        if (map_field.count(key))
            cerr << "Warning! the cell (" << key.first << ' ' << key.second
                 << ") has already been created before";
        else
            map_field[key] = 0;
    }

    this->fin.close();
}

// Normalizes a command string by removing space
void Parse_input_data::normalize_command(string &command)
{
    while (!command.empty() && isspace(command.back()))
        command.pop_back();
}
