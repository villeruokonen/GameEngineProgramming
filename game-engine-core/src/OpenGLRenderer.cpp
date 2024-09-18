#include "../include/OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer() :
	m_Context(nullptr),
	m_Resource(nullptr)
{
}

OpenGLRenderer::~OpenGLRenderer()
{
	if (m_Context)
	{
		wglMakeCurrent(m_Context, nullptr);
		wglDeleteContext(m_Resource);
		::ReleaseDC(IApplication::GetApp()->GetWindow(), m_Context);
		m_Context = nullptr;
	}
}

bool OpenGLRenderer::Create()
{
	m_Context = ::GetDC(IApplication::GetApp()->GetWindow());
	uint32_t flags = PFD_DRAW_TO_WINDOW
		| PFD_SUPPORT_OPENGL
		| PFD_DOUBLEBUFFER;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// size of type (C bullshit)
		1,								// version
		flags,
		PFD_TYPE_RGBA,					// frame buffer pixel type: RGBA or palette (color index)?
		32,								// color depth of frame buffer
		0, 0, 0, 0, 0, 0,				// RGB bits / shift
		0,								// alpha bits
		0,								// alpha shift
		0,								// accum bits
		0, 0, 0, 0,						// RGBA accum bits
		24,								// depth buffer bits
		8,								// stencil buffer bits
		0,								// aux buffers
		PFD_MAIN_PLANE,					// layer type
		0,								// is reserved?
		0, 0, 0							// masks						
	};

	// pixel format index
	int pixelFormat = ::ChoosePixelFormat(m_Context, &pfd);

	if (pixelFormat == 0)
	{
		// No compatible pixel format.
		return false;
	}

	if(!::SetPixelFormat(m_Context, pixelFormat, &pfd))
	{
		// Failed to set pixel format.
		return false;
	}

	// Create and activate rendering context
	m_Resource = wglCreateContext(m_Context);
	wglMakeCurrent(m_Context, m_Resource);

	SetDefaultSettings();

	return true;
}

void OpenGLRenderer::Flip()
{
	glFlush();

	SwapBuffers(m_Context);
}

void OpenGLRenderer::Clear(float r, float g, float b, float a, float depth, int32_t stencil)
{
	// Set up what to clear
	// TODO: glClearDepthf(depth);
	glClearStencil(stencil);

	glClearColor(r, g, b, a);

	// Actually perform clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderer::SetViewport(const glm::ivec4& area)
{
	glViewport(area.x, area.y, area.z, area.w);
}

bool OpenGLRenderer::SetTexture(uint32_t program, uint32_t texture, uint32_t slot, const std::string_view& uniformName)
{
	// TODO
	return false;
}

bool OpenGLRenderer::SetDefaultSettings()
{
	glStencilMask(0);
	glDisable(GL_SCISSOR_TEST);

	// TODO: glBindBuffer(GL_ARRAY_BUFFER, 0);
	// TODO: glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_FUNC);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);

	glDisable(GL_STENCIL_TEST);

	// TODO: glClearDepthf(1.0f);
	glClearStencil(0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// TODO: glBlendColor(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_CCW);

	return true;
}
