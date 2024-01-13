#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb.cpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "camera.h"
#include "functions.h"
#include "movements.h"

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
Camera camera(glm::vec3(3.0f, 2.9f, 4.7f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float speed_1 = 0.4f;
float speed_2 = 0.2f;


int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
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

    // SET CUBES TRANSLATIONS
    glm::vec3 cube_positions_translation[] = {
        glm::vec3(  0.0f,   0.0f,   0.0f),  // [0] CENTER OF THE CUBE
        glm::vec3(  0.0f,   1.0f,   0.0f),  // [1] WHITE CENTER [UP]
        glm::vec3(  0.0f,  -1.0f,   0.0f),  // [2] YELLOW CENTER [BOTTOM]
        glm::vec3(  0.0f,   0.0f,   1.0f),  // [3] RED CENTER [FRONT]
        glm::vec3(  0.0f,   0.0f,  -1.0f),  // [4] ORANGE CENTER [BACK]
        glm::vec3(  1.0f,   0.0f,   0.0f),  // [5] BLUE CENTER [RIGHT]
        glm::vec3( -1.0f,   0.0f,   0.0f),  // [6] GREEN CENTER [LEFT]

        glm::vec3(  0.0f,   1.0f,   1.0f),  // [7] EDGES ON WHITE SIDE [UP] 
        glm::vec3(  0.0f,   1.0f,  -1.0f),  // [8]
        glm::vec3(  1.0f,   1.0f,   0.0f),  // [9]
        glm::vec3( -1.0f,   1.0f,   0.0f),  // [10]

        glm::vec3(  0.0f,  -1.0f,   1.0f),  // [11] EDGES ON YELLOW SIDE [BOTTOM] 
        glm::vec3(  0.0f,  -1.0f,  -1.0f),  // [12]
        glm::vec3(  1.0f,  -1.0f,   0.0f),  // [13]
        glm::vec3( -1.0f,  -1.0f,   0.0f),  // [14]

        glm::vec3( -1.0f,   0.0f,   1.0f),  // [15] EDGES VERTICAL 
        glm::vec3(  1.0f,   0.0f,   1.0f),  // [16]
        glm::vec3( -1.0f,   0.0f,  -1.0f),  // [17]
        glm::vec3(  1.0f,   0.0f,  -1.0f),  // [18]

        glm::vec3( -1.0f,   1.0f,   1.0f),  // [19] CORNERS ON WHITE SIDE [UP]
        glm::vec3(  1.0f,   1.0f,   1.0f),  // [20]
        glm::vec3( -1.0f,   1.0f,  -1.0f),  // [21]
        glm::vec3(  1.0f,   1.0f,  -1.0f),  // [22]

        glm::vec3( -1.0f,  -1.0f,   1.0f),  // [23] CORNERS ON YELLOW SIDE [BOTTOM] 
        glm::vec3(  1.0f,  -1.0f,   1.0f),  // [24]
        glm::vec3( -1.0f,  -1.0f,  -1.0f),  // [25]
        glm::vec3(  1.0f,  -1.0f,  -1.0f),  // [26]
    };

    // CREATE 27 COPIES OF CUBES 
    int cube_positions_index[27], cube_positions_index_previous[27], cube_positions_index_next[27];
    const int numCopies = 27;
    const int vertices_main_size = sizeof(vertices_main) / sizeof(vertices_main[0]);
    float vertices[numCopies][vertices_main_size];
    float vertices_previous[numCopies][vertices_main_size];
    float vertices_next[numCopies][vertices_main_size];
    for (int i = 0; i < 27; ++i) {
        for (int j = 0; j < vertices_main_size; ++j) {
            vertices[i][j] = vertices_main[j];
            vertices_previous[i][j] = vertices_main[j];
            vertices_next[i][j] = vertices_main[j];
        }
        cube_positions_index[i] = i;
        cube_positions_index_previous[i] = i;
        cube_positions_index_next[i] = i;
    }

    // MAKE WALLS BLACK
    std::vector<int> walls_1 = { 1, 7,  8,  9,  10, 19, 20, 21, 22 };  // UP [WHITE]
    std::vector<int> walls_2 = { 2, 11, 12, 13, 14, 23, 24, 25, 26 };  // BOTTOM [YELLOW]
    std::vector<int> walls_3 = { 3, 7,  11, 15, 16, 19, 20, 23, 24 };  // FRONT [RED]
    std::vector<int> walls_4 = { 4, 8,  12, 17, 18, 21, 22, 25, 26 };  // BACK [ORANGE]
    std::vector<int> walls_5 = { 5, 9,  13, 16, 18, 20, 22, 24, 26 };  // RIGHT [BLUE]
    std::vector<int> walls_6 = { 6, 10, 14, 15, 17, 19, 21, 23, 25 };  // LEFT [GREEN]
    for (int j = 1; j < 27; j++) {
        if (std::find(walls_1.begin(), walls_1.end(), j) == walls_1.end()) {
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

    // MOVING CUBES 
    glm::vec3  vertices_cubeposition[27], vertices_cubeposition_previous[27], vertices_cubeposition_next[27];
    for (int i = 0; i < 27; i++) {
        float vertices_translated[72];  
        for (int j = 0; j < 288; j += 8) {
                glm::vec3 originalVertex(vertices_main[j], vertices_main[j+1], vertices_main[j+2]);
                vertices_cubeposition[i] = originalVertex + cube_positions_translation[i];
                vertices[i][j+0] = vertices_cubeposition[i].x;
                vertices[i][j+1] = vertices_cubeposition[i].y;
                vertices[i][j+2] = vertices_cubeposition[i].z;   
        }
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

    //bool isTransitioning = false;
    int isTransitioning[27];
    for (int i = 0; i < 27; i++) isTransitioning[i] = 0;

    // ANIMATION
    float transitionDuration = 1.0f; 
    float transitionProgress = 0.0f; 
    float rotationSpeed = 0.2f;
    float angle_f = 1.0f;
    int look_position = 0;
    int redner_cubes_number = 26;
    int number_of_elements = 0;
    int axis_of_rotation = 0;
    redner_cubes_number++;

    glm::vec3 axis_of_rotation_vec = glm::vec3(0.0f, 0.0f, 0.0f);

    // KEYBOARD
    int previousKeyState_Space = GLFW_RELEASE;
    int previousKeyState_W = GLFW_RELEASE;
    int previousKeyState_A = GLFW_RELEASE;
    int previousKeyState_S = GLFW_RELEASE;
    int previousKeyState_D = GLFW_RELEASE;

    int previousKeyState_1 = GLFW_RELEASE;
    int previousKeyState_2 = GLFW_RELEASE;
    int previousKeyState_3 = GLFW_RELEASE;
    int previousKeyState_4 = GLFW_RELEASE;
    int previousKeyState_5 = GLFW_RELEASE;
    int previousKeyState_6 = GLFW_RELEASE;

    int previousKeyState_RIGHT = GLFW_RELEASE;
    int previousKeyState_UP    = GLFW_RELEASE;
    int previousKeyState_DOWN  = GLFW_RELEASE;
    int previousKeyState_LEFT  = GLFW_RELEASE;

    // CAMERA POSITION
    camera.Yaw -= 30.0f;
    camera.Pitch-= 30.0f;
    camera.updateCameraVectors();

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

    // RENDER LOOP      ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window)) {
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
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);
        //camera.DisplayPosition();

        // RENDERNING CUBES 
        for (unsigned int i = 0; i < redner_cubes_number; i++) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices[i]), vertices[i]);
                        
            glBindVertexArray(VAO[i]);
            glm::mat4 model = glm::mat4(1.0f);
            if (i == 0) model = glm::scale(model, glm::vec3(0.1));

            // TRANSITION
            if (isTransitioning[i] == 1) {
                float angle = glm::radians(angle_f) * transitionProgress;
                if (axis_of_rotation != 0) axis_of_rotation_vec[axis_of_rotation-1] = 1.0f;

                model = glm::rotate(model, angle, axis_of_rotation_vec);
                axis_of_rotation_vec = glm::vec3(0.0f, 0.0f, 0.0f);
            } 
            
            // UPDATE TRANSITION PROGRESS
            if (isTransitioning[i] == 1) {
                transitionProgress += deltaTime * rotationSpeed;
                if (transitionProgress >= 1.0f) {
                    transitionProgress = 0.0f;
                    for (int k = 0; k < 27; k++) {
                        // END OF ANIMATION
                        if (isTransitioning[k] == 1) {
                            if (axis_of_rotation != 0) rotateCube(vertices[k], sizeof(vertices_main) / sizeof(float), angle_f, axis_of_rotation);
                            isTransitioning[k] = 0;
                        }  
                    }
                    axis_of_rotation = 0;
                }
            }

            // SET CURRENT MODEL 
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // KEYBOARD STATE 
        int currentKeyState_Space = glfwGetKey(window, GLFW_KEY_SPACE);
        int currentKeyState_W = glfwGetKey(window, GLFW_KEY_W);
        int currentKeyState_A = glfwGetKey(window, GLFW_KEY_A);
        int currentKeyState_S = glfwGetKey(window, GLFW_KEY_S);
        int currentKeyState_D = glfwGetKey(window, GLFW_KEY_D);

        int currentKeyState_1 = glfwGetKey(window, GLFW_KEY_1);
        int currentKeyState_2 = glfwGetKey(window, GLFW_KEY_2);
        int currentKeyState_3 = glfwGetKey(window, GLFW_KEY_3);
        int currentKeyState_4 = glfwGetKey(window, GLFW_KEY_4);
        int currentKeyState_5 = glfwGetKey(window, GLFW_KEY_5);
        int currentKeyState_6 = glfwGetKey(window, GLFW_KEY_6);

        int currentKeyState_RIGHT = glfwGetKey(window, GLFW_KEY_RIGHT);
        int currentKeyState_UP    = glfwGetKey(window, GLFW_KEY_UP);
        int currentKeyState_DOWN  = glfwGetKey(window, GLFW_KEY_DOWN);
        int currentKeyState_LEFT  = glfwGetKey(window, GLFW_KEY_LEFT);

        // KEYBOARD MOVEMENTS 
        if (transitionProgress == 0.0f){
            if (currentKeyState_1 == GLFW_PRESS && previousKeyState_1 == GLFW_RELEASE) movement_U (1, 1, 1, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
            if (currentKeyState_2 == GLFW_PRESS && previousKeyState_2 == GLFW_RELEASE) movement_D (1, 1, 1, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
            if (currentKeyState_3 == GLFW_PRESS && previousKeyState_3 == GLFW_RELEASE) movement_F (1, 1, 1, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
            if (currentKeyState_4 == GLFW_PRESS && previousKeyState_4 == GLFW_RELEASE) movement_B (1, 1, 1, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
            if (currentKeyState_5 == GLFW_PRESS && previousKeyState_5 == GLFW_RELEASE) movement_R (1, 1, 1, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
            if (currentKeyState_6 == GLFW_PRESS && previousKeyState_6 == GLFW_RELEASE) movement_L (1, 1, 1, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);

            if (currentKeyState_RIGHT == GLFW_PRESS && previousKeyState_RIGHT == GLFW_RELEASE) movement_Y(1, 1, 1, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
            if (currentKeyState_LEFT  == GLFW_PRESS && previousKeyState_LEFT  == GLFW_RELEASE) movement_Y(1, 1,-1, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
            if (currentKeyState_UP    == GLFW_PRESS && previousKeyState_UP    == GLFW_RELEASE) movement_X(1, 1, 1, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
            if (currentKeyState_DOWN  == GLFW_PRESS && previousKeyState_DOWN  == GLFW_RELEASE) movement_X(1, 1,-1, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
        }

        // SPACE LOOK
        if (currentKeyState_Space == GLFW_PRESS && previousKeyState_Space == GLFW_RELEASE && look_position == 0) {
            camera.Position.x -= 6.0f;
            camera.Yaw += 60.0f;
            camera.updateCameraVectors();    
            look_position = 1;
            previousKeyState_Space = currentKeyState_Space;
        }
        if (currentKeyState_Space == GLFW_PRESS && previousKeyState_Space == GLFW_RELEASE && look_position == 1) {
            camera.Position.x += 6.0f;
            camera.Yaw -= 60.0f;
            camera.updateCameraVectors();
            look_position = 0;
            previousKeyState_Space = currentKeyState_Space;
        }

        // KEYBOARD STATE
        previousKeyState_Space = currentKeyState_Space;
        previousKeyState_W = currentKeyState_W;
        previousKeyState_A = currentKeyState_A;
        previousKeyState_S = currentKeyState_S;
        previousKeyState_D = currentKeyState_D;

        previousKeyState_1 = currentKeyState_1;
        previousKeyState_2 = currentKeyState_2;
        previousKeyState_3 = currentKeyState_3;
        previousKeyState_4 = currentKeyState_4;
        previousKeyState_5 = currentKeyState_5;
        previousKeyState_6 = currentKeyState_6;

        previousKeyState_RIGHT = currentKeyState_RIGHT;
        previousKeyState_LEFT  = currentKeyState_LEFT;
        previousKeyState_UP    = currentKeyState_UP;
        previousKeyState_DOWN  = currentKeyState_DOWN;

        // SWAP BUFFERS
        glfwSwapBuffers(window);
        glfwPollEvents();

        for (int i = 0; i < 27; i++) {
            //glDeleteVertexArrays(1, &VAO[i]);
            //glDeleteBuffers(1, &VBO[i]);
        }
    }
    // DELETE BUFFERS
    glDeleteVertexArrays(1, &axisVAO);
    glDeleteBuffers(1, &axisVBO);
    glfwTerminate();
    return 0;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// START ANIMATION




// CAMERA MOVEMENT
void processInput(GLFWwindow* window) {
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
    //    camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

// FRAME BUFFER
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// MOUSE MOVEMENT
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) {
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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}