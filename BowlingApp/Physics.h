#pragma once

#include "../game-engine-core/include/IApplication.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>


class Physics
{
public:
	Physics();

private:
	std::unique_ptr<btBroadphaseInterface>		m_pBroadphase;

};

