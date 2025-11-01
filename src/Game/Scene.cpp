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


		/*
		 * ************
		 * 1.1 Szene Initialisierung
		 * ************
		 */

		//VBO Erzeugen



		transform();

		//a.)VBO erzeugen, activate  and upload data
		//ID erzeugen
		glGenBuffers(1, &vboID);
		//Aktivsetzen des entsprechenden Memory Buffers
		//GL_ARRAY_BUFFER - mit den eigentlichen Geometrie-Daten
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		//Hochladen der Daten auf die GPU
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVert), &cubeVert, GL_STATIC_DRAW);

		//b.) VAO erzeugen and binden/aktivieren
		//Für jedes zu rendernde Obkjekt wird ein VAO erzeugt
		glGenVertexArrays( 1, &vaoID);
		glBindVertexArray( vaoID);

		//c.)Vertex-Attribute definieren und binden
		//describe VBO in the VAO
		//SIZE: ist Anzahl der "Koordinaten" pro Vertex
		//STRIDE: Abstand zwischen Vertices

		//Configure the Vertex Attribute so that OpenGL know how to read the VBO
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//d.)Indexbuffers erstellen und binden.

		glGenBuffers(1, &iboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeInd), cubeInd, GL_STATIC_DRAW);


		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_GREATER);
		glClearDepth(0.0);

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
    *	1.2 Scene-render()
    *	ALle Objekte, die dargestellt werden sollen,
    *	müssen hier in die Rendering-Pipeline integriert werden.
    * ************
    */

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Reset Buffers before drawing
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //background color black

	//continuous rotation
	//Rotation um 45 Grad Y-Achse und 30 Graf auf X-Achse
	rumpf.rotate(glm::vec3(0.2f * dt, glm::radians(60.0f) * dt  , 0.0f));

	//a. VAO Binden.
	glBindVertexArray( vaoID);

	/****Körperteile rendern**/
	//transformBodyPart(robotGruppe, glm::mat4(1.0f)); // Parent
	transformBodyPart(rumpf, glm::mat4(1.0f)); // Child:Rumpf
	transformBodyPart(kopf, rumpf.getMatrix()); //Child:Kopf


	//c. Optionales Lösen der Bindung, um versehentliche Änderungen am VAO zu vermeiden
	glBindVertexArray(0);

}


void Scene::update(float dt)
{

}

OpenGLWindow * Scene::getWindow()
{
	return m_window;
}

void Scene::transform()
{
	//Robot-Gruppe (Parent Transformation - continous rotation)
	auto torsoPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	robotGruppe = Transform();
	robotGruppe.translate(torsoPosition); // Ursprung

	//Torso erstellen
	rumpf = Transform();
	rumpf.scale(glm::vec3(0.5f, 0.8f, 0.2f));

	//Kopf erstelle
	kopf = Transform();
	kopf.translate( glm::vec3(0.0f, 0.7f, 0.0f));
	kopf.scale(glm::vec3(0.4f, 0.2f, 1.0f));

	linkeArmGruppe = Transform();
	/*
	linkeObererArm
		linkeUntererArm
		rechteArmGruppe
		rechteObererArm
		rechteUntererArm
		linkesBein, rechtesBein

*/



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
