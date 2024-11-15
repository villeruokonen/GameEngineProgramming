#pragma once

#include "../include/Node.h"

class CameraNode : public Node
{
public:
	CameraNode()
	{
		SetProjectionParams(glm::half_pi<float>(), 1.0f, 1.0f, 500.0f);
	}
	CameraNode(float fov, float aspect, float nearplane, float farplane)
	{
		SetProjectionParams(fov, aspect, nearplane, farplane);
	}
	CameraNode(const glm::vec4& params)
	{
		SetProjectionParams(params.x, params.y, params.z, params.w);
	}

	void Activate(IRenderer& renderer) const
	{
		renderer.SetViewMatrix(GetViewMatrix());
		renderer.SetProjectionMatrix(m_mProjection);
	}

	void SetProjectionParams(float fov, float aspect, float nearplane, float farplane);
	inline void SetProjectionParams(const glm::vec4& params)
	{
		SetProjectionParams(params.x, params.y, params.z, params.w);
	}
	inline glm::vec4 GetProjectionParams() const
	{
		return glm::vec4(m_fFov, m_fAspect, m_fNearPlane, m_fFarPlane);
	}

	inline void SetAspect(float aspect)
	{
		SetProjectionParams(m_fFov, aspect, m_fNearPlane, m_fFarPlane);
	}

	inline void LookAt(const glm::vec3& from, const glm::vec3& at, const glm::vec3& up = { 0.0f, 1.0f, 0.0f })
	{
		m_mModel = glm::inverse(glm::lookAt(from, at, up));
	}

	inline glm::mat4 GetViewMatrix() const { return glm::inverse(GetWorldMatrix()); }

	inline glm::mat4& ProjectionMatrix() { return m_mProjection; }
	inline const glm::mat4& GetProjectionMatrix() const { return m_mProjection; }
	inline void SetProjectionMatrix(const glm::mat4& m) { m_mProjection = m; }

protected:
	glm::mat4			m_mProjection;

	// projection parameters
	float				m_fFov;
	float				m_fAspect;
	float				m_fNearPlane;
	float				m_fFarPlane;
};