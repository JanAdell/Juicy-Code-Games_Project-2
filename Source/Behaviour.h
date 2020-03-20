#ifndef __BEHAVIOUR_H__
#define __BEHAVIOUR_H__

#include "Component.h"

class Gameobject;

class Behaviour : public Component 
{
public:

	Behaviour(Gameobject* go, ComponentType type = BEHAVIOUR);
	virtual ~Behaviour() {}

public:

	int life = 10;

};

class B_Movable: public Behaviour
{
public:

	B_Movable(Gameobject* go, ComponentType type = B_MOVABLE) : Behaviour(go, type) {}
	virtual ~B_Movable() {}

public:
	
	float speed;
	float angle;

};

class B_Building : public Behaviour
{
public:

	B_Building(Gameobject* go, ComponentType type = B_BUILDING) : Behaviour(go, type) {}
	virtual ~B_Building() {}

	
public:

	int damage;

};

class B_Unit : public B_Movable
{
public:

	B_Unit(Gameobject* go, ComponentType type = B_UNIT) : B_Movable(go, type) {}
	virtual ~B_Unit() {}

	void RecieveEvent(const Event& e) override;

public:

	int damage = 5;

};


#endif // __BEHAVIOUR_H__