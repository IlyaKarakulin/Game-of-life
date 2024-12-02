#include <set>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <cctype>
#include <tuple>
#include <forward_list>
#include <array>
#include <cstdlib>
#include <string>

using namespace std;

// Custom hash function for std::pair to be used in unordered_map
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

// Class to store metadata about the game
class Meta_data
{
private:
    string game_version;  // Version of the game
    string name;          // Name of the game
    string size;          // Size of the game field
    string conditions;    // Conditions for cell survival and birth
    int game_mode;        // Game mode (0, 1, or 2)
    ofstream fout;        // Output file stream for saving data
    int count_iter;       // Number of iterations
    string out_file_name; // Name of the output file

public:
    Meta_data();
    ~Meta_data();

    // Getter methods to retrieve game metadata
    string get_version();
    string get_name();
    string get_size();
    string get_conditions();
    int get_game_mode();
    int get_count_iter();
    string get_out_file_name();

    // Setter methods to set game metadata
    void set_game_mode(int mode);
    void set_version(string game_version);
    void set_name(string name);
    void set_size(string size);
    void set_conditions(string conditional);
    void set_count_iter(int iter);
    void set_out_file_name(string out_file_name);
};

// Class to parse input data and command-line arguments
class Parse_input_data
{
private:
    vector<string> args; // Command-line arguments
    string in_file;      // Input file name
    ifstream fin;        // Input file stream

public:
    Parse_input_data(Meta_data &m_data, int argc, char **argv);
    ~Parse_input_data();

    // Parse the header of the input file and extract game metadata
    void parse_head(Meta_data &m_data, string &name, int &size, set<int> &life_condition, set<int> &birth_condition);

    // Parse the field data (coordinates of live cells) from the input file
    void parse_field(unordered_map<pair<int, int>, int, pair_hash> &map_field);

    // Return the size of the game field
    int get_size();

    // Return the input file name
    string get_input_file();

    // Normalize a command string by trimming trailing whitespace
    void normalize_command(string &command);

    // Parse the number of iterations from command-line arguments
    bool parse_args_iter(Meta_data &m_data);

    // Parse the output file name from command-line arguments
    bool parse_args_outfile(Meta_data &m_data);
};

// Class for handling the game's logic and updating the field state
class Field_calculation
{
public:
    Field_calculation(Meta_data &m_data, Parse_input_data &parser, int argc, char **argv);
    ~Field_calculation();

    // Perform one iteration of the game logic
    void calc_iter();

    // Check the conditions for deleting cells
    void check_del(auto &map_field, forward_list<pair<int, int>> &cell_on_del, auto &cell_on_check_birth);

    // Check the conditions for creating new cells
    void check_birth(auto &map_field, auto &cell_on_check_birth, forward_list<pair<int, int>> &cell_on_birth);

    // Check the neighbors of a specific cell to update its state
    void check_neubors(auto &map_field, auto &cell_on_check_birth, pair<int, int> cell);

    // Return the size of the game field
    int get_size();

    // Return the list of all live cells as a forward list of pairs (coordinates)
    forward_list<pair<int, int>> get_cells();

private:
    int size;                                                // Size of the game field
    string game_name;                                        // Name of the game
    set<int> set_survival_condition;                         // Set of conditions for cell survival
    set<int> set_birth_condition;                            // Set of conditions for cell birth
    vector<vector<bool>> field;                              // 2D field representation (alive or dead cells)
    unordered_map<pair<int, int>, int, pair_hash> map_field; // Map of active cells with their neighbor count
};

// Class to print the game field and save it to a file
class Print_field
{
private:
    int size; // Size of the game field
    bool is_print;
    vector<vector<bool>> matrix_field; // Matrix for storing the game field representation

public:
    // Print the current state of the game field to the console
    void print(Field_calculation &map_field);

    // Save the current game field state to a file
    void save(Field_calculation &map_field, Meta_data &m_data, string &file_name);

    // Clear the terminal output for the specified number of lines
    void clear(int lines);

    // Print help instructions for the game
    void print_help();

    // Constructor to initialize the print field with the given game field size
    Print_field(Field_calculation &map_field, bool is_print);

    ~Print_field();
};

// Class for controlling the game process and handling commands
class Game_process
{
public:
    Game_process();
    ~Game_process();

    // Start the game, choose the appropriate mode, and process commands
    void start_game(int argc, char **argv, bool is_pring);

    // Handle commands for modes 0 and 1 (interactive mode with default or custom feild)
    bool mode0_1(Field_calculation &game, Print_field &printer, Meta_data &m_data, Parse_input_data &parser, string command);

    // Handle automatic mode
    void mode2(Field_calculation &game, Print_field &printer, Meta_data &m_data, Parse_input_data &parser);
};
