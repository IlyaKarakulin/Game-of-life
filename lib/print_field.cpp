#include "fild_calc.hpp"

// Constructor: Initializes the print field with the given game field size
Print_field::Print_field(Field_calculation &map_field, bool is_print)
{
    this->size = map_field.get_size(); // Set the field size
    this->is_print = is_print;

    this->matrix_field.reserve(this->size); // Reserve memory for the field matrix

    // Initialize the matrix to represent the game field with all cells dead
    for (int i = 0; i < this->size; ++i)
        matrix_field.push_back(vector<bool>(this->size, false));
}

Print_field::~Print_field()
{
}

// Print the current state of the game field
void Print_field::print(Field_calculation &map_field)
{
    if (!this->is_print)
        return;
    // Mark live cells on the matrix based on the cells from the Field_calculation object
    for (const pair<int, int> cell : map_field.get_cells())
        matrix_field[cell.first][cell.second] = true;

    // Print the field to the console
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

// Save the current game state to a file in the live format
void Print_field::save(Field_calculation &map_field, Meta_data &m_data, string &file_name)
{
    ofstream fout(file_name); // Open the output file for writing

    // Write metadata to the file
    fout << m_data.get_version() << endl;
    fout << m_data.get_name() << endl;
    fout << m_data.get_size() << endl;
    fout << m_data.get_conditions() << endl;

    // Write the coordinates of live cells to the file
    for (const auto &[x, y] : map_field.get_cells())
    {
        fout << y << " " << x << endl;
    }

    fout.close();
}

// Clear the terminal
void Print_field::clear(int lines)
{
    for (int i = 0; (i < lines) && (this->is_print); ++i)
        std::cout << "\033[F\033[K";
}

// Print help instructions for the user
void Print_field::print_help()
{

    if (this->is_print)
    {
        cout << "You can specify the name of the output file and the number of steps at startup:" << endl;
        cout << "./game <name input file> -i <count step> -o <name out file>" << endl;
        cout << "For example: ./build/game game1.live --iterations=2 --output=./out3.live" << endl
             << endl;
        cout << "Or play step by step. At startup, you can transfer a file that" << endl;
        cout << "describes the field in the live 1.06 format." << endl;
        cout << "if you do not transfer the file, the default field opens" << endl
             << endl;
        cout << "Commands to play step by step:" << endl;
        cout << " - dump <out file> (by default, it is saved in out.live)" << endl;
        cout << " - tick <n> (by default n = 1)" << endl;
        cout << " - exit" << endl;
        cout << endl
             << "press ENTER to continue...";
    }
}
