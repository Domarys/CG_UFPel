#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

//headers
void framebuffer_size_callback(GLFWwindow* window, int width, int height); //atulização do tamanho da janela
void mouse_callback(GLFWwindow* window, double xpos, double ypos); // movimento mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); // movimento scroll
void processInput(GLFWwindow *window); // entrada dados teclado tela?

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// matriz
int numModel = 0;
float baseX = 0.0f;
int mutex = 0;

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
    Model ourModel(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));

    
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// ---------------------------- moved
	// render the loaded model
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down

	// render the loaded model ADICIONADO
	//glm::mat4 model, model2, model3, matrixModel[10];
	//matrixModel[numModel] = glm::translate(matrixModel[0], glm::vec3(-1.0f, -1.0f, 0.0f)); // translate it down so it's at the center of the scene
	//matrixModel[numModel] = glm::scale(matrixModel[0], glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
	//numModel++;
	//matrixModel[numModel] = glm::translate(matrixModel[1], glm::vec3(0.5f, -1.0f, 0.0f));
	//matrixModel[numModel] = glm::scale(matrixModel[1], glm::vec3(0.1f, 0.1f, 0.1f));
	//numModel++;
	// ---------------------------- moved

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
		
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

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
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        //glm::mat4 model;
        //model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
        //model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			model = glm::translate(model, glm::vec3(-0.2f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			model = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f)); // translate it down so it's at the center of the scene
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f)); // translate it down so it's at the center of the scene
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.5f)); // translate it down so it's at the center of the scene
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.5f)); // translate it down so it's at the center of the scene
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {

			//model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.1f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene

			double step, delta, tempoDefinido = 5, tempoAtual, tempoInicial = glfwGetTime();
			do {

				tempoAtual = glfwGetTime();
				delta = tempoAtual - tempoInicial;
				step = (0.002 * delta) / tempoDefinido;
				model = glm::rotate(model, glm::radians(1.0f),glm::vec3( 0.1f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
				glClearColor(0.4f, 0.02f, 0.5f, 0.5f); // aplica uma única cor em toda tela GLFW
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpa os buffers
				ourShader.setMat4("model", model);
				ourModel.Draw(ourShader);
				glfwSwapBuffers(window); // troca de buffers para que somente o pronto seja exibido
				glfwPollEvents(); // verifica a ocorrência de interações do usuário com a janela

			} while (tempoAtual <= (tempoInicial + tempoDefinido));

		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
			model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 0.1f, 0.0f)); 

		/*	double step, delta, tempoDefinido = 5, tempoAtual, tempoInicial = glfwGetTime();
			do {

				tempoAtual = glfwGetTime();
				delta = tempoAtual - tempoInicial;
				step = (0.002 * delta) / tempoDefinido;
				model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 0.1f, 0.0f)); // translate it down so it's at the center of the scene
				glClearColor(0.4f, 0.02f, 0.5f, 0.5f); // aplica uma única cor em toda tela GLFW
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpa os buffers
				ourShader.setMat4("model", model);
				ourModel.Draw(ourShader);
				glfwSwapBuffers(window); // troca de buffers para que somente o pronto seja exibido
				glfwPollEvents(); // verifica a ocorrência de interações do usuário com a janela


			} while (tempoAtual <= (tempoInicial + tempoDefinido)); */
		}

		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
			model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 0.1f)); 

		/*	double step, delta, tempoDefinido = 5, tempoAtual, tempoInicial = glfwGetTime();
			do {

				tempoAtual = glfwGetTime();
				delta = tempoAtual - tempoInicial;
				step = (0.002 * delta) / tempoDefinido;
				model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 0.1f)); // translate it down so it's at the center of the scene
				glClearColor(0.4f, 0.02f, 0.5f, 0.5f); // aplica uma única cor em toda tela GLFW
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpa os buffers
				ourShader.setMat4("model", model);
				ourModel.Draw(ourShader);
				glfwSwapBuffers(window); // troca de buffers para que somente o pronto seja exibido
				glfwPollEvents(); // verifica a ocorrência de interações do usuário com a janela


			} while (tempoAtual <= (tempoInicial + tempoDefinido)); */

		}
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		}
		// difença entre vetor e camera
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
					
			glm::vec3 dotMod(1.0f, 0.0f, 0.0f); //cameraTarget
			glm::vec3 direction = glm::normalize(camera.Position - dotMod);
			std::cout << "Angulo diferenca camera e modelo: " << std::endl;
			std::cout << "X: " <<  direction.x <<" "<< "Y: " << direction.y << " " << "Z: " << direction.z<< std::endl;
		
		}
		//animacao
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {

			double step, delta, tempoDefinido = 5, tempoAtual, tempoInicial = glfwGetTime();

			do { 

				tempoAtual = glfwGetTime();
				delta = tempoAtual - tempoInicial;
				step = (0.002 * delta)/ tempoDefinido;
				model = glm::translate(model, glm::vec3(step, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
				glClearColor(0.4f, 0.02f, 0.5f, 0.5f); // aplica uma única cor em toda tela GLFW
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpa os buffers
				ourShader.setMat4("model", model);
				ourModel.Draw(ourShader);
				glfwSwapBuffers(window); // troca de buffers para que somente o pronto seja exibido
				glfwPollEvents(); // verifica a ocorrência de interações do usuário com a janela

			} while (tempoAtual <= (tempoInicial + tempoDefinido));
		
		}	
		// new
		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {

			if (mutex == 0) {
			
				mutex = 1;

				//matrixModel[numModel] = glm::translate(matrixModel[numModel], glm::vec3(baseX, -1.0f, 0.0f));
				baseX = baseX + 1.0;
				//matrixModel[numModel] = glm::scale(matrixModel[numModel], glm::vec3(0.1f, 0.1f, 0.1f));
				numModel++;
				std::cout << "mutex" << mutex << std::endl;

				std::cout << "numModel" << numModel << std::endl;
				std::cout << "baseX" << baseX << std::endl;

				mutex = 0;
			}
		}
		// exclui
		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {


		}

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		/*	int detect, gm;
			double i, t;
			float x[4] = {100,200,300,400}; // posições bezier x
			float y[4] = {400,300,200,100}; // posições bezier y

			for (t = 0.0; t < 1.0; t += 0.0005) {
				float xt = pow(1 - t, 3)*x[0] + 3 * t*pow(1 - t, 2)*x[1] + 3 * pow(t, 2)*(1 - t)*x[2] + pow(t, 3)*x[3];
				float yt = pow(1 - t, 3)*y[0] + 3 * t*pow(1 - t, 2)*y[1] + 3 * pow(t, 2)*(1 - t)*y[2] + pow(t, 3)*y[3];
				model = glm::translate(model, glm::vec3(xt, yt, 0.0f)); // translate it down so it's at the center of the scene
				glClearColor(0.4f, 0.02f, 0.5f, 0.5f); // aplica uma única cor em toda tela GLFW
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpa os buffers
				ourShader.setMat4("model", model);
				ourModel.Draw(ourShader);
				glfwSwapBuffers(window); // troca de buffers para que somente o pronto seja exibido
				glfwPollEvents(); // verifica a ocorrência de interações do usuário com a janela

			}
			*/

			//pego a posicão atual do modelo
			glm::vec3 posicao_atual(model[0], model[1], model[2]), bezier[3];
			//vetor axiliar
			glm::vec3 translacao(0, 0, 0);
			glm::vec3 novo_ponto(0, 0, 0);
			bezier[0] = posicao_atual;
			bezier[1][0] = 200;
			bezier[1][1] = 300;
			bezier[1][2] = 0;
			bezier[2][0] = 300;
			bezier[2][1] = 200;
			bezier[2][2] = 0;

			for (double i = 0; i < 1; i += 0.001) {
				glm::vec3 aux1, aux2, aux3;
				aux1 = pow(glm::vec3(1 - i), glm::vec3(2)) * bezier[0];
				aux2 = (glm::vec3(2 * i*(1 - i))) * bezier[1];
				aux3 = glm::vec3(pow(i, 2)) * bezier[2];
				novo_ponto = aux1 + aux2 + aux3;

				translacao = novo_ponto - posicao_atual;
				model = translate(model,translacao);
				posicao_atual += translacao;
				glClearColor(0.4f, 0.02f, 0.5f, 0.5f); // aplica uma única cor em toda tela GLFW
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpa os buffers
				ourShader.setMat4("model", model);
				ourModel.Draw(ourShader);
				glfwSwapBuffers(window); // troca de buffers para que somente o pronto seja exibido
				glfwPollEvents(); // verifica a ocorrência de interações do usuário com a janela

			}

			
		}
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {


		}

		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);


		// printar objetos
		//for (int i = 0; i < numModel; i++) {

			//ourShader.setMat4("model", matrixModel[i]);
			//ourModel.Draw(ourShader);

		//}

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

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
