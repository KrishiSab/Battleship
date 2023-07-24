//
//  Game.cpp
//  Battleship
//
//  Created by Krishi Sabarwal on 7/18/22.
//

#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>

#include <vector>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
    
private:
    //used to hold info about ships, id will be index of vector
    struct Ship
    {
        int length;
        char symbol;
        string name;
    };
    
    int m_rows;
    int m_columns;
    vector<Ship> m_shipList; //id's correspond to indicies
    int m_numShips; //keeps track of number of ships, didn't just use vector.size() because that technically
                    //returns a size_t
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
: m_rows(nRows), m_columns(nCols), m_numShips(0)
{}

int GameImpl::rows() const
{
    return m_rows;
}

int GameImpl::cols() const
{
    return m_columns;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    //add ship and update # of ships
    m_shipList.push_back(Ship{length, symbol, name});
    m_numShips++;
    return true;
}

int GameImpl::nShips() const
{
    return m_numShips;
}

int GameImpl::shipLength(int shipId) const
{
    return m_shipList.at(shipId).length;
}

char GameImpl::shipSymbol(int shipId) const
{
    return m_shipList.at(shipId).symbol;
}

string GameImpl::shipName(int shipId) const
{
    return m_shipList.at(shipId).name;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    //place ships, if they fail return nullptr
    if (p1->placeShips(b1) == false || p2->placeShips(b2) == false)
        return nullptr;
    
    //see if p1 and p2 are humans
    bool isP1Human = p1->isHuman(),isP2Human = p2->isHuman();

    //while no one has won
    while (!b1.allShipsDestroyed() && !b2.allShipsDestroyed())
    {
        //cout correct statement
        cout << p1->name() << "'s turn.  Board for " << p2->name() << ":" << endl;
        
        b2.display(isP1Human); //display the board accordingly

        //will be passed to board::attack
        bool hit1, destroyed1;
        int id1;
        Point point1 = p1->recommendAttack();
        
        bool isAttackValid1 = b2.attack(point1, hit1, destroyed1, id1); //player 1 attacks player 2's board
        
        //player1 can record his attack result
        p1->recordAttackResult(point1, isAttackValid1, hit1, destroyed1, id1);
        
        //let player 2 know which place player 1 attacked
        p2->recordAttackByOpponent(point1);
        
        //check to see if valid attack was given
        if (isAttackValid1 == false)
        {
            cout << p1->name() << " wasted a shot at (" << point1.r << "," << point1.c << ")." << endl;
        }
        else
        {
            //cout attack result
            if (hit1)
            {
                if (destroyed1)
                {
                    cout << p1->name() << " attacked (" << point1.r << "," << point1.c << ") and destroyed the " << shipName(id1) << ", resulting in:" << endl;
                }
                else
                {
                    cout << p1->name() << " attacked (" << point1.r << "," << point1.c << ") and hit something, resulting in:" << endl;
                }
            }
            else
            {
                cout << p1->name() << " attacked (" << point1.r << "," << point1.c << ") and missed, resulting in:" << endl;
            }
                
            
            b2.display(isP1Human); //display the board after the attack
            
            //check to see if p1 won the game
            if (b2.allShipsDestroyed())
            {
                cout << p1->name() << " wins!" << endl;
                //if human loses display winner's full board
                if (isP2Human == true)
                {
                    cout << "Here is where " << p1->name() << "'s ships were:" << endl;
                    b1.display(false);
                }
                    
                
                return p1;
            }
        }
            
        //if shouldPause == true, after displaying results wait for user to press enter
        if (shouldPause)
            waitForEnter();
            
        
        
        
        //repeat for player 2=====================================================================
        
        //cout correct statement
        cout << p2->name() << "'s turn.  Board for " << p1->name() << ":" << endl;
        
        b1.display(isP2Human); //display the board accordingly

        //will be passed to board::attack
        bool hit2, destroyed2;
        int id2;
        Point point2 = p2->recommendAttack();
        
        //player 2 attacks player 1's board
        bool isAttackValid2 = b1.attack(point2, hit2, destroyed2, id2);
        
        //player2 can record his attack result
        p2->recordAttackResult(point2, isAttackValid2, hit2, destroyed2, id2);
        
        //let player 1 know which place player 2 attacked
        p1->recordAttackByOpponent(point2);
        
        
        //check to see if attack is valid
        if (isAttackValid2 == false)
        {
            cout << p2->name() << " wasted a shot at (" << point2.r << "," << point2.c << ")." << endl;
        }
        else
        {
            //cout attack result
            if (hit2)
            {
                if (destroyed2)
                {
                    cout << p2->name() << " attacked (" << point2.r << "," << point2.c << ") and destroyed the " << shipName(id2) << ", resulting in:" << endl;
                }
                else
                {
                    cout << p2->name() << " attacked (" << point2.r << "," << point2.c << ") and hit something, resulting in:" << endl;
                }
            }
            else
            {
                cout << p2->name() << " attacked (" << point2.r << "," << point2.c << ") and missed, resulting in:" << endl;
            }
            
            b1.display(isP2Human); //display board after attack
            
            //check to see if p2 won the game
            if (b1.allShipsDestroyed())
            {
                cout << p2->name() << " wins!" << endl;
                //if losing player is human display winner's full board
                if (isP1Human == true)
                {
                    cout << "Here is where " << p2->name() << "'s ships were:" << endl;
                    b2.display(false);
                }
                
                return p2;
            }
        }
        
        //if shouldPause == true, after displaying results wait for user to press enter
        if (shouldPause)
            waitForEnter();
        
    }
    
    //return something from all control paths
    return nullptr;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}


