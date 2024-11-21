#include "Physics.h"

Physics::Physics()
{
	// configure rigid body physics
	m_pBroadphase = std::make_unique<btDbvtBroadphase>();
}

