#pragma once
#include "../game-engine-core/include/IApplication.h"
#include "../game-engine-core/include/Geometry.h"
#include "../game-engine-core/include/Material.h"
#include "../game-engine-core/include/CameraNode.h"

#include "Physics.h"
#include "PhysicsNode.h"

class BowlingApp :
    public IApplication
{
public:
    BowlingApp();

	bool OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float frametime) override;
	void OnDraw(IRenderer& renderer) override;

private:
	bool OnKeyDown(uint32_t keyCode) override;

	OpenGLRenderer* GetOpenGLRenderer() { return static_cast<OpenGLRenderer*>(GetRenderer()); }

	GLuint							m_uVertexShader;
	GLuint							m_uFragmentShader;
	GLuint							m_uProgram;
	GLuint							m_uTexture;

	std::shared_ptr<Geometry>		m_pBox;
	std::shared_ptr<Material>		m_pMaterial;

	std::shared_ptr<Geometry>		m_pSphere;
	std::shared_ptr<Material>		m_pMaterial2;

	std::unique_ptr<Node>			m_pSceneRoot;

	std::shared_ptr<Physics>		m_pPhysics;
};

