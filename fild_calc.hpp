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

class Field_calculation
{
public:
    Field_calculation(int argc, char **argv);
    ~Field_calculation();
    void calc_iter(auto &map_field);
    void check_del(auto &map_field, list<pair<int, int>> &cell_on_del, list<pair<int, int>> &cell_on_check_birth);
    void check_birth(auto &map_field, list<pair<int, int>> &cell_on_check_birth);
    void check_neubors(auto &map_field, list<pair<int, int>> &cell_on_check_birth, pair<int, int> cell);
    unordered_map<pair<int, int>, int, pair_hash> map_field; //!!!!!!!

private:
    int size;
    string game_name;
    set<int> survival_condition;
    set<int> birth_condition;
    vector<vector<bool>> field;
};