#include "Scene.h"
#include <AssetManager.h>

#include <cmath>

Scene::Scene(OpenGLWindow * window) :
	m_window(window)
{
	assert(window != nullptr);
}

Scene::~Scene()
{}

void Scene::drawRobotPart(const glm::mat4& parentMatrix, Transform& transform) {
	m_shader->setUniform("model", parentMatrix * transform.getMatrix(), false);
	glDrawElements(GL_TRIANGLES, sizeof(cubeInd) / sizeof(int), GL_UNSIGNED_INT, nullptr);
}

void Scene::setRobotInitialScale()
{
	robotTransform.setScale(glm::vec3(0.75f, 0.75f, 0.75f));

	const auto torsoScale = glm::vec3(0.6f, 0.9f, 0.4f);
	torsoTransform.setScale(torsoScale);

	headTransform.setScale(glm::vec3(0.3f, 0.3f, 0.3f) / torsoScale);
	leftLegTransform.setScale(glm::vec3(0.1f, 0.6f, 0.1f) / torsoScale);
	rightLegTransform.setScale(glm::vec3(0.1f, 0.6f, 0.1f) / torsoScale);
	leftUpperArmTransform.setScale(glm::vec3(0.1f, 0.35f, 0.1f) / torsoScale);
	rightUpperArmTransform.setScale(glm::vec3(0.1f, 0.35f, 0.1f) / torsoScale);

	leftLowerArmTransform.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	rightLowerArmTransform.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
}

void Scene::setRobotInitialPosition()
{
	torsoTransform.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	headTransform.setPosition(glm::vec3(0.0f, 0.75f, 0.0f));
	leftLegTransform.setPosition(glm::vec3(-0.3f, -0.9f, 0.0f));
	rightLegTransform.setPosition(glm::vec3(0.3f, -0.9f, 0.0f));
	leftUpperArmTransform.setPosition(glm::vec3(-0.7f, 0.3f, 0.0f));
	rightUpperArmTransform.setPosition(glm::vec3(0.7f, 0.3f, 0.0f));
	leftLowerArmTransform.setPosition(glm::vec3(0.0f, -1.15f, 0.0f));
	rightLowerArmTransform.setPosition(glm::vec3(0.0f, -1.15f, 0.0f));

	robotTransform.setRotation(angleAxis(glm::radians(-50.0f), glm::vec3(0, 1, 0)));
}

void Scene::setupBuffers()
{
	// Setup VBO
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVert), cubeVert, GL_STATIC_DRAW);

	// Setup VAO
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Setup IBO
	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeInd), cubeInd, GL_STATIC_DRAW);

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


bool Scene::init()
{
	try
	{
		//Load shader
		m_assets.addShaderProgram("shader", AssetManager::createShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
		m_shader = m_assets.getShaderProgram("shader");
        m_shader->use();

		setupBuffers();

		// Enable backface culling
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);

		// Enable depth testing
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_GREATER);
		glClearDepth(0.0);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// Initialize robot
		setRobotInitialPosition();
		setRobotInitialScale();
		initialLeftLegTransform = leftLegTransform;
		initialRightLegTransform = rightLegTransform;
		initialLeftUpperArmTransform = leftUpperArmTransform;
		initialRightUpperArmTransform = rightUpperArmTransform;

        std::cout << "Scene initialization done\n";
        return true;
	}
	catch (std::exception& ex)
	{
	    throw std::logic_error("Scene initialization failed:\n" + std::string(ex.what()) + "\n");
	}
}

void Scene::render(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shader->use();
	m_shader->setUniform("time", animationTime);

	// // Rotate cube
	// glm::quat rotation = angleAxis(glm::radians(90.0f) * dt, glm::vec3(0, 1, 0)) * angleAxis(glm::radians(90.0f) * dt, glm::vec3(1, 0, 0));
	// cubeTransform.rotate(rotation);
	// m_shader->setUniform("model", cubeTransform.getMatrix(), false);

	// Activate VAO
	glBindVertexArray(vaoID);

	// glDrawElements(GL_TRIANGLES, sizeof(cubeInd) / sizeof(int), GL_UNSIGNED_INT, nullptr);

	// Robot
	glm::mat4 robotMatrix = robotTransform.getMatrix();
	// Torso
	glm::mat4 torsoMatrix = robotMatrix * torsoTransform.getMatrix();
	drawRobotPart(robotMatrix, torsoTransform);
	// Head
	drawRobotPart(torsoMatrix, headTransform);
	// Left arm
	glm::mat4 leftUpperArmMatrix = torsoMatrix * leftUpperArmTransform.getMatrix();
	drawRobotPart(torsoMatrix, leftUpperArmTransform);
	// Right arm
	glm::mat4 rightUpperArmMatrix = torsoMatrix * rightUpperArmTransform.getMatrix();
	drawRobotPart(torsoMatrix, rightUpperArmTransform);
	// Lower arms
	drawRobotPart(leftUpperArmMatrix, leftLowerArmTransform);
	drawRobotPart(rightUpperArmMatrix, rightLowerArmTransform);
	// Legs
	drawRobotPart(torsoMatrix, leftLegTransform);
	drawRobotPart(torsoMatrix, rightLegTransform);

	// Unbind VAO
	glBindVertexArray(0);
}

void Scene::update(float dt)
{
	animationTime += dt;

	robotTransform.rotate(angleAxis(glm::radians(90.0f) * dt, glm::vec3(0.0f, 1.0f, 0.0f)));

	const float angle = glm::radians(30.0f) * std::sin(animationTime * 4.0f);

	leftLegTransform = initialLeftLegTransform;
	leftLegTransform.rotateAroundPoint(glm::vec3(-0.3f, -0.45f, 0.0f), glm::vec3(angle, 0.0f, 0.0f));

	rightLegTransform = initialRightLegTransform;
	rightLegTransform.rotateAroundPoint(glm::vec3(0.3f, -0.45f, 0.0f), glm::vec3(-angle, 0.0f, 0.0f));

	leftUpperArmTransform = initialLeftUpperArmTransform;
	leftUpperArmTransform.rotateAroundPoint(glm::vec3(-0.3f, 0.3f, 0.0f), glm::vec3(-angle, 0.0f, 0.0f));

	rightUpperArmTransform = initialRightUpperArmTransform;
	rightUpperArmTransform.rotateAroundPoint(glm::vec3(0.3f, 0.3f, 0.0f),glm::vec3(angle, 0.0f, 0.0f));
}

OpenGLWindow * Scene::getWindow()
{
	return m_window;
}

void Scene::onKey(Key key, Action action, Modifier modifier)
{

}

void Scene::onMouseMove(MousePosition mouseposition)
{

}

void Scene::onMouseButton(MouseButton button, Action action, Modifier modifier)
{

}

void Scene::onMouseScroll(double xscroll, double yscroll)
{

}

void Scene::onFrameBufferResize(int width, int height)
{

}
void Scene::shutdown()
{

}
