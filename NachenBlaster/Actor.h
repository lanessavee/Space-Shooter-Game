#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(StudentWorld* world, int image, double x, double y, int dir, double size, int depth);
    virtual ~Actor();
    virtual void doSomething() = 0;
    void setDead();
    bool isAlive() const;
    StudentWorld* getWorld();
    bool didCollide(const Actor* a1, const Actor* a2);
    int getIID() const;
    virtual void sufferDamage(double amt);
    bool isProjectile(const Actor* a);
    virtual void whenDestroyed();
private:
    StudentWorld* m_world;
    bool m_alive;
    int m_image;
};

class DamageableObject : public Actor {
public:
    DamageableObject(StudentWorld* world, int imageID, double startX, double startY,
                     int startDir, double size, int depth, double hitPoints);
    virtual ~DamageableObject();
    virtual void sufferDamage(double amt);
    double getHP();
    virtual void incHP(double amt);
    virtual void decHP(double amt);
private:
    double m_hp;
};

class NachenBlaster : public DamageableObject {
public:
    NachenBlaster(StudentWorld* world);
    virtual ~NachenBlaster();
    virtual void doSomething();
    virtual void incHP(double amt);
    virtual void decHP(double amt);
    int getCabbage();
    int getTorpedoes();
    void incTorpedo();
private:
    int m_cabbage;
    int m_torpedoes;
};

class Alien : public DamageableObject {
public:
    Alien(StudentWorld* world, int imageID, double startX, double startY,
          double hitPoints, double damageAmt, double speed, unsigned int scoreValue);
    virtual ~Alien();
    int getSpeed();
    int getScoreValue();
    int getFlight();
    int getTravelDirection();
    void setTravelDirection(int dir);
    void setFlight(int amt);
    void setSpeed(double amt);
    virtual void dropGoodie();
    bool needFlight();
    void move(int dir);
    void whenDestroyed();
private:
    int m_scoreValue;
    int m_speed;
    int m_flight;
    double m_dmgAmt;
    int DOWN = 1;
    int UP = 2;
    int LEFT = 3;
    int m_dir;
};

class Smallgon : public Alien {
public:
    Smallgon(StudentWorld* world, double x, double y, double hp);
    virtual ~Smallgon();
    virtual void doSomething();
    virtual void sufferDamage(double amt);
};

class Smoregon : public Alien {
public:
    Smoregon(StudentWorld* world, double x, double y, double hp);
    virtual ~Smoregon();
    virtual void doSomething();
    virtual void dropGoodie();
    virtual void sufferDamage(double amt);
};

class Snagglegon : public Alien {
public:
    Snagglegon(StudentWorld* world, double x, double y, double hp);
    virtual ~Snagglegon();
    virtual void doSomething();
    virtual void dropGoodie();
    virtual void sufferDamage(double amt);
};

class Star : public Actor {
public:
    Star(StudentWorld* world, double x, double y, double size);
    virtual ~Star();
    virtual void doSomething();
private:
};

class Explosion : public Actor {
public:
    Explosion(StudentWorld* world, double x, double y);
    virtual ~Explosion();
    virtual void doSomething();
private:
    int m_ticks;
};

class Projectiles : public Actor {
public:
    Projectiles(StudentWorld* world, int image, double x, double y, int dir, double size, int depth);
    virtual ~Projectiles();
};

class Cabbage : public Projectiles {
public:
    Cabbage(StudentWorld* world, double x, double y);
    virtual ~Cabbage();
    virtual void doSomething();
};

class Torpedo : public Projectiles {
public:
    Torpedo(StudentWorld* world, double x, double y, int dir);
    virtual ~Torpedo();
};

class AlienTorpedo : public Torpedo {
public:
    AlienTorpedo(StudentWorld* world, double x, double y);
    virtual ~AlienTorpedo();
    virtual void doSomething();
};

class PlayerTorpedo : public Torpedo {
public:
    PlayerTorpedo(StudentWorld* world, double x, double y);
    virtual ~PlayerTorpedo();
    virtual void doSomething();
};

class Turnip : public Projectiles {
public:
    Turnip(StudentWorld* world, double x, double y);
    virtual ~Turnip();
    virtual void doSomething();
};

class Goodies : public Actor {
public:
    Goodies(StudentWorld* world, int image, double x, double y, int dir, double size, int depth);
    virtual ~Goodies();
    void whenCollided();
    void move();
private:
};

class ExtraLife : public Goodies {
public:
    ExtraLife(StudentWorld* world, double x, double y);
    virtual ~ExtraLife();
    virtual void doSomething();
private:
};

class RepairGoodie : public Goodies {
public:
    RepairGoodie(StudentWorld* world, double x, double y);
    virtual ~RepairGoodie();
    virtual void doSomething();
};

class TorpedoGoodie : public Goodies {
public:
    TorpedoGoodie(StudentWorld* world, double x, double y);
    virtual ~TorpedoGoodie();
    virtual void doSomething();
};

#endif // ACTOR_H_
