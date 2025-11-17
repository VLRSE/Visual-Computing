#include "Scene.h"
#include <AssetManager.h>
#include "CubeWithNormals.h"

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
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertWithNormals), &cubeVertWithNormals, GL_STATIC_DRAW);

		//b.) VAO erzeugen and binden/aktivieren
		//Für jedes zu rendernde Obkjekt wird ein VAO erzeugt
		glGenVertexArrays( 1, &vaoID);
		glBindVertexArray( vaoID);

		//c.)Vertex-Attribute definieren und binden
		//describe VBO in the VAO
		//SIZE: ist Anzahl der "Koordinaten" pro Vertex
		//STRIDE: Abstand zwischen Vertices

		//Configure the Vertex Attribute so that OpenGL know how to read the VBO
		//Vertex Positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//Colors
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//d.)Indexbuffers erstellen und binden.
/*
		glGenBuffers(1, &iboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeVertWithNormals), cubeVertWithNormals, GL_STATIC_DRAW);

*/
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClearDepth(1.0);

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


	//Aufgabe 3.1.1 view Matrix
	viewMatrix = glm::lookAt(kameraPosition, kameraRichtung, kameraUp);
	m_shader->setUniform("view", viewMatrix, false);

	//Aufgabe 3.1.2 Die ProjectionMatrix
	auto windowWidth = static_cast<float>(getWindow()->getWindowWidth());
	auto windowHeight = static_cast<float>(getWindow()->getWindowHeight());
	float aspectRatio = windowWidth / windowHeight;

	//45 Grad normaler Kamera und kleinerer Wert = Zoom
	projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	m_shader->setUniform("projection", projectionMatrix, false);


	//a. VAO Binden.
	glBindVertexArray( vaoID);

	//continuous rotation
	//Rotation um 45 Grad Y-Achse und 30 Graf auf X-Achse
	rumpf.rotate(glm::vec3(glm::radians(0.0f) * dt, glm::radians(23.0f) * dt, glm::radians(0.0f) * dt));

	/***
	 **Körperteile rendern
	 ***/

	//transformBodyPart(robotGruppe, glm::mat4(1.0f)); // Parent
	transformBodyPart(rumpf, glm::mat4(1.0f)); // Child:Rumpf
	transformBodyPart(kopf, rumpf.getMatrix()); //Child:Kopf

	//linke Arme rendern
	transformBodyPart(linkeObererArm ,rumpf.getMatrix());
	transformBodyPart(linkeUntererArm ,  rumpf.getMatrix());

	//rechte Arme rendern
	transformBodyPart(rechteObererArm , rumpf.getMatrix());
	transformBodyPart(rechteUntererArm , rumpf.getMatrix());

	//Beine rendern
	transformBodyPart(linkesBein , rumpf.getMatrix());
	transformBodyPart(rechtesBein , rumpf.getMatrix());


	//Bewegung
	auto swingAngle = float(glm::radians(30.0f) * glm::sin(5.0f * glfwGetTime()*0.75));
	linkeUntererArm.setRotation(glm::vec3(swingAngle/2,0.0 ,0.0 ));
	linkeObererArm.setRotation(glm::vec3(-swingAngle/6,0.0 ,0.0 ));
	rechteUntererArm.setRotation(glm::vec3(-swingAngle/2,0.0 ,0.0 ));
	rechteObererArm.setRotation(glm::vec3(swingAngle/6,0.0 ,0.0 ));
	linkesBein.setRotation(glm::vec3(-swingAngle/2,0.0 ,0.0 ));
	rechtesBein.setRotation(glm::vec3(swingAngle,0.0 ,0.0 ));

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

	auto rumpfPosition = glm::vec3(0.0, 0.0, 0.0);
    rumpf = Transform();
    rumpf.translate(rumpfPosition);

    kopf = Transform();
    kopf.translate(rumpfPosition + glm::vec3(0.0, 0.65, 0.0));

    linkeObererArm = Transform();
    linkeObererArm.translate(rumpfPosition + glm::vec3(-0.65, 0.3, 0.0));

    linkeUntererArm = Transform();
    linkeUntererArm.translate(rumpfPosition + glm::vec3(0.65, -0.15, 0.0));

    rechteObererArm = Transform();
    rechteObererArm.translate(rumpfPosition + glm::vec3(0.65, 0.3, 0.0));

    rechteUntererArm = Transform();
    rechteUntererArm.translate(rumpfPosition + glm::vec3(-0.65, -0.15, 0.0));

    linkesBein = Transform();
    linkesBein.translate(rumpfPosition + glm::vec3(-0.15, -0.7, 0.0));

    rechtesBein = Transform();
    rechtesBein.translate(rumpfPosition + glm::vec3(0.15, -0.7, 0.0));

    //skalierung
    rumpf.scale(glm::vec3(0.3, 0.7, 0.3));
    kopf.scale(glm::vec3(0.35, 0.2, 0.3));
    linkeObererArm.scale(glm::vec3(0.2, 0.4, 0.25));
    linkeUntererArm.scale(glm::vec3(0.2, 0.4, 0.25));
    rechteObererArm.scale(glm::vec3(0.2, 0.4, 0.25));
    rechteUntererArm.scale(glm::vec3(0.2, 0.4, 0.25));
    linkesBein.scale(glm::vec3(0.25, 0.45, 0.25));
    rechtesBein.scale(glm::vec3(0.25, 0.45, 0.25));


    //rotation
    linkesBein.rotateAroundPoint(rumpfPosition + glm::vec3(0.0, -0.7, 0.0), glm::vec3(0.0, 0.0, -3.0f));
    rechtesBein.rotateAroundPoint(rumpfPosition + glm::vec3(0.0, -0.7, 0.0), glm::vec3(0.0, 0.0, 3.0f));
    linkeObererArm.rotateAroundPoint(linkeObererArm.getPosition(), glm::vec3(glm::radians(0.0f), 0.0, glm::radians(0.0f)));
    rechteObererArm.rotateAroundPoint(rechteObererArm.getPosition(), glm::vec3(glm::radians(0.0f), 0.0, glm::radians(0.0f)));
    rechteUntererArm.rotateAroundPoint(rechteObererArm.getPosition(), glm::vec3(glm::radians(0.0f), 0.0, glm::radians(0.0f)));
    linkeUntererArm.rotateAroundPoint(linkeObererArm.getPosition(), glm::vec3(glm::radians(0.0f), 0.0, glm::radians(0.0f)));
    kopf.rotateAroundPoint(kopf.getPosition(), glm::vec3(glm::radians(0.0f), 0.0, glm::radians(0.0f)));
    rumpf.rotate(glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

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

void Scene::transformBodyPart(Transform &transform, glm::mat4 baseMatrix) {

	//parent und lokal transform kombinieren
	glm::mat4 modelMatrix = baseMatrix * transform.getMatrix();

	//Schick model matrix zum aktiven shader
	m_shader->setUniform("model", modelMatrix,false);
	glDrawArrays(GL_TRIANGLES, 0, 36);


}

void Scene::shutdown()
{

}
