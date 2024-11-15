#pragma once

#include "../include/Node.h"

class WasdControllerNode : public Node
{
public:
	void Update(float frametime) override;
	void SetControlNode(std::shared_ptr<Node> node) { m_pControlNode = node; }

private:
	std::shared_ptr<Node>		m_pControlNode;
	glm::vec2					m_vCameraAngle;
};

