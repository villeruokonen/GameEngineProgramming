#pragma once

#include "../include/OpenGLRenderer.h"

class Node
{
public:
	Node();
	Node(const std::string_view name);
	virtual ~Node() {}

	virtual void Update(float frameTime);

	virtual void Render(IRenderer& renderer, GLuint program);

	void AddNode(std::shared_ptr<Node> node);
	inline Node* GetParent() { return m_pParent; }
	inline auto& GetNodes() { return m_arrNodes;  }
	inline const auto& GetNodes() const { return m_arrNodes; }

	Node* FindNode(const std::string_view name);

	inline const std::string& GetName() const { return m_strName; }
	inline void SetName(const std::string_view name) { m_strName = name; }

	inline void SetPos(const glm::vec3& pos)
	{
		m_mModel[3][0] = pos.x;
		m_mModel[3][1] = pos.y;
		m_mModel[3][2] = pos.z;
	}

	inline glm::vec3 GetPos() const { return m_mModel[3]; }

	inline auto& Matrix() { return m_mModel; }
	inline void SetMatrix(const glm::mat4& mat) { m_mModel = mat; }

	inline glm::mat4 GetWorldMatrix() const
	{
		return m_pParent
				? m_pParent->GetWorldMatrix() * m_mModel
				: m_mModel;
	}

protected:
	glm::mat4							m_mModel;

	Node*								m_pParent;
	std::vector<std::shared_ptr<Node>>	m_arrNodes;

private:
	std::string							m_strName;
};