//
//  Board.cpp
//  Battleship
//
//  Created by Krishi Sabarwal on 7/18/22.
//

#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>


using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      // TODO:  Decide what private members you need.  Here's one that's likely to be useful:
    
    //ship-extention, stores remaining length of ship and whether or not it has been placed
    struct ShipExt
    {
        int lengthLeft;
        bool hasBeenPlaced;
    };
    const Game& m_game;
    char m_arr[10][10]; //upto 10x10 array
    vector<ShipExt> m_shipExtList; //stores shipExtenstion data
    
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g)
{
    //initialize board with '.'
    for (int i = 0; i < m_game.rows(); i++)
    {
        for (int j = 0; j < m_game.cols(); j++)
            m_arr[i][j] = '.';
    }
    
    //initialize shipExtensionList
    for (int j = 0; j < g.nShips(); j++)
    {
        int len = g.shipLength(j);
        m_shipExtList.push_back(ShipExt{len, false}); //initially, lengthLeft = length && hasBeenPlaced = false
    }
    
}

void BoardImpl::clear()
{
    for (int i = 0; i < m_game.rows(); i++)
    {
        for (int j = 0; j < m_game.cols(); j++)
            m_arr[i][j] = '.'; //"clearing" the board by making every entry '.'
    }
}

//========================================================================================CHECK================
void BoardImpl::block()
{
    //how many cells have been blocked
    int count = 0;
    //how many cells need to be blocked
    const int HALF = (m_game.rows() * m_game.cols()) / 2;
    
    
    int r, c;
    //keep blocking cells until half the cells are blocked
    while (count != HALF)
    {
        for (r = 0; r < m_game.rows(); r++)
        {
            for (c = 0; c < m_game.cols(); c++)
            {
                //if count == half stop function
                if (count == HALF)
                {
                    return;
                }
                
                
                if (randInt(2) == 0)
                {
                    if (m_arr[r][c] == '#')
                    {
                        //do nothing if already blocked
                    }
                    else
                    {
                        m_arr[r][c] = '#'; //'\0' == blocked
                        count++; //update count
                    }
                }
            }
        }
        
        r = 0;
        c = 0;
        
                
    }
    
}
//========================================================================================CHECK================

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            //'.' == unblock
            if (m_arr[r][c] == '#')
            {
                m_arr[r][c] = '.';
            }
        }
    }
        
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    //check to see if invalid shipId given
    if (shipId < 0 || shipId >= m_game.nShips())
    {
        return false;
    }
    //check to see if ship has already been placed
    if (m_shipExtList.at(shipId).hasBeenPlaced == true)
    {
        return false;
    }
    
    
    
    //Check to see if there is enough space to place ship Horizontally
    if (dir == HORIZONTAL)
    {
        Point temp = topOrLeft;
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            //if invalid point given return false
            if (!m_game.isValid(temp))
            {
                return false;
            }
            //if point is not "open" return false
            if (m_arr[temp.r][temp.c] != '.')
            {
                return false;
            }
            temp.c += 1;
        }
        
        //if ship can be placed, place ship
        for (int j = 0; j < m_game.shipLength(shipId); j++)
        {
            m_arr[topOrLeft.r][topOrLeft.c] = m_game.shipSymbol(shipId);
            topOrLeft.c += 1;
        }
        
        //update shipExtList to say that ship at id has been placed;
        m_shipExtList.at(shipId).hasBeenPlaced = true;
        //ship was successfully placed
        return true;
    }
    //dir == VERTICAL
    else
    {
        Point temp = topOrLeft;
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            //if invalid point given return false
            if (!m_game.isValid(temp))
            {
                return false;
            }
            //if point is not "open" return false
            if (m_arr[temp.r][temp.c] != '.')
            {
                return false;
            }
            temp.r += 1;
        }
        
        //if ship can be placed, place ship
        for (int j = 0; j < m_game.shipLength(shipId); j++)
        {
            m_arr[topOrLeft.r][topOrLeft.c] = m_game.shipSymbol(shipId);
            topOrLeft.r += 1;
        }
        
        //update shipExtList to say that ship at id has been placed;
        m_shipExtList.at(shipId).hasBeenPlaced = true;
        //ship was successfully placed
        return true;
    }
}


bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    //if invalid shipId given return false
    if (shipId < 0 || shipId >= m_game.nShips())
    {
        return false;
    }
    
    //if ship has NOT been placed on the board return false
    if (m_shipExtList.at(shipId).hasBeenPlaced == false)
    {
        return false;
    }
    
    //stores current symbol we are looking for
    char currSymbol = m_game.shipSymbol(shipId);
    
    Point temp = topOrLeft;
    if (dir == HORIZONTAL)
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            //if invlaid point return false
            if (m_game.isValid(temp) == false)
            {
                return false;
            }
            //if char at point isn't the ship symbol, can't remove it
            if (m_arr[temp.r][temp.c] != currSymbol)
            {
                return false;
            }
            temp.c += 1;
        }
        
        //if can be removed, remove ship
        for (int j = 0; j < m_game.shipLength(shipId); j++)
        {
            m_arr[topOrLeft.r][topOrLeft.c] = '.';
            topOrLeft.c += 1;
        }
        
        //update to say ship has not been placed
        m_shipExtList.at(shipId).hasBeenPlaced = false;
        //ship successfully removed
        return true;
    }
    //dir == VERTICAL
    else
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            //if invlaid point return false
            if (m_game.isValid(temp) == false)
            {
                return false;
            }
            //if char at point isn't the ship symbol, can't remove it
            if (m_arr[temp.r][temp.c] != currSymbol)
            {
                return false;
            }
            temp.r += 1;
        }
        //if can be removed, remove ship
        for (int j = 0; j < m_game.shipLength(shipId); j++)
        {
            m_arr[topOrLeft.r][topOrLeft.c] = '.';
            topOrLeft.r += 1;
        }
        
        //update to say ship has not been placed
        m_shipExtList.at(shipId).hasBeenPlaced = false;
        //ship successfully removed
        return true;
    }
}

void BoardImpl::display(bool shotsOnly) const
{
    //start with char '0'
    char startRows = '0';
    cout << "  "; //cout the two spaces
    //fill in the first row
    for (int i = 0; i < m_game.cols(); i++)
    {
        cout << startRows;
        startRows++;
    }
    //finish first row
    cout << endl;
    
    //start for columns
    char startCols = '0';
    
    //cout board
    for (int r = 0; r < m_game.rows(); r++)
    {
        cout << startCols << " ";
        
        for (int c = 0; c < m_game.cols();c++)
        {
            //if shotsOnly == true, don't cout ship symbols
            if (shotsOnly == true)
            {
                if (m_arr[r][c] == '.' || m_arr[r][c] == 'o' || m_arr[r][c] == 'X')
                {
                    cout << m_arr[r][c];
                }
                else
                {
                    cout << '.';
                }
            }
            //shotsOnly == false, cout normal board
            else
            {
                cout << m_arr[r][c];
            }
        }
        //update column number
        startCols++;
        //end row
        cout << endl;
    }
    

}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    //check to see if point is on the board
    if (m_game.isValid(p) == false)
    {
        return false;
    }
    //check to see if already attacked there
    if (m_arr[p.r][p.c] == 'o' || m_arr[p.r][p.c] == 'X')
    {
        return false;
    }

    //check to see if attack hit a ship
    if (m_arr[p.r][p.c] != '.')
    {
        //UPDATE shotHit
        shotHit = true;
        
        int IdOfShip = 0;
        //find Id of ship which was hit
        for (int i = 0; i < m_game.nShips(); i++)
        {
            if (m_game.shipSymbol(i) == m_arr[p.r][p.c])
            {
                IdOfShip = i;
                break;
            }
        }
        
        //decrease the lengthLeft of the ship that was hit
        m_shipExtList[IdOfShip].lengthLeft -= 1;
        
        //if ship destroyed, update parameters accordingly
        if (m_shipExtList[IdOfShip].lengthLeft <= 0)
        {
            shipDestroyed = true;
            shipId = IdOfShip;
        }
        //if ship wasn't destroyed, update parameters accordingly
        else
        {
            shipDestroyed = false;
        }
        
        m_arr[p.r][p.c] = 'X'; //Damaged segment of ship
    }
    //attack missed ship
    else
    {
        shotHit = false;
        shipDestroyed = false;
        m_arr[p.r][p.c] = 'o'; //attack missed, but was valid
    }
    
    //return true if valid point given
    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    for (int i = 0; i < m_game.nShips(); i++)
    {
        //if length left in any given ship is not zero return false
        if (m_shipExtList.at(i).lengthLeft != 0)
            return false;
    }
    //else return true
    return true;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
