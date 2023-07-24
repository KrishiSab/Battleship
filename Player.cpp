//
//  Player.cpp
//  Battleship
//
//  Created by Krishi Sabarwal on 7/18/22.
//

#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
using namespace std;


#include <vector>
#include <stack>


//========================================================================
// Timer t;                 // create a timer and start it
// t.start();               // start the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>

class Timer
{
  public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
                          std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
  private:
    std::chrono::high_resolution_clock::time_point m_time;
};

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

bool getLineWithOneChar(char& c)
{
    bool result(cin >> c);
    if (!result)
        cin.clear();
    cin.ignore(10000, '\n');
    return result;
}


// TODO:  You need to replace this with a real class declaration and
//        implementation.
class HumanPlayer: public Player
{
  public:
    HumanPlayer(string nm, const Game& g);
    virtual ~HumanPlayer();
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    
};

HumanPlayer::HumanPlayer(string nm, const Game& g)
: Player(nm, g)
{}

HumanPlayer::~HumanPlayer()
{}

bool HumanPlayer::isHuman() const
{
    return true;
}


bool HumanPlayer::placeShips(Board& b)
{
    //tell user how many ships they must place
    cout << name() << " must place " << game().nShips() << " ships." << endl;
    
    for (int i = 0; i < game().nShips(); i++)
    {
        //display the board
        b.display(false);
        
        //stores direction
        char dir = 'a';
        
        while (dir != 'h' && dir != 'v')
        {
            //prompt for direction
            cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
            
            getLineWithOneChar(dir);
            
            if (dir == 'h' || dir == 'v')
            {
                break;
            }
            else
            {
                cout << "Direction must be h or v." << endl;
            }
            
        }
        
        bool a = false;
        
        while (a == false)
        {
            //cout appropriate thing;
            if (dir == 'h')
            {
                cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
            }
            //dir == 'v'
            else
            {
                cout << "Enter row and column of topmost cell (e.g., 3 5): ";
            }
            
            //get the point of placement
            int r = -1, c = -1;
            a = getLineWithTwoIntegers(r, c);
            
            //see if valid input was given
            if (!a)
                cout << "You must enter two integers." << endl;
            else
            {
                if (dir == 'h')
                {
                    a = b.placeShip(Point(r,c), i, HORIZONTAL);
                }
                else
                {
                    a = b.placeShip(Point(r,c), i, VERTICAL);
                }
                
                if (a == false)
                    cout << "The ship can not be placed there." << endl;
            }
            
        }
        
    }
    
    return true;
}

Point HumanPlayer::recommendAttack()
{
    //will create the point
    int r = -1,c = -1;
    //if a == false, keep prompting user for data
    bool a = false;
    while (a == false)
    {
        cout << "Enter the row and column to attack (e.g., 3 5): ";
        a = getLineWithTwoIntegers(r, c);
        //if integers weren't give
        if (!a)
            cout << "You must enter two integers." << endl;
    }
    
    return Point(r,c);
}




//does nothing for HumanPlayer
void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,bool shipDestroyed, int shipId)
{
    return;
}

//does nothing for HumanPlayer
void HumanPlayer::recordAttackByOpponent(Point p)
{
    return;
}





//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.

class MediocrePlayer: public Player
{
  public:
    MediocrePlayer (string nm, const Game& g);
    virtual ~MediocrePlayer();
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    
  private:
    bool placeShipsHelper(Board& b, int shipId); //performs the recursion
    int state; //keeps track of state
    vector<Point> vp; //list of all used points
    vector<Point> specialAttacks; //list of points that shoudl be attacked in state 2
    
    
};

//initially player is in state 1
MediocrePlayer::MediocrePlayer(string nm, const Game& g)
:Player(nm, g)
{
    //set state
    state = 1;
}

MediocrePlayer::~MediocrePlayer()
{}


bool MediocrePlayer::placeShips(Board& b)
{
    int i = 0;
    //try placing the ships 50 times
    while (i < 50)
    {
        b.block();
        
        if (placeShipsHelper(b, 0))
        {
            /**/b.display(false);
            b.unblock();
            return true;
        }

        b.unblock();
        i++;
    }
    
    //if after 50 tries the ships are sill not able to be placed return false
    return false;
}

//===========================================================================================
bool MediocrePlayer::placeShipsHelper(Board& b, int shipId)
{
    //base case: if all ships are placed return true
    if (shipId >= game().nShips())
        return true;
    
    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
        {
            Point temp(r,c);
            
            //if placing the ship horizontally returns true
            if (b.placeShip(temp, shipId, HORIZONTAL))
            {
                if(placeShipsHelper(b, shipId + 1)) //check to see if all other ships can be placed
                    return true;
                
                b.unplaceShip(temp, shipId, HORIZONTAL);
            }
            
            //if placing the ship vertically returns true
            if (b.placeShip(temp, shipId, VERTICAL))
            {
                if(placeShipsHelper(b, shipId + 1)) //check to see if all other ships can be placed
                    return true;
                
                b.unplaceShip(temp, shipId, VERTICAL);
            }
        }
    }
    
    
    //if ships are unable to be placed return false
    return false;
}
//===========================================================================================

Point MediocrePlayer::recommendAttack()
{
    if (state == 1)
    {
        //rows and columns
        int r = -1;
        int c = -1;
        
        //return a random point which has NOT been used
        bool hasBeenUsed = true;
        
        //check to see if the point was already used
        for(;hasBeenUsed;)
        {
            hasBeenUsed = false;
            r = randInt(game().rows());
            c = randInt(game().cols());
            for (int i = 0; i < vp.size(); i++)
            {
                if (vp[i].r == r && vp[i].c == c)
                    hasBeenUsed = true;
            }
        }
        
        return Point(r,c);
    }
    
    //state 2
    else
    {
        //get random point from specialAttacks
        int x = randInt(specialAttacks.size());
        Point temp = specialAttacks[x];
        
        //delete point from specialAttacks
        vector<Point>::iterator p = specialAttacks.begin();
        for (int i = 0; i < x; i++)
            p++;
        
        if (!specialAttacks.empty())
            specialAttacks.erase(p);
        
        return temp;
    }
    
    
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    //add point to vector of points already used
    vp.push_back(p);
    
    if (state == 1)
    {
        if(validShot == true && shotHit == true && shipDestroyed == false)
        {
            for (int i = 1; i < 5; i++)
            {
                //check if certain point can be placed
                bool canBePlacedUp = true;
                bool canBePlacedDown = true;
                bool canBePlacedRight = true;
                bool canBePlacedLeft = true;
                
                //create points in the 4 neighboring directions
                Point up(p.r-i,p.c);
                Point down(p.r+i,p.c);
                Point right(p.r,p.c+i);
                Point left(p.r,p.c-i);
                
                //check if the points have already been used
                for (int j = 0; j < vp.size(); j++)
                {
                    if (vp[j].r == up.r && vp[j].c == up.c)
                        canBePlacedUp = false;
                    if (vp[j].r == down.r && vp[j].c == down.c)
                        canBePlacedDown = false;
                    if (vp[j].r == right.r && vp[j].c == right.c)
                        canBePlacedRight = false;
                    if (vp[j].r == left.r && vp[j].c == left.c)
                        canBePlacedLeft = false;
                }
                
                //if points haven't been used add them to special attacks
                if (game().isValid(up) && canBePlacedUp)
                    specialAttacks.push_back(up);
                if (game().isValid(down) && canBePlacedDown)
                    specialAttacks.push_back(down);
                if (game().isValid(right) && canBePlacedRight)
                    specialAttacks.push_back(right);
                if (game().isValid(left) && canBePlacedLeft)
                    specialAttacks.push_back(left);
                
            }
            
            
            
            if (!specialAttacks.empty())
                state = 2; //switch to state 2 if specialAttacks has something in it
        }
    }
    else
    {
        //check if specialAttacks is empty
        if (specialAttacks.empty())
            state = 1;
        //if specialAttacks is not empty
        else
        {
            if (validShot == true && shotHit == true && shipDestroyed == true)
            {
                //clear vector
                specialAttacks.clear();
                //change state
                state = 1;
            }
        }
    }
    
    
    

}


//does nothign for MediocrePlayer
void MediocrePlayer::recordAttackByOpponent(Point p)
{
    return;
}


// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.








//*********************************************************************
//  GoodPlayer
//*********************************************************************


// TODO:  You need to replace this with a real class declaration and
//        implementation.
class GoodPlayer: public Player
{
  public:
    GoodPlayer(string nm, const Game& g);
    virtual ~GoodPlayer();
    
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    

  private:
    bool placeShipsHelper(Board& b, int shipId); //performs the recursion
    bool hasNotBeenUsed(Point p); //returns true if point has not been used
    Direction dir; //what direction is the ship
    int state; //keeps track of state
    int positiveOrNegative; //helps right/left OR up/down if dir == Horizontal/vertical
    vector<Point> vp; //list of all available points
    stack<Point> specialAttacks; //stack of points that should be attacked in state 3
    Point hit; //"records the point hit"
};

//===========================================================================================
GoodPlayer::GoodPlayer(string nm, const Game& g)
:Player(nm, g)
{
    //set state to 1
    state = 1;
}

GoodPlayer::~GoodPlayer()
{}

//========================================================================================

bool GoodPlayer::hasNotBeenUsed(Point p)
{
    for (int i = 0; i < vp.size(); i++)
    {
        if (vp[i].r == p.r && vp[i].c == p.c)
            return false;
    }
    
    return true;
}


//========================================================================================



bool GoodPlayer::placeShips(Board& b)
{
    Timer t;
    t.start();
    
    //try placing the ships using similar pattern as mediocre player
    while (t.elapsed() < 900)
    {
        b.block();
        
        if (placeShipsHelper(b, 0))
        {
            b.unblock();
            return true;
        }
        
        b.unblock();
    }
    
    //if timer is about to run out,
    //check to see if it is possible to place all the ships on the given board with no blocking
    return placeShipsHelper(b, 0);
}

//================================================================================================
bool GoodPlayer::placeShipsHelper(Board& b, int shipId)
{
    //base case: if all ships are placed return true
    if (shipId >= game().nShips())
        return true;
    
    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
        {
            Point temp(r,c);
            
            //if placing the ship horizontally returns true
            if (b.placeShip(temp, shipId, HORIZONTAL))
            {
                if(placeShipsHelper(b, shipId + 1)) //check to see if all other ships can be placed
                    return true;
                
                b.unplaceShip(temp, shipId, HORIZONTAL);
            }
            
            //if placing the ship vertically returns true
            if (b.placeShip(temp, shipId, VERTICAL))
            {
                if(placeShipsHelper(b, shipId + 1)) //check to see if all other ships can be placed
                    return true;
                
                b.unplaceShip(temp, shipId, VERTICAL);
            }
        }
    }
    
    //if ships are unable to be placed return false
    return false;
}

//===========================================================================================================
Point GoodPlayer::recommendAttack()
{
    //keeps track of difference between rows and cols
    int diffRows;
    int diffCols;
    
    switch (state)
    {
    //state 1
            //returns a random point
    case 1:
        do {
            diffRows = randInt(game().rows());
            diffCols = randInt(game().cols());
        } while (!hasNotBeenUsed(Point(diffRows,diffCols)));
        return Point(diffRows, diffCols);
        break;
    //state 2 & 3
    case 2:
    case 3:
        if (specialAttacks.empty())
        {
            state = 1;
            do {
                diffRows = randInt(game().rows());
                diffCols = randInt(game().cols());
            } while (!hasNotBeenUsed(Point(diffRows, diffCols)));
            return Point(diffRows, diffCols);
        }
            //get top of stack
        Point temp = specialAttacks.top();
         //delete top of stack
        specialAttacks.pop();
            
            
         //return top most point
        return temp;
            
        break;
    }
    
    //return random point so there is a valid point through each path
    return Point(0,0);
}


//===========================================================================================================

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    //add point to the points that have been used
    vp.push_back(p);
    
    
    if (state == 1 && validShot && shotHit && shipDestroyed == false)
    {
        //set hit to the point p
        hit = p;

       
        
        //push the directions of all 4 points
        Point right(p.r, p.c + 1);
        Point left(p.r, p.c - 1);
        Point up(p.r - 1, p.c);
        Point down(p.r + 1, p.c);
        
        //push the 4 points on the stack
        if (game().isValid(left) && hasNotBeenUsed(left))
            specialAttacks.push(left);
        if (game().isValid(right) && hasNotBeenUsed(right))
            specialAttacks.push(right);
        if (game().isValid(up) && hasNotBeenUsed(up))
            specialAttacks.push(up);
        if (game().isValid(down) && hasNotBeenUsed(down))
            specialAttacks.push(down);
        
        //change state to 2
        state = 2;
        return;
    }
    
    else if (state == 2 && validShot && shotHit && shipDestroyed == false)
    {
        //change state to 3
        state = 3;
        
        //keeps track of change in rows/cols
        int diffCols = p.c - hit.c, diffRows = p.r - hit.r;
                                        
        if (diffRows != 0)
        {
            dir = VERTICAL;
            positiveOrNegative = diffRows;
        }
        else
        {
            positiveOrNegative = diffCols;
            dir = HORIZONTAL;
        }
    }
    
    if (state == 3 && validShot == true && shotHit == true && shipDestroyed == false)
    {
        if (dir == VERTICAL)
        {
            if (hasNotBeenUsed(Point(p.r + positiveOrNegative, p.c)) && game().isValid(Point(p.r + positiveOrNegative, p.c)))
            {
                specialAttacks.push(Point(p.r + positiveOrNegative, p.c));
            }
                
            else
            {
                positiveOrNegative *= -1;
                if (game().isValid(Point(hit.r + positiveOrNegative, p.c)) && hasNotBeenUsed(Point(hit.r + positiveOrNegative, p.c)))
                specialAttacks.push(Point(hit.r + positiveOrNegative, p.c));
            }
            
        }
        
        //replica of above code
        else if (dir == HORIZONTAL)
        {
            //if dir horizontal add posOrNeg
            if (hasNotBeenUsed(Point(p.r, p.c + positiveOrNegative)) && game().isValid(Point(p.r, p.c + positiveOrNegative)))
            {
                specialAttacks.push(Point(p.r, p.c + positiveOrNegative));
            }
            
            //POINT INVALID/opposite direction
            else
            {
                positiveOrNegative *= -1;
                if (game().isValid(Point(hit.r, hit.c + positiveOrNegative)) && hasNotBeenUsed(Point(hit.r, hit.c + positiveOrNegative)))
                    specialAttacks.push(Point(hit.r, hit.c + positiveOrNegative));
                
            }
        }
        
        //make sure that stack isn't empty
        if (specialAttacks.empty())                                                                                   state = 1; // if empty move back to state 1
        return;
    }
    
    //shot missed in state 3
    if (state == 3 && validShot && shotHit == false)
    {
        //change dir
        positiveOrNegative *= -1;
        
        //push point on stack if dir Horizontal
        Point temp(hit.r, hit.c + positiveOrNegative);
        if (dir == HORIZONTAL && hasNotBeenUsed(temp) && game().isValid(temp))
            specialAttacks.push(Point(hit.r, hit.c + positiveOrNegative));
        
        //push point on stack if dir Vertical
        Point temp1(hit.r + positiveOrNegative, hit.c);
        if (dir == VERTICAL && hasNotBeenUsed(temp1) && game().isValid(temp1))
            specialAttacks.push(temp1);
        
        //make sure stack isn't empty
        if (specialAttacks.empty())
            state = 1; //if empty change to state 1
        return;
    }
    
    //if ship was destroyed in either state 2 or 3
    if ((state == 2 || state == 3) && validShot == true && shotHit == true && shipDestroyed == true)
    {
        //change state to 1
        state = 1;
        
        //clear stack
        while (!specialAttacks.empty())
            specialAttacks.pop();
    }
}


//========================================================================================
//does nothing for good player
void GoodPlayer::recordAttackByOpponent(Point p)
{
    return;
}
//========================================================================================






//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}

