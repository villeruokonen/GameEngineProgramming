#include "../include/CameraNode.h"

void CameraNode::SetProjectionParams(float fov, float aspect, float nearplane, float farplane)
{
	m_mProjection = glm::perspective(fov, aspect, nearplane, farplane);

	m_fFov = fov;
	m_fAspect = aspect;
	m_fNearPlane = nearplane;
	m_fFarPlane = farplane;
}

