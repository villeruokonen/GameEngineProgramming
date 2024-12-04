#include "PhysicsNode.h"

#include "../game-engine-core/glm-master/glm/gtc/quaternion.hpp"

PhysicsNode::~PhysicsNode()
{
	if (m_pPhysics && m_Collision.m_pRigidBody)
	{
		m_pPhysics->GetDynamicsWorld()->removeRigidBody(m_Collision.m_pRigidBody.get());
	}
}


void PhysicsNode::SetPhysics(std::shared_ptr<Physics>& physics, float mass, CollisionShape shape, const glm::vec3& size)
{
	if (physics)
	{
		m_pPhysics = physics;

		switch (shape)
		{
		case CollisionShape::None:
			m_Collision.m_pShape = nullptr;
			break;

		case CollisionShape::Box:
			m_Collision.m_pShape = std::make_unique<btBoxShape>(btVector3(size.x, size.y, size.z));
			break;

		case CollisionShape::Sphere:
			m_Collision.m_pShape = std::make_unique<btSphereShape>(GetRadius());
			break;
		}

		if (m_Collision.m_pShape)
		{
			btVector3 fallInertia(0.0f, 0.0f, 0.0f);
			m_Collision.m_pShape->calculateLocalInertia(mass, fallInertia);
			m_fMass = mass;

			// setup initial motion state
			const auto worldMatrix = GetWorldMatrix();
			const glm::quat q = glm::quat_cast(worldMatrix);
			const btQuaternion qq(q.x, q.y, q.z, q.w);
			const btVector3 pos(worldMatrix[3][0], worldMatrix[3][1], worldMatrix[3][2]);
			m_Collision.m_pMotion = std::make_unique<btDefaultMotionState>(btTransform(qq, pos));

			// rigid body
			btRigidBody::btRigidBodyConstructionInfo ci(mass,
				m_Collision.m_pMotion.get(),
				m_Collision.m_pShape.get(),
				fallInertia);
			ci.m_restitution = 0.5f;
			ci.m_friction = 1.0f;
			m_Collision.m_pRigidBody = std::make_unique<btRigidBody>(ci);

			m_pPhysics->GetDynamicsWorld()->addRigidBody(m_Collision.m_pRigidBody.get());
		}
	}
	else
	{
		if (m_pPhysics) m_pPhysics->GetDynamicsWorld()->removeRigidBody(m_Collision.m_pRigidBody.get());
		m_Collision.m_pRigidBody = nullptr;
		m_Collision.m_pMotion = nullptr;
		m_Collision.m_pShape = nullptr;
		m_pPhysics = nullptr;
	}
}


void PhysicsNode::Update(float frametime)
{
	if (m_pPhysics)
	{
		btTransform trans;
		m_Collision.m_pRigidBody->getMotionState()->getWorldTransform(trans);
		trans.getOpenGLMatrix(&m_mModel[0][0]);

		// update all child nodes
		for (auto& node : m_arrNodes)
		{
			node->Update(frametime);
		}
	}
	else
	{
		GeometryNode::Update(frametime);
	}
}

