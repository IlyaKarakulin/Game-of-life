#include "fild_calc.hpp"

Game_process::Game_process()
{
}

Game_process::~Game_process()
{
}

// Handles game modes 0 (static) and 1 (interactive)
bool Game_process::mode0_1(Field_calculation &game, Print_field &printer, Meta_data &m_data, Parse_input_data &parser, string command)
{
    string output_file_name = "out.live"; // Default output file name
    static int count_str_to_del = 0;      // how many additional lines should be deleted in the terminal
    int count_tick = 1;                   // Number of iterations
    bool flag = true;                     // Controls the game loop

    // Handle "dump" command (save game state to a file)
    if (command.starts_with("dump"))
    {
        printer.clear(1);

        if (command.length() == 4) // Command is just "dump" without output file name
        {
            printer.save(game, m_data, output_file_name); // Save to default file
        }
        else if (!command.ends_with(".live") || (command.length() < 11)) // Invalid file format
        {
            cerr << "Error! Incorrect file format" << endl;
            ++count_str_to_del;
        }
        else
        {
            output_file_name = command.substr(5);
            printer.save(game, m_data, output_file_name); // Save to specified file
        }
    }

    // Handle "tick" command (calc one or more iterations)
    else if (command.starts_with("tick"))
    {
        printer.clear(game.get_size() + 2 + count_str_to_del);
        count_str_to_del = 0;

        if (command.length() > 5) // additionally, the number of iterations has been introduced
            count_tick = stoi(command.substr(5));

        for (int i = 0; i < count_tick; ++i)
            game.calc_iter();

        printer.print(game); // Print the updated game state
    }
    // Handle "exit" command (exit the game loop)
    else if (command == "exit")
    {
        printer.clear(1 + count_str_to_del);
        count_str_to_del = 0;

        flag = false; // Exit the loop
    }
    // Handle "help" command (display help menu)
    else if (command == "help")
    {
        printer.clear(1 + count_str_to_del);
        count_str_to_del = 0;

        printer.print_help(); // Print help information

        cout << endl
             << "press ENTER to continue...";
        cin.get();         // Wait for user input
        printer.clear(14); // Clear help information
    }
    // Handle unknown commands
    else
    {
        printer.clear(count_str_to_del);
        count_str_to_del = 0;

        cout << "Error!" << endl
             << command << " not found" << endl;
        count_str_to_del += 3; // Add error lines to terminal cleanup counter
    }

    return flag; // Return whether the loop should continue
}

// Handles game mode 2 (automatic mode)
void Game_process::mode2(Field_calculation &game, Print_field &printer, Meta_data &m_data, Parse_input_data &parser)
{
    // Parse command-line arguments for iteration count and output file name
    bool flag = parser.parse_args_iter(m_data);
    bool flag2 = parser.parse_args_outfile(m_data);

    if (flag || flag2)
        exit(1); // Exit if parsing failed

    // Perform the specified number of iterations
    for (int i = 0; i < m_data.get_count_iter(); ++i)
        game.calc_iter();

    // Save the final game state to the output file
    string file = m_data.get_out_file_name();
    printer.save(game, m_data, file);

    // Print the final state of the game
    printer.print(game);
}

// Starts the game and selects the appropriate game mode
void Game_process::start_game(int argc, char **argv)
{
    Meta_data m_data;                                   // Stores metadata about the game
    Parse_input_data parser(m_data, argc, argv);        // Parses command-line arguments
    Field_calculation game(m_data, parser, argc, argv); // Initializes the game state
    Print_field printer(game);                          // Handles printing and saving the game field
    string command;                                     // Stores user commands
    int count_tick = 1;                                 // Default number of iterations
    bool flag = true;                                   // Controls the game loop

    // Print the initial state of the game
    printer.print(game);
    parser.normalize_command(command);

    // Interactive or static mode
    if (m_data.get_game_mode() != 2)
    {
        while (flag)
        {
            // Get user command and process it
            getline(cin, command);
            flag = mode0_1(game, printer, m_data, parser, command);
        }
    }
    // Automatic mode
    else
    {
        mode2(game, printer, m_data, parser);
    }
}
