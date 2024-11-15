#include "../include/WasdControllerNode.h"

void WasdControllerNode::Update(float frametime)
{
	if (m_pControlNode)
	{
		const auto* app = IApplication::GetApp();
		constexpr float moveSpeed = 20.0f;
		constexpr float turnSpeed = 0.5f;

		glm::mat4 model(1.0f);

		m_vCameraAngle.x -= app->GetMousePosDelta().y * turnSpeed * frametime;
		m_vCameraAngle.y -= app->GetMousePosDelta().x * turnSpeed * frametime;

		// limit up/down angle
		constexpr float tiltLimit = glm::half_pi<float>() - 0.25f;
		if (m_vCameraAngle.x > tiltLimit) m_vCameraAngle.x = tiltLimit;
		else if (m_vCameraAngle.x < -tiltLimit) m_vCameraAngle.x = -tiltLimit;

		// normalize y-axis rotation
		constexpr float pi2 = glm::two_pi<float>();
		while (m_vCameraAngle.y > pi2) m_vCameraAngle.y -= pi2;
		while (m_vCameraAngle.y < -pi2) m_vCameraAngle.y += pi2;

		// build new model matrix
		model = glm::rotate(model, m_vCameraAngle.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(model, m_vCameraAngle.x, glm::vec3(1.0f, 0.0f, 0.0f));

		// apply movements
		glm::vec2 move(0.0f);
		auto pos = m_pControlNode->GetPos();
		const float posY = pos.y;

		if (IApplication::IsKeyDown(VK_LEFT) || IApplication::IsKeyDown('A'))
		{
			move.x += -moveSpeed;
		}
		if (IApplication::IsKeyDown(VK_RIGHT) || IApplication::IsKeyDown('D'))
		{
			move.x += moveSpeed;
		}

		if (IApplication::IsKeyDown(VK_UP) || IApplication::IsKeyDown('W'))
		{
			move.y = -moveSpeed;
		}
		if (IApplication::IsKeyDown(VK_DOWN) || IApplication::IsKeyDown('S'))
		{
			move.y = moveSpeed;
		}

		if (move.x != 0.0f || move.y != 0.0f)
		{
			// strafe
			glm::vec3 dir(model[0]);
			pos += dir * move.x * frametime;

			// forward/back
			dir = model[2];
			pos += dir * move.y * frametime;

			pos.y = posY;
		}

		model[3] = glm::vec4(pos, 1.0f);
		m_pControlNode->SetMatrix(model);
	}
	Node::Update(frametime);
}
