#include "engine.h"

void Engine::simulate(float amount) {
	for(PhysObj obj : Engine::objects) {
		obj.move(amount);
	}
}