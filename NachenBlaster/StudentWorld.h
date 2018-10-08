#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <vector>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class NachenBlaster;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    NachenBlaster* getPlayer();
    
    void checkCollision(Actor* a);
    
    void addActor(Actor* a);
    
    void recordAlienDestroyed();
    bool needAliens();
    void addAlien();
    void decAlien();
    
    double euclidianDist(double x1, double y1, double x2, double y2) const;
    double min(double a, double b);
    void updateStats();
    
private:
    std::vector<Actor*> m_actor;
    NachenBlaster* m_player;
    double m_aliensDestroyed;
    double m_aliensOnMap;
};

#endif // STUDENTWORLD_H_
