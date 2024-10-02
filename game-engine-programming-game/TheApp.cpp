#include "TheApp.h"

TheApp::TheApp() :
	m_uVertexShader(0),
	m_uFragmentShader(0),
	m_uProgram(0)
{
}

bool TheApp::OnCreate()
{
	OpenGLRenderer* renderer = GetOpenGLRenderer();

	if (!renderer)
	{
		Debug("Failed to create OpenGL renderer.\n");
		return false;
	}

	m_uVertexShader = renderer->CreateVertexShaderFromFile("phongshader.vert");
	m_uFragmentShader = renderer->CreateFragmentShaderFromFile("phongshader.frag");
	m_uProgram = renderer->CreateProgram(m_uVertexShader, m_uFragmentShader);

	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram)
	{
		Debug("Failed to create program.\n");
		return false;
	}

	m_pSphere = std::make_shared<Geometry>();
	m_pSphere->GenSphere(glm::vec3(0.5f));

	return true;
}

void TheApp::OnDestroy()
{
	m_pSphere = nullptr;

	glDeleteProgram(m_uProgram);
	glDeleteShader(m_uVertexShader);
	glDeleteShader(m_uFragmentShader);
}

void TheApp::OnUpdate(float frametime)
{
	// Debug(std::string("FPS: ") + std::to_string(1.0f / frametime) + "\n");
}

void TheApp::OnDraw(IRenderer& renderer)
{
	renderer.Clear(0.2f, 0.2f, 0.2f, 1.0f);

	glUseProgram(m_uProgram);
	m_pSphere->SetAttribs(m_uProgram);
	m_pSphere->Draw(renderer);
}
