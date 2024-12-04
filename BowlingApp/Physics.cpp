#include "Physics.h"

Physics::Physics()
{
	// configure rigid body physics
	m_pBroadphase = std::make_unique<btDbvtBroadphase>();
	m_pCollisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
	m_pDispatcher = std::make_unique<btCollisionDispatcher>(m_pCollisionConfiguration.get());
	m_pSolver = std::make_unique<btSequentialImpulseConstraintSolver>();

	m_pDynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(m_pDispatcher.get(),
		m_pBroadphase.get(),
		m_pSolver.get(),
		m_pCollisionConfiguration.get());
	m_pDynamicsWorld->setGravity({ 0.0f, -9.8f, 0.0 });
}

Physics::~Physics()
{
	if (m_pDynamicsWorld)
	{
		for (const auto& coll : m_arrCollisionObjects)
		{
			m_pDynamicsWorld->removeRigidBody(coll.m_pRigidBody.get());
		}
	}

	m_arrCollisionObjects.clear();
}

void Physics::Update(float frametime)
{
	if (m_pDynamicsWorld)
	{
		m_pDynamicsWorld->stepSimulation(frametime);
	}
}

void Physics::AddStaticCollisionPlane(const glm::vec4& plane)
{
	if (m_pDynamicsWorld)
	{
		STATIC_COLLISION collision;
		btTransform startTransform;
		startTransform.setIdentity();

		collision.m_pShape = std::make_unique<btStaticPlaneShape>(btVector3(plane.x, plane.y, plane.z), plane.w);
		collision.m_pMotion = std::make_unique<btDefaultMotionState>(startTransform);

		btRigidBody::btRigidBodyConstructionInfo ci(0.0f,
			collision.m_pMotion.get(),
			collision.m_pShape.get());
		collision.m_pRigidBody = std::make_unique<btRigidBody>(ci);

		collision.m_pRigidBody->setGravity({ 0.0f, 0.0f, 0.0f });
		collision.m_pRigidBody->setActivationState(0);

		m_pDynamicsWorld->addRigidBody(collision.m_pRigidBody.get());
		m_arrCollisionObjects.push_back(std::move(collision));
	}
}

