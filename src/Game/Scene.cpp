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

		//2D + Farbe (xyrgb)
        float vertices[] = {-0.5, -0.5, 0.0, 0.0, 1.0,
                            0.5, -0.5, 0.0, 0.0, 1.0,
                            0.5, 0.5, 0.0, 1.0, 0.0,
                            0.0, 1.0, 1.0, 0.0, 0.0,
                            -0.5, 0.5, 0.0, 1.0, 0.0};

        int indices[] = {0, 1, 2,
                         0, 2, 4,
                         4, 2, 3};

		/*
		 * ************
		 * 1.1 Szene Initialisierung
		 * ************
		 */

		//VBO Erzeugen
		GLuint vaoID, vboID;

		//a.)VBO erzeugen, activate  and upload data
		glGenVertexArrays(1, &vaoID);
		glBindBuffer(GL_ARRAY_BUFFER, vaoID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW)

		//b.) VAO erzeugen and binden
		glGenVertexArrays( 1, &vaoID);
		glBindVertexArray( vaoID);


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
