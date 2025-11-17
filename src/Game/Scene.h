#pragma once

#include "OpenGLWindow.h"
#include <ShaderProgram.h>
#include <memory>
#include <AssetManager.h>
#include "Transform.h"
#include "CubeWithNormals.h"

class Scene
{
public:
	Scene(OpenGLWindow* window);
	~Scene();

	bool init();
	void shutdown();
	void render(float dt);
	void update(float dt);
	OpenGLWindow* getWindow();
	void transform();

	void onKey(Key key, Action action, Modifier modifier);
	void onMouseMove(MousePosition mouseposition);
	void onMouseButton(MouseButton button, Action action, Modifier modifier);
	void onMouseScroll(double xscroll, double yscroll);
	void onFrameBufferResize(int width, int height);
	
	void transformBodyPart(Transform& transform, glm::mat4 baseMatrix);

private:
	OpenGLWindow* m_window;
	AssetManager m_assets;
    ShaderProgram* m_shader;
    GLuint vaoID, vboID, iboID;

	glm::mat4 viewMatrix;
	glm::vec3 kameraPosition = glm::vec3(0.0f, 0.0, 0.0); // Kamera im Ursprung

	/*in Normalized Device Space (NDC), Die Kamera schaut entlang der negativen Z-Achse
	 *
	 */
	glm::vec3 kameraRichtung = glm::vec3(0.0f, 0.0f, -1.0f); //Blickrichtung -Z

	//Up Vektor
	glm::vec3 kameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 projectionMatrix;

	float m_time = 0.0f;

	Transform robotGruppe,rumpf, kopf,
		armGruppe,
		linkeArmGruppe, linkeObererArm, linkeUntererArm,
		rechteArmGruppe, rechteObererArm,  rechteUntererArm,
		beinGruppe, linkesBein, rechtesBein;

};

