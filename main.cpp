//
//  main.cpp
//  Battleship
//
//  Created by Krishi Sabarwal on 7/18/22.
//

#include "globals.h"
#include "Board.h"
#include "Player.h"
#include "Game.h"
#include <iostream>
#include <sstream>
#include <streambuf>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cassert>

using namespace std;

bool operator<(Point lhs, Point rhs)
{
    if (lhs.r < rhs.r) return true;
    if (rhs.r < lhs.r) return false;
    return lhs.c < rhs.c;
}

bool allShipsPlaced(Board& b, const Game& g)
{
    set<Point> hits;
    vector<bool> destroyedShips(g.nShips(), false);
    for (int r = 0; r < g.rows(); r++)
    {
        for (int c = 0; c < g.cols() ; c++)
        {
            bool hit;
            bool destroyed;
            int id;
            b.attack(Point(r,c), hit, destroyed, id);
            if (hit)
            {
                hits.insert(Point(r,c));
                if (destroyed)
                {
                    assert(!destroyedShips[id]);
                    destroyedShips[id] = true;
                }
            }
        }
    }
    int t = 0;
    for (int s = 0; s < g.nShips(); s++)
        t += g.shipLength(s);
    return hits.size() == t  &&  find(destroyedShips.begin(),
                    destroyedShips.end(), false) == destroyedShips.end();
}

class AwfulPlayer2 : public Player
{
  public:
    AwfulPlayer2(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                               bool shipDestroyed, int shipId)
    {}
    virtual void recordAttackByOpponent(Point p)
    {}
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer2::AwfulPlayer2(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer2::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer2::recommendAttack()
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

void testone(int n)
{
    Game g(10, 10);
    g.addShip(5, 'A', "aircraft carrier");
    g.addShip(4, 'B', "battleship");
    g.addShip(3, 'D', "destroyer");
    g.addShip(3, 'S', "submarine");
    g.addShip(2, 'P', "patrol boat");
    map<int,vector<int> > lenToShipMap;
    for (int s = 0; s < g.nShips(); s++)
        lenToShipMap[g.shipLength(s)].push_back(s);
    Board b(g);

    switch (n)
    {
                     case  1: {
        bool anyfail = false;
        for (int k = 0; k < 100; k++)
        {
            Board b2(g);
            b2.block();
            if (!b2.placeShip(Point(0,0), lenToShipMap[5][0], HORIZONTAL))
            {
                anyfail = true;
                break;
            }
        }
        assert(anyfail);
        assert(b.placeShip(Point(0,0), 0, HORIZONTAL));  // ensure returning true is possible
            } break; case  2: {
        bool anyfail = false;
        for (int k = 0; k < 100; k++)
        {
            b.block();
            b.unblock();
            if (!b.placeShip(Point(0,0), lenToShipMap[5][0], HORIZONTAL))
            {
                anyfail = true;
                break;
            }
            b.unplaceShip(Point(0,0), lenToShipMap[5][0], HORIZONTAL);
        }
        assert(!anyfail);
        assert(!b.placeShip(Point(0,9), lenToShipMap[5][0], HORIZONTAL) ||
            !b.placeShip(Point(0,8), lenToShipMap[4][0], HORIZONTAL)); // ensure returning false is possible
            } break; case  3: {
        assert(!b.placeShip(Point(-1,0), lenToShipMap[5][0], HORIZONTAL));
        assert(!b.placeShip(Point(0,-1), lenToShipMap[5][0], HORIZONTAL));
        assert(b.placeShip(Point(0,0), 0, HORIZONTAL));  // ensure returning true is possible
            } break; case  4: {
        assert(!b.placeShip(Point(0,0), -1, HORIZONTAL));
        assert(b.placeShip(Point(0,0), 0, HORIZONTAL));  // ensure returning true is possible
            } break; case  5: {
        assert(!b.placeShip(Point(0,0), 5, HORIZONTAL));
        assert(b.placeShip(Point(0,0), 0, HORIZONTAL));  // ensure returning true is possible
            } break; case  6: {
        assert(!b.placeShip(Point(0,6), lenToShipMap[5][0], HORIZONTAL));
        assert(b.placeShip(Point(0,0), 0, HORIZONTAL));  // ensure returning true is possible
            } break; case  7: {
        assert(!b.placeShip(Point(7,0), lenToShipMap[4][0], VERTICAL));
        assert(b.placeShip(Point(0,0), 0, HORIZONTAL));  // ensure returning true is possible
            } break; case  8: {
        assert(b.placeShip(Point(3,5), lenToShipMap[5][0], HORIZONTAL));
        assert(!b.placeShip(Point(0,7), lenToShipMap[4][0], VERTICAL));
            } break; case  9: {
        assert(b.placeShip(Point(3,4), lenToShipMap[5][0], HORIZONTAL));
        assert(!b.placeShip(Point(0,7), lenToShipMap[4][0], VERTICAL));
        assert(b.placeShip(Point(0,6), lenToShipMap[3][0], HORIZONTAL));
        assert(b.placeShip(Point(1,6), lenToShipMap[3][1], HORIZONTAL));
        assert(b.placeShip(Point(2,6), lenToShipMap[2][0], HORIZONTAL));
            } break; case 10: {
        assert(b.placeShip(Point(3,4), lenToShipMap[5][0], HORIZONTAL));
        assert(!b.unplaceShip(Point(3,4), 5, HORIZONTAL));
            } break; case 11: {
        assert(b.placeShip(Point(3,4), lenToShipMap[5][0], HORIZONTAL));
        assert(!b.unplaceShip(Point(3,4), lenToShipMap[4][0], HORIZONTAL));
            } break; case 12: {
        assert(b.placeShip(Point(3,4), lenToShipMap[5][0], HORIZONTAL));
        assert(!b.unplaceShip(Point(3,5), lenToShipMap[5][0], HORIZONTAL));
            } break; case 13: {
        assert(b.placeShip(Point(3,4), lenToShipMap[5][0], HORIZONTAL));
        assert(b.unplaceShip(Point(3,4), lenToShipMap[5][0], HORIZONTAL));
        assert(b.placeShip(Point(3,2), lenToShipMap[4][0], HORIZONTAL));
            } break; case 14: {
        Game g2(1, 7);
        g2.addShip(5, 'A', "aircraft carrier");
        g2.addShip(2, 'P', "patrol boat");
        Board b2(g2);
        lenToShipMap.clear();
        for (int s = 0; s < g2.nShips(); s++)
            lenToShipMap[g2.shipLength(s)].push_back(s);
        assert(!b2.placeShip(Point(0,3), lenToShipMap[5][0], HORIZONTAL));
        assert(!b2.placeShip(Point(0,1), lenToShipMap[2][0], VERTICAL));
        assert(b.placeShip(Point(0,0), 0, HORIZONTAL));  // ensure returning true is possible
            } break; case 15: {
        assert(b.placeShip(Point(3,4), lenToShipMap[3][0], HORIZONTAL));
        bool hit = false;
        bool destroyed = false;
        int id = -999;
        assert(!b.attack(Point(-1,0), hit, destroyed, id));
        assert(!b.attack(Point(0,10), hit, destroyed, id));
        assert(b.attack(Point(3,4), hit, destroyed, id));
            } break; case 16: {
        assert(b.placeShip(Point(3,4), lenToShipMap[3][0], HORIZONTAL));
        bool hit = true;
        bool destroyed = false;
        int id = -999;
        assert(b.attack(Point(3,7), hit, destroyed, id));
        assert(!hit);
            } break; case 17: {
        assert(b.placeShip(Point(3,4), lenToShipMap[3][0], HORIZONTAL));
        bool hit = false;
        bool destroyed = true;
        int id = -999;
        assert(b.attack(Point(3,6), hit, destroyed, id));
        assert(hit  &&  !destroyed);
            } break; case 18: {
        assert(b.placeShip(Point(3,4), lenToShipMap[3][0], HORIZONTAL));
        bool hit = false;
        bool destroyed = true;
        int id = -999;
        assert(b.attack(Point(3,6), hit, destroyed, id));
        assert(!b.attack(Point(3,6), hit, destroyed, id));
            } break; case 19: {
        assert(b.placeShip(Point(3,4), lenToShipMap[3][0], HORIZONTAL));
        bool hit = false;
        bool destroyed = true;
        int id = -999;
        assert(b.attack(Point(3,6), hit, destroyed, id));
        assert(hit  &&  !destroyed);
        hit = false;
        destroyed = true;
        assert(b.attack(Point(3,4), hit, destroyed, id));
        assert(hit  &&  !destroyed);
        hit = false;
        destroyed = true;
        assert(b.attack(Point(3,5), hit, destroyed, id));
        assert(hit  &&  destroyed  &&  id == lenToShipMap[3][0]);
            } break; case 20: {
        Game g2(10, 10);
        g2.addShip(3, 'S', "submarine");
        g2.addShip(2, 'P', "patrol boat");
        Board b2(g2);
        lenToShipMap.clear();
        for (int s = 0; s < g2.nShips(); s++)
            lenToShipMap[g2.shipLength(s)].push_back(s);
        assert(b2.placeShip(Point(3,5), lenToShipMap[3][0], HORIZONTAL));
        assert(b2.placeShip(Point(4,5), lenToShipMap[2][0], HORIZONTAL));
        bool hit = false;
        bool destroyed = true;
        int id = -999;
        assert(!b2.allShipsDestroyed());
        assert(b2.attack(Point(3,5), hit, destroyed, id));
        assert(!b2.allShipsDestroyed());
        assert(b2.attack(Point(3,6), hit, destroyed, id));
        assert(!b2.allShipsDestroyed());
        assert(b2.attack(Point(4,5), hit, destroyed, id));
        assert(!b2.allShipsDestroyed());
        assert(b2.attack(Point(4,6), hit, destroyed, id));
        assert(!b2.allShipsDestroyed());
        assert(b2.attack(Point(3,7), hit, destroyed, id));
        assert(b2.allShipsDestroyed());
            } break; case 21: {
        Game g2(10, 10);
        g2.addShip(2, 'P', "patrol boat");
        Board b2(g2);
        Player* m = createPlayer("mediocre", "Mimi", g2);
        assert(m != nullptr);
        assert(m->placeShips(b2));
        assert(allShipsPlaced(b2, g2));
            } break; case 22: {
        Game g2(10, 10);
        g2.addShip(2, 'A', "PT-A");
        g2.addShip(2, 'B', "PT-B");
        g2.addShip(2, 'C', "PT-C");
        g2.addShip(2, 'D', "PT-D");
        Board b2(g2);
        Player* m = createPlayer("mediocre", "Mimi", g2);
        assert(m != nullptr);
        assert(m->placeShips(b2));
        assert(allShipsPlaced(b2, g2));
            } break; case 23: {
        Game g2(4, 3);
        g2.addShip(2, 'A', "PT-A");
        g2.addShip(2, 'B', "PT-B");
        g2.addShip(2, 'C', "PT-C");
        g2.addShip(2, 'D', "PT-D");
        Board b2(g2);
        Player* m = createPlayer("mediocre", "Mimi", g2);
        assert(m != nullptr);
        assert(!m->placeShips(b2));
        Game g3(10, 10);
        g3.addShip(2, 'P', "patrol boat");
        Board b3(g3);
        Player* m3 = createPlayer("mediocre", "Miriam", g3);
        assert(m3 != nullptr);
        assert(m3->placeShips(b3)); // ensure returning true is possible
            } break;
        case 24: {
        Player* m = createPlayer("mediocre", "Mimi", g);
        assert(m != nullptr);
        assert(m->placeShips(b));
        assert(allShipsPlaced(b, g));
            } break;
        
        
        
        
        
        
        
        case 25: {
        Game g2(1, 10);
        g2.addShip(5, 'A', "aircraft carrier");
        
        bool hasSucceeded = false;
        bool hasFailed = false;
            // Prob of succeeding is 1/42 per try; for 50 tries, is .7.
            // With 30 trials, prob of not having at least one of each
            // outcome is about 2 out of 100,000.
        for (int k = 0; k < 30; k++)
        {
            Board b2(g2);
            Player* m = createPlayer("mediocre", "Mimi", g2);
            assert(m != nullptr);
            if (m->placeShips(b2))
            {
                hasSucceeded = allShipsPlaced(b2, g2);
                if (hasFailed)
                    break;
            }
            else
            {
                hasFailed = true;
                if (hasSucceeded)
                    break;
            }
            delete m;
        }
            
            
        assert(hasSucceeded  &&  hasFailed);
            } break;
        
        
        
        
        
        
        
        
        
        
        
        case 26: {
        Game g2(10, 10);
        g2.addShip(5, 'A', "aircraft carrier");
        Board b2(g2);
        Player* m = createPlayer("mediocre", "Mimi", g2);
        assert(m != nullptr);
        m->placeShips(b2);
        Board b3(g2);
        b3.placeShip(Point(8,3), 0, HORIZONTAL);
        set<Point> attacked;
        set<Point> hit;
        set<Point> state2attacks;
        Point firsthit;
        int state = 1;
        while (attacked.size() != 100)
        {
            Point p = m->recommendAttack();
            assert(g2.isValid(p)  &&
                    attacked.find(p) == attacked.end());
            attacked.insert(p);
            if (p.r == 8  &&  p.c >= 3  &&  p.c <= 7)
            {
                if (state == 1)
                {
                    firsthit = p;
                    state = 2;
                }
                hit.insert(p);
                if (hit.size() == 5)
                    break;
                m->recordAttackResult(p, true, true, false, -9);
            }
            else
                m->recordAttackResult(p, true, false, true, -9);
            if (state == 2)
                state2attacks.insert(p);
        }
        assert(attacked.size() != 100);
        Point cross[14] = {  // some here are invalid
            Point(4, firsthit.c), Point(5, firsthit.c),
            Point(6, firsthit.c), Point(7, firsthit.c),
            Point(8, firsthit.c-4), Point(8, firsthit.c-3),
            Point(8, firsthit.c-2), Point(8, firsthit.c-1),
            firsthit,
            Point(8, firsthit.c+1), Point(8, firsthit.c+2),
            Point(8, firsthit.c+3), Point(8, firsthit.c+4),
            Point(9, firsthit.c)
        };
        assert(includes(cross, cross+14,
                state2attacks.begin(), state2attacks.end()));
            } break; case 27: {
        int bound[2] = { 3, 5 };
        for (int k = 0; k <= 1; k++)
        {
            Game g2(bound[k], bound[1-k]);
            g2.addShip(4, 'A', "PT-A");
            g2.addShip(3, 'B', "PT-B");
            g2.addShip(2, 'C', "PT-C");
            g2.addShip(2, 'D', "PT-D");
            g2.addShip(2, 'E', "PT-E");
            g2.addShip(2, 'F', "PT-F");
            Board b2(g2);
            Player* gp = createPlayer("good", "Gudrun", g2);
            assert(gp != nullptr);
            assert(gp->placeShips(b2)  &&  allShipsPlaced(b2, g2));
        }
            } break; case 28: {
        Game g2(5, 3);
        g2.addShip(5, 'A', "PT-A");
        g2.addShip(3, 'B', "PT-B");
        g2.addShip(3, 'C', "PT-C");
        g2.addShip(3, 'D', "PT-D");
        Board b2(g2);
        Player* gp = createPlayer("good", "Gudrun", g2);
        assert(gp != nullptr);
        assert(!gp->placeShips(b2));
        Game g3(4, 3);
        g3.addShip(4, 'A', "PT-A");
        g3.addShip(3, 'B', "PT-B");
        g3.addShip(3, 'C', "PT-C");
        g3.addShip(2, 'D', "PT-D");
        Board b3(g3);
        Player* gp3 = createPlayer("good", "Gunter", g3);
        assert(gp3 != nullptr);
        assert(gp3->placeShips(b3)  &&  allShipsPlaced(b3, g3));  // ensure returning true is possible
            } break; case 29: {
        Player* gp = createPlayer("good", "Gudrun", g);
        assert(gp != nullptr);
        for (int n = 0; n < 10; n++)
        {
            Board b2(g);
            assert(gp->placeShips(b2)  &&  allShipsPlaced(b2, g));
        }
            } break; case 30: {
        Board b2(g);
        Player* gp = createPlayer("good", "Gudrun", g);
        assert(gp != nullptr);
        assert(gp->placeShips(b2));
        b.placeShip(Point(2,7), lenToShipMap[5][0], VERTICAL);
        b.placeShip(Point(5,2), lenToShipMap[4][0], VERTICAL);
        b.placeShip(Point(2,1), lenToShipMap[3][0], VERTICAL);
        b.placeShip(Point(1,4), lenToShipMap[3][1], HORIZONTAL);
        b.placeShip(Point(7,8), lenToShipMap[2][0], HORIZONTAL);
        for (int k = 0; k < 100  &&  !b.allShipsDestroyed(); k++)
        {
            Point p = gp->recommendAttack();
            bool hit;
            bool destroyed;
            int id;
            assert(b.attack(p, hit, destroyed, id));
            gp->recordAttackResult(p, true, hit, destroyed, id);
            gp->recordAttackByOpponent(Point(randInt(10), randInt(10)));
        }
        assert(b.allShipsDestroyed());
            } break; case 31: {
        int nMediocreWins = 0;
        for (int k = 1; k <= 20; k++)
        {
            Game g2(10, 10);
            g2.addShip(5, 'A', "aircraft carrier");
            g2.addShip(4, 'B', "battleship");
            g2.addShip(3, 'D', "destroyer");
            g2.addShip(3, 'S', "submarine");
            g2.addShip(2, 'P', "patrol boat");
            AwfulPlayer2 p1("Audrey", g2);
            Player* p2 = createPlayer("mediocre", "Mimi", g2);
            Player* winner = (k % 2 == 1 ?
                             g2.play(&p1, p2, false) : g2.play(p2, &p1, false));
            if (winner == p2)
                nMediocreWins++;
            delete p2;
        }
        assert(nMediocreWins >= 16);
            } break; case 32: {
        int nGoodWins = 0;
        for (int k = 1; k <= 20; k++)
        {
            Game g2(10, 10);
            g2.addShip(5, 'A', "aircraft carrier");
            g2.addShip(4, 'B', "battleship");
            g2.addShip(3, 'D', "destroyer");
            g2.addShip(3, 'S', "submarine");
            g2.addShip(2, 'P', "patrol boat");
            AwfulPlayer2 p1("Audrey", g2);
            Player* p2 = createPlayer("good", "Gudrun", g2);
            Player* winner = (k % 2 == 1 ?
                             g2.play(&p1, p2, false) : g2.play(p2, &p1, false));
            if (winner == p2)
                nGoodWins++;
            delete p2;
        }
        assert(nGoodWins >= 16);
            } break; case 33: {
        Game g2(2,2);
        g2.addShip(2, 'R', "boat");
        AwfulPlayer2 p1("A1", g2);
        AwfulPlayer2 p2("A2", g2);
        istringstream iss(string(30, '\n'));
        streambuf* sb = cin.rdbuf(iss.rdbuf());
        Player* winner = g2.play(&p1, &p2, false);
        cin.rdbuf(sb);
        assert(winner == &p1);
            } break;
    }
}

int main()
{
    cout << "Enter test number: ";
    int n;
    cin >> n;
    streambuf* sb = cout.rdbuf(cerr.rdbuf());
    testone(n);
    cout.rdbuf(sb);
    cout << "Passed" << endl;
}
/*#include "Game.h"
#include "Player.h"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;


#include "Board.h"




bool addStandardShips(Game& g)
{
    return g.addShip(5, 'A', "aircraft carrier")  &&
           g.addShip(4, 'B', "battleship")  &&
           g.addShip(3, 'D', "destroyer")  &&
           g.addShip(3, 'S', "submarine")  &&
           g.addShip(2, 'P', "patrol boat");
}

int main()
{
    const int NTRIALS = 200;

    cout << "Select one of these choices for an example of the game:" << endl;
    cout << "  1.  A mini-game between two mediocre players" << endl;
    cout << "  2.  A mediocre player against a human player" << endl;
    cout << "  3.  A " << NTRIALS
         << "-game match between a mediocre and an awful player, with no pauses"
         << endl;
    cout << "Enter your choice: ";
    string line;
    getline(cin,line);
    if (line.empty())
    {
        cout << "You did not enter a choice" << endl;
    }
    else if (line[0] == '1')
    {
        Game g(2, 3);
        g.addShip(2, 'R', "rowboat");
        Player* p1 = createPlayer("mediocre", "Popeye", g);
        Player* p2 = createPlayer("mediocre", "Bluto", g);
        cout << "This mini-game has one ship, a 2-segment rowboat." << endl;
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '2')
    {
        Game g(10, 10);
        addStandardShips(g);
        Player* p1 = createPlayer("mediocre", "Mediocre Midori", g);
        Player* p2 = createPlayer("human", "Shuman the Human", g);
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '3')
    {
        int nMediocreWins = 0;

        for (int k = 1; k <= NTRIALS; k++)
        {
            cout << "============================= Game " << k
                 << " =============================" << endl;
            Game g(10, 10);
            addStandardShips(g);
            Player* p1 = createPlayer("awful", "Awful Audrey", g);
            Player* p2 = createPlayer("mediocre", "Mediocre Mimi", g);
            Player* winner = (k % 2 == 1 ?
                                g.play(p1, p2, false) : g.play(p2, p1, false));
            if (winner == p2)
                nMediocreWins++;
            delete p1;
            delete p2;
        }
        cout << "The mediocre player won " << nMediocreWins << " out of "
             << NTRIALS << " games." << endl;
          // We'd expect a mediocre player to win most of the games against
          // an awful player.  Similarly, a good player should outperform
          // a mediocre player.
    }
    else if (line[0] == '4')
    {
        int nGoodWins = 0;

        for (int k = 1; k <= NTRIALS; k++)
        {
            cout << "============================= Game " << k
                 << " =============================" << endl;
            Game g(10, 10);
            addStandardShips(g);
            Player* p1 = createPlayer("mediocre", "Medicore Mini", g);
            Player* p2 = createPlayer("good", "Good Goober", g);
            Player* winner = (k % 2 == 1 ?
                                g.play(p1, p2, false) : g.play(p2, p1, false));
            if (winner == p2)
                nGoodWins++;
            delete p1;
            delete p2;
        }
        cout << "The good player won " << nGoodWins << " out of "
             << NTRIALS << " games." << endl;
          // We'd expect a mediocre player to win most of the games against
          // an awful player.  Similarly, a good player should outperform
          // a mediocre player.
    }
    else
    {
       cout << "That's not one of the choices." << endl;
    }
}
 */
