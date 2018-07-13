#ifndef VISIBLE_OBJECT_CAST_H
#define VISIBLE_OBJECT_CAST_H

#include <vector>
#include <Box2D/Box2D.h>

class VisibleObjectsCast : public b2QueryCallback {
  public:
      std::vector<b2Body*> foundBodies;

      bool ReportFixture(b2Fixture* fixture) override
      {
          foundBodies.push_back( fixture->GetBody() ); 
          return true;
      }
  };

#endif
