#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb.cpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "camera.h"

#include <cmath>
#include <string>
#include <vector>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

void rotateX(float& x, float& y, float& z, float angle);
void rotateY(float& x, float& y, float& z, float angle);
void rotateZ(float& x, float& y, float& z, float angle);
void rotateCube(float vertices[], int numVertices, float angle, int choose_axis);
void make_is_transtioning(int isTransitioning[], int cube_positions_index[], int make_transitioning[]);

void change_cube_postions_index(int cube_positions_index_previous[], int cube_positions_index_next[], int cube_positions_index[], int indexes_to_change[]);



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

        glm::vec3( -1.0f,   5.0f,   1.0f),  // [19] CORNERS ON WHITE SIDE [UP]
        glm::vec3(  1.0f,   5.0f,   1.0f),  // [20]
        glm::vec3( -1.0f,   5.0f,  -1.0f),  // [21]
        glm::vec3(  1.0f,   5.0f,  -1.0f),  // [22]

        glm::vec3( -1.0f,  -5.0f,   1.0f),  // [23] CORNERS ON YELLOW SIDE [BOTTOM] 
        glm::vec3(  1.0f,  -5.0f,   1.0f),  // [24]
        glm::vec3( -1.0f,  -5.0f,  -1.0f),  // [25]
        glm::vec3(  1.0f,  -5.0f,  -1.0f),  // [26]
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
    
    // KEYBOARD
    int previousKeyState_u = GLFW_RELEASE;
    int previousKeyState_r = GLFW_RELEASE;

    //bool isTransitioning = false;
    int isTransitioning[27];
    for (int i = 0; i < 27; i++) isTransitioning[i] = 0;

    // ANIMATION
    float transitionDuration = 1.0f; // Duration of the transition in seconds
    float transitionProgress = 0.0f; // Progress of the transition (0.0 to 1.0)
    float rotationSpeed = 0.2f;

    // ROTATION
    int x=0, y=0, z=0;
    int u = 0, r = 0;
    float angle_f = 1.0f;

    glm::vec3 axis_of_rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    int redner_cubes_number = 26;
    redner_cubes_number++;

    // RENDER LOOP      -----------------------------------------------------------------------------------------------------------------------------
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


        // RENDERNING CUBES 
        for (unsigned int i = 0; i < redner_cubes_number; i++) {
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
            
            glBindVertexArray(VAO[i]);
            glm::mat4 model = glm::mat4(1.0f);
            if (i == 0) model = glm::scale(model, glm::vec3(0.1));

            // TRANSITION
            if (isTransitioning[i] == 1) {
                float angle = glm::radians(angle_f) * transitionProgress;
                if (x == 1) axis_of_rotation[0] = 1.0f;
                if (y == 1) axis_of_rotation[1] = 1.0f;
                if (z == 1) axis_of_rotation[2] = 1.0f;

                model = glm::rotate(model, angle, axis_of_rotation);
                axis_of_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
            } 
            
            // UPDATE TRANSITION PROGRESS
            if (isTransitioning[i] == 1) {
                transitionProgress += deltaTime * rotationSpeed;
                if (transitionProgress >= 1.0f) {
                    transitionProgress = 0.0f;
                    for (int k = 0; k < 27; k++) {
                        if (isTransitioning[k] == 1) {
                            if (x == 1) rotateCube(vertices[k], sizeof(vertices_main) / sizeof(float), angle_f,0);
                            if (y == 1) rotateCube(vertices[k], sizeof(vertices_main) / sizeof(float), angle_f,1);
                            if (z == 1) rotateCube(vertices[k], sizeof(vertices_main) / sizeof(float), angle_f,2);
                            isTransitioning[k] = 0;
                        }  
                    }
                    x = y = z = 0;   
                }
            }

            // SET CURRENT MODEL 
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // KEYBOARD STATE 
        int currentKeyState_u = glfwGetKey(window, GLFW_KEY_U);
        int currentKeyState_r = glfwGetKey(window, GLFW_KEY_R);

        if (transitionProgress == 0.0f){
            // MOVE U
            if (currentKeyState_u == GLFW_PRESS && previousKeyState_u == GLFW_RELEASE) {
                transitionProgress = 0.0f;
                angle_f = -90.0f;
                y = 1;
                int make_transitioning[] = { 1, 7, 8, 9, 10 };
                make_is_transtioning(isTransitioning, cube_positions_index, make_transitioning);
                int indexes_to_change[] = { 7, 10, 8, 9 };
                change_cube_postions_index(cube_positions_index_previous, cube_positions_index_next,
                    cube_positions_index, indexes_to_change);
            }

            // MOVE R
            if (currentKeyState_r == GLFW_PRESS && previousKeyState_r == GLFW_RELEASE) {
                transitionProgress = 0.0f;
                angle_f = -90.0f;
                x = 1;
                int make_transitioning[] = { 5, 9, 18, 13, 16 };
                make_is_transtioning(isTransitioning, cube_positions_index, make_transitioning);
                int indexes_to_change[] = { 18, 13, 16, 9 };
                change_cube_postions_index(cube_positions_index_previous, cube_positions_index_next,
                    cube_positions_index, indexes_to_change);
            }
        }

        // KEYBOARD STATE
        previousKeyState_u = currentKeyState_u;
        previousKeyState_r = currentKeyState_r;

        // SWAP BUFFERS
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // DELETE BUFFERS
    for (int i = 0; i < 27; i++) {
        glDeleteVertexArrays(1, &axisVAO);
        glDeleteBuffers(1, &axisVBO);
        glDeleteVertexArrays(1, &VAO[i]);
        glDeleteBuffers(1, &VBO[i]);
    }
    glfwTerminate();
    return 0;
}
// -------------------------------------------------------------------------------------------------------------------------------

// START ANIMATION
void make_is_transtioning(int isTransitioning[], int cube_positions_index[], int make_transitioning[]) {
    for (int i = 0; i < 5; i++) isTransitioning[cube_positions_index[make_transitioning[i]]] = 1;
} 

void change_cube_postions_index(int cube_positions_index_previous[], int cube_positions_index_next[], int cube_positions_index[], int indexes_to_change[]){
    cube_positions_index_previous[indexes_to_change[0]] = cube_positions_index[indexes_to_change[0]];
    cube_positions_index_previous[indexes_to_change[1]] = cube_positions_index[indexes_to_change[1]];
    cube_positions_index_previous[indexes_to_change[2]] = cube_positions_index[indexes_to_change[2]];
    cube_positions_index_previous[indexes_to_change[3]] = cube_positions_index[indexes_to_change[3]];

    cube_positions_index_next[indexes_to_change[0]] = cube_positions_index_previous[indexes_to_change[3]];
    cube_positions_index_next[indexes_to_change[1]] = cube_positions_index_previous[indexes_to_change[0]];
    cube_positions_index_next[indexes_to_change[2]] = cube_positions_index_previous[indexes_to_change[1]];
    cube_positions_index_next[indexes_to_change[3]] = cube_positions_index_previous[indexes_to_change[2]];

    cube_positions_index[indexes_to_change[0]] = cube_positions_index_next[indexes_to_change[0]];
    cube_positions_index[indexes_to_change[1]] = cube_positions_index_next[indexes_to_change[1]];
    cube_positions_index[indexes_to_change[2]] = cube_positions_index_next[indexes_to_change[2]];
    cube_positions_index[indexes_to_change[3]] = cube_positions_index_next[indexes_to_change[3]];
}

// ROTATION
void rotateCube(float vertices[], int numVertices, float angle, int choose_axis) {
    for (int i = 0; i < numVertices; i += 8) {
        float x = vertices[i];
        float y = vertices[i + 1];
        float z = vertices[i + 2];

        if (choose_axis == 0) rotateX(x, y, z, angle);
        if (choose_axis == 1) rotateY(x, y, z, angle);
        if (choose_axis == 2) rotateZ(x, y, z, angle);

        vertices[i] = x;
        vertices[i + 1] = y;
        vertices[i + 2] = z;
    }
}

void rotateX(float& x, float& y, float& z, float angle) {
    float radians = glm::radians(angle);
    float cosTheta = std::cos(radians);
    float sinTheta = std::sin(radians);

    float newY = cosTheta * y - sinTheta * z;
    float newZ = sinTheta * y + cosTheta * z;

    y = newY;
    z = newZ;
}

void rotateY(float& x, float& y, float& z, float angle) {
    float radians = glm::radians(angle);
    float cosTheta = std::cos(radians);
    float sinTheta = std::sin(radians);

    float newX = cosTheta * x + sinTheta * z;
    float newZ = -sinTheta * x + cosTheta * z;

    x = newX;
    z = newZ;
}

void rotateZ(float& x, float& y, float& z, float angle) {
    float radians = glm::radians(angle);
    float cosTheta = std::cos(radians);
    float sinTheta = std::sin(radians);

    float newX = cosTheta * x - sinTheta * y;
    float newY = sinTheta * x + cosTheta * y;

    x = newX;
    y = newY;
}

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
        camera.ProcessKeyboard(UP, deltaTime);
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