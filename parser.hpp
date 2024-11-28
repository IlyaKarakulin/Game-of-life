#include <set>
#include <string>

class Parse_input_data
{
private:
    int arg_count;
    char **arg_arr;
    std::string in_file;
    int game_mode;

public:
    Parse_input_data(int args, char **argv);
    ~Parse_input_data();
    void parse_head(std::string &name, int &size, std::set<int> &life_condition, std::set<int> &birth_condition);
    std::string get_input_file();
    int get_game_mode();
};
