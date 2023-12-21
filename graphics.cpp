#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{
	// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
  // cout << glewGetString(GLEW_VERSION) << endl;
	glewExperimental = GL_TRUE;

	auto status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		return false;
	}
#endif



	// Init Camera
	m_camera = new Camera();
	if (!m_camera->Initialize(width, height))
	{
		printf("Camera Failed to Initialize\n");
		return false;
	}


	// Set up the shaders
	m_shader = new Shader();
	if (!m_shader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_shader->AddShader(GL_VERTEX_SHADER))
	{
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shader->AddShader(GL_FRAGMENT_SHADER))
	{
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_shader->Finalize())
	{
		printf("Program to Finalize\n");
		return false;
	}

	// Populate location bindings of the shader uniform/attribs
	if (!collectShPrLocs()) {
		printf("Some shader attribs not located!\n");
	}

	
	// Starship
	m_mesh = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\SpaceShip-1.obj", "assets\\SpaceShip-1.png");

	// The Sun
	m_sphere = new Sphere(64, "assets\\2k_sun.jpg");

	// Mercury
	m_sphere4 = new Sphere(48, "assets\\Mercury.jpg");

	// Venus
	m_sphere5 = new Sphere(48, "assets\\Venus.jpg");

	// The Earth
	m_sphere2 = new Sphere(48, "assets\\2k_earth_daymap.jpg");

	// The moon
	m_sphere3 = new Sphere(48, "assets\\2k_moon.jpg");

	// Mars
	m_sphere6 = new Sphere(48, "assets\\Mars.jpg");

	// Jupiter
	m_sphere8 = new Sphere(48, "assets\\Jupiter.jpg");

	// Saturn
	m_sphere9 = new Sphere(48, "assets\\Saturn.jpg");

	// Saturn's Rings
	m_ring = new Ring(48, "assets\\Saturn_ring.png");

	// Uranus
	m_sphere10 = new Sphere(48, "assets\\Uranus.jpg");

	// Neptune
	m_sphere11 = new Sphere(48, "assets\\Neptune.jpg");

	// Ceres - Asteroid
	m_sphere7 = new Sphere(48, "assets\\Ceres.jpg");

	// Eris - Asteroid
	m_sphere12 = new Sphere(48, "assets\\Eris.jpg");

	// Haumea - Asteroid
	m_sphere13 = new Sphere(48, "assets\\Haumea.jpg");

	// SkyBox
	m_skybox = new SkyBox("assts\\Cubemaps\\Galaxy-cubemap2.png");
	m_skybox->setupModelMatrix(glm::vec3(.0f, .0f, .0f), 0.0f, 100.0f);


	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	return true;
}

void Graphics::set_mode(bool b) {
	if (b != observation_mode) {
		observation_mode = b;
		if (b) {
			EnterObservational();
		}
		else {
			ExitObservational();
		}
	}
}

void Graphics::EnterObservational() {
	CameraProjectionBackup = m_camera->GetProjection();
	CameraViewBackup = m_camera->GetView();
	CameraPosBackup = m_camera->GetPos();
	CameraFrontBackup = m_camera->GetFront();
	CameraUpBackup = m_camera->GetUp();
	xBackup = m_camera->Getx();
	yBackup = m_camera->Gety();
	zBackup = m_camera->Getz();
	// todo determine which planet is nearest
	// todo maybe animate transition to observational mode
}

void Graphics::ObservationModeUpdate(double dt) {
	float orbiter_scale = 0.5;  // scale the sizes of all the moons, planets, asteroids, and spaceships
	int col;

	std::vector<float> speed, dist, rotSpeed, scale;
	glm::vec3 rotVector;
	glm::mat4 localTransform;

	// position of the camera
	speed = { 0.3, 0.3, 0.3 };
	dist = { 1.25, .50, 1.25 };
	// transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale](float& c) { return c * system_distances_scale; });
	rotVector = { 1.,1.,1. };
	rotSpeed = { .0, .0, .0 };
	scale = { .20f, .20f, .20f };
	transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store moon-planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_camera != NULL)
		col = 0;
		glm::vec3 pos = { localTransform[col].x, localTransform[col].y, localTransform[col].z };
		//col = 1;
		//glm::vec3 front = { localTransform[col].x, localTransform[col].y, localTransform[col].z };
		//col = 2;
		//glm::vec3 up = { localTransform[col].x, localTransform[col].y, localTransform[col].z };
		pos = { 100.0f, 100.0f, 100.0f };
		m_camera->PlanetaryObsMode(pos);
		//m_camera->cameraPosVert(0.5f);
	modelStack.pop();	// Camera
}

void Graphics::ExitObservational() { 
	m_camera->SetProjection(CameraProjectionBackup);
	m_camera->SetView(CameraViewBackup);
	m_camera->SetPos(CameraPosBackup);
	m_camera->SetFront(CameraFrontBackup);
	m_camera->SetUp(CameraUpBackup);
	m_camera->Setx(xBackup);
	m_camera->Sety(yBackup);
	m_camera->Setz(zBackup);
	//m_camera->PlanetaryObsMode(CameraPosBackup, CameraFrontBackup, CameraUpBackup);
	// todo maybe animate transition out of observational mode
}

void Graphics::HierarchicalUpdate2(double dt) {
	int col;

	float system_distances_scale = 0.4;  // scales all the distance vectors down from their realistic values
	float exoplanet_distances_scale = 0.18;  // adjust the distance of the exoplanets and Neptune from the sun
	float orbiter_scale = 0.5;  // scale the sizes of all the moons, planets, asteroids, and spaceships

	std::vector<float> speed, dist, rotSpeed, scale;
	glm::vec3 rotVector;
	glm::mat4 localTransform;

	// Updating the skybox
	m_skybox->Update(dt);

	// position of the sun	
	modelStack.push(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)));  // sun's coordinate
	localTransform = modelStack.top();		// The sun origin
	localTransform *= glm::rotate(glm::mat4(1.0f), (float)dt * .5f, glm::vec3(0.f, 1.f, 0.f));
	localTransform *= glm::scale(glm::vec3(1.25, 1.25, 1.25));
	if (m_sphere != NULL)
		m_sphere->Update(localTransform);

	// Mercury
	speed = { 0.25, 0.25, 0.25 };
	dist = { 3. , 0., 3. };
	transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale](float& c) { return c * system_distances_scale * 1.8; });
	rotVector = { 0. , .98, 0. };
	rotSpeed = { 0.01, 0.01, 0.01 };
	scale = { .75,.75,.75 };
	transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_sphere4 != NULL)
		m_sphere4->Update(localTransform);

	modelStack.pop();	// Mercury

	// Venus
	speed = { .1, 0.1, .1 };
	dist = { 8.8 , 0., 8.8 }; // originally 8., 0., 8. 
	transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale](float& c) { return c * system_distances_scale; });
	rotVector = { 0. , 1., 0. };
	rotSpeed = { 0.005, 0.005, 0.005 };
	scale = { .5f,.5f,.5f };
	transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_sphere5 != NULL)
		m_sphere5->Update(localTransform);

	if (observation_mode) {
		// position of the camera
		speed = { 0.3, 0.3, 0.3 };
		dist = { 1.25, .50, 1.25 };
		transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale](float& c) { return c * 20; });
		rotVector = { 1.,1.,1. };
		rotSpeed = { .0, .0, .0 };
		scale = { .20f, .20f, .20f };
		transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
		localTransform = modelStack.top();
		localTransform *= glm::translate(glm::mat4(1.f),
			glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
		modelStack.push(localTransform);			// store moon-planet-sun coordinate
		localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
		localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
		if (m_camera != NULL) {
			col = 1;
			glm::vec3 pos = { localTransform[col].x, localTransform[col].y, localTransform[col].z };
			pos = { 20.0f, 20.0f, 20.0f };
			m_camera->PlanetaryObsMode(pos);
			//m_camera->cameraPosVert(0.5f);
		}
		modelStack.pop();	// Camera
	}

	modelStack.pop();	// Venus


	// position of the Earth
	speed = { .075, .075, .075 };
	dist = { 15. , 0., 15.};
	transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale](float& c) { return c * system_distances_scale; });
	rotVector = { 0. , 1., 0. };
	rotSpeed = { 0.1, .1, .1 };
	scale = { .75,.75,.75 };
	transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_sphere2 != NULL)
		m_sphere2->Update(localTransform);
	
	// position of the moon
	speed = { 0.3, 0.3, 0.3 };
	dist = { 1.25, .50, 1.25 };
	// transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale](float& c) { return c * system_distances_scale; });
	rotVector = { 1.,1.,1. };
	rotSpeed = { .025, .025, .025 };
	scale = { .20f, .20f, .20f };
	transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store moon-planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_sphere3 != NULL)
		m_sphere3->Update(localTransform);

	modelStack.pop();	// Moon
	modelStack.pop();	// Earth

	// Mars
	speed = { .070, .070, .070 };
	dist = { 23. , 0., 23. };
	transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale](float& c) { return c * system_distances_scale; });
	rotVector = { 0. , 1., 0. };
	rotSpeed = { .11, .11, .11 };
	scale = { .75,.75,.75 };
	transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_sphere6 != NULL)
		m_sphere6->Update(localTransform);

	modelStack.pop();	// Mars

	// Ceres - Asteroid
	speed = { .075, .075, .075 };
	dist = { 40. , 0., 40. };  // todo could be more realistic
	transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale](float& c) { return c * system_distances_scale; });
	rotVector = { 0. , 1., 0. };
	rotSpeed = { .1, .1, .1 };
	scale = { .75,.75,.75 };
	transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_sphere7 != NULL)
		m_sphere7->Update(localTransform);

	modelStack.pop();	// Ceres


	// Jupiter
	speed = { .05, .05, .05 };
	dist = { 60. , 0., 76. };
	transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale](float& c) { return c * system_distances_scale * 0.5; });
	rotVector = { 0. , 1., 0. };
	rotSpeed = { .2, .2, .2 };
	scale = { .75,.75,.75 };
	transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_sphere8 != NULL)
		m_sphere8->Update(localTransform);

	modelStack.pop();	// Jupiter

	// Saturn
	speed = { .04, .04, .04 };
	dist = { 83. , 0., 83. };
	transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale](float& c) { return c * system_distances_scale * 0.5; });
	rotVector = { 0. , 1., 0. };
	rotSpeed = { .19, .19, .19 };
	scale = { .75,.75,.75 };
	transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_sphere9 != NULL)
		m_sphere9->Update(localTransform);

	
	// Saturn's Rings

	speed = { 0.7, 0.7, 0.7 };
	dist = { .0, .0, .0 };  // todo I think this probably has to be 0'd out, but I'm leaving it like this to see Saturn's rings orbit in a funny way
	// transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale](float& c) { return c * system_distances_scale; });
	rotVector = { 0.,0.,1. };
	rotSpeed = { .25, .0, .0 };
	scale = { 1.5f, 1.5f, 1.5f };  // todo adjust
	transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store moon-planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ring != NULL)
		m_ring->Update(localTransform);

	modelStack.pop();	// Saturn's Rings
	modelStack.pop();	// Saturn

	// Uranus
	speed = { .03, .03, .03 };
	dist = { 203. , 0., 203. };
	transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale, exoplanet_distances_scale](float& c) { return c * system_distances_scale * 0.25; });
	rotVector = { 0. , 1., 0. };
	rotSpeed = { .15, .15, .15 };
	scale = { .75,.75,.75 };
	transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_sphere10 != NULL)
		m_sphere10->Update(localTransform);

	modelStack.pop();	// Uranus

	// Neptune
	speed = { .02, .02, .02 };
	dist = { 323. , 0., 323. };
	transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale, exoplanet_distances_scale](float& c) { return c * system_distances_scale * exoplanet_distances_scale; });
	rotVector = { 0. , 1., 0. };
	rotSpeed = { .14, .14, .14 };
	scale = { .75,.75,.75 };
	transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_sphere11 != NULL)
		m_sphere11->Update(localTransform);

	modelStack.pop();	// Neptune

	// Haumea - Asteroid
	speed = { .075, .075, .075 };
	dist = { 350. , 0., 350. };  // todo
	transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale, exoplanet_distances_scale](float& c) { return c * system_distances_scale * exoplanet_distances_scale; });
	rotVector = { 0. , 1., 0. };
	rotSpeed = { .1, .1, .1 };
	scale = { .75,.75,.75 };
	transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_sphere13 != NULL)
		m_sphere13->Update(localTransform);

	modelStack.pop();	// Haumea


	// Eris - Asteroid
	speed = { .075, .075, .075 };
	dist = { 400. , 0., 400. };  // todo
	transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale, exoplanet_distances_scale](float& c) { return c * system_distances_scale * exoplanet_distances_scale; });
	rotVector = { 0. , 1., 0. };
	rotSpeed = { .1, .1, .1 };
	scale = { .75,.75,.75 };
	transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_sphere12 != NULL)
		m_sphere12->Update(localTransform);

	modelStack.pop();	// Eris

	// Starship
	speed = { 1, 1., 1. };
	dist = { 0, 6., 6. };
	transform(dist.begin(), dist.end(), dist.begin(), [system_distances_scale](float& c) { return c * system_distances_scale; });
	rotVector = { 1 , 0, 0 };
	rotSpeed = { 1, 1, 1. };
	scale = { .02,.02,.02 };
	transform(scale.begin(), scale.end(), scale.begin(), [orbiter_scale](float& c) { return c * orbiter_scale; });
	localTransform = modelStack.top();	
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(sin(speed[0] * dt) * dist[0], cos(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), -80.f, glm::vec3(1, 0, 0));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt , rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mesh != NULL)
		m_mesh->Update(localTransform);

	modelStack.pop(); 	// back to the planet coordinate
	modelStack.pop(); 	// back to the sun coordinate

	//modelStack.pop();	// empy stack


}


void Graphics::ComputeTransforms(double dt, std::vector<float> speed, std::vector<float> dist, 
	std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat) {
	tmat = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2])
	);
	rmat = glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	smat = glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
}

void Graphics::Render()
{
	//clear the screen
	//glClearColor(0.5, 0.2, 0.2, 1.0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start the correct program
	m_shader->Enable();


	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	// SkyBox
	if (m_sphere != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere->GetModel()));
		if (m_sphere->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_skybox->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);

			m_sphere->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	
	// Starship
	if (m_mesh != NULL) {
		glUniform1i(m_hasTexture, false);
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mesh->GetModel()));
		if (m_mesh->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mesh->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Sun
	if (m_sphere != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere->GetModel()));
		if (m_sphere->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			
			m_sphere->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Mercury
	if (m_sphere4 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere4->GetModel()));
		if (m_sphere4->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere4->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);

			m_sphere4->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Venus
	if (m_sphere5 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere5->GetModel()));
		if (m_sphere5->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere5->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);

			m_sphere5->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	
	// Earth
	if (m_sphere2 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere2->GetModel()));
		if (m_sphere2->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere2->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sphere2->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Render Moon
	if (m_sphere3 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere3->GetModel()));
		if (m_sphere3->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere3->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sphere3->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Mars
	if (m_sphere6 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere6->GetModel()));
		if (m_sphere6->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere6->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sphere6->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Ceres - Asteroid
	if (m_sphere7 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere7->GetModel()));
		if (m_sphere7->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere7->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sphere7->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Jupiter
	if (m_sphere8 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere8->GetModel()));
		if (m_sphere8->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere8->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sphere8->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Saturn
	if (m_sphere9 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere9->GetModel()));
		if (m_sphere9->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere9->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sphere9->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Saturn's Rings
	if (m_ring != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ring->GetModel()));
		if (m_ring->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ring->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ring->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Uranus
	if (m_sphere10 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere10->GetModel()));
		if (m_sphere10->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere10->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sphere10->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Neptune
	if (m_sphere11 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere11->GetModel()));
		if (m_sphere11->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere11->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sphere11->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Eris - Asteroid
	if (m_sphere12 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere12->GetModel()));
		if (m_sphere12->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere12->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sphere12->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Haumea - Asteroid
	if (m_sphere13 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere13->GetModel()));
		if (m_sphere13->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere13->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sphere13->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Get any errors from OpenGL
	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}

	// unbind cube map texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// Disable to shader program
	glUseProgram(0);

}

bool Graphics::collectShPrLocs() {
	bool anyProblem = true;
	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_projectionMatrix not found\n");
		anyProblem = false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_viewMatrix not found\n");
		anyProblem = false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_modelMatrix not found\n");
		anyProblem = false;
	}

	// Locate the position vertex attribute
	m_positionAttrib = m_shader->GetAttribLocation("v_position");
	if (m_positionAttrib == -1)
	{
		printf("v_position attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_colorAttrib = m_shader->GetAttribLocation("v_color");
	if (m_colorAttrib == -1)
	{
		printf("v_color attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_tcAttrib = m_shader->GetAttribLocation("v_tc");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	m_hasTexture = m_shader->GetUniformLocation("hasTexture");
	if (m_hasTexture == INVALID_UNIFORM_LOCATION) {
		printf("hasTexture uniform not found\n");
		anyProblem = false;
	}

	return anyProblem;
}

std::string Graphics::ErrorString(GLenum error)
{
	if (error == GL_INVALID_ENUM)
	{
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
	}

	else if (error == GL_INVALID_VALUE)
	{
		return "GL_INVALID_VALUE: A numeric argument is out of range.";
	}

	else if (error == GL_INVALID_OPERATION)
	{
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	}

	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
	{
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
	}

	else if (error == GL_OUT_OF_MEMORY)
	{
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	}
	else
	{
		return "None";
	}
}

