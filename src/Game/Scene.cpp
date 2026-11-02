#include "Scene.h"
#include <AssetManager.h>

Scene::Scene(OpenGLWindow * window) :
	m_window(window)
{
	assert(window != nullptr);
}

Scene::~Scene()
{}

bool Scene::init()
{
	try
	{
		//Load shader
		m_assets.addShaderProgram("shader", AssetManager::createShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
		m_shader = m_assets.getShaderProgram("shader");
        m_shader->use();

        float vertices[] = {
        	// C 0-9
        	0.25, -0.25, 1.0, 0.0, 0.0,
        	0.25, 0.25, 1.0, 0.0, 0.0,

        	0.3, -0.25, 1.0, 0.0, 0.0,
        	0.3, 0.25, 1.0, 0.0, 0.0,

        	0.75, -0.25, 1.0, 0.0, 0.0,
        	0.75, 0.25, 1.0, 0.0, 0.0,

        	0.75, -0.2, 1.0, 0.0, 0.0,
        	0.75, 0.2, 1.0, 0.0, 0.0,

        	0.3, -0.2, 1.0, 0.0, 0.0,
			0.3, 0.2, 1.0, 0.0, 0.0,

        	// R 10-13
			-0.75, 0.25, 1.0, 0.0, 0.0,
			-0.75, -0.25, 1.0, 0.0, 0.0,
			-0.7, 0.25, 1.0, 0.0, 0.0,
			-0.7, -0.25, 1.0, 0.0, 0.0,

        	// 14-16
        	-0.7, 0.2, 1.0, 0.0, 0.0,
        	-0.45, 0.2, 1.0, 0.0, 0.0,
        	-0.45, 0.25, 1.0, 0.0, 0.0,

        	// 17-20
        	-0.5, 0.05, 1.0, 0.0, 0.0,
        	-0.45, 0, 1.0, 0.0, 0.0,
        	-0.5, 0, 1.0, 0.0, 0.0,
        	-0.5, 0.25, 1.0, 0.0, 0.0,

			// 21-24
        	-0.7, 0, 1.0, 0.0, 0.0,
        	-0.7, 0.05, 1.0, 0.0, 0.0,
        	-0.5, -0.25, 1.0, 0.0, 0.0,
        	-0.45, -0.25, 1.0, 0.0, 0.0,
        };

        int indices[] = {
        	// C
        	0, 2, 1,
        	3, 1, 2,

        	6, 2, 4,
        	8, 2, 6,

        	7, 5, 9,
        	3, 9, 5,

        	// R
        	13, 10, 11,
        	12, 10, 13,

        	12, 15, 16,
        	14, 15, 12,

        	18, 20, 19,
        	16, 20, 18,

        	21, 19, 17,
        	17, 22, 21,

        	22, 23, 24,
        	21, 23, 22,
        };

		indicesSize = sizeof(indices) / sizeof(int);

		/*
		 * ************
		 * Place your code here!
		 * ************
		 */

		// Setup VBO
		glGenBuffers(1, &vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Setup VAO
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);

		glVertexAttribPointer(0, 2, GL_FLOAT, false, 5 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Setup IBO
		glGenBuffers(1, &iboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Unbind
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Enable backface culling
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);

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

    /*
    * ************
    * Place your code here!
    * ************
    */

	// Use shader
	m_shader->use();

	// Activate VAO
	glBindVertexArray(vaoID);

	// Render call
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, nullptr);

	// Unbind VAO
	glBindVertexArray(0);

}

void Scene::update(float dt)
{

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
