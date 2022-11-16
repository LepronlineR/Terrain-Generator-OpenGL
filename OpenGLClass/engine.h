#ifndef __ENGINE__H__
#define __ENGINE__H__

#include "PhysObj.h"
#include <vector>

class Engine {
public:
	Engine() {}

	void add(PhysObj obj) { objects.push_back(obj); }
	PhysObj& getObj(int index) { return objects[index]; }
	int size() { return objects.size(); }
	void simulate(float amount);

private:
	std::vector<PhysObj> objects;
};

#endif