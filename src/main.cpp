/*
 * Lab 5 base code (could also be used for Program 2)
 * includes modifications to shape and initGeom in preparation to load
 * multi shape objects 
 * CPE 471 Cal Poly Z. Wood + S. Sueda + I. Dunn
 */

#define _USE_MATH_DEFINES

#include <iostream>
#include <unordered_map>
#include <cmath>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "WindowManager.h"
#include "Mesh.h"
#include "Camera.h"
#include "Obstacle.h"
#include "Texture.h"
#include "Interpolater.h"
#include "GameWorld.h"
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Application : public EventCallbacks {

public:

	WindowManager * windowManager = nullptr;

	// Our shader programs
	std::shared_ptr<Program> matShader;
    std::shared_ptr<Program> texShader;
    std::shared_ptr<Program> skyShader;

    //texture data
    shared_ptr<Texture> texture0;
    shared_ptr<Texture> texture1;
    shared_ptr<Texture> texture2;
    shared_ptr<Texture> texture3;
    shared_ptr<Texture> texture4;
    shared_ptr<Texture> texture5;
    shared_ptr<Texture> texture6;
    shared_ptr<Texture> texture7;
    shared_ptr<Texture> texture8;
    shared_ptr<Texture> texture9;
    shared_ptr<Texture> texture10;
    shared_ptr<Texture> texture11;
    shared_ptr<Texture> texture12;
    unsigned int cubeMapTexture;


    unordered_map<string, Mesh*> geometry;

    GameWorld Game = GameWorld();

    //skybox
    vector<std::string> skyfaces{
        "night_right.tga",
        "night_left.tga",
        "night_top.tga",
        "night_bottom.tga",
        "night_front.tga",
        "night_back.tga"
    };

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our triangle to OpenGL
	GLuint VertexBufferID;

	//example data that might be useful when trying to compute bounds on multi-shape
	vec3 gMin;
    vec3 gMax;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
        if (key == GLFW_KEY_C && action == GLFW_PRESS) {
            Game.player->getCam()->enableCursor(windowManager);
        }
        if (key == GLFW_KEY_C && action == GLFW_RELEASE) {
            glfwSetInputMode(windowManager->getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            Game.player->moveF = true;
        }
        if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
            Game.player->moveF = false;
        }
        if (key == GLFW_KEY_S && action == GLFW_PRESS) {
            Game.player->moveB = true;
        }
        if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
            Game.player->moveB = false;
        }
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {
            Game.player->moveL = true;
        }
        if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
            Game.player->moveL = false;
        }
        if (key == GLFW_KEY_D && action == GLFW_PRESS) {
            Game.player->moveR = true;
        }
        if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
            Game.player->moveR = false;
        }
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            Game.player->jump = true;
        }
        if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
            Game.player->jump = false;
        }
        if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
            Game.player->setSpeed(0.05);
        }
        if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
            Game.player->setSpeed(0.03);
        }
		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		}
		if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}
	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;

		if (action == GLFW_PRESS)
		{
			 glfwGetCursorPos(window, &posX, &posY);
			 cout << "Pos X " << posX <<  " Pos Y " << posY << endl;
		}
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

    void scrollCallback(GLFWwindow* window, double deltaX, double deltaY) {}

	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(.12f, .34f, .56f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		// Initialize the GLSL program.
		matShader = make_shared<Program>();
		matShader->setVerbose(true);
		matShader->setShaderNames(resourceDirectory + "/simple_vert.glsl", resourceDirectory + "/simple_frag.glsl");
		matShader->init();
        matShader->addUniform("P");
        matShader->addUniform("V");
        matShader->addUniform("M");
        matShader->addUniform("MatAmb");
        matShader->addUniform("MatDif");
        matShader->addUniform("MatSpec");
        matShader->addUniform("shine");
        matShader->addUniform("eye");
        matShader->addUniform("lightPositions");
        matShader->addUniform("lightColInt");
        matShader->addUniform("lightABC");
        matShader->addUniform("numLights");
        matShader->addAttribute("vertPos");
        matShader->addAttribute("vertNor");
        matShader->addAttribute("vertTex");

        texShader = make_shared<Program>();
        texShader->setVerbose(true);
        texShader->setShaderNames(resourceDirectory + "/tex_vert.glsl", resourceDirectory + "/tex_frag0.glsl");
        texShader->init();
        texShader->addUniform("P");
        texShader->addUniform("V");
        texShader->addUniform("M");
        texShader->addUniform("Texture0");
        texShader->addUniform("lightPositions");
        texShader->addUniform("lightColInt");
        texShader->addUniform("lightABC");
        texShader->addUniform("numLights");
        texShader->addUniform("flip");
        texShader->addAttribute("vertPos");
        texShader->addAttribute("vertNor");
        texShader->addAttribute("vertTex");

        skyShader = make_shared<Program>();
        skyShader->setVerbose(true);
        skyShader->setShaderNames(resourceDirectory + "/cube_vert.glsl", resourceDirectory + "/cube_frag.glsl");
        skyShader->init();
        skyShader->addUniform("P");
        skyShader->addUniform("V");
        skyShader->addUniform("M");
        skyShader->addUniform("skybox");
        skyShader->addAttribute("vertPos");
        skyShader->addAttribute("vertNor");

	}

	void initGeom(const std::string& resourceDirectory)
	{

		//EXAMPLE set up to read one shape from one obj file - convert to read several
		// Initialize mesh
		// Load geometry
 		// Some obj files contain material information.We'll ignore them for this assignment.
 		vector<tinyobj::shape_t> TOshapes;
 		vector<tinyobj::material_t> objMaterials;
 		string errStr;
		//load in the mesh and make the shape(s)

        initTex(resourceDirectory);
        cubeMapTexture = createSky(resourceDirectory + "/skybox/", skyfaces);

        bool rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/cube.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["cube_notex"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/cube_tex.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["cube_tex"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/floortile.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["floortile"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/wooden_fence.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["fence"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/mesh_Fountain.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["fountain"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/mesh_WaterPlane.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["fountain_water"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/archway.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["archway"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/wall.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["wall"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/brick.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["brick"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/barrel.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["barrel"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/hedge.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["hedge"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/wagon.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            Mesh* temp = new Mesh(TOshapes);

            for (int i = 0; i < 5; i++)
            {
                Mesh *temp2 = new Mesh(TOshapes);
                temp2->shapes = vector<shared_ptr<Shape>>{ temp->shapes[i] };
                geometry["wagon" + std::to_string(i)] = temp2;
            }
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/crates.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["crates"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/torch.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["torch"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/fire.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["fire"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/player.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["player"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/floor.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["floor"] = new Mesh(TOshapes);
        }

        rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/playerhitbox.obj").c_str());

        if (!rc) {
            cerr << errStr << endl;
        }
        else {
            geometry["playerhitbox"] = new Mesh(TOshapes);
        }

	}

    void initWorld()
    {  
        Game.player = new Player(vec3(-0.5, -1.4, 3.5), vec3(0.006, 0.006, 0.006), -M_PI/2, M_PI/2, 0.0, geometry["player"], nullptr, 1, 1.0, geometry["playerhitbox"]);

        //fountain
        Game.addEntity(new Obstacle(vec3(-0.5, -1.5, 0.5), vec3(0.002, 0.002, 0.002), 0.0f, 0.0f, 0.0f, geometry["fountain"], texture1, 0));
        Game.addEntity(new Obstacle(vec3(-0.5, -1.5, 0.5), vec3(0.002, 0.002, 0.002), 0.0f, 0.0f, 0.0f, geometry["fountain_water"], nullptr, 14));

        //hedges around fountain
        {
            Game.addEntity(new Obstacle(vec3(1, -1.5, -0.6), vec3(0.004, 0.004, 0.004), -glm::radians(90.0), glm::radians(90.0), 0.0f, geometry["hedge"], texture6, 0));
            Game.addEntity(new Obstacle(vec3(-1.95, -1.5, -0.6), vec3(0.004, 0.004, 0.004), -glm::radians(90.0), glm::radians(90.0), 0.0f, geometry["hedge"], texture6, 0));
            Game.addEntity(new Obstacle(vec3(-1.95, -1.5, 1.65), vec3(0.004, 0.004, 0.004), -glm::radians(90.0), glm::radians(90.0), 0.0f, geometry["hedge"], texture6, 0));
            Game.addEntity(new Obstacle(vec3(1, -1.5, 1.65), vec3(0.004, 0.004, 0.004), -glm::radians(90.0), glm::radians(90.0), 0.0f, geometry["hedge"], texture6, 0));

            Game.addEntity(new Obstacle(vec3(0.6, -1.5, -1.0), vec3(0.004, 0.004, 0.004), -glm::radians(90.0), 0.0f, 0.0f, geometry["hedge"], texture6, 0));
            Game.addEntity(new Obstacle(vec3(-1.55, -1.5, -1.0), vec3(0.004, 0.004, 0.004), -glm::radians(90.0), 0.0f, 0.0f, geometry["hedge"], texture6, 0));
            Game.addEntity(new Obstacle(vec3(-1.55, -1.5, 2.05), vec3(0.004, 0.004, 0.004), -glm::radians(90.0), 0.0f, 0.0f, geometry["hedge"], texture6, 0));
            Game.addEntity(new Obstacle(vec3(0.6, -1.5, 2.05), vec3(0.004, 0.004, 0.004), -glm::radians(90.0), 0.0f, 0.0f, geometry["hedge"], texture6, 0));
        }

        //archway
        Game.addEntity(new Obstacle(vec3(-0.9+0.33, -1.3, -4.5), vec3(0.0003, 0.0005, 0.0005), 0.0f, 0.0f, 0.0f, geometry["fence"], texture0, 0));
        Game.addEntity(new Obstacle(vec3(-0.9, -2.07, -4.5), vec3(0.2, 0.2, 0.2), 0.0f, 0.0f, 0.0f, geometry["archway"], texture2, 0));

        //clutter
        Game.addEntity(new Obstacle(vec3(3, -1.18, -4), vec3(0.002, 0.002, 0.002), 0.0f, 0.0f, 0.0f, geometry["barrel"], texture5, 0));
        Game.addEntity(new Obstacle(vec3(3.1, -1.33, -3.4), vec3(0.001, 0.001, 0.001), 0.0f, glm::radians(90.0), 0.0f, geometry["barrel"], texture5, 0));
        Game.addEntity(new Obstacle(vec3(3.5, -1.245, -3.6), vec3(0.5, 0.5, 0.5), 0.0f, 0.0f, 0.0f, geometry["cube_tex"], texture0, 0));
        Game.addEntity(new Obstacle(vec3(4.35, -1.09, -3.95), vec3(1.24, 0.8, 0.8), 0.0f, 0.0f, 0.0f, geometry["cube_tex"], texture0, 0));
        Game.addEntity(new Obstacle(vec3(4.45, -1.25, 4.6), vec3(0.5, 0.5, 0.5), 0.0f, glm::radians(180.0), 0.0f, geometry["crates"], texture0, 0));

        //torches
        {
            Game.addEntity(new Obstacle(vec3(-0.5, 0, -4.5), vec3(1, 1, 1), glm::radians(30.0), 0.0f, 0.0f, geometry["torch"], texture11, 11));
            Game.addEntity(new Obstacle(vec3(-0.5, 0.23, -4.38), vec3(1, 1, 1), glm::radians(30.0), 0.0f, 0.0f, geometry["fire"], texture12, 11));
            Game.addLight(vec3(-0.5, 0.43, -4.25), vec3(1.0, 0.57647, 0.16078), vec3(0, 1, 0.1));

            Game.addEntity(new Obstacle(vec3(0.25, -0.75, -4.5), vec3(1, 1, 1), glm::radians(30.0), 0.0f, 0.0f, geometry["torch"], texture11, 11));
            Game.addEntity(new Obstacle(vec3(0.25, -0.52, -4.38), vec3(1, 1, 1), glm::radians(30.0), 0.0f, 0.0f, geometry["fire"], texture12, 11));
            Game.addLight(vec3(0.25, -0.32, -4.25), vec3(1.0, 0.57647, 0.16078), vec3(0, 1, 0.1));

            Game.addEntity(new Obstacle(vec3(-1.25, -0.75, -4.5), vec3(1, 1, 1), glm::radians(30.0), 0.0f, 0.0f, geometry["torch"], texture11, 11));
            Game.addEntity(new Obstacle(vec3(-1.25, -0.52, -4.38), vec3(1, 1, 1), glm::radians(30.0), 0.0f, 0.0f, geometry["fire"], texture12, 11));
            Game.addLight(vec3(-1.25, -0.32, -4.25), vec3(1.0, 0.57647, 0.16078), vec3(0, 1, 0.1));

            Game.addEntity(new Obstacle(vec3(5, -0.75, 3), vec3(1, 1, 1), 0.0f, 0.0f, glm::radians(30.0), geometry["torch"], texture11, 11));
            Game.addEntity(new Obstacle(vec3(4.88, -0.52, 3), vec3(1, 1, 1), 0.0f, 0.0f, glm::radians(30.0), geometry["fire"], texture12, 11));
            Game.addLight(vec3(4.75, -0.32, 3), vec3(1.0, 0.57647, 0.16078), vec3(0, 1, 0.1));

            Game.addEntity(new Obstacle(vec3(5, -0.75, -2.5), vec3(1, 1, 1), 0.0f, 0.0f, glm::radians(30.0), geometry["torch"], texture11, 11));
            Game.addEntity(new Obstacle(vec3(4.88, -0.52, -2.5), vec3(1, 1, 1), 0.0f, 0.0f, glm::radians(30.0), geometry["fire"], texture12, 11));
            Game.addLight(vec3(4.75, -0.32, -2.5), vec3(1.0, 0.57647, 0.16078), vec3(0, 1, 0.1));

            Game.addEntity(new Obstacle(vec3(3.25, -0.75, 5.65), vec3(1, 1, 1), glm::radians(-30.0), 0.0f, 0.0f, geometry["torch"], texture11, 11));
            Game.addEntity(new Obstacle(vec3(3.25, -0.52, 5.53), vec3(1, 1, 1), glm::radians(-30.0), 0.0f, 0.0f, geometry["fire"], texture12, 11));
            Game.addLight(vec3(3.25, -0.32, 5.40), vec3(1.0, 0.57647, 0.16078), vec3(0, 1, 0.1));

            Game.addEntity(new Obstacle(vec3(-4.25, -0.75, 5.65), vec3(1, 1, 1), glm::radians(-30.0), 0.0f, 0.0f, geometry["torch"], texture11, 11));
            Game.addEntity(new Obstacle(vec3(-4.25, -0.52, 5.53), vec3(1, 1, 1), glm::radians(-30.0), 0.0f, 0.0f, geometry["fire"], texture12, 11));
            Game.addLight(vec3(-4.25, -0.32, 5.40), vec3(1.0, 0.57647, 0.16078), vec3(0, 1, 0.1));
        }

        //wagon
        {
            vec3 wagonPos = vec3(3, -1.28, 4.3);
            vec3 wagonScale = vec3(0.3, 0.3, 0.3);
            Game.addEntity(new Obstacle(wagonPos, wagonScale, 0.0f, glm::radians(-30.0), 0.0f, geometry["wagon0"], texture7, 0));
            Game.addEntity(new Obstacle(wagonPos, wagonScale, 0.0f, glm::radians(-30.0), 0.0f, geometry["wagon1"], texture8, 0));
            Game.addEntity(new Obstacle(wagonPos, wagonScale, 0.0f, glm::radians(-30.0), 0.0f, geometry["wagon2"], texture7, 0));
            Game.addEntity(new Obstacle(wagonPos, wagonScale, glm::radians(10.0), glm::radians(-30.0), 0.0f, geometry["wagon3"], texture9, 0));
            Game.addEntity(new Obstacle(wagonPos, wagonScale, glm::radians(10.0), glm::radians(-30.0), 0.0f, geometry["wagon4"], texture10, 0));
        }

        //hedge wall
        {
            Game.addEntity(new Obstacle(vec3(-5.6, -1.5, 0.25), vec3(0.004, 0.012, 0.016), -glm::radians(90.0), glm::radians(90.0), 0.0f, geometry["hedge"], texture6, 0));
            Game.addEntity(new Obstacle(vec3(-5.6, -1.5, 3.75), vec3(0.004, 0.012, 0.016), -glm::radians(90.0), glm::radians(90.0), 0.0f, geometry["hedge"], texture6, 0));
            Game.addEntity(new Obstacle(vec3(-5.6, -1.5, -2.75), vec3(0.004, 0.012, 0.016), -glm::radians(90.0), glm::radians(90.0), 0.0f, geometry["hedge"], texture6, 0));
        }

        // walls
        {
            for (int i = 0; i <= 2; i++)
            {
                Game.addEntity(new Obstacle(vec3(1.5 + (1.5*i), -1.5, -4.6), vec3(1, 1, 1), 0.0f, 0.0f, 0.0f, geometry["wall"], texture4, 11));
                Game.addEntity(new Obstacle(vec3(1.5 + (1.5*i), -0.3, -4.6), vec3(1, 1, 1), 0.0f, 0.0f, 0.0f, geometry["wall"], texture4, 11));
            }
            for (int i = 0; i <= 2; i++)
            {
                Game.addEntity(new Obstacle(vec3(-2.5 + (-1.5*i), -1.5, -4.4), vec3(1, 1, 1), 0.0f, glm::radians(180.0), 0.0f, geometry["wall"], texture4, 11));
                Game.addEntity(new Obstacle(vec3(-2.5 + (-1.5*i), -0.3, -4.4), vec3(1, 1, 1), 0.0f, glm::radians(180.0), 0.0f, geometry["wall"], texture4, 11));
            }
            for (int i = 0; i <= 7; i++)
            {
                Game.addEntity(new Obstacle(vec3(4.5 + (-1.5*i), -1.5, 5.6), vec3(1, 1, 1), 0.0f, 0.0f, 0.0f, geometry["wall"], texture4, 11));
                Game.addEntity(new Obstacle(vec3(4.5 + (-1.5*i), -0.3, 5.6), vec3(1, 1, 1), 0.0f, 0.0f, 0.0f, geometry["wall"], texture4, 11));
            }
            for (int i = 0; i <= 6; i++)
            {
                Game.addEntity(new Obstacle(vec3(5, -1.5, -4 + (1.5*i)), vec3(1, 1, 1), 0.0f, glm::radians(90.0), 0.0f, geometry["wall"], texture4, 11));
                Game.addEntity(new Obstacle(vec3(5, -0.3, -4 + (1.5*i)), vec3(1, 1, 1), 0.0f, glm::radians(90.0), 0.0f, geometry["wall"], texture4, 11));
            }
            for (int i = 0; i <= 6; i++)
            {
                Game.addEntity(new Obstacle(vec3(-6, -1.5, -4 + (1.5*i)), vec3(1, 1, 1), 0.0f, glm::radians(90.0), 0.0f, geometry["wall"], texture4, 11));
                Game.addEntity(new Obstacle(vec3(-6, -0.3, -4 + (1.5*i)), vec3(1, 1, 1), 0.0f, glm::radians(90.0), 0.0f, geometry["wall"], texture4, 11));
            }
            Game.addEntity(new Obstacle(vec3(-1.5, 0.1, -4.4), vec3(1, 1, 1), 0.0f, glm::radians(180.0), 0.0f, geometry["wall"], texture4, 11));
            Game.addEntity(new Obstacle(vec3(0, 0.075, -4.4), vec3(1, 1, 1), 0.0f, glm::radians(180.0), 0.0f, geometry["wall"], texture4, 11));
            Game.addEntity(new Obstacle(vec3(0.65, 0.06, -4.45), vec3(0.015, 0.015, 0.015), 0.0f, 0.0f, 0.0f, geometry["brick"], texture4, 11));

            Game.addEntity(new Obstacle(vec3(0, -0.65, -5), vec3(14, 3, 0.1), 0.0f, 0.0f, 0.0f, geometry["cube_notex"], nullptr, 15));
            Game.addEntity(new Obstacle(vec3(5.3, -0.65, 0), vec3(0.1, 3, 14), 0.0f, 0.0f, 0.0f, geometry["cube_notex"], nullptr, 15));
            Game.addEntity(new Obstacle(vec3(-6, -0.65, 0), vec3(0.1, 3, 14), 0.0f, 0.0f, 0.0f, geometry["cube_notex"], nullptr, 15));
            Game.addEntity(new Obstacle(vec3(0, -0.65, 6), vec3(14, 3, 0.1), 0.0f, 0.0f, 0.0f, geometry["cube_notex"], nullptr, 15));
        }

        // floor
        Game.addEntity(new Obstacle(vec3(0, -1.5, 0), vec3(1, 1, 1), 0.0f, 0.0f, 0.0f, geometry["floor"], texture3, 11));
        
    }

    void initTex(const std::string& resourceDirectory) {
        texture0 = make_shared<Texture>();
        texture0->setFilename(resourceDirectory + "/crate.jpg");
        texture0->init();  texture0->setUnit(0);
        texture0->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        texture1 = make_shared<Texture>();
        texture1->setFilename(resourceDirectory + "/tex_Fountain.png");
        texture1->init();  texture1->setUnit(1);
        texture1->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        texture2 = make_shared<Texture>();
        texture2->setFilename(resourceDirectory + "/archway_stone.jpg");
        texture2->init();  texture2->setUnit(2);
        texture2->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        texture3 = make_shared<Texture>();
        texture3->setFilename(resourceDirectory + "/street.jpg");
        texture3->init();  texture3->setUnit(3);
        texture3->setWrapModes(GL_REPEAT, GL_REPEAT);

        texture4 = make_shared<Texture>();
        texture4->setFilename(resourceDirectory + "/wall.png");
        texture4->init();  texture4->setUnit(4);
        texture4->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        texture5 = make_shared<Texture>();
        texture5->setFilename(resourceDirectory + "/barrel.png");
        texture5->init();  texture5->setUnit(5);
        texture5->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        texture6 = make_shared<Texture>();
        texture6->setFilename(resourceDirectory + "/hedge_tex.jpg");
        texture6->init();  texture6->setUnit(6);
        texture6->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        texture7 = make_shared<Texture>();
        texture7->setFilename(resourceDirectory + "/wagon_metal.jpg");
        texture7->init();  texture7->setUnit(7);
        texture7->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        texture8 = make_shared<Texture>();
        texture8->setFilename(resourceDirectory + "/wagon_wood1.jpg");
        texture8->init();  texture8->setUnit(8);
        texture8->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        texture9 = make_shared<Texture>();
        texture9->setFilename(resourceDirectory + "/wagon_wood2.jpg");
        texture9->init();  texture9->setUnit(9);
        texture9->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        texture10 = make_shared<Texture>();
        texture10->setFilename(resourceDirectory + "/wagon_wood.jpg");
        texture10->init();  texture10->setUnit(10);
        texture10->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        texture11 = make_shared<Texture>();
        texture11->setFilename(resourceDirectory + "/torch.png");
        texture11->init();  texture11->setUnit(11);
        texture11->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        texture12 = make_shared<Texture>();
        texture12->setFilename(resourceDirectory + "/fire_tex.png");
        texture12->init();  texture12->setUnit(11);
        texture12->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    }

    unsigned int createSky(string dir, vector<string> faces) {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(false);
        for (GLuint i = 0; i < faces.size(); i++) {
            unsigned char *data = stbi_load((dir + faces[i]).c_str(), &width, &height, &nrChannels, 0);
            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            else {
                cout << "failed to load: " << (dir + faces[i]).c_str() << endl;
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        cout << " creating cube map any errors : " << glGetError() << endl;

        return textureID;
    }

	void setModel(std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack>M) {
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
   }

    void SetMaterial(int i) {
        switch (i) {
        case 0: //shiny blue plastic
            glUniform3f(matShader->getUniform("MatAmb"), 0.02, 0.04, 0.2);
            glUniform3f(matShader->getUniform("MatDif"), 0.0, 0.16, 0.9);
            glUniform3f(matShader->getUniform("MatSpec"), 0.14, 0.2, 0.8);
            glUniform1f(matShader->getUniform("shine"), 120.0);
            break;
        case 1: // skin tone
            glUniform3f(matShader->getUniform("MatAmb"), 0.2125, 0.1275, 0.1);
            glUniform3f(matShader->getUniform("MatDif"), 0.714, 0.4284, 0.36);
            glUniform3f(matShader->getUniform("MatSpec"), 0.393548, 0.271906, 0.32);
            glUniform1f(matShader->getUniform("shine"), 10.0);
            break;
        case 2: //brass 
            glUniform3f(matShader->getUniform("MatAmb"), 0.3294, 0.2235, 0.02745);
            glUniform3f(matShader->getUniform("MatDif"), 0.7804, 0.5686, 0.11373);
            glUniform3f(matShader->getUniform("MatSpec"), 0.9922, 0.941176, 0.80784);
            glUniform1f(matShader->getUniform("shine"), 27.9);
            break;
        case 3: //red
            glUniform3f(matShader->getUniform("MatAmb"), 0.5, 0, 0);
            glUniform3f(matShader->getUniform("MatDif"), 0.4, 0, 0.1);
            glUniform3f(matShader->getUniform("MatSpec"), 0.1, 0, 0.07);
            glUniform1f(matShader->getUniform("shine"), 4.0);
            break;
        case 4: //green
            glUniform3f(matShader->getUniform("MatAmb"), 0, 0.13, 0);
            glUniform3f(matShader->getUniform("MatDif"), 0.1, 0.8, 0);
            glUniform3f(matShader->getUniform("MatSpec"), 0.07, 0.4, 0);
            glUniform1f(matShader->getUniform("shine"), 4.0);
            break;
        case 5: //shiny yellow plastic
            glUniform3f(matShader->getUniform("MatAmb"), 0.2, 0.2, 0.04);
            glUniform3f(matShader->getUniform("MatDif"), 0.6, 0.6, 0);
            glUniform3f(matShader->getUniform("MatSpec"), 0.9, 0.9, 0.2);
            glUniform1f(matShader->getUniform("shine"), 120.0);
            break;
        case 6: // brown leather
            glUniform3f(matShader->getUniform("MatAmb"), 0.25, 0.148, 0.06475);
            glUniform3f(matShader->getUniform("MatDif"), 0.4, 0.2368, 0.1036);
            glUniform3f(matShader->getUniform("MatSpec"), 0.774597, 0.458561, 0.200621);
            glUniform1f(matShader->getUniform("shine"), 76.8);
            break;
        case 7: // gold
            glUniform3f(matShader->getUniform("MatAmb"), 0.24725*0.55, 0.1995*0.55, 0.0745*0.55);
            glUniform3f(matShader->getUniform("MatDif"), 0.75164*0.55, 0.60648*0.55, 0.22648*0.55);
            glUniform3f(matShader->getUniform("MatSpec"), 0.628281, 0.555802, 0.366065);
            glUniform1f(matShader->getUniform("shine"), 51.2);
            break;
        case 8: // flat gold
            glUniform3f(matShader->getUniform("MatAmb"), 0.24725*0.55, 0.1995*0.55, 0.0745*0.55);
            glUniform3f(matShader->getUniform("MatDif"), 0.75164*0.55, 0.60648*0.55, 0.22648*0.55);
            glUniform3f(matShader->getUniform("MatSpec"), 0.628281, 0.555802, 0.366065);
            glUniform1f(matShader->getUniform("shine"), 4.0);
            break;
        case 9: // black rubber
            glUniform3f(matShader->getUniform("MatAmb"), 0.02, 0.02, 0.02);
            glUniform3f(matShader->getUniform("MatDif"), 0.01, 0.01, 0.01);
            glUniform3f(matShader->getUniform("MatSpec"), 0.4, 0.4, 0.4);
            glUniform1f(matShader->getUniform("shine"), 10.0);
            break;
        case 10: // white sock
            glUniform3f(matShader->getUniform("MatAmb"), 0.05, 0.05, 0.05);
            glUniform3f(matShader->getUniform("MatDif"), 0.5, 0.5, 0.5);
            glUniform3f(matShader->getUniform("MatSpec"), 0.7, 0.7, 0.7);
            glUniform1f(matShader->getUniform("shine"), 10.0);
            break;
        case 11: // flat grey
            glUniform3f(matShader->getUniform("MatAmb"), 0.13, 0.13, 0.14);
            glUniform3f(matShader->getUniform("MatDif"), 0.3, 0.3, 0.4);
            glUniform3f(matShader->getUniform("MatSpec"), 0.3, 0.3, 0.4);
            glUniform1f(matShader->getUniform("shine"), 4.0);
            break;
        case 12: // chrome
            glUniform3f(matShader->getUniform("MatAmb"), 0.25, 0.25, 0.25);
            glUniform3f(matShader->getUniform("MatDif"), 0.4, 0.4, 0.4);
            glUniform3f(matShader->getUniform("MatSpec"), 0.774597, 0.774597, 0.774597);
            glUniform1f(matShader->getUniform("shine"), 76.8);
            break;
        case 13: // red rubber
            glUniform3f(matShader->getUniform("MatAmb"), 0.05, 0.0, 0.0);
            glUniform3f(matShader->getUniform("MatDif"), 0.5, 0.4, 0.4);
            glUniform3f(matShader->getUniform("MatSpec"), 0.7, 0.04, 0.04);
            glUniform1f(matShader->getUniform("shine"), 0);
            break;
        case 14: //"water"
            glUniform3f(matShader->getUniform("MatAmb"), 0.02, 0.04, 0.2);
            glUniform3f(matShader->getUniform("MatDif"), 0.0, 0.32, 0.9);
            glUniform3f(matShader->getUniform("MatSpec"), 0.14, 0.4, 0.8);
            glUniform1f(matShader->getUniform("shine"), 400.0);
            break;
        case 15: //"black"
            glUniform3f(matShader->getUniform("MatAmb"), 0.0, 0.0, 0.0);
            glUniform3f(matShader->getUniform("MatDif"), 0.0, 0.0, 0.0);
            glUniform3f(matShader->getUniform("MatSpec"), 0.0, 0.0, 0.0);
            glUniform1f(matShader->getUniform("shine"), 0.0);
            break;
        }

    }

	void render() {
		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		glViewport(0, 0, width, height);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float aspect = width/(float)height;

		// Create the matrix stacks - please leave these alone for now
		auto Projection = make_shared<MatrixStack>();
		auto View = make_shared<MatrixStack>();
		auto Model = make_shared<MatrixStack>();

		// Apply perspective projection.
		Projection->pushMatrix();
		Projection->perspective(45.0f, aspect, 0.01f, 300.0f);

        vector<glm::vec3> lightPositions = Game.getLightPositions();
        vector<glm::vec3> lightColorIntensity = Game.getLightColorIntensity();
        vector<glm::vec3> lightABC = Game.getLightABC();
        int numLights = lightPositions.size();

        Game.setUpWorld();
        Game.player->getCam()->setUpCam(windowManager);

		View->pushMatrix();
			View->loadIdentity();
            View->lookAt(Game.player->getCam()->eye, Game.player->getCam()->lookAt, Game.player->getCam()->upVector);
            
        matShader->bind();
		glUniformMatrix4fv(matShader->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
		glUniformMatrix4fv(matShader->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
        glUniform3f(matShader->getUniform("eye"), Game.player->getCam()->eye.x, Game.player->getCam()->eye.y, Game.player->getCam()->eye.z);

        glUniform3fv(matShader->getUniform("lightPositions"), numLights, &lightPositions[0].x);
        glUniform3fv(matShader->getUniform("lightColInt"), numLights, &lightColorIntensity[0].x);
        glUniform3fv(matShader->getUniform("lightABC"), numLights, &lightABC[0].x);
        glUniform1f(matShader->getUniform("numLights"), numLights);
        
        // Set up scene and draw 
        Model->pushMatrix();
            Model->loadIdentity();
            for (Entity* e : Game.getNoTexEntities())
            {
                Model->pushMatrix();
                e->setUp(Model);
                setModel(matShader, Model);
                SetMaterial(e->materialID);
                for (shared_ptr<Shape> s : e->mesh->shapes)
                    s->draw(matShader);
                Model->popMatrix();
            }
            Model->pushMatrix();
                Game.player->setUp(Model);
                setModel(matShader, Model);
                SetMaterial(Game.player->materialID);
                for (shared_ptr<Shape> s : Game.player->mesh->shapes)
                    s->draw(matShader);
            Model->popMatrix();
        Model->popMatrix();

        matShader->unbind();

        texShader->bind();
        glUniformMatrix4fv(texShader->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
        glUniformMatrix4fv(texShader->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
        glUniform3fv(texShader->getUniform("lightPositions"), numLights, &lightPositions[0].x);
        glUniform3fv(texShader->getUniform("lightColInt"), numLights, &lightColorIntensity[0].x);
        glUniform3fv(texShader->getUniform("lightABC"), numLights, &lightABC[0].x);
        glUniform1f(texShader->getUniform("numLights"), numLights);

        // Set up scene and draw 
        Model->pushMatrix();
        Model->loadIdentity();
        for (Entity* e : Game.getTexturedEntities())
        {
            if (e->tex)
            {
                Model->pushMatrix();
                e->setUp(Model);
                setModel(texShader, Model);
                glUniform1f(texShader->getUniform("flip"), 1);
                e->tex->bind(texShader->getUniform("Texture0"));
                for (shared_ptr<Shape> s : e->mesh->shapes)
                    s->draw(texShader);
                Model->popMatrix();
            }
        }
        Model->popMatrix();

        texShader->unbind();
        
        {
            skyShader->bind();
            glUniformMatrix4fv(skyShader->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
            glDepthFunc(GL_LEQUAL);
            glUniformMatrix4fv(skyShader->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));

            Model->pushMatrix();
            Model->loadIdentity();
            Model->scale(vec3(300, 300, 300));
            setModel(skyShader, Model);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
            for (shared_ptr<Shape> s : geometry["cube_notex"]->shapes)
                s->draw(skyShader);
            Model->popMatrix();

            glDepthFunc(GL_LESS);

            skyShader->unbind();

            // Pop matrix stacks.
            Projection->popMatrix();
            View->popMatrix();
        }
	}
};

int main(int argc, char *argv[])
{
	// Where the resources are loaded from
	std::string resourceDir = "../resources";

	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(1280, 720);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state

	application->init(resourceDir);
	application->initGeom(resourceDir);
    application->initWorld();

	// Loop until the user closes the window.
	while (! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
