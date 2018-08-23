#ifndef VISIBLE_OBJECT_CAST_H
#define VISIBLE_OBJECT_CAST_H

#include <vector>
#include <Box2D/Box2D.h>

class VisibleObjectsCast : public b2QueryCallback {
public:
	std::vector<b2Body*> foundBodies;

	bool ReportFixture(b2Fixture* fixture) override {
		foundBodies.push_back(fixture->GetBody());
		return true;
	}

	void sortBodies() {
		std::sort(foundBodies.begin(), foundBodies.end(), [](b2Body* p1, b2Body* p2) {
			return p1->GetPosition().y < p2->GetPosition().y;
		});
	}
};

#endif
