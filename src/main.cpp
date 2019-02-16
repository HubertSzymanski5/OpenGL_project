#include <main.h>

// ____ ZMIENNE GLOBALNE _____
const char* APP_TITLE = "Introduction to Modert OpenGL - Loading OBJ Models";

//------------------------------------------------------------------------------------------
// ____ FUNKCJA MAIN _____
//------------------------------------------------------------------------------------------

int main(){

	// definicja poczatkowa zmiennych globalnych
	gWindowWidth = 1024;
	gWindowHeight = 768;
	gFullScreen = false;
	gWireframe = false;
	gFlashlightOn = false;

//	gYaw = 0.0f;
//	gPitch = 0.0f;
//	gRadius = 10.0f;


	// inicjalizacja okna OpenGL
	if ( !initOpenGL() ){
		std::cerr << "GLFW initialization FAILED" << std::endl;
		return -1;
	}

	// odpalenie maszyny losujacej
	srand(time(NULL));

	//------------------------------------------------------------------------------------------
	//		LADOWANIE SHADEROW ITP
	//------------------------------------------------------------------------------------------

	// uruchamiamy shadery (z folderu src)
	ShaderProgram lightShader;
	lightShader.loadShaders("../src/basic.vert", "../src/basic.frag");

	ShaderProgram lightingShader;
	lightingShader.loadShaders("../src/lighting.vert", "../src/lighting.frag");

	//------------------------------------------------------------------------------------------
	//		WCZYTYWANIE MODELI ITP
	//------------------------------------------------------------------------------------------

	// wsiatki i tekstury modeli stacjonarnych
	const int numModels = 5;
	Mesh mesh[numModels];
	Texture2D texture[numModels];

	// wczytanie siatek
	mesh[0].loadOBJ("../models/ground.obj");
	mesh[1].loadOBJ("../models/wall.obj");
	mesh[2].loadOBJ("../models/lampPost.obj");
	mesh[3].loadOBJ("../models/robot.obj");
	mesh[4].loadOBJ("../models/bench.obj");
//	mesh[5].loadOBJ("../models/barrel.obj");
//	mesh[6].loadOBJ("../models/woodcrate.obj");


	// wczytanie tekstur
	texture[0].loadTexture("../textures/groundTex.png", true);
	texture[1].loadTexture("../textures/wallTex.png", true);
	texture[2].loadTexture("../textures/lamp_post_diffuse.png", true);
	texture[3].loadTexture("../textures/robot_diffuse.jpg", true);
	texture[4].loadTexture("../textures/benchTex.png", true);
//	texture[5].loadTexture("../textures/barrel_diffuse.png", true);
//	texture[6].loadTexture("../textures/woodcrate_diffuse.jpg", true);


	Mesh lightMesh;
	lightMesh.loadOBJ("../models/light.obj");
	Mesh lampMesh;
	lampMesh.loadOBJ("../models/light.obj");

	// pozycje modeli stacjonarnych
	glm::vec3 modelPos[] = {
		// 		   + RIG   + UP   -FR
		glm::vec3(  0.0f,  0.0f,  0.0f ),	// ground
		glm::vec3(  0.0f,  0.0f,  0.0f ),	// wall
		glm::vec3(  4.0f,  0.0f, -5.0f ),	// lampa
		glm::vec3( -5.5f,  0.0f, -8.0f ),	// robot
		glm::vec3(  4.0f,  0.0f, -9.0f ),	// lawka
	//	glm::vec3( -5.5f,  0.0f, -10.0f ),	// beczka
	//	glm::vec3( -3.5f,  0.0f, -6.0f ),	// skrzynka
	};

	// skala modeli
	glm::vec3 modelScale[] = {
		glm::vec3(  2.0f,  1.0f,  2.0f ),	// ground
		glm::vec3(  2.0f,  2.0f,  2.0f ),	// wall
		glm::vec3(  1.0f,  1.0f,  1.0f ),	// lamp
		glm::vec3(  1.0f,  1.0f,  1.0f ),	// robot
		glm::vec3(  1.0f,  1.0f,  1.0f ),	// lawka
	//	glm::vec3(  0.5f,  0.5f,  0.5f ),	// beczka
	//	glm::vec3(  1.0f,  1.0f,  1.0f ),	// skrzynka
	};

	// jakis obrot ?
	float modelRotate[] = {
		 0.0f,		// ground
		 0.0f,		// wall
		 0.0f,		// lamp
		45.0f, 		// robot
		 0.0f,		// lawka
//		 0.0f,		// beczka
//		 0.0f		// skrzynka
	};

	// wczytanie wody
	Mesh water;
	Texture2D waterTex;

	water.loadOBJ("../models/water.obj");
	waterTex.loadTexture("../textures/water.png");

	glm::vec3 waterPos = glm::vec3(0.0f, -0.5f, 50.0f);
	glm::vec3 waterScale = glm::vec3(5.0f, 0.0f, 5.0f);

	// wczytywanie drzewkow
	const int numTrees = 2;
	Mesh tree[numTrees];
	Texture2D treeTex[numTrees];

	// wczytanie siatek
	tree[0].loadOBJ("../models/spiceTree.obj");
	tree[1].loadOBJ("../models/roundTree.obj");

	// wczytanie tekstur
	treeTex[0].loadTexture("../textures/spiceTreeTex.png", true);
	treeTex[1].loadTexture("../textures/roundTreeTex.png", true);

	// polozenia i skala drzewek spicy
	const int sTreesN = 70;
	glm::vec3 sTreePos[sTreesN];
	glm::vec3 sTreeScale[sTreesN];
	for ( int i = 0; i < sTreesN; i++ ) {
		// polozenie
		sTreePos[i].x = (float)(rand()%61) - 30.0f;
		// wypchniecie na boki
		if (sTreePos[i].x > -6.0f && sTreePos[i].x <= 0.0f) sTreePos[i].x -= 7.0f;
		else if (sTreePos[i].x < 6.0f && sTreePos[i].x >= 0.0f) sTreePos[i].x += 7.0f;
		sTreePos[i].y = 0.0f;
		sTreePos[i].z = (float)(rand()%81) - 40.0f;

		// dodajemy troche roznorodnosci
		sTreeScale[i].x = 1.0f + (float)(rand()%9) / 10.0f - 0.3f; 
		sTreeScale[i].y = 1.0f + (float)(rand()%5) / 10.0f - 0.4f;
		sTreeScale[i].z = sTreeScale[i].x; // zeby nie wygladaly jak nalesniki
	}

	// polozenia i skala drzewek okraglych
	const int rTreesN = 25;
	glm::vec3 rTreePos[rTreesN];
	glm::vec3 rTreeScale[rTreesN];
	float rTreeRotate[rTreesN];
	for ( int i = 0; i < rTreesN; i++ ) {
		// polozenie
		rTreePos[i].x = (float)(rand()%61) - 30.0f;
		// wypchniecie na boki
		if (rTreePos[i].x > -7.0f && rTreePos[i].x <= 0.0f) rTreePos[i].x -= 8.0f;
		else if (rTreePos[i].x < 10.0f && rTreePos[i].x >= 0.0f) rTreePos[i].x += 11.0f;
		rTreePos[i].y = 0.0f;
		rTreePos[i].z = (float)(rand()%81) - 40.0f;

		// dodajemy troche roznorodnosci
		rTreeScale[i].x = 2.0f + (float)(rand()%9) / 10.0f - 0.3f; 
		rTreeScale[i].y = 2.0f + (float)(rand()%5) / 10.0f - 0.4f;
		rTreeScale[i].z = rTreeScale[i].x; // zeby nie wygladaly jak nalesniki

		rTreeRotate[i] = (float)(rand()%360);
	}

	//------------------------------------------------------------------------------------------
	//		WCZYTYWANIE PARAMETROW SWIATEL
	//------------------------------------------------------------------------------------------

	// swiatlo 1
	glm::vec3 lightPos(0.0f, 5.0f, 90.0f);
	glm::vec3 lightColor(1.0f, 0.6f , 0.2f);
	glm::vec3 lightSpecular(1.0f, 1.0f , 1.0f);

	// swiatlo lampy
	glm::vec3 lampPos = modelPos[2];
	lampPos.y += 3.8;
	glm::vec3 lampColor(1.0f, 1.0f , 0.5f);
	glm::vec3 lampSpecular(1.0f, 1.0f , 0.5f);
		// maly dodatek w postaci dodania pozycji otoczki swiatla
		glm::vec3 bulbPos = lampPos;
		bulbPos.y -= 0.05f;

	// latarka
	glm::vec3 latPos; // do wczytania w glownym petli glownej
	glm::vec3 latColor = glm::vec3(0.9f, 0.9f, 1.0f); // niech bedzie lekko niebieskawa
	glm::vec3 latSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

	//------------------------------------------------------------------------------------------
	//		PRZYGOTOWANIE I WYRYSOWANIE SCENY
	//------------------------------------------------------------------------------------------

	// definicja macierzy widoku itp itd
	glm::mat4 model, view, projection;

	double lastTime = glfwGetTime();
	float angle = 0.0f;

	// glowna petla
	while ( !glfwWindowShouldClose(gWindow) ) {

		//pokaz FPS
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		// przechwytujemy EVENTY
		glfwPollEvents();
		update(deltaTime);

		// kolorujemy okno
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ustawienia kamery w zaleznosci od ruchu myszy
//		orbitCamera.setLookAt(cubePos);
//		orbitCamera.rotate(gYaw, gPitch);
//		orbitCamera.setRadius(gRadius);

//		view = orbitCamera.getViewMatrix();

		view = fpsCamera.getViewMatrix();

		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);

		glm::vec3 viewPos;
		viewPos = fpsCamera.getPosition();


		//---------------------------------
		// Zachod Slonca
		//---------------------------------
		angle += (float)deltaTime * 50.0f;
		lightPos.y -= (float)deltaTime * 0.8f;
		if ( lightPos.y < 5.0f && lightPos.y >= 0.0f ) {
			// swiatlo przechodzi w czerwone
			// skladowej czerwonej nie zmienam
			lightColor.y = 0.8f * lightPos.y / 5.0f;
			lightColor.z = 0.2f * lightPos.y / 5.0f;

			// odbicie też by trzeba przyciachac
			//	glm::vec3 lightSpecular(1.0f, 0.8f , 0.5f);
			lightSpecular.y = 0.8f * lightPos.y / 5.0f;
			lightSpecular.z = 0.5f * lightPos.y / 5.0f;
		}
		else if ( lightPos.y < 0.0f && lightPos.y >= -10.0f ) {
			// swiatlo wygasa - wycinam ostatnia skladowa R
			lightColor.x = 0.1f * lightPos.y + 1.0f;
			lightSpecular.x = 0.1f * lightPos.y + 1.0f;
		}
		else if ( lightPos.y < -10.0f ) {
			lightColor = glm::vec3(0.0f, 0.0f , 0.0f);
			lightSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
			// to jako zabezpieczenie zeby nam Slonko do piekla nie ucieklo
			lightPos.y = -11.0f;
		}

		// aktywujemy program shaderoww
		lightingShader.use();

		// przekazujemy macierze
		lightingShader.setUniform("view", view);
		lightingShader.setUniform("projection", projection);
		lightingShader.setUniform("viewPos", viewPos);

		// dla swiatla Slonca
		lightingShader.setUniform("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setUniform("light.diffuse", lightColor);
		lightingShader.setUniform("light.specular", lightSpecular);
		lightingShader.setUniform("light.position", lightPos);

		// dla lamp0
		lightingShader.setUniform("lamp.ambient", glm::vec3(0.2f, 0.2f, 0.0f));
		lightingShader.setUniform("lamp.diffuse", lampColor);
		lightingShader.setUniform("lamp.specular", lampSpecular);
		lightingShader.setUniform("lamp.position", lampPos);
		lightingShader.setUniform("lamp.constant", 1.0f);
		lightingShader.setUniform("lamp.linear", 0.027f);
		lightingShader.setUniform("lamp.exponent", 0.0028f);

		// dla latarki
			// najpierw nalezy pamietac ze ustawienie sie zmienia (lazi z nami ta latarka)
		latPos = fpsCamera.getPosition();
		latPos.y -= 0.5f; // dla lepszego efektu troszke jakby w rece lezala
			// teraz przekazujemy mundurki :)
		lightingShader.setUniform("lat.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
		lightingShader.setUniform("lat.diffuse", latColor);
		lightingShader.setUniform("lat.specular", latSpecular);
		lightingShader.setUniform("lat.position", latPos);
		lightingShader.setUniform("lat.direction", fpsCamera.getLook());
		lightingShader.setUniform("lat.constant", 1.0f);
		lightingShader.setUniform("lat.linear", 0.027f);		//.07
		lightingShader.setUniform("lat.exponent", 0.0028f);	//.017
		lightingShader.setUniform("lat.cosInnerCone", glm::cos(glm::radians(15.0f)));
		lightingShader.setUniform("lat.cosOuterCone", glm::cos(glm::radians(20.0f)));
		lightingShader.setUniform("lat.on", gFlashlightOn);



		// render obiektow stalych
		for (int i = 0; i < numModels; i++) {
			model = glm::translate( glm::mat4(1.0f), modelPos[i]) * glm::scale(glm::mat4(1.0f), modelScale[i]) * glm::rotate(glm::mat4(1.0f) ,glm::radians(modelRotate[i]), glm::vec3(0, 1, 0));
			lightingShader.setUniform("model", model);

			lightingShader.setUniform("material.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
			lightingShader.setUniformSampler("material.diffuseMap", 0);
			lightingShader.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			lightingShader.setUniform("material.shininess", 150.0f);
			
			texture[i].bind(0);
			mesh[i].draw();
			texture[i].unbind(0);
		}

		// render drzewek sTree
		for (int i = 0; i < sTreesN; i++ ) {
			model = glm::translate( glm::mat4(1.0f), sTreePos[i]) * glm::scale(glm::mat4(1.0f), sTreeScale[i]);
			lightingShader.setUniform("model", model);

			lightingShader.setUniform("material.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
			lightingShader.setUniformSampler("material.diffuseMap", 0);
			lightingShader.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			lightingShader.setUniform("material.shininess", 150.0f);
			
			treeTex[0].bind(0);
			tree[0].draw();
			treeTex[0].unbind(0); // sTree ma index [0]
		}

		// render drzewek rTree
		for (int i = 0; i < rTreesN; i++ ) {
			model = glm::translate( glm::mat4(1.0f), rTreePos[i]) * glm::scale(glm::mat4(1.0f), rTreeScale[i]) * glm::rotate(glm::mat4(1.0f), glm::radians(rTreeRotate[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			lightingShader.setUniform("model", model);

			lightingShader.setUniform("material.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
			lightingShader.setUniformSampler("material.diffuseMap", 0);
			lightingShader.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			lightingShader.setUniform("material.shininess", 150.0f);

			
			treeTex[1].bind(0);
			tree[1].draw();
			treeTex[1].unbind(0); // rTree ma index [1]
		}

		// render wody
		model = glm::translate( glm::mat4(1.0f), waterPos) * glm::scale(glm::mat4(1.0f), waterScale);
		lightingShader.setUniform("model", model);

			lightingShader.setUniform("material.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
			lightingShader.setUniformSampler("material.diffuseMap", 0);
			lightingShader.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			lightingShader.setUniform("material.shininess", 150.0f);
			
			waterTex.bind(0);
			water.draw();
			waterTex.unbind(0);

		// render swiatla -- SLONCA !!!
		model = glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
		lightShader.use();
		lightShader.setUniform("lightColor", lightColor);
		lightShader.setUniform("model", model);
		lightShader.setUniform("view", view);		
		lightShader.setUniform("projection", projection);
		lightShader.setUniform("alpha", 1.0f);
		lightMesh.draw();

		// render swiatla -- LAMPA !!!
		model = glm::translate(glm::mat4(1.0f), bulbPos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
//		lightShader.use();
		lightShader.setUniform("lightColor", lampColor);
		lightShader.setUniform("model", model);
		lightShader.setUniform("view", view);		
		lightShader.setUniform("projection", projection);
		lightShader.setUniform("alpha", 0.5f);
		lampMesh.draw();
		// odpalamy Double Buffer
		glfwSwapBuffers(gWindow);

		lastTime = currentTime;

	}

	// czyscimy wszystko
	glfwTerminate();
	return 0;
}

//------------------------------------------------------------------------------------------
// ____ METODY I FUNKCJE - DEFINICJE _____
//------------------------------------------------------------------------------------------

bool initOpenGL() {

	if ( !glfwInit() ){
		std::cerr << "GLFW initialization FAILED" << std::endl;
		return false;
	}

	// inicjalizacja GLFW i OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE ,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// tworzymy okno
	gWindow = NULL;
	if ( !gFullScreen ) 
		gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	else {
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);
		if ( pVmode != NULL ) {
			gWindow = glfwCreateWindow(pVmode->width, pVmode->height, APP_TITLE, pMonitor, NULL);
		}
	}
	if ( gWindow == NULL ) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// ustawiamy okno glowne
	glfwMakeContextCurrent(gWindow);

	glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetCursorPosCallback(gWindow, glfw_onMouseMove);
	glfwSetScrollCallback(gWindow, glfw_onMouseScroll);

	// chowa i przechwytuje kursor, niekonczoncy sie ruch
	glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

	// inicjalizujemy GLEW
	glewExperimental = GL_TRUE;
	if ( glewInit() != GLEW_OK ) {
		std::cerr << "GLEW initialization FAILED" << std::endl;
		return false;
	}

	glViewport(0, 0, gWindowWidth, gWindowHeight);
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	// ustawiamy glebie
	glEnable(GL_DEPTH_TEST);
	// ustawiamy ze mozna przenikalnosc dac ?
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
	if ( key == GLFW_KEY_F10 && action == GLFW_PRESS ) {
		gWireframe = !gWireframe;
		if ( gWireframe ) 
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else 
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if ( key == GLFW_KEY_F && action == GLFW_PRESS ) {
		gFlashlightOn = !gFlashlightOn;
	}
}

void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height) {
	// tutaj przyjmujemy wielkosci nowe
	gWindowWidth = height;
	gWindowHeight = width;
	// ustawienie portu widzenia - wsp. ekranowe na normalizowane OpenGL'a
	glViewport(0, 0, gWindowWidth, gWindowHeight);
}

void glfw_onMouseMove(GLFWwindow *window, double posX, double posY) {
/*	static glm::vec2 lastMousePos = glm::vec2(0, 0);

	// Aktualizuj katy w oparciu o LPM aby orbitowac wokol kostki
	if ( glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_LEFT) == 1 ) {
		// czulosc myszki
		gYaw -= ((float)posX - lastMousePos.x ) * MOUSE_SENSITIVITY;
		gPitch += ((float)posY - lastMousePos.y ) * MOUSE_SENSITIVITY;
	}

	// zmien promien orbity w oparciu o PPM
	if ( glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_RIGHT) == 1 ) {
		float dx = 0.01f * ((float)posX - lastMousePos.x);
		float dy = 0.01f * ((float)posY - lastMousePos.y);
		gRadius += dx - dy;
	}

	lastMousePos.x = (float)posX;
	lastMousePos.y = (float)posY;
*/
}

void glfw_onMouseScroll(GLFWwindow *window, double deltaX, double deltaY) {
	double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

	fov = glm::clamp(fov, 1.0, 120.0);

	fpsCamera.setFOV((float)fov);
}

void update(double elapsedTime) {
	// orientacja kamery
	double mouseX, mouseY;

	// odczutyjemy pozycje kursora
	glfwGetCursorPos(gWindow, &mouseX, &mouseY);

	// obracamy kamere
	fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY);

	// ustawiamy kursor na srodku ekranu
	glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

	// RUCH KAMERY FPS

	// przod / tyl
	if ( glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS )
		fpsCamera.move(MOVE_SPEED* (float)elapsedTime * fpsCamera.getLook());
	else if ( glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS ) 
		fpsCamera.move(MOVE_SPEED* (float)elapsedTime * -fpsCamera.getLook());

	// lewo / prawo
	if ( glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS )
		fpsCamera.move(MOVE_SPEED* (float)elapsedTime * -fpsCamera.getRight());
	else if ( glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS ) 
		fpsCamera.move(MOVE_SPEED* (float)elapsedTime * fpsCamera.getRight());

	// gora / dol
	if ( glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS )
		fpsCamera.move(MOVE_SPEED* (float)elapsedTime * fpsCamera.getUp());
	else if ( glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS )
		fpsCamera.move(MOVE_SPEED* (float)elapsedTime * -fpsCamera.getUp());
}

void showFPS(GLFWwindow* window) {
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); // zwraca liczbe sekund od kiedy GLFW rozpoczelo prace

	elapsedSeconds = currentSeconds - previousSeconds;

	// ograniczenie update do 4 razy na sekunde
	if ( elapsedSeconds > 0.25 ) {
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		std::ostringstream outs;
		outs.precision(3);
		outs << std::fixed
			 << APP_TITLE << "\t"
			 << "FPS: " << fps << "\t"
			 << "Frame Time: " << msPerFrame << " (ms)";
		glfwSetWindowTitle(window, outs.str().c_str());

		frameCount = 0;
	}
	frameCount++;
}