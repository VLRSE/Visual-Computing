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
	rumpf.rotate(glm::vec3(0.0f * dt, glm::radians(80.0f) * dt  , 0.0f));

	//a. VAO Binden.
	glBindVertexArray( vaoID);

	m_time += dt;

	//einfache hin-und-zurück Bewegung mit sin wave
	float bewegungGeschwindigkeit= 2.0f;
	float bewegungAmplitude = 0.3f;

	/**
	 *Rotation Beine - "schwenken" Gehanimation
	 **/
	float beinAngle = sin(m_time * bewegungGeschwindigkeit) * glm::radians(30.0f) * dt;
	glm::vec3 hüftePoint = glm::vec3(0.0f, -0.8f, 0.0f);
	linkesBein.rotateAroundPoint(hüftePoint,  glm::vec3(beinAngle , 0.0f, 0.0f));
	rechtesBein.rotateAroundPoint(hüftePoint,  glm::vec3(-beinAngle , 0.0f, 0.0f));

	/**
	 *Rotation obere Arme - "schwenken"
	 **/
	float obereArmAngle = sin(m_time * bewegungGeschwindigkeit) * glm::radians(20.0f) * dt;
	glm::vec3 obereArmePoint = glm::vec3(0.0f, 0.7f, 0.0f);
	linkeObererArm.rotateAroundPoint(obereArmePoint,  glm::vec3(obereArmAngle , 0.0f, 0.0f));
	rechteObererArm.rotateAroundPoint(hüftePoint,  glm::vec3(-obereArmAngle , 0.0f, 0.0f));

	/**
	 *Rotation obere Arme - "schwenken"
	 **/
	float untereArmAngle = sin(m_time * bewegungGeschwindigkeit) * glm::radians(50.0f) * dt;
	glm::vec3 untereArmePoint = glm::vec3(0.0f, 0.7f, 0.0f);
	linkeUntererArm.rotateAroundPoint(untereArmePoint,  glm::vec3(untereArmAngle , 0.0f, 0.0f));
	rechteUntererArm.rotateAroundPoint(untereArmePoint,  glm::vec3(-untereArmAngle , 0.0f, 0.0f));


	/***
	 **Körperteile rendern
	 ***/

	//transformBodyPart(robotGruppe, glm::mat4(1.0f)); // Parent
	transformBodyPart(rumpf, glm::mat4(1.0f)); // Child:Rumpf
	transformBodyPart(kopf, rumpf.getMatrix()); //Child:Kopf

	//linke Arme rendern
	glm::mat4 linkerArmGruppeMatrix = rumpf.getMatrix() * linkeArmGruppe.getMatrix() * armGruppe.getMatrix()  ;
	transformBodyPart(linkeObererArm , linkerArmGruppeMatrix);
	transformBodyPart(linkeUntererArm ,  linkerArmGruppeMatrix);

	//rechte Arme rendern
	glm::mat4 rechterArmArmMatrix = rumpf.getMatrix() * rechteArmGruppe.getMatrix() * armGruppe.getMatrix() ;
	transformBodyPart(rechteObererArm , rechterArmArmMatrix);
	transformBodyPart(rechteUntererArm , rechterArmArmMatrix);

	//Beine rendern
	glm::mat4 rechtesBeinArmMatrix = rumpf.getMatrix() * beinGruppe.getMatrix() ;
	transformBodyPart(linkesBein , rechtesBeinArmMatrix);
	transformBodyPart(rechtesBein , rechtesBeinArmMatrix);





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
	/***
	 *Robot-Gruppe (Parent Transformation - continous rotation)
	 *Translation zur Ursprung
	*/
	auto rumpfPosition= glm::vec3(0.0f, 0.0f, 0.0f);
	robotGruppe = Transform();
	robotGruppe.translate(rumpfPosition); // Ursprung

	/***
	 *Rumpf Transformation
	 *Skalierung vom Rumpf
	*/
	rumpf = Transform();
	rumpf.scale(glm::vec3(0.5f, 0.8f, 0.2f));
	rumpf.rotate(glm::vec3(0.1f , glm::radians(60.0f)   , 0.0f));

	/***
	 *Kopf Transformation
	 *Skalierung und Translation um Y-Achse
	*/
	kopf = Transform();
	kopf.translate( glm::vec3(0.0f, 0.7f, 0.0f));
	kopf.scale(glm::vec3(0.4f, 0.2f, 1.0f));

	/***
	 *Parent Transformation für die Armen
	 *Skalierung und Translation nach oben für alle Teile der beiden Arme
	*/
	armGruppe = Transform();
	armGruppe.translate(glm::vec3(0.0f, 0.2f, 0.0f));
	armGruppe.scale(glm::vec3(0.2f, 0.3f, 0.6f));

	/***
	 *Parent Transformation für die linken Armen
	 *Translation für alle Teile der linken Arme nach links
	*/

	linkeArmGruppe = Transform(); //Einheitsmatrix erzeugen
	linkeArmGruppe.translate(glm::vec3(-0.7f, 0.0f, 0.0f));

	/***
	 * Transformation für den linkeObererArm Arm
	 *Skalierung für alle Teile der beiden Arme
	*/
	linkeObererArm = Transform();
	linkeObererArm.translate(glm::vec3(0.0f, 0.5f, 0.0f));

	linkeUntererArm = Transform();
	linkeUntererArm.translate(glm::vec3(0.0f, -0.6f, 0.0f));

	/***
	 *Parent Transformation für die rechten Armen
	 *Translation für alle Teile der rechten Arme nach rechts
	*/
	rechteArmGruppe = Transform();
	rechteArmGruppe.translate(glm::vec3(0.7f, 0.0f, 0.0f)); //Translation linker Arm nach rechts und oben

	rechteObererArm = Transform();
	rechteObererArm.translate(glm::vec3(0.0f, 0.5f, 0.0f));

	rechteUntererArm = Transform();
	rechteUntererArm.translate(glm::vec3(0.0f, -0.6f, 0.0f));

	/******
	 *Parent Transformationen für die Beine
	*/

	beinGruppe = Transform();
	beinGruppe.translate(glm::vec3(0.0f, -0.8f, 0.0f));
	beinGruppe.scale(glm::vec3(0.2f, 0.5f, 0.6f));

	linkesBein = Transform();
	linkesBein.translate(glm::vec3(-0.8f, 0.0f, 0.0f));

	rechtesBein = Transform();
	rechtesBein.translate(glm::vec3(0.8f, 0.0f, 0.0f));

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
	glDrawElements(GL_TRIANGLES, sizeof(cubeInd)/sizeof(float), GL_UNSIGNED_INT, 0);

}

void Scene::shutdown()
{

}
