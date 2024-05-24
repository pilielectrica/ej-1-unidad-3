/*
 *	Soft circle body for Box2D based on the code by Maciej Sawitus, 2011
 *	Feel free to use for any projects (commercial or non-commercial) without letting me know.
 *	I do not guarantee that this code works.
 */
#include "b2ExSoftCircleBody.h"

struct b2ExSoftCircleBody
{
  int m_numParts;
  b2Body** m_parts;
  b2Body* m_center;
  b2Joint** m_partJoints;
  b2Joint** m_centerJoints;
  b2World* m_world;
};

b2ExSoftCircleBody* b2ExSoftCircleBody_Create(b2World* world, const b2ExSoftCircleBodyDef* def)
{
    b2Assert(0.0f <= def->softness && def->softness <= 1.0f);

    // Allocate memory
    char* memory = (char*)b2Alloc(sizeof(b2ExSoftCircleBody) + sizeof(b2Body*) + sizeof(b2Joint*) * 2);
    if (!memory)
        return NULL;

    b2ExSoftCircleBody* circle = (b2ExSoftCircleBody*)memory;
    memory += sizeof(b2ExSoftCircleBody);
    circle->m_world = world;
    circle->m_numParts = 1; // Solo una parte
    circle->m_parts = (b2Body**)memory;
    memory += sizeof(b2Body*);
    circle->m_partJoints = (b2Joint**)memory;
    memory += sizeof(b2Joint*);
    circle->m_centerJoints = (b2Joint**)memory;

    // Create part (pelota)
    b2CircleShape sd;
    sd.m_radius = def->radius;

    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position = def->center;
    b2Body* body = world->CreateBody(&bd);
    body->CreateFixture(&sd, def->density);
    circle->m_parts[0] = body;

    // Create center circle (la misma pelota)
    circle->m_center = body;

    // Create links between circles (articulaciones)
    b2DistanceJointDef jointDef;

    // 1st link between center and part
    jointDef.Initialize(
        circle->m_parts[0], circle->m_center,
        circle->m_parts[0]->GetWorldCenter(), def->center);
    jointDef.collideConnected = true;
    b2LinearStiffness(jointDef.stiffness, jointDef.damping, def->jointFrequencyHz, def->jointDampingRatio, jointDef.bodyA, jointDef.bodyB);
    circle->m_centerJoints[0] = world->CreateJoint(&jointDef);

    return circle;
}



void b2ExSoftCircleBody_Destroy(b2ExSoftCircleBody* circle)
{
  for (int i = 0; i < circle->m_numParts; i++)
  {
    circle->m_world->DestroyJoint(circle->m_partJoints[i]);
    circle->m_world->DestroyJoint(circle->m_centerJoints[i]);
  }
  for (int i = 0; i < circle->m_numParts; i++)
    circle->m_world->DestroyBody(circle->m_parts[i]);
  circle->m_world->DestroyBody(circle->m_center);
  b2Free(circle);
}

int b2ExSoftCircleBody_GetNumParts(b2ExSoftCircleBody* circle)
{
  return circle->m_numParts;
}

b2Body* b2ExSoftCircleBody_GetPart(b2ExSoftCircleBody* circle, int index)
{
  b2Assert(0 <= index && index < circle->m_numParts);
  return circle->m_parts[index];
}

b2Body* b2ExSoftCircleBody_GetCenter(b2ExSoftCircleBody* circle)
{
  return circle->m_center;
}