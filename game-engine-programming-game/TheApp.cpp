#include "TheApp.h"

TheApp::TheApp() :
    m_uVertexShader(0),
    m_uFragmentShader(0),
    m_uProgram(0),
    m_uTexture(0)
{
}

bool TheApp::OnCreate()
{
    auto renderer = GetOpenGLRenderer();
    m_uVertexShader = renderer->CreateVertexShaderFromFile("phongshader.vert");
    m_uFragmentShader = renderer->CreateFragmentShaderFromFile("phongshader.frag");
    m_uProgram = renderer->CreateProgram(m_uVertexShader, m_uFragmentShader);
    m_uTexture = renderer->CreateTexture("nice-texture.jpg");
    if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram || !m_uTexture)
    {
        return false;
    }

    // generate geometry
    constexpr float radius = 1.0f;
    m_pSphere = std::make_shared<Geometry>();
    m_pSphere->GenSphere(glm::vec3(radius));

    // init material
    m_pMaterial = std::make_shared<Material>();
    m_pMaterial->m_cAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    m_pMaterial->m_cDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    m_pMaterial->m_cSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //m_pMaterial->m_cEmissive = glm::vec4(0.2f, 0.2f, 1.0f, 1.0f);


    // create the scene root
    m_pSceneRoot = std::make_unique<Node>();

    // create camera
    auto camera = std::make_shared<CameraNode>();
    camera->SetName("camera");
    camera->SetAspect(GetAspect());
    camera->SetPos({ 0.0f, 0.0f, 10.0f });
    //camera->SetAcceleration({ 0.0f, 0.0f, -1.0f });
    m_pSceneRoot->AddNode(camera);

    // create wasd controller
    auto wasd = std::make_shared<WasdControllerNode>();
    wasd->SetControlNode(camera);
    m_pSceneRoot->AddNode(wasd);

    // create scene
    for (size_t i = 0; i < 20; ++i)
    {
        auto node = std::make_shared<GeometryNode>(m_pSphere, m_pMaterial);
        node->SetTexture(0, m_uTexture);

        node->SetPos(glm::vec3(glm::linearRand(-5.0f, 5.0f),
            glm::linearRand(-5.0f, 5.0f),
            glm::linearRand(-10.0f, 0.0f)));

        if (glm::linearRand(0, 5) == 0)
        {
            for (size_t j = 0; j < 5; ++j)
            {
                auto node2 = std::make_shared<GeometryNode>(m_pSphere, m_pMaterial);
                node2->SetTexture(0, m_uTexture);
                node2->SetPos({ 0.0f, radius * (j + 1) * 2.0f, 0.0f });
                SetRandomRotation(*node2);
                node->AddNode(node2);
            }
            SetRandomRotation(*node);
        }

        //node->SetVelocity({ 0.0f, 10.0f, 0.0f });
        //node->SetAcceleration({ 0.0f, -9.81f, 0.0f });
        //SetRandomRotation(*node);

        m_pSceneRoot->AddNode(node);
    }


	return true;
}

void TheApp::OnDestroy()
{
    m_pSceneRoot = nullptr;
    m_pSphere = nullptr;

    glDeleteTextures(1, &m_uTexture);
    glDeleteProgram(m_uProgram);
    glDeleteShader(m_uFragmentShader);
    glDeleteShader(m_uVertexShader);
}

void TheApp::OnUpdate(float frametime)
{
    if (m_pSceneRoot)
    {
        m_pSceneRoot->Update(frametime);

        if (IsKeyDown(VK_LEFT))
        {
            m_pSceneRoot->SetVelocity({ -5.0f, 0.0f, 0.0f });
        }
        else if (IsKeyDown(VK_RIGHT))
        {
            m_pSceneRoot->SetVelocity({ 5.0f, 0.0f, 0.0f });
        }
        else
        {
            m_pSceneRoot->SetVelocity({ 0.0f, 0.0f, 0.0f });
        }

        /*
        auto& nodes = m_pSceneRoot->GetNodes();
        for (auto& node : nodes)
        {
            auto pos = node->GetPos();
            if (pos.y < -4.0f)
            {
                pos.y = -4.0f;
                node->Velocity() *= -0.5f;
                node->SetPos(pos);

                if (glm::length(node->Velocity()) < 0.5f)
                {
                    node->SetRotationSpeed(0.0f);
                }
                else
                {
                    SetRandomRotation(*node);
                }
            }
        }
        */
    }
}

void TheApp::OnDraw(IRenderer& renderer)
{
    renderer.Clear(0.2f, 0.2f, 0.2f, 1.0f);

    glUseProgram(m_uProgram);

    // activate the camera
    const auto* camera = static_cast<CameraNode*>(m_pSceneRoot->FindNode("camera"));
    camera->Activate(renderer);

    // set the camera position
    OpenGLRenderer::SetUniformVec3(m_uProgram, "cameraPosition", -renderer.GetViewMatrix()[3]);

    // set the light direction
    const glm::vec3 lightDirection(glm::normalize(glm::vec3(1.0f, 0.1f, -1.0f)));
    OpenGLRenderer::SetUniformVec3(m_uProgram, "lightDirection", lightDirection);

    if (m_pSceneRoot)
    {
        m_pSceneRoot->Render(renderer, m_uProgram);
    }
}

void TheApp::OnScreenChanged(uint32_t widthPixels, uint32_t heightPixels)
{
    if (m_pSceneRoot)
    {
        auto camera = static_cast<CameraNode*>(m_pSceneRoot->FindNode("camera"));
        camera->SetAspect(GetAspect());
    }
}

bool TheApp::OnKeyDown(uint32_t keyCode)
{
    if (keyCode == VK_ESCAPE)
    {
        Close();
        return true;
    }
    return false;
}

void TheApp::SetRandomRotation(Node& node) const
{
    const glm::vec3 axis(glm::normalize(glm::vec3(glm::linearRand(-1.0f, 1.0f),
        glm::linearRand(-1.0f, 1.0f),
        glm::linearRand(-1.0f, 1.0f))));
    node.SetRotationAxis(axis);
    node.SetRotationAngle(glm::linearRand(0.0f, glm::two_pi<float>()));
    node.SetRotationSpeed(glm::linearRand(-3.0f, 3.0f));
}

