#include <iostream>
#include <fstream>

#include <ctime>

#include <cstdlib>

#include <string>

#include <unordered_map>

#include "mineSweeper.h"

using namespace std;

mineSweeper::mineSweeper()
{
    this->row = 0;
    this->column = 0;
    this->bombCount = 0;
    this->remainBlankCount = 0;
    this->reload = 0;
}
mineSweeper::~mineSweeper()
{
    for (int i = 0; i < this->row; i++)
    {
        delete[] this->answer[i];
        delete[] this->interface[i];
    }
    delete[] this->answer;
    delete[] this->interface;
}

void mineSweeper::setRowAndColumn(int row, int column)
{
    if (this->reload)
    {
        this->deleteBoard();
    }
    this->row = row;
    this->column = column;
}

void mineSweeper::setBombAmount(int amount)
{
    this->bombCount = amount;
}

void mineSweeper::setBombSpawnRate(float rate)
{
    this->bombCount = (int)((this->row * this->column) * rate);
}

int mineSweeper::getRow()
{
    return this->row;
}
int mineSweeper::getColumn()
{
    return this->column;
}

void mineSweeper::setGameState(int state)
{
    this->stateFlag = state;
}

int mineSweeper::getGameState()
{
    return stateFlag;
}

int mineSweeper::getBombCount()
{
    return this->bombCount;
}
int mineSweeper::getFlagCount()
{
    return this->flagCount;
}
int mineSweeper::getOpenBlankCount()
{
    return this->openBlankCount;
}
int mineSweeper::getRemainBlankCount()
{
    return this->remainBlankCount;
}

void mineSweeper::initialize()
{

    this->answer = new char* [this->row];
    this->interface = new char* [this->row];
    for (int i = 0; i < this->row; i++)
    {
        this->answer[i] = new char[this->column];
        this->interface[i] = new char[this->column];
    }

    this->remainBlankCount = row * column - bombCount;

    for (int i = 0; i < this->row; i++)
    {
        for (int j = 0; j < this->column; j++)
        {
            this->answer[i][j] = '0';
            this->interface[i][j] = '#';
        }
    }

    this->randomMine();

    this->reload = 1;
}

void mineSweeper::randomMine()
{
    srand((unsigned int)time(NULL));
    for (int k = 0; k < this->bombCount; k++)
    {
        int x = rand() % (row);
        int y = rand() % (column);
        //cout<<x<<" "<<y<<"\n";
        if (this->answer[x][y] == 'X')
        {
            k--;
        }
        else
        {
            this->answer[x][y] = 'X';
            countMine(x, y);
        }

    }
}
void mineSweeper::countMine(int x, int y)
{
    for (int i = x - 1; i < x + 2; i++)
    {
        for (int j = y - 1; j < y + 2; j++)
        {
            if (i < this->row && j < this->column && i >= 0 && j >= 0)
            {
                if (this->answer[i][j] != 'X')
                {
                    this->answer[i][j] += 1;
                }
            }
        }
    }
}

void mineSweeper::initWithFile(ifstream& board)
{
    int givenRow, givenColumn;
    string firstLine;
    getline(board, firstLine);
    givenRow = (int)firstLine[0] - 48;
    givenColumn = (int)firstLine[2] - 48;

    if (this->reload)
    {
        this->deleteBoard();
    }

    this->row = givenRow;
    this->column = givenColumn;

    this->answer = new char* [this->row];
    this->interface = new char* [this->row];
    for (int i = 0; i < this->row; i++)
    {
        this->answer[i] = new char[this->column];
        this->interface[i] = new char[this->column];
    }


    for (int i = 0; i < this->row; i++)
    {
        for (int j = 0; j < this->column; j++)
        {
            this->answer[i][j] = '0';
            this->interface[i][j] = '#';
        }
    }

    this->reload = 1;
    for (int i = 0; i < givenRow; i++)
    {
        string line;
        getline(board, line);
        for (int j = 0; j < givenColumn; j++)
        {
            if (line[j] == 'X')
            {
                this->answer[i][j] = 'X';
                this->bombCount++;
                this->countMine(i, j);
            }
        }
    }
    this->remainBlankCount = row * column - bombCount;
}

void mineSweeper::reveal(int x, int y)
{
    this->interface[x][y] = this->answer[x][y];
    this->openBlankCount++;
    this->remainBlankCount--;
    // cout<<x<<" "<<y<<"\n";
    if (this->answer[x][y] == '0')
    {
        openSurround(x, y);
    }
}
void mineSweeper::openSurround(int x, int y)
{
    for (int i = x - 1; i < x + 2; i++)
    {
        for (int j = y - 1; j < y + 2; j++)
        {
            if (i >= 0 && j >= 0 && i < this->row && j < this->column && this->interface[i][j] == '#')
            {
                reveal(i, j);
            }
        }
    }
}

void mineSweeper::leftClick()
{
    int x, y;
    cin >> x >> y;
    cout << "<LeftClick " << x << " " << y << " >: ";
    if (this->validLeft(x, y))
    {
        cout << "Success\n";
        this->reveal(x, y);
        //this->printGameBoard();
        if (answer[x][y] == 'X')
        {
            this->isLose();
        }
        else if (this->getRemainBlankCount() == 0)
        {
            this->isWin();
        }
    }
    else
    {
        cout << "Failed\n";
    }
}

void mineSweeper::rightClick()
{
    int x, y;
    cin >> x >> y;
    cout << "<RightClick " << x << " " << y << " >: ";
    if (this->validRight(x, y))
    {
        cout << "Success\n";
        this->changeBlankState(x, y);
        //this->printGameBoard();
        if (this->getRemainBlankCount() == 0)
        {
            this->isWin();
        }
    }
    else
    {
        cout << "Failed\n";
    }
}
void mineSweeper::changeBlankState(int x, int y)
{
    if (this->interface[x][y] == '#')
    {
        this->interface[x][y] = 'f';
        this->flagCount++;
        this->remainBlankCount--;
    }
    else if (this->interface[x][y] == 'f')
    {
        this->interface[x][y] = '?';
        this->flagCount--;
    }
    else if (this->interface[x][y] == '?')
    {
        this->interface[x][y] = '#';
        this->remainBlankCount++;
    }
}

bool mineSweeper::validLeft(int x, int y)
{
    if (y < 0 && x < 0 && x >= this->row && y >= this->column)
    {
        return 0;
    }
    else if (this->interface[x][y] != '#')
    {
        return 0;
    }
    return 1;
}
bool mineSweeper::validRight(int x, int y)
{
    if (y < 0 && x < 0 && x >= this->row && y >= this->column)
    {
        return 0;
    }
    else if (this->interface[x][y] != '#' && this->interface[x][y] != 'f' && this->interface[x][y] != '?')
    {
        return 0;
    }
    return 1;
}

void mineSweeper::printAnswer()
{
    cout << "\n";
    for (int i = 0; i < this->row; i++)
    {
        for (int j = 0; j < this->column; j++)
        {
            cout << this->answer[i][j] << " ";
        }
        cout << "\n";
    }
}

void mineSweeper::printGameBoard()
{
    cout << "\n";
    for (int i = 0; i < this->row; i++)
    {
        for (int j = 0; j < this->column; j++)
        {
            cout << this->interface[i][j] << " ";
        }
        cout << "\n";
    }
}

void mineSweeper::printGameState()
{
    vector<string> GameState = { "Standby","Playing","Game Over" };
    cout << GameState[this->getGameState()] << "\n";
}

void mineSweeper::isWin()
{
    this->setGameState(2);
    cout << "You win the game\n";
}

void mineSweeper::isLose()
{
    this->setGameState(2);
    cout << "You lose the game\n";
}

void mineSweeper::deleteBoard()
{
    for (int i = 0; i < this->row; i++)
    {
        delete[] this->answer[i];
        delete[] this->interface[i];
    }
    delete[] this->answer;
    delete[] this->interface;
}