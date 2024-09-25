#pragma once

#include "IRenderer.h"
#include "IApplication.h"

#include <gl/GL.h>
#include "./GL/glext.h"
#include "./GL/myGL.h"

class OpenGLRenderer : public IRenderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	/**
	 * Create (from IRenderer)
	 * @return true if succesful, or false to cancel app startup
	 */
	bool Create() override;

	/**
	 * Flip (from IRenderer)
	 * transfer graphics to screen
	 */
	void Flip() override;

	/**
	 * Clear
	 * clear the color buffer, depth buffer and stencil
	 */
	void Clear(float r, float g, float b, float a, float depth = 1.0f, int32_t stencil = 0) override;

	/**
	 * SetViewport
	 * set viewport area of the screen
	 * @param area, x, y, width, height of the viewport area
	 */
	void SetViewport(const glm::ivec4& area) override;

	/**
	 * SetTexture
	 * @param program
	 * @param texture handle
	 * @param slot index of the texture slot
	 * @param uniformName name of the uniform in shader
	 * @return true if uniform is found and set
	 */
	bool SetTexture(GLuint program, GLuint texture, uint32_t slot, const std::string_view& uniformName) override;

	GLuint CreateTexture(const std::string_view& filename);

	GLuint CreateVertexShader(const char* sourceCode);
	GLuint CreateVertexShaderFromFile(const std::string_view& filename);

	GLuint CreateFragmentShader(const char* sourceCode);
	GLuint CreateFragmentShaderFromFile(const std::string_view& filename);

	GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);

	void PrintShaderError(GLuint shader);
	void PrintProgramError(GLuint program);

private:
	/**
	 * InitFunctions
	 * static helper to get OpenGL function pointers after context has been created
	 */
	static bool InitFunctions();
	bool SetDefaultSettings();

	HDC			m_Context;		// handle to device context
	HGLRC		m_Resource;		// handle to resource context
};

