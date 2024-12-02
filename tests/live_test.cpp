#include <gtest/gtest.h>
#include "../lib/fild_calc.hpp"
#include <memory>
#include <cstring>
#include <cstdlib>

using namespace std;

void cmp(ifstream &field, ifstream &field_ans)
{
    if (!field.is_open())
    {
        cerr << "Err" << endl;
        exit(1);
    }
    if (!field_ans.is_open())
    {
        cerr << "Err" << endl;
        exit(1);
    }

    string line, line_ans;

    while (getline(field, line) && getline(field_ans, line_ans))
    {
        EXPECT_EQ(line, line_ans);
    }
}

TEST(Interactive_mode_test, gefault_game)
{

    int argc = 1;
    char **argv = new char *[argc];
    const char *args[] = {"./build/test/live_test"};

    for (int i = 0; i < argc; ++i)
    {
        argv[i] = new char[strlen(args[i]) + 1];
        strcpy(argv[i], args[i]);
    }

    istringstream fakeInput("tick 200\ndump out_def_file_tick_200.live\nexit");

    auto *oldCin = cin.rdbuf();
    cin.rdbuf(fakeInput.rdbuf());

    Game_process game;
    game.start_game(argc, argv, false); // don't print field

    for (int i = 0; i < argc; ++i)
        delete[] argv[i];

    delete[] argv;

    cin.rdbuf(oldCin);

    ifstream field("out_def_file_tick_200.live");
    ifstream field_ans("./answers/out_def_file_tick_200_ans.live");
    cmp(field, field_ans);
}

TEST(Interactive_mode_test, game1)
{

    int argc = 2;
    char **argv = new char *[argc];
    const char *args[] = {"./build/test/live_test", "game1.live"};

    for (int i = 0; i < argc; ++i)
    {
        argv[i] = new char[strlen(args[i]) + 1];
        strcpy(argv[i], args[i]);
    }

    istringstream fakeInput("tick 9\ndump out_game1_tick_9.live\nexit");

    auto *oldCin = cin.rdbuf();
    cin.rdbuf(fakeInput.rdbuf());

    Game_process game;
    game.start_game(argc, argv, false); // don't print field

    for (int i = 0; i < argc; ++i)
        delete[] argv[i];

    delete[] argv;

    cin.rdbuf(oldCin);

    ifstream field("out_game1_tick_9.live");
    ifstream field_ans("./answers/out_game1_tick_9_ans.live");
    cmp(field, field_ans);
}

TEST(Auto_mode_test, game1)
{

    int argc = 6;
    char **argv = new char *[argc];
    const char *args[] = {"./build/test/live_test",
                          "game1.live",
                          "-i",
                          "30",
                          "-o",
                          "out_game1_tick_30.live"};

    for (int i = 0; i < argc; ++i)
    {
        argv[i] = new char[strlen(args[i]) + 1];
        strcpy(argv[i], args[i]);
    }

    Game_process game;
    game.start_game(argc, argv, false); // don't print field

    ifstream field("out_game1_tick_30.live");
    ifstream field_ans("./answers/out_game1_tick_30_ans.live");
    cmp(field, field_ans);
}

TEST(Auto_mode_test1, game1)
{

    int argc = 6;
    char **argv = new char *[argc];
    const char *args[] = {
        "./build/test/live_test",
        "game1.live",
        "-o",
        "out_game1_tick_30.live",
        "--iterations=30",
    };

    for (int i = 0; i < argc; ++i)
    {
        argv[i] = new char[strlen(args[i]) + 1];
        strcpy(argv[i], args[i]);
    }

    Game_process game;
    game.start_game(argc, argv, false); // don't print field

    for (int i = 0; i < argc; ++i)
        delete[] argv[i];

    delete[] argv;

    ifstream field("out_game1_tick_30.live");
    ifstream field_ans("./answers/out_game1_tick_30_ans.live");
    cmp(field, field_ans);
}

TEST(Auto_mode_test2, game1)
{

    int argc = 4;
    char **argv = new char *[argc];
    const char *args[] = {
        "./build/game",
        "./game1.live",
        "--iterations=30",
        "--output=out_game1_tick_30.live",
    };

    for (int i = 0; i < argc; ++i)
    {
        argv[i] = new char[strlen(args[i]) + 1];
        strcpy(argv[i], args[i]);
    }

    Game_process game;
    game.start_game(argc, argv, false); // don't print field

    for (int i = 0; i < argc; ++i)
        delete[] argv[i];

    delete[] argv;

    ifstream field("out_game1_tick_30.live");
    ifstream field_ans("./answers/out_game1_tick_30_ans.live");
    cmp(field, field_ans);
}
