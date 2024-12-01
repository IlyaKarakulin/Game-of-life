#include <set>
#include <vector>
#include <unordered_map>

using namespace std;

struct pair_hash
{
    template <typename T1, typename T2>
    size_t operator()(const pair<T1, T2> &pair) const
    {
        size_t h1 = hash<T1>{}(pair.first);
        size_t h2 = hash<T2>{}(pair.second);
        return h1 ^ (h2 << 1);
    }
};

class Meta_data
{
private:
    string game_version;
    string name;
    string size;
    string conditions;
    int game_mode;
    ofstream fout;
    int count_iter;
    string out_file_name;

public:
    Meta_data();
    ~Meta_data();

    string get_version();
    string get_name();
    string get_size();
    string get_conditions();
    int get_game_mode();
    int get_count_iter();
    string get_out_file_name();

    void set_game_mode(int mode);
    void set_version(string game_version);
    void set_name(string name);
    void set_size(string size);
    void set_conditions(string conditional);
    void set_count_iter(int iter);
    void set_out_file_name(string out_file_name);
};

class Parse_input_data
{
private:
    vector<string> args;
    string in_file;
    ifstream fin;

public:
    Parse_input_data(Meta_data &m_data, int argc, char **argv);
    ~Parse_input_data();
    void parse_head(Meta_data &m_data, string &name, int &size, set<int> &life_condition, set<int> &birth_condition);
    void parse_field(auto &map_field);
    int get_size();
    string get_input_file();
    void normalize_command(string &command);
    bool parse_args_iter(Meta_data &m_data);
    bool parse_args_outfile(Meta_data &m_data);
};

class Field_calculation
{
public:
    Field_calculation(Meta_data &m_data, Parse_input_data &parser, int argc, char **argv);
    ~Field_calculation();
    void calc_iter();
    void check_del(auto &map_field, forward_list<pair<int, int>> &cell_on_del, auto &cell_on_check_birth);
    void check_birth(auto &map_field, auto &cell_on_check_birth, forward_list<pair<int, int>> &cell_on_birth);
    void check_neubors(auto &map_field, auto &cell_on_check_birth, pair<int, int> cell);
    int get_size();
    forward_list<pair<int, int>> get_cells();

private:
    int size;
    string game_name;
    set<int> set_survival_condition;
    set<int> set_birth_condition;
    vector<vector<bool>> field;
    unordered_map<pair<int, int>, int, pair_hash> map_field;
};

class Print_field
{
private:
    int size;
    vector<vector<bool>> matrix_field;

public:
    void print(Field_calculation &map_field);
    void save(Field_calculation &map_field, Meta_data &m_data, string &file_name);
    void clear(int lines);
    void print_help();
    Print_field(Field_calculation &map_field);
    ~Print_field();
};

class Game_process
{
public:
    Game_process();
    ~Game_process();
    void start_game(int argc, char **argv);
    bool mode0_1(Field_calculation &game, Print_field &printer, Meta_data &m_data, Parse_input_data &parser, string command, string output_file_name);
    bool mode2(Field_calculation &game, Print_field &printer, Meta_data &m_data, Parse_input_data &parser);
};
