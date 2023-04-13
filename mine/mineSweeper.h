#pragma once

#include <fstream>
class mineSweeper
{
private:
    int row;
    int column;
    int bombCount;
    int flagCount = 0;
    int openBlankCount = 0;
    int remainBlankCount = 0;
    int stateFlag = 0;
    bool resultFlag = 0;
    char** answer;
    char** interface;
    bool reload;
public:
    mineSweeper();
    ~mineSweeper();

    void initialize();
    void randomMine();
    void countMine(int x, int y);
    void initWithFile(std::ifstream& board);

    void setRowAndColumn(int row, int column);
    void setBombAmount(int amount);
    void setBombSpawnRate(float rate);

    int getRow();
    int getColumn();
    int getBombCount();
    int getFlagCount();
    int getOpenBlankCount();
    int getRemainBlankCount();

    void leftClick();
    bool validLeft(int x, int y);
    void reveal(int x, int y);
    void openSurround(int x, int y);

    void rightClick();
    bool validRight(int x, int y);
    void changeBlankState(int x, int y);

    void setGameState(int state);
    int getGameState();

    void printAnswer();
    void printGameBoard();
    void printGameState();

    void isWin();
    void isLose();

    void deleteBoard();
};