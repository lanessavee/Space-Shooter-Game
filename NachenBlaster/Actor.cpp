#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// ============================ ACTOR CLASS IMPLEMENTATION ============================
Actor::Actor(StudentWorld* world, int image, double x, double y, int dir, double size, int depth)
: GraphObject(image, x, y, dir, size, depth)
{
    m_alive = true;
    m_world = world;
    m_image = image;
}

Actor::~Actor()
{
    
}

void Actor::setDead()
{
    m_alive = false;
}

bool Actor::isAlive() const
{
    return m_alive;
}

StudentWorld* Actor::getWorld()
{
    return m_world;
}

bool Actor::didCollide(const Actor* a1, const Actor* a2)
{
    double x1 = a1->getX();
    double y1 = a1->getY();
    
    double x2 = a2->getX();
    double y2 = a2->getY();
    
    double r1 = a1->getRadius();
    double r2 = a2->getRadius();
    
    if(getWorld()->euclidianDist(x1, y1, x2, y2) < .75 * (r1 + r2))
        return true;
    return false;
}

int Actor::getIID() const
{
    return m_image;
}

void Actor::sufferDamage(double amt)
{
    
}

bool Actor::isProjectile(const Actor* a)
{
    if(a->getIID() == IID_CABBAGE || a->getIID() == IID_TORPEDO)
        return true;
    return false;
}

void Actor::whenDestroyed()
{
    
}
// ====================== DAMAGEABLE OBJECT CLASS IMPLEMENTATION ======================

DamageableObject::DamageableObject(StudentWorld* world, int imageID, double startX, double startY,
                 int startDir, double size, int depth, double hitPoints)
: Actor(world, imageID, startX, startY, startDir, size, depth)
{
    m_hp = hitPoints;
}

DamageableObject::~DamageableObject()
{
    
}

void DamageableObject::sufferDamage(double amt)
{
    decHP(amt);
}

double DamageableObject::getHP()
{
    return m_hp;
}

void DamageableObject::incHP(double amt)
{
    m_hp += amt;
}

void DamageableObject::decHP(double amt)
{
    m_hp -= amt;
}

// ======================== NACHENBLASTER CLASS IMPLEMENTATION ========================
NachenBlaster::NachenBlaster(StudentWorld* world)
: DamageableObject(world, IID_NACHENBLASTER, 0, 128, 0, 1.0, 0, 50)
{
    m_cabbage = 30;
    m_torpedoes = 0;
}

NachenBlaster::~NachenBlaster()
{ }

void NachenBlaster::doSomething()
{
    if(!isAlive())
        return;

    int key;
    if(getWorld()->getKey(key))
    {
        switch (key) {
            case KEY_PRESS_SPACE:
                if(m_cabbage >= 5)
                {
                    m_cabbage -= 5;
                    getWorld()->addActor(new Cabbage(getWorld(), getX()+12, getY()));
                    getWorld()->playSound(IID_CABBAGE);
                }
                break;
            case KEY_PRESS_TAB:
                if(m_torpedoes > 0) {
                    --m_torpedoes;
                    getWorld()->addActor(new PlayerTorpedo(getWorld(), getX()+12, getY()));
                    getWorld()->playSound(IID_TORPEDO);
                }
                break;
            case KEY_PRESS_UP:
                if(getY() >= VIEW_HEIGHT)
                    break;
                moveTo(getX(), getY()+6);
                break;
            case KEY_PRESS_DOWN:
                if(getY() < 0)
                    break;
                moveTo(getX(), getY()-6);
                break;
            case KEY_PRESS_LEFT:
                if(getX() < 0)
                    break;
                moveTo(getX()-6, getY());
                break;
            case KEY_PRESS_RIGHT:
                if(getX() >= VIEW_WIDTH)
                    break;
                moveTo(getX()+6, getY());
                break;
            default:
                break;
        }
    }
    
    
    if(m_cabbage < 30)
        m_cabbage++;
    if(getHP() <= 0)
        setDead();
}

void NachenBlaster::incHP(double amt)
{
    DamageableObject::incHP(amt);
}

void NachenBlaster::decHP(double amt)
{
    DamageableObject::decHP(amt);
}

int NachenBlaster::getCabbage()
{
    int cabbagePerc = (m_cabbage/30.0) * 100;
    return cabbagePerc;
}

int NachenBlaster::getTorpedoes()
{
    return m_torpedoes;
}

void NachenBlaster::incTorpedo()
{
    m_torpedoes += 5;
}

// ============================= STAR CLASS IMPLEMENTATION =============================
Star::Star(StudentWorld* world, double x, double y, double size)
: Actor(world, IID_STAR, x, y, 0, size, 3)
{
    // DONE!!!!!!
}

Star::~Star()
{ }

void Star::doSomething()
{
    if(!isAlive())
        return;
    
    if(getX() <= 1) {
        setDead();
        return;
    }
    
    moveTo(getX()-1, getY());
}

// =========================== EXPLOSION CLASS IMPLEMENTATION ===========================
Explosion::Explosion(StudentWorld* world, double x, double y)
: Actor(world, IID_EXPLOSION, x, y, 0, 1, 0)
{
    m_ticks = 4;
}

Explosion::~Explosion()
{
    
}

void Explosion::doSomething()
{
    if(!isAlive())
        return;
    
    setSize(getSize()*1.5);
    m_ticks--;
    if(m_ticks == 0)
        setDead();
}

// ========================== PROJECTILES CLASS IMPLEMENTATION ==========================
Projectiles::Projectiles(StudentWorld* world, int image, double x, double y, int dir, double size, int depth)
: Actor(world, image, x, y, dir, size, depth)
{
    
}

Projectiles::~Projectiles()
{
    
}

// ============================ CABBAGE CLASS IMPLEMENTATION ============================
Cabbage::Cabbage(StudentWorld* world, double x, double y)
: Projectiles(world, IID_CABBAGE, x, y, 0, 0.5, 1)
{
    
}

Cabbage::~Cabbage()
{
    
}

void Cabbage::doSomething()
{
    if(!isAlive())
        return;
    
    if(getX() >= VIEW_WIDTH)
        setDead();
    
    moveTo(getX()+8, getY());
    setDirection(getDirection()+20);
}

// ============================ TORPEDO CLASS IMPLEMENTATION ============================
Torpedo::Torpedo(StudentWorld* world, double x, double y, int dir)
: Projectiles(world, IID_TORPEDO, x, y, dir, 0.5, 1)
{
    
}

Torpedo::~Torpedo()
{
    
}

// ========================= ALIEN TORPEDO CLASS IMPLEMENTATION =========================
AlienTorpedo::AlienTorpedo(StudentWorld* world, double x, double y)
: Torpedo(world, x, y, 180)
{
    
}

AlienTorpedo::~AlienTorpedo()
{
    
}

void AlienTorpedo::doSomething()
{
    if(!isAlive())
        return;
    if(getX() < 0)
        setDead();
    
    if(didCollide(this, getWorld()->getPlayer())) {
        getWorld()->getPlayer()->decHP(2);
        setDead();
        return;
    }
    
    moveTo(getX()-8, getY());
    
    if(didCollide(this, getWorld()->getPlayer())) {
        getWorld()->getPlayer()->decHP(2);
        setDead();
        return;
    }
}

// ======================== PLAYER TORPEDO CLASS IMPLEMENTATION ========================
PlayerTorpedo::PlayerTorpedo(StudentWorld* world, double x, double y)
: Torpedo(world, x, y, 0)
{
    
}

PlayerTorpedo::~PlayerTorpedo()
{
    
}

void PlayerTorpedo::doSomething()
{
    if(!isAlive())
        return;
    if(getX() >= VIEW_WIDTH)
        setDead();
    
    moveTo(getX()+8, getY());
}

// ============================ TURNIP CLASS IMPLEMENTATION ============================
Turnip::Turnip(StudentWorld* world, double x, double y)
: Projectiles(world, IID_TURNIP, x, y, 0, 0.5, 1)
{
    
}

Turnip::~Turnip()
{
    
}

void Turnip::doSomething()
{
    if(!isAlive())
        return;
    
    if(getX() < 0)
        setDead();
    
    if(didCollide(this, getWorld()->getPlayer())) {
        getWorld()->getPlayer()->decHP(2);
        setDead();
        return;
    }
    
    moveTo(getX()-6, getY());
    setDirection(getDirection()+20);
    
    if(didCollide(this, getWorld()->getPlayer())) {
        getWorld()->getPlayer()->decHP(2);
        setDead();
        return;
    }
}

// ============================= GOODIES CLASS IMPLEMENTATION =============================
Goodies::Goodies(StudentWorld* world, int image, double x, double y, int dir, double size, int depth)
: Actor(world, image, x, y, dir, size, depth)
{
    
}

Goodies::~Goodies()
{
    
}

void Goodies::whenCollided()
{
    getWorld()->increaseScore(100);
    setDead();
    getWorld()->playSound(SOUND_GOODIE);
}

void Goodies::move()
{
    moveTo(getX()-0.75, getY()-0.75);
}

// ======================== EXTRA LIFE GOODIE CLASS IMPLEMENTATION ========================

ExtraLife::ExtraLife(StudentWorld* world, double x, double y)
: Goodies(world, IID_LIFE_GOODIE, x, y, 0, 0.5, 1)
{
    
}

ExtraLife::~ExtraLife()
{
    
}

void ExtraLife::doSomething()
{
    if(!isAlive())
        return;
    
    if(getX() < 0 || getY() < 0)
        setDead();
    
    if(didCollide(this, getWorld()->getPlayer())) {
        whenCollided();
        getWorld()->incLives();
        return;
    }
    
    move();
}

// ======================== REPAIR GOODIE CLASS IMPLEMENTATION ========================

RepairGoodie::RepairGoodie(StudentWorld* world, double x, double y)
: Goodies(world, IID_REPAIR_GOODIE, x, y, 0, 0.5, 1)
{
    
}

RepairGoodie::~RepairGoodie()
{
    
}

void RepairGoodie::doSomething()
{
    if(!isAlive())
        return;
    
    if(getX() < 0 || getY() < 0)
        setDead();
    
    if(didCollide(this, getWorld()->getPlayer())) {
        whenCollided();
        getWorld()->getPlayer()->incHP(10);
        return;
    }
    
    move();
}

// ======================== TORPEDO GOODIE CLASS IMPLEMENTATION ========================

TorpedoGoodie::TorpedoGoodie(StudentWorld* world, double x, double y)
: Goodies(world, IID_TORPEDO_GOODIE, x, y, 0, 0.5, 1)
{
    
}

TorpedoGoodie::~TorpedoGoodie()
{
    
}

void TorpedoGoodie::doSomething()
{
    if(!isAlive())
        return;
    
    if(getX() < 0 || getY() < 0)
        setDead();
    
    if(didCollide(this, getWorld()->getPlayer())) {
        whenCollided();
        getWorld()->getPlayer()->incTorpedo();
        return;
    }
    
    move();
}

// ============================== ALIEN CLASS IMPLEMENTATION ==============================
Alien::Alien(StudentWorld* world, int imageID, double startX, double startY,
             double hitPoints, double damageAmt, double speed, unsigned int scoreValue)
: DamageableObject(world, imageID, startX, startY, 0, 1.5, 1, hitPoints)
{
    m_scoreValue = scoreValue;
    m_speed = speed;
    m_dmgAmt = damageAmt;
    m_flight = 0;
    m_dir = 0;
}

Alien::~Alien()
{
    
}

int Alien::getScoreValue()
{
    return m_scoreValue;
}

int Alien::getFlight()
{
    return m_flight;
}

void Alien::setFlight(int amt)
{
    m_flight = amt;
}

int Alien::getSpeed()
{
    return m_speed;
}

void Alien::setSpeed(double amt)
{
    m_speed = amt;
}


bool Alien::needFlight()
{
    if(m_flight == 0 || getY() >= VIEW_HEIGHT || getY() <= 0)
        return true;
    return false;
}

void Alien::setTravelDirection(int dir)
{
    if(getY() >= VIEW_HEIGHT-1) {
        m_dir = DOWN;
    }
    else if(getY() <= 0) {
        m_dir = UP;
    }
    else if(m_flight == 0) {
        m_flight = randInt(1, 32);
        m_dir = randInt(1, 3);
    }
    else
        m_dir = dir;
}

int Alien::getTravelDirection()
{
    return m_dir;
}

void Alien::move(int dir)
{
    switch (dir) {
        case 1:
            moveTo(getX()-m_speed, getY()-m_speed);
            break;
        case 2:
            moveTo(getX()-m_speed, getY()+m_speed);
            break;
        case 3:
            moveTo(getX()-m_speed, getY());
            break;
    }
    m_flight--;
}

void Alien::whenDestroyed()
{
    setDead();
    getWorld()->recordAlienDestroyed();
    getWorld()->increaseScore(m_scoreValue);
    getWorld()->playSound(SOUND_DEATH);
    getWorld()->addActor(new Explosion(getWorld(), getX(), getY()));
}

void Alien::dropGoodie()
{
    
}

// ============================= SMALLGON CLASS IMPLEMENTATION =============================
Smallgon::Smallgon(StudentWorld* world, double x, double y, double hp)
: Alien(world, IID_SMALLGON, x, y, hp, 5,  2.0, 250)
{
    
}

Smallgon::~Smallgon()
{
    
}

void Smallgon::doSomething()
{
    if(!isAlive()) {
        return;
    }
    else if(getX() < 0) {
        getWorld()->decAlien();
        setDead();
        return;
    }
    if(getHP() <= 0) {
        whenDestroyed();
        return;
    }
    
    getWorld()->checkCollision(this);
    
    if(needFlight())
        setTravelDirection(getTravelDirection());
    
    double playerX = getWorld()->getPlayer()->getX();
    double playerY = getWorld()->getPlayer()->getY();

    if(playerX < getX() && (abs(playerY - getY())<=4) ) {
        int level = getWorld()->getLevel();
        int n = ((20/level) + 5);
        if(randInt(1, n) == 1) {
            getWorld()->addActor(new Turnip(getWorld(), getX()-14, getY()));
            getWorld()->playSound(SOUND_ALIEN_SHOOT);
            return;
        }
    }
    
    move(getTravelDirection());
    
    getWorld()->checkCollision(this);
}

void Smallgon::sufferDamage(double amt)
{
    DamageableObject::decHP(amt);
}

// ============================= SMOREGON CLASS IMPLEMENTATION =============================
Smoregon::Smoregon(StudentWorld* world, double x, double y, double hp)
: Alien(world, IID_SMOREGON, x, y, hp, 5, 2.0, 250)
{
    
}

Smoregon::~Smoregon()
{
    
}

void Smoregon::doSomething()
{
    if(!isAlive()) {
        return;
    }
    else if(getX() < 0) {
        getWorld()->decAlien();
        setDead();
        return;
    }
    if(getHP() <= 0) {
        whenDestroyed();
        if(randInt(1, 3 == 1))
            dropGoodie();
        return;
    }
    
    getWorld()->checkCollision(this);
    
    if(needFlight())
        setTravelDirection(getTravelDirection());
    
    double playerX = getWorld()->getPlayer()->getX();
    double playerY = getWorld()->getPlayer()->getY();
    
    if(playerX < getX() && (abs(playerY - getY())<=4) ) {
        int level = getWorld()->getLevel();
        int n = ((20/level) + 5);
        
        if(randInt(1, n) == 1) {
            getWorld()->addActor(new Turnip(getWorld(), getX()-14, getY()));
            getWorld()->playSound(SOUND_ALIEN_SHOOT);
            return;
        }
        else if(randInt(1, n) == 2) {
            setFlight(VIEW_WIDTH);
            setSpeed(5);
            setTravelDirection(3);
        }
    }
    
    move(getTravelDirection());
    
    getWorld()->checkCollision(this);
}

void Smoregon::dropGoodie()
{
    if(randInt(1, 2) == 1)
        getWorld()->addActor(new RepairGoodie(getWorld(), getX(), getY()));
    else if (randInt(1, 2) == 2)
        getWorld()->addActor(new TorpedoGoodie(getWorld(), getX(), getY()));
}

void Smoregon::sufferDamage(double amt)
{
    DamageableObject::decHP(amt);
}

// ============================ SNAGGLEGON CLASS IMPLEMENTATION ============================
Snagglegon::Snagglegon(StudentWorld* world, double x, double y, double hp)
: Alien(world, IID_SNAGGLEGON, x, y, hp, 15, 2.0, 1000)
{
    
}

Snagglegon::~Snagglegon()
{
    
}

void Snagglegon::doSomething()
{
    if(!isAlive()) {
        return;
    }
    else if(getX() < 0) {
        getWorld()->decAlien();
        setDead();
        return;
    }
    if(getHP() <= 0) {
        whenDestroyed();
        dropGoodie();
        return;
    }
    
    getWorld()->checkCollision(this);
    
    if(needFlight())
        setTravelDirection(getTravelDirection());
    
    double playerX = getWorld()->getPlayer()->getX();
    double playerY = getWorld()->getPlayer()->getY();
    
    if(playerX < getX() && (abs(playerY - getY())<=4) ) {
        int level = getWorld()->getLevel();
        int n = ((15/level) + 10);
        if(randInt(1, n) == 1) {
            getWorld()->addActor(new AlienTorpedo(getWorld(), getX()-14, getY()));
            getWorld()->playSound(SOUND_TORPEDO);
            return;
        }
    }
    
    move(getTravelDirection());
    
    getWorld()->checkCollision(this);
}

void Snagglegon::dropGoodie()
{
    if(randInt(1, 6) == 1)
        getWorld()->addActor(new ExtraLife(getWorld(), getX(), getY()));
}

void Snagglegon::sufferDamage(double amt)
{
    DamageableObject::decHP(amt);
}
