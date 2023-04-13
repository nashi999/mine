#include <iostream>
#include <fstream>

#include <ctime>

#include <cstdlib>

#include <string>

#include <vector>

#include <unordered_map>

#include "mineSweeper.h"
using namespace std;
#define STANDBY 0
#define PLAYING 1
#define OVERED 2

vector<string>  allModes = { "BoardFile","RandomCount","RandomRate" };
bool loaded = 0;
enum GameMode
{
    FILETYPE = 0,
    COUNT,
    RATE
};

void loadBoard(mineSweeper& mine, string type)
{
    if (type == allModes[GameMode::FILETYPE])
    {
        ifstream board;
        string filePath;
        char garbage;
        garbage = getchar();
        getline(cin, filePath);
        board.open(filePath);
        cout << "<Load " << type << " " << filePath << ">: ";
        if (board.is_open())
        {
            mine.initWithFile(board);
            cout << "Success\n";
            loaded = 1;
        }
        else
        {
            cout << "Failed\n";
        }
        board.close();
    }
    else if (type == allModes[GameMode::COUNT])
    {
        int row, column, amount;
        cin >> row >> column >> amount;
        mine.setRowAndColumn(row, column);
        mine.setBombAmount(amount);
        mine.initialize();
        cout << "<Load " << type << " " << row << " " << column << " " << amount << ">: Success\n";
        loaded = 1;
    }
    else if (type == allModes[GameMode::RATE])
    {
        int row, column;
        float spawnRate;
        cin >> row >> column >> spawnRate;
        mine.setRowAndColumn(row, column);
        mine.setBombSpawnRate(spawnRate);
        mine.initialize();
        cout << "<Load " << type << " " << row << " " << column << " " << spawnRate << ">: Success\n";
        loaded = 1;
    }
}

void printInfo(mineSweeper& mine, string info)
{

    cout << "<Print " << info << ">: ";

    if (info == "GameBoard")
    {
        mine.printGameBoard();
    }
    else if (info == "GameAnswer")
    {
        mine.printAnswer();
    }
    else if (info == "GameState")
    {
        mine.printGameState();
    }
    else if (info == "BombCount")
    {
        cout << mine.getBombCount() << "\n";
    }
    else if (info == "FlagCount")
    {
        cout << mine.getFlagCount() << "\n";
    }
    else if (info == "OpenBlankCount")
    {
        cout << mine.getOpenBlankCount() << "\n";
    }
    else if (info == "RemainBlankCount")
    {
        cout << mine.getRemainBlankCount() << "\n";
    }
}

int main()
{
    string mode, command;
    int currentMode = 0;



    bool withFile = 0, enableLoad = 0, loadFileSuceed = 0;
    while (1)
    {
        mineSweeper mine;
        while (mine.getGameState() == STANDBY)
        {
            cin >> command;
            if (command == "Load")
            {
                string loadType;
                cin >> loadType;
                loadBoard(mine, loadType);
            }
            else if (command == "Print")
            {
                string info;
                cin >> info;
                printInfo(mine, info);
            }
            else if (command == "StartGame")
            {
                cout << "<StartGame>:";
                if (loaded)
                {
                    mine.setGameState(PLAYING);
                    cout << "Success\n";
                }
                else
                {
                    cout << "Failed\n";
                }
            }
        }
        while (mine.getGameState() == PLAYING)
        {
            cin >> command;
            if (command == "Print")
            {
                string info;
                cin >> info;
                printInfo(mine, info);
            }
            else if (command == "LeftClick")
            {
                mine.leftClick();
            }
            else if (command == "RightClick")
            {
                mine.rightClick();
            }
        }
        cin >> command;
        if (command == "Replay")
        {
            mine.setGameState(STANDBY);
            mine.deleteBoard();
            loaded = 0;
        }
        else if (command == "Quit")
        {
            break;
        }
    }

}
