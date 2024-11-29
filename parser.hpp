#include <set>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

class Parse_input_data
{
private:
    int arg_count;
    char **arg_arr;
    string in_file;
    int game_mode;
    ifstream fin;

public:
    Parse_input_data(int args, char **argv);
    ~Parse_input_data();
    void parse_head(string &name, int &size, set<int> &life_condition, set<int> &birth_condition);
    void parse_field(auto &map_field);
    string get_input_file();
    int get_game_mode();
};
