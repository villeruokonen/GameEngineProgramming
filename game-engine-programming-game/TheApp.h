#pragma once
#include "../game-engine-core/include/IApplication.h"
#include "../game-engine-core/include/Geometry.h"
#include "../game-engine-core/include/Material.h"


class TheApp : public IApplication
{
public:
	TheApp();

	bool OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float frametime) override;
	void OnDraw(IRenderer& renderer) override;

protected:
	void OnScreenChanged(uint32_t widthPixels, uint32_t heightPixels) override;

private:
	OpenGLRenderer* GetOpenGLRenderer() { return static_cast<OpenGLRenderer*>(GetRenderer()); }

	GLuint							m_uVertexShader;
	GLuint							m_uFragmentShader;
	GLuint							m_uProgram;

	GLuint							m_uTexture;

	std::shared_ptr<Geometry>		m_pSphere;
	std::shared_ptr<Material>		m_pMaterial;

	glm::mat4						m_mModel;
};


