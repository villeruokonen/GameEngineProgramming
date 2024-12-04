#include "BowlingApp.h"

BowlingApp::BowlingApp() :
	m_uVertexShader(0),
	m_uFragmentShader(0),
	m_uProgram(0),
	m_vPinSize(1.0f, 4.0f, 1.0f),
	m_eState(State::Aiming),
	m_fSpacebarTime(0.0f)
{
}

bool BowlingApp::OnCreate()
{
	auto renderer = GetOpenGLRenderer();
	m_uVertexShader = renderer->CreateVertexShaderFromFile("phongshader.vert");
	m_uFragmentShader = renderer->CreateFragmentShaderFromFile("phongshader.frag");
	m_uProgram = renderer->CreateProgram(m_uVertexShader, m_uFragmentShader);
	m_arrTextures.push_back(renderer->CreateTexture("white.jpg"));
	m_arrTextures.push_back(renderer->CreateTexture("pin.jpg"));

	// start the physics
	m_pPhysics = std::make_shared<Physics>();

	// add static collision plane
	m_pPhysics->AddStaticCollisionPlane(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

	// geometry
	constexpr float size = 1.0f;
	m_pPin = std::make_shared<Geometry>();
	m_pPin->GenCube(glm::vec3(1.0f, 4.0f, 1.0f));

	m_pSphere = std::make_shared<Geometry>();
	m_pSphere->GenSphere(glm::vec3(m_vPinSize.y * 0.33f));

	// make quad for plane
	m_pFloor = std::make_shared<Geometry>();
	m_pFloor->GenQuad({ 1000.0f, 1000.f });

	// materials
	m_pMaterial = std::make_shared<Material>();
	m_pMaterial->m_cDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pMaterial->m_fSpecularPower = 75.0f;

	m_pMaterial2 = std::make_shared<Material>();
	m_pMaterial2->m_cDiffuse = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	m_pMaterial2->m_cSpecular = glm::vec4(0.4f, 0.4f, 1.0f, 1.0f);
	m_pMaterial2->m_fSpecularPower = 25.0f;

	m_pFloorMaterial = std::make_shared<Material>();
	m_pFloorMaterial->m_cDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pFloorMaterial->m_fSpecularPower = 0.0f;

	ResetGame();

	return true;
}


void BowlingApp::OnDestroy()
{
	m_pSceneRoot = nullptr;

	glDeleteTextures((GLsizei)m_arrTextures.size(), m_arrTextures.data());
	glDeleteProgram(m_uProgram);
	glDeleteShader(m_uFragmentShader);
	glDeleteShader(m_uVertexShader);
}


void BowlingApp::OnUpdate(float frametime)
{
	if (m_pSceneRoot)
	{
		m_pSceneRoot->Update(frametime);

		if (m_eState == State::Aiming)
		{
			auto ball = static_cast<PhysicsNode*>(m_pSceneRoot->FindNode("ball"));
			auto rb = ball->GetRigidBody();

			constexpr float ballStrafeSpd = 8;
			if (IsKeyDown(VK_LEFT))
			{
				rb->setActivationState(1);
				rb->setLinearVelocity(btVector3(-ballStrafeSpd, 0.0f, 0.0f));
			}
			else if (IsKeyDown(VK_RIGHT))
			{
				rb->setActivationState(1);
				rb->setLinearVelocity(btVector3(ballStrafeSpd, 0.0f, 0.0f));
			}
			else
			{
				auto vel = rb->getLinearVelocity();
				vel.setX(vel.getX() * frametime);
				rb->setActivationState(1);
				rb->setLinearVelocity(vel);
			}

			if (IsKeyDown(VK_SPACE))
			{
				m_fSpacebarTime += frametime;
				rb->setAngularVelocity(btVector3(-m_fSpacebarTime * 10.0f, 0, 0));
				rb->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
			}
			else if (m_fSpacebarTime > 0.0f)
			{
				// throw ball
				auto ball = static_cast<PhysicsNode*>(m_pSceneRoot->FindNode("ball"));
				ball->GetRigidBody()->setActivationState(1);
				ball->GetRigidBody()->setLinearVelocity(btVector3(0.0f, 2.0f, -60.0f * m_fSpacebarTime));

				m_fSpacebarTime = 0.0f;
				m_eState = State::Running;
			}
		}
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
	else if (keyCode == VK_RETURN)
	{
		ResetGame();
	}

	return false;
}

void BowlingApp::ResetGame()
{
	m_pSceneRoot = nullptr;

	// scenegraph
	m_pSceneRoot = std::make_unique<Node>();

	// floor
	auto floor = std::make_shared<GeometryNode>(m_pFloor, m_pFloorMaterial);
	glm::mat4 r = glm::rotate(glm::mat4(1.0f), glm::half_pi<float>(), glm::vec3(-1.0f, 0.0f, 0.0f));
	floor->SetMatrix(r);
	m_pSceneRoot->AddNode(floor);

	// spawn pins
	//  X X X X
	//   X X X
	//    X X
	//     X

	constexpr float zOffset = -30.0f;
	constexpr float pinMass = 2.0f;

	// Initial position of the first pin
	glm::vec3 basePos(0.0f, m_vPinSize.y * 0.5f, zOffset);

	int rows = 4;
	for (int row = 0; row < rows; ++row)
	{
		// Calculate the number of pins in the current row
		int pinsInRow = rows - row;

		// Calculate the X offset to center the pins for this row
		float xStartOffset = -(pinsInRow - 1) * m_vPinSize.x;

		for (int i = 0; i < pinsInRow; ++i)
		{
			// Calculate the position for the current pin
			glm::vec3 pos = basePos;
			pos.x += xStartOffset + i * m_vPinSize.x * 2.0f; // X position
			pos.z += row * m_vPinSize.z * 2.0f;             // Z position for the row

			// Create and place the pin
			auto pin = std::make_shared<PhysicsNode>(m_pPin, m_pMaterial);
			pin->SetPos(pos);
			pin->SetPhysics(m_pPhysics, pinMass, PhysicsNode::CollisionShape::Box, m_vPinSize);
			pin->SetTexture(0, m_arrTextures[1]);

			m_pSceneRoot->AddNode(pin);
		}
	}

	// spawn ball
	constexpr float ballMass = 10.0f;
	auto ball = std::make_shared<PhysicsNode>(m_pSphere, m_pMaterial2);
	ball->SetName("ball");
	ball->SetRadius(m_vPinSize.y * 0.33f);
	ball->SetPos({ glm::linearRand(-m_vPinSize.x * 4.0f, m_vPinSize.x * 4.0f), ball->GetRadius(), 30.0f });
	// ball->SetPos({ 0.0f, ball->GetRadius(), 15.0f });
	ball->SetPhysics(m_pPhysics, ballMass, PhysicsNode::CollisionShape::Sphere);

	m_pSceneRoot->AddNode(ball);

	// camera
	auto camera = std::make_shared<CameraNode>();
	camera->SetName("camera");
	camera->SetProjectionParams({ 0.61f, GetAspect(), 1.0f, 500.0f });
	camera->SetPos(glm::vec3(0.0f, 7.0f, ball->GetPos().z + 30.0f));
	// camera->LookAt({ 0.0f, 10.0f, 3.0f }, { 0.0f, 0.0f, 0.0f });
	m_pSceneRoot->AddNode(camera);

	// state
	m_eState = State::Aiming;
	m_fSpacebarTime = 0.0f;
}
