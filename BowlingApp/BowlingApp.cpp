#include "BowlingApp.h"

BowlingApp::BowlingApp() :
    m_uVertexShader(0),
    m_uFragmentShader(0),
    m_uProgram(0),
    m_uTexture(0)
{
}

bool BowlingApp::OnCreate()
{
    auto renderer = GetOpenGLRenderer();
    m_uVertexShader = renderer->CreateVertexShaderFromFile("phongshader.vert");
    m_uFragmentShader = renderer->CreateFragmentShaderFromFile("phongshader.frag");
    m_uProgram = renderer->CreateProgram(m_uVertexShader, m_uFragmentShader);
    m_uTexture = renderer->CreateTexture("white.jpg");

    // start the physics
    m_pPhysics = std::make_shared<Physics>();

    // add static collision plane
    m_pPhysics->AddStaticCollisionPlane(glm::vec4(0.0f, 1.0f, 0.0f, -5.0f));

    // geometry
    constexpr float size = 1.0f;
    m_pBox = std::make_shared<Geometry>();
    m_pBox->GenCube(glm::vec3(size * 2.0f));

    m_pSphere = std::make_shared<Geometry>();
    m_pSphere->GenSphere(glm::vec3(size));

    // materials
    m_pMaterial = std::make_shared<Material>();
    m_pMaterial->m_cDiffuse = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    m_pMaterial->m_fSpecularPower = 75.0f;

    m_pMaterial2 = std::make_shared<Material>();
    m_pMaterial2->m_cDiffuse = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    m_pMaterial2->m_cSpecular = glm::vec4(0.4f, 0.4f, 1.0f, 1.0f);
    m_pMaterial2->m_fSpecularPower = 25.0f;

    // scenegraph
    m_pSceneRoot = std::make_unique<Node>();

    // camera
    auto camera = std::make_shared<CameraNode>();
    camera->SetName("camera");
    camera->SetProjectionParams({ 0.61f, GetAspect(), 1.0f, 500.0f });
    camera->LookAt({ 0.0f, 4.0f, 30.0f }, { 0.0f, 0.0f, 0.0f });
    m_pSceneRoot->AddNode(camera);

    return true;
}


void BowlingApp::OnDestroy()
{
    m_pSceneRoot = nullptr;

    glDeleteTextures(1, &m_uTexture);
    glDeleteProgram(m_uProgram);
    glDeleteShader(m_uFragmentShader);
    glDeleteShader(m_uVertexShader);
}


void BowlingApp::OnUpdate(float frametime)
{
    if (m_pSceneRoot)
    {
        m_pSceneRoot->Update(frametime);
    }
    if (m_pPhysics)
    {
        m_pPhysics->Update(frametime);
    }
}

void BowlingApp::OnDraw(IRenderer& renderer)
{
    renderer.Clear(0.2f, 0.2f, 0.2f, 1.0f);

    glUseProgram(m_uProgram);

    // setup the camera matrices to renderer
    const auto* camera = static_cast<CameraNode*>(m_pSceneRoot->FindNode("camera"));
    renderer.SetViewMatrix(camera->GetViewMatrix());
    renderer.SetProjectionMatrix(camera->GetProjectionMatrix());

    // setup light mode uniforms
    const glm::vec3 lightDirection(glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f)));
    const glm::vec3 cameraPos(-renderer.GetViewMatrix()[3]);
    OpenGLRenderer::SetUniformVec3(m_uProgram, "lightDirection", lightDirection);
    OpenGLRenderer::SetUniformVec3(m_uProgram, "cameraPosition", cameraPos);

    if (m_pSceneRoot)
    {
        m_pSceneRoot->Render(renderer, m_uProgram);
    }
}


bool BowlingApp::OnKeyDown(uint32_t keyCode)
{
    if (keyCode == VK_ESCAPE)
    {
        Close();
        return true;
    }
    else if (keyCode == VK_SPACE)
    {
        // spawn new physics node
        auto geom = m_pBox;
        auto mat = m_pMaterial;
        auto shape = PhysicsNode::CollisionShape::Box;

        if (glm::linearRand(0.0f, 1.0f) > 0.5)
        {
            geom = m_pSphere;
            mat = m_pMaterial2;
            shape = PhysicsNode::CollisionShape::Sphere;
        }

        auto node = std::make_shared<PhysicsNode>(geom, mat);
        node->SetTexture(0, m_uTexture);
        node->SetPos(glm::vec3(glm::linearRand(-5.0f, 5.0f), 15.0f, glm::linearRand(-10.0f, 0.0f)));
        node->SetRadius(1.0f);
        node->SetPhysics(m_pPhysics, 1.0f, shape);
        m_pSceneRoot->AddNode(node);
    }

    return false;
}
