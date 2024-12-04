#pragma once
#include "../game-engine-core/include/GeometryNode.h"
#include "Physics.h"

class PhysicsNode :
    public GeometryNode
{
public:
    enum class CollisionShape
    {
        None = 0,
        Box = 1,
        Sphere = 2
    };

    PhysicsNode(const std::shared_ptr<Geometry>& geometry,
        const std::shared_ptr<Material>& material) :

        GeometryNode(geometry, material),
        m_fMass(1.0f)
    {
    }

    ~PhysicsNode();

    void SetPhysics(std::shared_ptr<Physics>& physics, float mass, CollisionShape shape);

    void Update(float frametime) override;

private:
    std::shared_ptr<Physics>            m_pPhysics;

    Physics::STATIC_COLLISION           m_Collision;

    float                               m_fMass;
};

