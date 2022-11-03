#include "Behaviours.h"

BehaviourTable::BehaviourTable()
{};

void BehaviourTable::add(Behaviour *b)
{
	behaviours[num] = b;
	num++;
};

Behaviour * BehaviourTable::get(String n)
{
	for( int i = 0; i < num; i++ )
	{
		if( behaviours[i]->name() == n)
		{
			return behaviours[i];
		}
	}

	return nullptr;
};

int BehaviourTable::get_num_behaviours()
{
	return num;
};

Behaviour * BehaviourTable::get_by_num(int n)
{
	return behaviours[n];
};

Behaviour::Behaviour(String name) : _name(name)
{};

Behaviour::~Behaviour()
{};

boolean Behaviour::is_temp()
{
	return _temp;
};

void Behaviour::update()
{};

boolean Behaviour::is_interruptable()
{
	return _interruptable;
};

boolean Behaviour::is_priority()
{
	return _priority;
};

boolean Behaviour::is_running()
{
	return _running;
};

String Behaviour::name()
{
	return _name;
};

String Behaviour::start(String args)
{
	Serial.println("Base start called <"+args+">");
};

boolean Behaviour::is_background()
{
	return _background;
};

void Behaviour::stop()
{
	_running = false;
};

char * Behaviour::args()
{
	return "null";
};
