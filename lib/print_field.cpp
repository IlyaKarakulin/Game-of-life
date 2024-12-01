#include "fild_calc.hpp"

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
    cout << "./game <name input file> -i <count step> -o <name out file>" << endl;
    cout << "For example: ./build/game game1.live --iterations=2 --output=./out3.live" << endl
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
