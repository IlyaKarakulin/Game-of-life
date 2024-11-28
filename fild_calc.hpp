#include <set>
#include <vector>

class Field_calculation
{
public:
    Field_calculation(int argc, char **argv);
    ~Field_calculation();

private:
    std::string game_name;
    int size;
    std::set<int> life_condition;
    std::set<int> birth_condition;
    std::vector<std::vector<bool>> field;
};