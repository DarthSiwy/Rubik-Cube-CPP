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
ourShader.setInt("texture2", 1)


glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, texture2);


 glm::vec3  vertices_cubeposition[27], vertices_cubeposition_previous[27], vertices_cubeposition_next[27];

//float vertices_previous[numCopies][vertices_main_size];
//float vertices_next[numCopies][vertices_main_size];