#pragma once

#include "../game-engine-core/include/IApplication.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>


class Physics
{
public:
	struct STATIC_COLLISION
	{
		std::unique_ptr<btCollisionShape>	m_pShape;
		std::unique_ptr<btMotionState>		m_pMotion;
		std::unique_ptr<btRigidBody>		m_pRigidBody;
	};

	Physics();
	~Physics();

	void Update(float frametime);

	void AddStaticCollisionPlane(const glm::vec4& plane);

	btDiscreteDynamicsWorld* GetDynamicsWorld() { return m_pDynamicsWorld.get(); }
	const btDiscreteDynamicsWorld* GetDynamicsWorld() const { return m_pDynamicsWorld.get(); }

private:
	std::unique_ptr<btBroadphaseInterface>					m_pBroadphase;
	std::unique_ptr<btDefaultCollisionConfiguration>		m_pCollisionConfiguration;
	std::unique_ptr<btCollisionDispatcher>					m_pDispatcher;
	std::unique_ptr<btSequentialImpulseConstraintSolver>	m_pSolver;
	std::unique_ptr<btDiscreteDynamicsWorld>				m_pDynamicsWorld;

	std::vector<STATIC_COLLISION>							m_arrCollisionObjects;
};

