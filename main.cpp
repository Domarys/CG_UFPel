#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/spline.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>
#include <glm/ext.hpp>

//headers
void framebuffer_size_callback(GLFWwindow* window, int width, int height); //atulização do tamanho da janela
void mouse_callback(GLFWwindow* window, double xpos, double ypos); // movimento mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); // movimento scroll
void processInput(GLFWwindow *window); // entrada dados teclado tela?

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
vector <Camera> matrixCam;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// matriz
int idModel = 0;
int idCam = 0;
float currentTime = 0.0, lastTime = 0.0;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit(); // inicializa GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // versão x.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // versao 0.x
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // usar modo nucleo que permite varias funções

#ifdef __APPLE__// se for no MAC
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "AgoraVaiHein!!!", NULL, NULL); // cria janela
    if (window == NULL) // testa se a criação foi bem sucedida
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // coloca a atual window como a do contexto
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // liga window e buffer
    glfwSetCursorPosCallback(window, mouse_callback); // liga window e cursor
    glfwSetScrollCallback(window, scroll_callback); // liga window e scroll

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // captura mouse

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // carrega os ponteiros passados pelo glfw
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader(FileSystem::getPath("resources/cg_ufpel.vs").c_str(), FileSystem::getPath("resources/cg_ufpel.fs").c_str());

    // load models
    // -----------
	//vector<Model> ourModel;
    Model ourModel(FileSystem::getPath("resources/objects/cyborg/cyborg.obj"));
	//Model ourModel(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));
	//Model ourModel(FileSystem::getPath("resources/objects/planet/planet.obj"));
	//Model ourModel(FileSystem::getPath("resources/objects/rock/rock.obj"));


    
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// ---------------------------- moved
	// render the loaded model
	vector <glm::mat4> matrixModel;
	matrixModel.push_back(glm::mat4(1.0));
	matrixModel.back() = glm::translate(matrixModel.back(), glm::vec3(0.0f, -1.0f, 0.0f)); // translate it down so it's at the center of the scene
	matrixModel.back() = glm::scale(matrixModel.back(), glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
	idModel = (matrixModel.size() - 1);
	// ---------------------------- moved

	// view/projection transformations
	Camera cam = glm::vec3(0.0f, 0.0f, 3.0f);
	matrixCam.push_back(cam);
	glm::mat4 projection = glm::perspective(glm::radians(matrixCam[0].Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = matrixCam[0].GetViewMatrix();
	ourShader.setMat4("projection", projection);
	ourShader.setMat4("view", view);
	float tmp = 0.0,tropeco = 0;

	glm::mat4 aux;

	float radius = 10.0f;
	int sinal = 0;


    // render loop
    // -----------
	while (!glfwWindowShouldClose(window))
	{

		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		currentTime = glfwGetTime();


		// input
		// -----
		processInput(window); // verifica a entrada de dados na janela

		// render
		// ------
		glClearColor(0.4f, 0.02f, 0.5f, 0.5f); // aplica uma única cor em toda tela GLFW
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpa os buffers

		// don't forget to enable shader before setting uniforms
		ourShader.use();

		// view/projection transformations
		projection= glm::perspective(glm::radians(matrixCam[idCam].Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = matrixCam[idCam].GetViewMatrix();
		//std::cout << "camera:" << idCam << endl;
		
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);		
		//ourShader.setMat4("view", view);

		//for (int i = 0; i < matrixModel.size(); i++) {
			//ourShader.setMat4("model", matrixModel[i]);
			//ourModel.Draw(ourShader);
		//}

		// render the loaded model
		//glm::mat4 model;
		//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // translacao -x
			matrixModel[idModel] = glm::translate(matrixModel[idModel], glm::vec3(-0.2f, 0.0f, 0.0f)); 

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // translacao +x
			matrixModel[idModel] = glm::translate(matrixModel[idModel], glm::vec3(0.2f, 0.0f, 0.0f)); 

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // translacao +y
			matrixModel[idModel] = glm::translate(matrixModel[idModel], glm::vec3(0.0f, 0.2f, 0.0f)); 

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // translacao -y
			matrixModel[idModel] = glm::translate(matrixModel[idModel], glm::vec3(0.0f, -0.2f, 0.0f)); 

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // translacao +z
			matrixModel[idModel] = glm::translate(matrixModel[idModel], glm::vec3(0.0f, 0.0f, 0.5f)); 

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // translacao -z
			matrixModel[idModel] = glm::translate(matrixModel[idModel], glm::vec3(0.0f, 0.0f, -0.5f)); 

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) { // rotacao x com tempo
			//model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.1f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
			double step, delta, tempoDefinido = 5, tempoAtual, tempoInicial = glfwGetTime();
			do {

				tempoAtual = glfwGetTime();
				delta = tempoAtual - tempoInicial;
				step = (0.002 * delta) / tempoDefinido;
				matrixModel[idModel] = glm::rotate(matrixModel[idModel], glm::radians(1.0f), glm::vec3(0.1f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
				glClearColor(0.4f, 0.02f, 0.5f, 0.5f); // aplica uma única cor em toda tela GLFW
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpa os buffers

				for (int i = 0; i < matrixModel.size(); i++) {
					ourShader.setMat4("model", matrixModel[i]);
					ourModel.Draw(ourShader);
				}

				glfwSwapBuffers(window); // troca de buffers para que somente o pronto seja exibido
				glfwPollEvents(); // verifica a ocorrência de interações do usuário com a janela
			} while (tempoAtual <= (tempoInicial + tempoDefinido));
		}

		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) { // rotacao y via key
			matrixModel[idModel] = glm::rotate(matrixModel[idModel], glm::radians(1.0f), glm::vec3(0.0f, 0.1f, 0.0f));
			/*	double step, delta, tempoDefinido = 5, tempoAtual, tempoInicial = glfwGetTime();
				do {

					tempoAtual = glfwGetTime();
					delta = tempoAtual - tempoInicial;
					step = (0.002 * delta) / tempoDefinido;
					model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 0.1f, 0.0f)); // translate it down so it's at the center of the scene
					glClearColor(0.4f, 0.02f, 0.5f, 0.5f); // aplica uma única cor em toda tela GLFW
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpa os buffers

					for (int i = 0; i < matrixModel.size(); i++) {
						ourShader.setMat4("model", matrixModel[i]);
						ourModel.Draw(ourShader);
					}

					glfwSwapBuffers(window); // troca de buffers para que somente o pronto seja exibido
					glfwPollEvents(); // verifica a ocorrência de interações do usuário com a janela
				} while (tempoAtual <= (tempoInicial + tempoDefinido)); */
		}

		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) { // rotacao z via key
			matrixModel[idModel] = glm::rotate(matrixModel[idModel], glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 0.1f));
			/*	double step, delta, tempoDefinido = 5, tempoAtual, tempoInicial = glfwGetTime();
				do {

					tempoAtual = glfwGetTime();
					delta = tempoAtual - tempoInicial;
					step = (0.002 * delta) / tempoDefinido;
					model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 0.1f)); // translate it down so it's at the center of the scene
					glClearColor(0.4f, 0.02f, 0.5f, 0.5f); // aplica uma única cor em toda tela GLFW
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpa os buffers

					for (int i = 0; i < matrixModel.size(); i++) {
						ourShader.setMat4("model", matrixModel[i]);
						ourModel.Draw(ourShader);
					}

					glfwSwapBuffers(window); // troca de buffers para que somente o pronto seja exibido
					glfwPollEvents(); // verifica a ocorrência de interações do usuário com a janela
				} while (tempoAtual <= (tempoInicial + tempoDefinido)); */
		}

		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) { // ativa wirwframe
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) { // desativa wirwframe
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) { // difença entre vetor e camera
			glm::vec3 dotMod(1.0f, 0.0f, 0.0f); //cameraTarget
			glm::vec3 direction = glm::normalize(matrixCam[0].Position - dotMod);
			std::cout << "Angulo diferenca camera e modelo: " << std::endl;
			std::cout << "X: " << direction.x << " " << "Y: " << direction.y << " " << "Z: " << direction.z << std::endl;
		}

		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) { 		//animacao????????

			double step, delta, tempoDefinido = 5, tempoAtual, tempoInicial = glfwGetTime();

			do {
				tempoAtual = glfwGetTime();
				delta = tempoAtual - tempoInicial;
				step = (0.002 * delta) / tempoDefinido;
				matrixModel[idModel] = glm::translate(matrixModel[idModel], glm::vec3(step, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
				glClearColor(0.4f, 0.02f, 0.5f, 0.5f); // aplica uma única cor em toda tela GLFW
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpa os buffers

				for (int i = 0; i < matrixModel.size(); i++) {
					ourShader.setMat4("model", matrixModel[i]);
					ourModel.Draw(ourShader);
				}

				glfwSwapBuffers(window); // troca de buffers para que somente o pronto seja exibido
				glfwPollEvents(); // verifica a ocorrência de interações do usuário com a janela

			} while (tempoAtual <= (tempoInicial + tempoDefinido));
		}
		
		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && (currentTime-lastTime) > 1) { //new
			matrixModel.push_back(glm::mat4(1.0));
			matrixModel.back() = glm::translate(matrixModel.back(), glm::vec3(0.0f, -1.0f, 0.0f)); // translate it down so it's at the center of the scene
			matrixModel.back() = glm::scale(matrixModel.back(), glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
			lastTime = currentTime;
			idModel = (matrixModel.size()-1);
		}
		
		if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS && (currentTime - lastTime) > 1) { //idModel ++
			if ((idModel+1) >= matrixModel.size()) {			
				idModel = 0;			}
			else {
				idModel++;
			}
			lastTime = currentTime;
		}
	
		if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS && (currentTime - lastTime) > 1) { //idModel --
			if ((idModel-1) <= 0) {
				idModel = matrixModel.size()-1;
			}
			else {
				idModel--;
			}
			lastTime = currentTime;
		}

		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {


		}

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && (currentTime - lastTime) > 1) { // bspline

			if (tmp <= 1.0) {
					glm::vec3 b = glm::catmullRom(
						glm::vec3(matrixModel[idModel][0][0], matrixModel[idModel][1][1] - 5, matrixModel[idModel][2][2]),
						glm::vec3(matrixModel[idModel][0][0], matrixModel[idModel][1][1], matrixModel[idModel][2][2]),
						glm::vec3(matrixModel[idModel][0][0] + 1.0, matrixModel[idModel][1][1], matrixModel[idModel][2][2]),
						glm::vec3(matrixModel[idModel][0][0], matrixModel[idModel][1][1], matrixModel[idModel][2][2]),
						tmp);
					matrixModel[idModel][3][0] = b.x;
					matrixModel[idModel][3][1] = b.y;
					matrixModel[idModel][3][2] = b.z;
					tmp += 0.001;
				}
				else {
				tmp = 0.0;
				}
		}
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) { // bezier

			glm::vec3 a(2.5f, -3.0f, 0.0f),	b(0.0f, 0.0f, 0.0f), c(5.0f, -2.3f, -1.0f),	d(3.5, 3.0f, -1.0f);

			if (tmp <= 1.0f) {
				glm::vec3 ponto = (float)pow(1 - tmp, 3) * a +
					3 * (float)pow(1 - tmp, 2) * tmp * b +
					3 * (1 - tmp) * (float)pow(tmp, 2) * c +
					(float)pow(tmp, 3) * d;
				matrixModel[idModel][3][0] = ponto.x;
				matrixModel[idModel][3][1] = ponto.y;
				matrixModel[idModel][3][2] = ponto.z;
				tmp += 0.001;
			} else {
				tmp = 0.0;
			}


		}
		// CAMERA
		if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS  && (currentTime - lastTime) > 1) { //new cam
			Camera cam = glm::vec3(0.0f, 0.0f, 3.0f);
			matrixCam.push_back(cam);
			lastTime = currentTime;
			idCam = (matrixCam.size() - 1);
		}

		if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS && (currentTime - lastTime) > 1) { //idCam ++
			if ((idCam + 1) >= matrixCam.size()) {
				idCam = 0;
			}
			else {
				idCam++;
			}
			lastTime = currentTime;
		}

		if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS && (currentTime - lastTime) > 1) { //idCam --
			if ((idCam - 1) <= 0) {
				idCam = matrixCam.size() - 1;
			}
			else {
				idCam--;
			}
			lastTime = currentTime;
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && (currentTime - lastTime) > 1) {// CAM: translacao +y 			
			float MovementSpeed = 1;
			deltaTime = 0.01;
			float velocity = MovementSpeed * deltaTime;			
			matrixCam[idCam].Position += matrixCam[idCam].Front * velocity;
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && (currentTime - lastTime) > 1) {// CAM: translacao -y			
			float MovementSpeed = 1;
			deltaTime = 0.01;
			float velocity = MovementSpeed * deltaTime;
			matrixCam[idCam].Position -= matrixCam[idCam].Front * velocity;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && (currentTime - lastTime) > 1) {// CAM: translacao +x 			
			float MovementSpeed = 1;
			deltaTime = 0.01;
			float velocity = MovementSpeed * deltaTime;
			matrixCam[idCam].Position += matrixCam[idCam].Right * velocity;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && (currentTime - lastTime) > 1) {// CAM: translacao -y		
			float MovementSpeed = 1;
			deltaTime = 0.01;
			float velocity = MovementSpeed * deltaTime;
			matrixCam[idCam].Position -= matrixCam[idCam].Right * velocity;
		}

		if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS && (currentTime - lastTime) > 1) {// CAM: rotação eixo 	
			glm::vec3 dotMod(1.0f, 0.0f, 0.0f); //cameraTarget
			glm::vec3 vecAux;

			vecAux[0] = matrixModel[idModel][3][0];
			vecAux[1] = matrixModel[idModel][3][1];

			vecAux[2] = matrixModel[idModel][3][2];
			glm::vec3 direction = glm::normalize(dotMod - (0.0f, -1.0f, 0.0f));
			std::cout << "Angulo diferenca camera e modelo: " << std::endl;
			std::cout << "X: " << direction.x << " " << "Y: " << direction.y << " " << "Z: " << direction.z << std::endl;

			float camX = sin(glfwGetTime()) * direction.z;
			float camZ = cos(glfwGetTime()) * direction.z;
			glm::mat4 view;
			view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
			ourShader.setMat4("projection", projection);
			ourShader.setMat4("view", view);		
		}

		if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS && (currentTime - lastTime) > 1) {// CAM: zoom		
			if (matrixCam[idCam].Zoom >= 1.0f && matrixCam[idCam].Zoom <= 45.0f)
				matrixCam[idCam].Zoom -= 1;
			if (matrixCam[idCam].Zoom <= 1.0f)
				matrixCam[idCam].Zoom = 1.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS && (currentTime - lastTime) > 1) {// CAM: retirada zoom	
			if (matrixCam[idCam].Zoom >= 1.0f && matrixCam[idCam].Zoom <= 45.0f)
				matrixCam[idCam].Zoom += 1;
			if (matrixCam[idCam].Zoom >= 45.0f)
				matrixCam[idCam].Zoom = 45.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_F7) == GLFW_PRESS && (currentTime - lastTime) > 1) {// CAM: lookat	
			float camX = sin(glfwGetTime()) * 10;
			float camZ = cos(glfwGetTime()) * 10;
			glm::mat4 view;
			view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));
			ourShader.setMat4("projection", projection);
			ourShader.setMat4("view", view);
		}

		if (glfwGetKey(window, GLFW_KEY_F8) == GLFW_PRESS && (currentTime - lastTime) > 1) { // CAM: spline
			if (tmp <= 1.0) {
				glm::vec3 b = glm::catmullRom(
					glm::vec3(2.0f, 1.0f, 0.0f),
					glm::vec3(0.0f, -0.8f, 0.0f),
					glm::vec3(2.5f, -2.3f, -1.0f), 
					glm::vec3(2.5, 1.2f, -1.0f),
					tmp);
				matrixCam[idCam].Position.x = b.x;
				matrixCam[idCam].Position.y = b.y;
				matrixCam[idCam].Position.z = b.z;
				tmp += 0.001;
			}
			else {
				tmp = 0.0;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS && (currentTime - lastTime) > 1) {// CAM: bezier
			glm::vec3 a(2.5f, -3.0f, 0.0f), b(0.0f, 0.0f, 0.0f), c(5.0f, -2.3f, -1.0f), d(3.5, 3.0f, -1.0f);

			if (tmp <= 1.0f) {
				glm::vec3 ponto = (float)pow(1 - tmp, 3) * a +
					3 * (float)pow(1 - tmp, 2) * tmp * b +
					3 * (1 - tmp) * (float)pow(tmp, 2) * c +
					(float)pow(tmp, 3) * d;
				matrixModel[idModel][3][0] = ponto.x;
				matrixModel[idModel][3][1] = ponto.y;
				matrixModel[idModel][3][2] = ponto.z;
				tmp += 0.001;
			}
			else {
				tmp = 0.0;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS && (currentTime - lastTime) > 1) {// CAM: trôpego	
			float MovementSpeed = 1;
			deltaTime = 0.01;
			float velocity = MovementSpeed * deltaTime;
			matrixCam[idCam].Position += matrixCam[idCam].Front * velocity;
			std::cout << "velocity: "<< velocity << std::endl;


			if (tropeco >= 1.0) {	
				sinal = -1;			
			} 
			if ((tropeco <= -1)|| (tropeco == 0)) {
				sinal = 1;
			}
			if (sinal == 1) {
				matrixCam[idCam].Position -= matrixCam[idCam].Up * 0.005; // velocity em 0.01 foi mudada para suavizar o tropeço
				tropeco += 0.01;
			}
			if (sinal == -1) {
				matrixCam[idCam].Position += matrixCam[idCam].Up * 0.005;
				tropeco -= 0.01;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS && (currentTime - lastTime) > 1) {// CAM: retirada zoom	
			if (matrixCam[idCam].Zoom >= 1.0f && matrixCam[idCam].Zoom <= 45.0f)
				matrixCam[idCam].Zoom += 1;
			if (matrixCam[idCam].Zoom >= 45.0f)
				matrixCam[idCam].Zoom = 45.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_F12) == GLFW_PRESS && (currentTime - lastTime) > 1) {// CAM: retirada zoom	
			if (matrixCam[idCam].Zoom >= 1.0f && matrixCam[idCam].Zoom <= 45.0f)
				matrixCam[idCam].Zoom += 1;
			if (matrixCam[idCam].Zoom >= 45.0f)
				matrixCam[idCam].Zoom = 45.0f;
		}

		//printar objetos
		//ourShader.setMat4("model", model);
		//ourModel.Draw(ourShader);
		for (int i = 0; i < matrixModel.size(); i++) {
		ourShader.setMat4("model", matrixModel[i]);
		ourModel.Draw(ourShader);
		}	

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window); // troca de buffers para que somente o pronto seja exibido
        glfwPollEvents(); // verifica a ocorrência de interações do usuário com a janela
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // verifica naquele meio de comunicaçao se determinado evento foi acionado
        glfwSetWindowShouldClose(window, true);
		/*
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		matrixCam[idCam].ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		matrixCam[idCam].ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		matrixCam[idCam].ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		matrixCam[idCam].ProcessKeyboard(RIGHT, deltaTime); */
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

	matrixCam[0].ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	matrixCam[0].ProcessMouseScroll(yoffset);
}
