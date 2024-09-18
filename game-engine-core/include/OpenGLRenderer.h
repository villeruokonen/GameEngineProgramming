#pragma once

#include "IRenderer.h"
#include "IApplication.h"

#include <gl/GL.h>

class OpenGLRenderer : public IRenderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	bool Create() override;
	void Flip() override;
	void Clear(float r, float g, float b, float a, float depth, int32_t stencil) override;
	void SetViewport(const glm::ivec4& area) override;
	bool SetTexture(uint32_t program, uint32_t texture, uint32_t slot, const std::string_view& uniformName) override;

private:
	bool SetDefaultSettings();

	HDC			m_Context;	// handle to device context 
	HGLRC		m_Resource; // OpenGL handle to resource context 

};
