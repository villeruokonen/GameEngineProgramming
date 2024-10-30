#include "../include/Node.h"

Node::Node() :
	m_mModel(1.0f),
	m_pParent(nullptr)
{
}

Node::Node(const std::string_view name) :
	m_mModel(1.0f),
	m_pParent(nullptr),
	m_strName(name)
{
}

void Node::Update(float frameTime)
{
	// Update child nodes
	for (auto& node : m_arrNodes)
	{
		node->Update(frameTime);
	}
}

void Node::Render(IRenderer& renderer, GLuint program)
{
	// Update child nodes
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
	if (m_strName == name)
		return this;

	for (const auto& node : m_arrNodes)
	{
		auto n = node->FindNode(name);
		if (n)
			return n;
	}

	return nullptr;
}
