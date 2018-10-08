#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cstdlib>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
    
}

StudentWorld::~StudentWorld()
{
    vector<Actor*>::iterator it;
    for(it = m_actor.begin(); it != m_actor.end();)
    {
        delete(*it);
        it = m_actor.erase(it);
    }
    m_actor.clear();
}

int StudentWorld::init()
{
    m_player = new NachenBlaster(this);
    m_actor.push_back(m_player);
    
    m_aliensDestroyed = 0;
    m_aliensOnMap = 0;
    
    for(int i = 0; i < 30; i++) {

        int starX = randInt(0, VIEW_WIDTH-1);
        int starY = randInt(0, VIEW_HEIGHT-1);

        int n = randInt(5, 50);
        double size = n/100.0;
        Star* stars = new Star(this, starX, starY, size);
        m_actor.push_back(stars);
    }

    updateStats();
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    
    vector<Actor*>::iterator it;
    for(it = m_actor.begin(); it != m_actor.end(); it++)
    {
        if((*it)->isAlive())
            (*it)->doSomething();
    }
    
    bool playerAlive = true;
    
    while(needAliens())
        addAlien();

    for(it = m_actor.begin(); it != m_actor.end();)
    {
        if(!(*it)->isAlive())
        {
            if((*it)->getIID() == IID_NACHENBLASTER)
                playerAlive = false;
            delete(*it);
            it = m_actor.erase(it);
        }
        else it++;
    }
    int n = randInt(1, 15);
    if(n == 1)
    {
        int starY = randInt(0, VIEW_HEIGHT-1);
        int n = randInt(5, 50);
        double size = n/100.0;
        Star* newStar = new Star(this, VIEW_WIDTH-1, starY, size);
        m_actor.push_back(newStar);
    }
    
    updateStats();
    
    if(playerAlive == false) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    if(m_aliensDestroyed == 6 + (4 * getLevel()))
        return GWSTATUS_FINISHED_LEVEL;

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it;
    for(it = m_actor.begin(); it != m_actor.end();)
    {
        delete(*it);
        it = m_actor.erase(it);
    }
    m_actor.clear();
}

NachenBlaster* StudentWorld::getPlayer()
{
    return m_player;
}

void StudentWorld::checkCollision(Actor* a)
{
    for(int i = 0; i < m_actor.size(); i++) {
        if(m_actor[i]->isProjectile(m_actor[i]))
        {
            if(a->didCollide(a, m_actor[i])) {
                m_actor[i]->setDead();
                if(m_actor[i]->getIID() == IID_CABBAGE)
                    a->sufferDamage(2);
                else if(m_actor[i]->getIID() == IID_TORPEDO)
                    a->sufferDamage(8);
            }
        }
    }
    if(m_player->didCollide(m_player, a))
    {
        a->setDead();
        recordAlienDestroyed();
        if(a->getIID() == IID_SNAGGLEGON) {
            increaseScore(1000);
            m_player->decHP(15);
        }
        else {
            increaseScore(250);
            m_player->decHP(5);
        }
        playSound(SOUND_DEATH);
        addActor(new Explosion(this, a->getX(), a->getY()));
    }
}

double StudentWorld::euclidianDist(double x1, double y1, double x2, double y2) const
{
    double x = x2 - x1;
    double y = y2 - y1;
    
    double xSq = pow(x, 2);
    double ySq = pow(y, 2);
    double inside = xSq + ySq;
    
    double result = sqrt(inside);
    return result;
}

void StudentWorld::recordAlienDestroyed()
{
    m_aliensDestroyed++;
    m_aliensOnMap--;
}

bool StudentWorld::needAliens()
{
    double d = m_aliensDestroyed;
    double total = 6 + (4*getLevel());     // ships needed to destroy
    double r = total - d;  // ships left to be destroyed
    double m = 4 + (0.5*getLevel());
    if(m_aliensOnMap < min(m, r))
        return true;
    return false;
}

void StudentWorld::addActor(Actor* a)
{
    m_actor.push_back(a);
}

void StudentWorld::addAlien()
{
    int s1 = 60;
    int s2 = 20 + (getLevel() * 5);     // at level 1 = 25
    int s3 = 5 + (getLevel() * 10);     // at level 1 = 15
    int s = s1 + s2 + s3;   // at level 1 = 100

    double smallgonHP = 5 * (1 + (getLevel() - 1) * 0.1);
    double smoregonHP = 5 * (1 + (getLevel() - 1) * 0.1);
    double snagglegonHP = 10 * (1 + (getLevel() - 1) * 0.1);

    int y = randInt(0, VIEW_HEIGHT-1);
    int chance = randInt(1, s);
    if(chance <= s1) {
        addActor(new Smallgon(this, VIEW_WIDTH-1, y, smallgonHP));
        m_aliensOnMap++;
    }

    else if(chance <= s1+s2) {
        addActor(new Smoregon(this, VIEW_WIDTH-1, y, smoregonHP));
        m_aliensOnMap++;
    }

    else if(chance <= s) {
        addActor(new Snagglegon(this, VIEW_WIDTH-1, y, snagglegonHP));
        m_aliensOnMap++;
    }
}

void StudentWorld::decAlien()
{
    m_aliensOnMap--;
}

double StudentWorld::min(double a, double b)
{
    if(a < b)
        return a;
    return b;
}

void StudentWorld::updateStats()
{
    ostringstream oss;
    string lives = "Lives: ";
    string health = "Health: ";
    string score = "Score: ";
    string level = "Level: ";
    string cabbages = "Cabbages: ";
    string torpedoes = "Torpedoes: ";
    double playerHP = (m_player->getHP()/50)*100.0;
    oss << lives << getLives();
    oss << setw(health.size()+2) << health << playerHP << "%";
    oss << setw(score.size()+2) << score << getScore();
    oss << setw(level.size()+2) << level << getLevel();
    oss << setw(cabbages.size()+2) << cabbages << m_player->getCabbage() << "%";
    oss << setw(torpedoes.size()+2) << torpedoes << m_player->getTorpedoes();
    setGameStatText(oss.str());
}
