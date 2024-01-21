#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb.cpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include <ft2build.h>

#include "camera.h"
#include "rotations.h"
#include "movements.h"
#include "cubes.h"
#include "stickers.h"

#include <map>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>

#include FT_FREETYPE_H

extern const unsigned int SCR_WIDTH = 1920;
extern const unsigned int SCR_HEIGHT = 1080;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);


// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
unsigned int text_VAO, text_VBO;

// SETTINGS
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    Shader mainShader("vertex_shader", "fragment_shader");
    Shader textShader("text_vertex_shader", "text_fragment_shader");



    // TEXT
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    textShader.use();
    glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft)){
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }
    // find path to font
    std::string font_name = ("resources/fonts/font_1.ttf");
    if (font_name.empty()){
        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        return -1;
    }
    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);
        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++){
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)){
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    glGenVertexArrays(1, &text_VAO);
    glGenBuffers(1, &text_VBO);
    glBindVertexArray(text_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // AXIS 
    float axisVertices[36];
    make_axis(axisVertices);
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

    // CUBES 
    float vertices_main[288];
    int cube_positions_index[27], cube_positions_index_previous[27], cube_positions_index_next[27];
    const int numCopies = 27, vertices_main_size = 288;
    float vertices[numCopies][vertices_main_size];
    
    make_vertices(vertices_main);
    cube_indexes(cube_positions_index, cube_positions_index_previous, cube_positions_index_next, vertices, vertices_main);
    move_cubes(vertices, vertices_main);
    make_walls_black(vertices); 

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
    data = stbi_load("resources/textures/rectangle.png", &width, &height, &nrChannels, 4);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else std::cout << "Failed to load texture" << std::endl;
    stbi_image_free(data);
    mainShader.use();
    mainShader.setInt("texture1", 0);

    // -----------------------------------------------------------------------------------------

    // VARIABLES 
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
    int S = 1;
    int current_speed = 1;

    glm::vec3 axis_of_rotation_vec = glm::vec3(0.0f, 0.0f, 0.0f);

    // KEYBOARD
    int previousKeyState_W = GLFW_RELEASE;
    int previousKeyState_A = GLFW_RELEASE;
    int previousKeyState_S = GLFW_RELEASE;
    int previousKeyState_D = GLFW_RELEASE;
    int previousKeyState_R = GLFW_RELEASE;
    int previousKeyState_F = GLFW_RELEASE;

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

    int previousKeyState_SHIFT = GLFW_RELEASE;
    int previousKeyState_Space = GLFW_RELEASE;

    // CAMERA POSITION
    camera.Yaw -= 30.0f;
    camera.Pitch-= 30.0f;
    camera.updateCameraVectors();


    // STICKERS 
    int stickers[54];
    for (int i = 0;  i < 9;  i++) stickers[i] = 1;
    for (int i = 9;  i < 18; i++) stickers[i] = 2;
    for (int i = 18; i < 27; i++) stickers[i] = 3;
    for (int i = 27; i < 36; i++) stickers[i] = 4;
    for (int i = 36; i < 45; i++) stickers[i] = 5;
    for (int i = 45; i < 54; i++) stickers[i] = 6;

    stickers[2] = 7;
    stickers[3] = 8;

    print_stickers(stickers);



    // RENDER LOOP      ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // RENDER LOOP      ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // RENDER LOOP      ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window)) {
        // Time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        mainShader.use();

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        // Background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the axis lines
        glm::mat4 model = glm::mat4(1.0f);
        mainShader.setMat4("model", model);
        glBindVertexArray(axisVAO);
        glDrawArrays(GL_LINES, 0, 6);

        // Activate Textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // Camera position default
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        mainShader.setMat4("projection", projection);
        glm::mat4 view = camera.GetViewMatrix();
        mainShader.setMat4("view", view);
        //camera.DisplayPosition();
        int move = 0;

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
            mainShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // TEXT PRINTING
        RenderText(textShader, "Current speed: " + std::to_string(current_speed), 25.0f, 1000.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));


        // KEYBOARD STATE 
        int currentKeyState_W = glfwGetKey(window, GLFW_KEY_W);
        int currentKeyState_A = glfwGetKey(window, GLFW_KEY_A);
        int currentKeyState_S = glfwGetKey(window, GLFW_KEY_S);
        int currentKeyState_D = glfwGetKey(window, GLFW_KEY_D);
        int currentKeyState_R = glfwGetKey(window, GLFW_KEY_R);
        int currentKeyState_F = glfwGetKey(window, GLFW_KEY_F);

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

        int currentKeyState_Space = glfwGetKey(window, GLFW_KEY_SPACE);
        int currentKeyState_SHIFT = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);

        // KEYBOARD MOVEMENTS 
        S = 1;
        if (transitionProgress == 0.0f){
            if (currentKeyState_SHIFT == GLFW_PRESS ) S = -1;
            if (currentKeyState_W == GLFW_PRESS && previousKeyState_W == GLFW_RELEASE) move = 1;
            if (currentKeyState_F == GLFW_PRESS && previousKeyState_F == GLFW_RELEASE) move = 2;
            if (currentKeyState_S == GLFW_PRESS && previousKeyState_S == GLFW_RELEASE) move = 3;
            if (currentKeyState_R == GLFW_PRESS && previousKeyState_R == GLFW_RELEASE) move = 4;
            if (currentKeyState_D == GLFW_PRESS && previousKeyState_D == GLFW_RELEASE) move = 5;
            if (currentKeyState_A == GLFW_PRESS && previousKeyState_A == GLFW_RELEASE) move = 6;

            if (currentKeyState_RIGHT == GLFW_PRESS && previousKeyState_RIGHT == GLFW_RELEASE) move = 7;
            if (currentKeyState_LEFT  == GLFW_PRESS && previousKeyState_LEFT  == GLFW_RELEASE) move = 8;
            if (currentKeyState_UP    == GLFW_PRESS && previousKeyState_UP    == GLFW_RELEASE) move = 9;
            if (currentKeyState_DOWN  == GLFW_PRESS && previousKeyState_DOWN  == GLFW_RELEASE) move = 10;
        }

        if (move == 1) movement_U(0, 1, 1 * S, stickers, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
        if (move == 2) movement_D(1, 1, 1 * S, stickers, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
        if (move == 3) movement_F(1, 1, 1 * S, stickers, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
        if (move == 4) movement_B(1, 1, 1 * S, stickers, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
        if (move == 5) movement_R(1, 1, 1 * S, stickers, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
        if (move == 6) movement_L(1, 1, 1 * S, stickers, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);

        if (move == 7 ) movement_Y(1, 1, 1, stickers, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
        if (move == 8 ) movement_Y(1, 1,-1, stickers, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
        if (move == 9 ) movement_X(1, 1, 1, stickers, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
        if (move == 10) movement_X(1, 1,-1, stickers, rotationSpeed, transitionProgress, angle_f, axis_of_rotation, isTransitioning, cube_positions_index, cube_positions_index_previous, cube_positions_index_next);
        
        move = 0;

        if (currentKeyState_1 == GLFW_PRESS && previousKeyState_1 == GLFW_RELEASE)  current_speed = 1;
        if (currentKeyState_2 == GLFW_PRESS && previousKeyState_2 == GLFW_RELEASE)  current_speed = 2;

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
        previousKeyState_W = currentKeyState_W;
        previousKeyState_A = currentKeyState_A;
        previousKeyState_S = currentKeyState_S;
        previousKeyState_D = currentKeyState_D;
        previousKeyState_F = currentKeyState_F;
        previousKeyState_R = currentKeyState_R;

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

        previousKeyState_SHIFT = currentKeyState_SHIFT;
        previousKeyState_Space = currentKeyState_Space;

        

        // SWAP BUFFERS
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // DELETE BUFFERS
    glDeleteVertexArrays(1, &axisVAO);
    glDeleteBuffers(1, &axisVBO);
    glfwTerminate();
    return 0;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// CAMERA MOVEMENT
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    /*
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        //camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        //camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        //camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        //camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        //camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        //camera.ProcessKeyboard(DOWN, deltaTime);
    */
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
    //camera.ProcessMouseMovement(xoffset, yoffset);
}

// SCROOL MOUSE ZOOM
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    //camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// RENDER TEXT
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color){
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader.use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(text_VAO);
    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++){
        Character ch = Characters[*c];
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}