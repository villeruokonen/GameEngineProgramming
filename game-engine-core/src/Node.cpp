#include "../include/Node.h"

Node::Node() :
	m_mModel(1.0f),
	m_pParent(nullptr),
	m_vVelocity(0.0f),
	m_vAcceleration(0.0f),
	m_vRotationAxis(0.0f, 1.0f, 0.0f),
	m_fRotationAngle(0.0f),
	m_fRotationSpeed(0.0f),
	m_fRadius(1.0f)
{
}

Node::Node(const std::string_view name) :
	m_mModel(1.0f),
	m_pParent(nullptr),
	m_vVelocity(0.0f),
	m_vAcceleration(0.0f),
	m_vRotationAxis(0.0f, 1.0f, 0.0f),
	m_fRotationAngle(0.0f),
	m_fRotationSpeed(0.0f),
	m_fRadius(1.0f),
	m_strName(name)
{
}


void Node::Update(float frametime)
{
	auto pos = GetPos();

	pos += m_vVelocity * frametime;
	m_vVelocity += m_vAcceleration * frametime;

	if (m_fRotationSpeed != 0.0f)
	{
		m_mModel = glm::rotate(glm::mat4(1.0f), m_fRotationAngle, m_vRotationAxis);

		m_fRotationAngle += m_fRotationSpeed * frametime;
		constexpr float pi2 = glm::two_pi<float>();
		while (m_fRotationAngle > pi2) m_fRotationAngle -= pi2;
		while (m_fRotationAngle < -pi2) m_fRotationAngle += pi2;
	}

	SetPos(pos);

	// update child nodes
	for (auto& node : m_arrNodes)
	{
		node->Update(frametime);
	}
}


void Node::Render(IRenderer& renderer, GLuint program)
{
	for (auto& node : m_arrNodes)
	{
		node->Render(renderer, program);
	}
}


void Node::AddNode(std::shared_ptr<Node> node)
{
	node->m_pParent = this;
	m_arrNodes.push_back(node);
}


Node* Node::FindNode(const std::string_view name)
{
	if (m_strName == name) return this;

	for (const auto& node : m_arrNodes)
	{
		auto n = node->FindNode(name);
		if (n) return n;
	}

	return nullptr;
}


