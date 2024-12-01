#include "fild_calc.hpp"

Game_process::Game_process()
{
}

Game_process::~Game_process()
{
}

bool Game_process::mode0_1(Field_calculation &game, Print_field &printer, Meta_data &m_data, Parse_input_data &parser, string command)
{
    string output_file_name = "out.live";

    static int count_str_to_del = 0;
    int count_tick = 1;
    bool flag = true;

    if (command.starts_with("dump"))
    {
        printer.clear(1);

        if (command.length() == 4)
        {
            printer.save(game, m_data, output_file_name);
        }
        else if (!command.ends_with(".live") || (command.length() < 11)) // "dump ?.live" min correct command (len = 11)
        {
            cerr << "Error! Incorrect file format" << endl;
            ++count_str_to_del;
        }
        else
        {
            output_file_name = command.substr(5);
            printer.save(game, m_data, output_file_name);
        }
    }
    else if (command.starts_with("tick"))
    {
        printer.clear(game.get_size() + 2 + count_str_to_del);
        count_str_to_del = 0;

        if (command.length() > 5)
            count_tick = stoi(command.substr(5));

        for (int i = 0; i < count_tick; ++i)
            game.calc_iter();

        printer.print(game);
    }
    else if (command == "exit")
    {
        printer.clear(1 + count_str_to_del);
        count_str_to_del = 0;

        flag = false;
    }
    else if (command == "help")
    {
        printer.clear(1 + count_str_to_del);
        count_str_to_del = 0;

        printer.print_help();

        cout << endl
             << "press ENTER to continue...";
        // cin.ignore();
        cin.get();
        printer.clear(14);
    }
    else
    {
        printer.clear(count_str_to_del);
        count_str_to_del = 0;

        cout << "Error!" << endl
             << command << " not found" << endl;
        count_str_to_del += 3;
    }

    return flag;
}

void Game_process::mode2(Field_calculation &game, Print_field &printer, Meta_data &m_data, Parse_input_data &parser)
{
    bool flag = parser.parse_args_iter(m_data);
    bool flag2 = parser.parse_args_outfile(m_data);

    if (flag || flag2)
        exit(1);

    for (int i = 0; i < m_data.get_count_iter(); ++i)
        game.calc_iter();

    string file = m_data.get_out_file_name();
    printer.save(game, m_data, file);
    printer.print(game);
}

void Game_process::start_game(int argc, char **argv)
{
    Meta_data m_data;
    Parse_input_data parser(m_data, argc, argv);
    Field_calculation game(m_data, parser, argc, argv);
    Print_field printer(game);
    string command;
    int count_tick = 1;
    bool flag = true;
    printer.print(game);
    parser.normalize_command(command);

    if (m_data.get_game_mode() != 2)
    {
        while (flag)
        {
            getline(cin, command);
            flag = mode0_1(game, printer, m_data, parser, command);
        }
    }
    else
    {
        mode2(game, printer, m_data, parser);
    }
}
