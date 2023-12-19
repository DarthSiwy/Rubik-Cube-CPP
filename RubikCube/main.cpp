#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb.cpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "camera.h"
//#include "Cube.h"

#include <cmath>
#include <string>
#include <vector>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    Shader ourShader("vertex_shader", "fragment_shader");

    // AXIS 
    float axisVertices[] = {
        //   POSITION             COLOR 
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   // X-axis
        5.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    
        0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   // Y-axis
        0.0f, 5.0f, 0.0f,   0.0f, 1.0f, 0.0f,
                                          
        0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   // Z-axis
        0.0f, 0.0f, 5.0f,   0.0f, 0.0f, 1.0f
    };
    unsigned int axisVBO, axisVAO;
    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);
    glBindVertexArray(axisVAO);

    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);



    // CUBE
    float vertices_main[] = {
        //     POSITION          TEXTURE             COLOR
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     1.0f, 1.0f, 1.0f,   // UP
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,     1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     1.0f, 1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     1.0f, 1.0f, 0.0f,   // BOTTOM
         0.5f, -0.5f, -0.5f,    1.0f, 1.0f,     1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     1.0f, 0.0f, 0.0f,   // FRONT
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,     1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     1.0f, 0.5f, 0.0f,   // BACK 
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     1.0f, 0.5f, 0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     1.0f, 0.5f, 0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     1.0f, 0.5f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     1.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     1.0f, 0.5f, 0.0f,

         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f, 0.0f, 1.0f,   // RIGHT
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f, 1.0f, 0.0f,   // LEFT
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f, 1.0f, 0.0f  
    };

    // CREATE 27 COPIES OF CUBES 
    int cube_positions[27], cube_positions_previous[27], cube_positions_next[27];
    const int numCopies = 27;
    const int vertices_main_size = sizeof(vertices_main) / sizeof(vertices_main[0]);
    float vertices[numCopies][vertices_main_size];
    for (int i = 0; i < numCopies; ++i) {
        for (int j = 0; j < vertices_main_size; ++j) {
            vertices[i][j] = vertices_main[j];
        }
        cube_positions[i] = i;
        cube_positions_previous[i] = i;
        cube_positions_next[i] = i;
    } 

    // MAKE WALLS BLACK
    std::vector<int> walls_1 = { 1, 7,  8,  9,  10, 19, 20, 21, 22 };  // UP [WHITE]
    std::vector<int> walls_2 = { 2, 11, 12, 13, 14, 23, 24, 25, 26 };  // BOTTOM [YELLOW]
    std::vector<int> walls_3 = { 3, 7,  11, 15, 16, 19, 20, 23, 24 };  // FRONT [RED]
    std::vector<int> walls_4 = { 4, 8,  12, 17, 18, 21, 22, 25, 26 };  // BACK [ORANGE]
    std::vector<int> walls_5 = { 5, 9,  13, 16, 18, 20, 22, 24, 26 };  // RIGHT [BLUE]
    std::vector<int> walls_6 = { 6, 10, 14, 15, 17, 19, 21, 23, 25 };  // LEFT [GREEN]
    for (int j = 1; j < 27; j++){
        if (std::find(walls_1.begin(), walls_1.end(), j) == walls_1.end()){
            for (int i = 5; i < 48; i += 8) {       // UP       
                vertices[j][i] = 0.0f;
                vertices[j][i + 1] = 0.0f;
                vertices[j][i + 2] = 0.0f;
            }
        }
        if (std::find(walls_2.begin(), walls_2.end(), j) == walls_2.end()) {
            for (int i = 53; i < 96; i += 8) {      // BOTTOM
                vertices[j][i] = 0.0f;
                vertices[j][i + 1] = 0.0f;
                vertices[j][i + 2] = 0.0f;
            }
        }
        if (std::find(walls_3.begin(), walls_3.end(), j) == walls_3.end()) {
            for (int i = 101; i < 144; i += 8) {      // FRONT
                vertices[j][i] = 0.0f;
                vertices[j][i + 1] = 0.0f;
                vertices[j][i + 2] = 0.0f;
            }
        }
        if (std::find(walls_4.begin(), walls_4.end(), j) == walls_4.end()) {
            for (int i = 149; i < 192; i += 8) {      // BACK 
                vertices[j][i] = 0.0f;
                vertices[j][i + 1] = 0.0f;
                vertices[j][i + 2] = 0.0f;
            }
        }
        if (std::find(walls_5.begin(), walls_5.end(), j) == walls_5.end()) {
            for (int i = 197; i < 240; i += 8) {      // RIGHT 
                vertices[j][i] = 0.0f;
                vertices[j][i + 1] = 0.0f;
                vertices[j][i + 2] = 0.0f;
            }
        }
        if (std::find(walls_6.begin(), walls_6.end(), j) == walls_6.end()) {
            for (int i = 245; i < 288; i += 8) {      // LEFT
                vertices[j][i] = 0.0f;
                vertices[j][i + 1] = 0.0f;
                vertices[j][i + 2] = 0.0f;
            }
        }
    }
   
    // CREATE 27 VERTEXES
    unsigned int VBO[27], VAO[27];
    for (int i = 0; i < 27; i++) {
        glGenVertexArrays(1, &VAO[i]);
        glGenBuffers(1, &VBO[i]);

        glBindVertexArray(VAO[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attributes 
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture color attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    // SET CUBES POSITIONS
    glm::vec3 cubePositions[] = {
        glm::vec3(  0.0f,   0.0f,   0.0f),  // [0] CENTER OF THE CUBE
        glm::vec3(  0.0f,   1.1f,   0.0f),  // [1] WHITE CENTER [UP]
        glm::vec3(  0.0f,  -1.1f,   0.0f),  // [2] YELLOW CENTER [BOTTOM]
        glm::vec3(  0.0f,   0.0f,   1.1f),  // [3] RED CENTER [FRONT]
        glm::vec3(  0.0f,   0.0f,  -1.1f),  // [4] ORANGE CENTER [BACK]
        glm::vec3(  1.1f,   0.0f,   0.0f),  // [5] BLUE CENTER [RIGHT]
        glm::vec3( -1.1f,   0.0f,   0.0f),  // [6] GREEN CENTER [LEFT]
  
        glm::vec3( 0.0f,   1.1f,   1.1f),   // [7] EDGES ON WHITE SIDE [UP] 
        glm::vec3( 0.0f,   1.1f,  -1.1f),   // [8]
        glm::vec3( 1.1f,   1.1f,   0.0f),   // [9]
        glm::vec3(-1.1f,   1.1f,   0.0f),   // [10]

        glm::vec3( 0.0f,  -1.1f,   1.1f),   // [11] EDGES ON YELLOW SIDE [BOTTOM] 
        glm::vec3( 0.0f,  -1.1f,  -1.1f),   // [12]
        glm::vec3( 1.1f,  -1.1f,   0.0f),   // [13]
        glm::vec3(-1.1f,  -1.1f,   0.0f),   // [14]

        glm::vec3(-1.1f,   0.0f,   1.1f),   // [15] EDGES VERTICAL 
        glm::vec3( 1.1f,   0.0f,   1.1f),   // [16]
        glm::vec3(-1.1f,   0.0f,  -1.1f),   // [17]
        glm::vec3( 1.1f,   0.0f,  -1.1f),   // [18]

        glm::vec3(-1.1f,   1.1f,   1.1f),   // [19] CORNERS ON WHITE SIDE [UP]
        glm::vec3( 1.1f,   1.1f,   1.1f),   // [20]
        glm::vec3(-1.1f,   1.1f,  -1.1f),   // [21]
        glm::vec3( 1.1f,   1.1f,  -1.1f),   // [22]

        glm::vec3(-1.1f,  -1.1f,   1.1f),   // [23] CORNERS ON YELLOW SIDE [BOTTOM] 
        glm::vec3( 1.1f,  -1.1f,   1.1f),   // [24]
        glm::vec3(-1.1f,  -1.1f,  -1.1f),   // [25]
        glm::vec3( 1.1f,  -1.1f,  -1.1f),   // [26]
    };

    glm::vec3 cubePositions_previous[27], cubePositions_next[27];
    glm::vec3 cubeRotate[27];
    for (int i = 0; i < 27; i++){
        cubePositions_previous[i] = cubePositions[i];
        cubePositions_next[i] = cubePositions[i];
        cubeRotate[i] = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    // TEXTURES
    unsigned int texture1, texture2;
    unsigned char* data;
    int width, height, nrChannels;
    
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    data = stbi_load("textures/rectangle.png", &width, &height, &nrChannels, 4);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("textures/wall.jpg", &width, &height, &nrChannels, 4);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    int redner_cubes_number = 7;
    float angle[27], angle_previous[27], angle_next[27];
    for (int i = 0; i < 27; i++){
        angle[i] = 1;
        angle_previous[i] = 1;
        angle_next[i] = 1;
    }
    redner_cubes_number++;
    
   
    int previousKeyState_u = GLFW_RELEASE;
   

    // Cube transition variables
    //bool isTransitioning = false;

    int isTransitioning[27];
    for (int i = 0; i < 27; i++) isTransitioning[i] = 0;

    float transitionDuration = 1.0f; // Duration of the transition in seconds
    float transitionProgress = 0.0f; // Progress of the transition (0.0 to 1.0)
    float rotationSpeed = 1.0f; 

    // RENDER LOOP
    while (!glfwWindowShouldClose(window)){
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        ourShader.use();

        // Background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the axis lines
        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model", model);
        glBindVertexArray(axisVAO);
        glDrawArrays(GL_LINES, 0, 6);

        // Activate Textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // Camera position default
        glm::mat4 projection =  glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);
       



        for (unsigned int i = 0; i < redner_cubes_number; i++) {
            glBindVertexArray(VAO[i]);
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 model_1 = glm::mat4(1.0f);
            if (i == 0) model = glm::scale(model, glm::vec3(0.3));
            

            if (isTransitioning[i]==1) {
                float angle = glm::radians(-90.f) * transitionProgress;


                //auto pos = cubePositions_previous[i];
                
                model = glm::translate(model, cubePositions_previous[i]);
                model_1 = glm::translate(model_1, cubePositions[1]);

                //model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f)); // kazda kostka musi miec swoje glm rotate, 
                model = glm::translate(model, glm::vec3(1.0f*sin(angle) * sqrt(2), 0.0f, 1.0f * cos(angle) * sqrt(2)));
                // Interpolate rotation for a smooth transition
                
                //model = glm::translate(model, cubePositions_previous[1] - cubePositions_previous[i]);


                
                
            }
            else {
                //model = glm::rotate(model, glm::radians(360.f), glm::vec3(0.0f, 1.0f, 0.0f));
                //float angle = glm::radians(-90.f) * transitionProgress;
                //model = glm::rotate(model, angle, glm::vec3(0.0f, 2.0f, 0.0f));
                model = glm::translate(model, cubePositions[i]);
            }

            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);


            // Update transition progress
            if (isTransitioning[i] == 1) {
                transitionProgress += deltaTime * rotationSpeed;
                if (transitionProgress >= 1.0f) {
                    //isTransitioning = 0;
                    transitionProgress = 0.0f;

                    isTransitioning[7] = 0;
                    isTransitioning[8] = 0;
                    isTransitioning[9] = 0;
                    isTransitioning[10] = 0;
                }
            }
        }


        




        int currentKeyState_u = glfwGetKey(window, GLFW_KEY_U);

        // MOVE U
        if (currentKeyState_u == GLFW_PRESS && previousKeyState_u == GLFW_RELEASE) {
            
            cubePositions_previous[7] = cubePositions[7];
            cubePositions_previous[8] = cubePositions[8];
            cubePositions_previous[9] = cubePositions[9];
            cubePositions_previous[10] = cubePositions[10];

            cubePositions_next[9] = cubePositions_previous[7];
            cubePositions_next[7] = cubePositions_previous[10];
            cubePositions_next[10] = cubePositions_previous[8];
            cubePositions_next[8] = cubePositions_previous[9];

            cubePositions[7] = cubePositions_next[7];
            cubePositions[8] = cubePositions_next[8];
            cubePositions[9] = cubePositions_next[9];
            cubePositions[10] = cubePositions_next[10];

            isTransitioning[7] = 1;
            isTransitioning[8] = 1;
            isTransitioning[9] = 1;
            isTransitioning[10] = 1;

            transitionProgress = 0.0f;

        }
        previousKeyState_u = currentKeyState_u;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // DELETE BUFFERS
    for (int i = 0; i < 27; i++){
        glDeleteVertexArrays(1, &axisVAO);
        glDeleteBuffers(1, &axisVBO);
        glDeleteVertexArrays(1, &VAO[i]);
        glDeleteBuffers(1, &VBO[i]);
    }
    glfwTerminate();
    return 0;
}


// CAMERA MOVEMENT
void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);  
}

// FRAME BUFFER
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

// MOUSE MOVEMENT
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// SCROOL MOUSE ZOOM
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}