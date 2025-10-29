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

			//3D + Farbe (xyzrgb)
        float vertices[] = {
        	//----------V----------

        	-0.75, 0.8, 0.1, 0.1, 0.1, 0.0, //0
            -1.0, 0.8, 0.2, 0.1, 0.5, 0.0, //1
            -0.8, 0.0,0.1, 1.0, 0.6, 0.0, //2
            -0.5, -0.5,0.1, 1.0, 0.0, 0.0, //3
            -0.6, -0.8,0.1, 0.0, 1.0, 0.0, //4
	        -0.4, -0.8,0.1, 0.0, 1.0, 0.0, //5
        	-0.2, 0.0, 0.1,0.0, 1.0, 0.0, //6
        	0.0, 0.8,0.1, 0.0, 1.0, 0.0, //7
        	-0.25, 0.8,0.1, 0.0, 1.0, 0.0, //8


        	//----------D----------
        		0.7, 0.8, 0.2, 0.2, 0.2, 1.0, //  right top //9
				0.1, 0.8, 0.5, 0.0, 0.0, 0.5, // left top //10
				0.4, 0.6, 0.1, 0.3f, 0., 0.8, // right top //11
				0.1, -0.8, 0.5, 0.0, 0.0, 0.5, // left bottom //12
				0.4, -0.6, 0.1, 0.2, 0.0, 0.5, // mid center lower, //13
				0.8, -0.8, 0.1, 0.1, 0.0, 0.5, // right bottom  //14
				0.9, -0.5, 0.1,  0.1, 0.0, 0.0, //15
				0.6,  -0.4, 0.5,  0.8, 0.0, 1.0, //16
				0.9, 0.5, 0.1,  0.5, 0.0, 0.5,  //17
        		0.6, 0.4, 0.1,  0.5, 0.0, 1.0,  //18

        	//----------D Shadow----------
        /*	-0.1, 0.4, 0.1,  0.0, 0.0, 0.3,  //19
        	-0.1, -1.0, 0.1, 0.0, 0.0, 0.3, //20
        	0.6, -1.0, 0.1, 0.0, 0.0, 0.3, //21

        	*/





        };

        int indices[] = {
        	//----------V----------
        	0, 1, 2,
			2, 3, 0,
			3, 2, 4,
			4, 5 , 3,
			3, 5, 6,
			6, 7, 8,
			8, 3, 6,




        	// D
        	9, 10, 11,
        	11, 10, 12,
        	11, 12, 13,
        	12, 14, 13,
        	16, 13, 14,
        	14, 15, 16,
        	15, 17, 16,
        	17, 18, 16,
			9, 11, 18,
        	9, 18, 17,

        	//D Shadow
        	/*10, 19, 12,
        	19, 20, 12,
        	12, 20, 14,
        	14, 20,21,*/

        };


		/*
		 * ************
		 * 1.1 Szene Initialisierung
		 * ************
		 */

		//VBO Erzeugen
		GLuint vaoID, vboID;

		//a.)VBO erzeugen, activate  and upload data
		//ID erzeugen
		glGenBuffers(1, &vboID);
		//Aktivsetzen des entsprechenden Memory Buffers
		//GL_ARRAY_BUFFER - mit den eigentlichen Geometrie-Daten
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		//Hochladen der Daten auf die GPU
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

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
		GLuint iboID;
		glGenBuffers(1, &iboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


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
    *	1.2 Scene-render()
    *	ALle Objekte, die dargestellt werden sollen,
    *	müssen hier in die Rendering-Pipeline integriert werden.
    * ************
    */

	//a. VAO Binden.
	glBindVertexArray( vaoID);

	//b.Elemente Zeichen (render call)
	//COUNT: jedes Dreieck hat 3 Indizes
	glDrawElements(GL_TRIANGLES, 51 , GL_UNSIGNED_INT, 0);

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
