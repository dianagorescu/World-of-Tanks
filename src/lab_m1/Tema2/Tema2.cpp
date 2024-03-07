#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraTarget = false;

    camera = new implemented::Camera();
    //camera->Set(glm::vec3(t.position.x, t.position.y, t.position.z), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    glm::vec3 tankForward = glm::vec3(glm::cos(t.rotation), 0.0f,  - glm::sin(t.rotation));

    // Set the camera's position and orientation based on the tank's position and rotation
    // camera->Set(glm::vec3(t.position.x-1, t.position.y - 1, t.position.z-1), t.position + tankForward, glm::vec3(0, 1, 0));

    
     camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("corp");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "corp.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("roti");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "roti.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("tun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "tun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("turela");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "turela.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("cladire");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }


    for (int i = 0;i < 20 ;i++) {
        cladire[i].px = static_cast<float>(rand()) / RAND_MAX * (10.0f + 8.0f) - 8.0f;
        cladire[i].pz = static_cast<float>(rand()) / RAND_MAX * (10.0f + 8.0f) - 8.0f;

        cladire[i].scalex = static_cast<float>(rand()) / RAND_MAX * (4.0f - 1.0f) + 1.0f;
        cladire[i].scaley = static_cast<float>(rand()) / RAND_MAX * (6.0f - 0.0f) + 0.0f;
        cladire[i].scalez = static_cast<float>(rand()) / RAND_MAX * (2.0f - 1.0f) + 1.0f;
       
    }
    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Shader* shader = new Shader("TemaShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        lightPosition = glm::vec3(0, 1, 1);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
    }
    

    // TODO(student): After you implement the changing of the projection
    
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);

}

Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices, const int &objectType)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // TODO(student): If you look closely in the `Init()` and `Update()`
    // functions, you will see that we have three objects which we load
    // and use in three different ways:
    // - LoadMesh   + LabShader (this lab's shader)
    // - CreateMesh + VertexNormal (this shader is already implemented)
    // - CreateMesh + LabShader (this lab's shader)
    // To get an idea about how they're different from one another, do the
    // following experiments. What happens if you switch the color pipe and
    // normal pipe in this function (but not in the shader)? Now, what happens
    // if you do the same thing in the shader (but not in this function)?
    // Finally, what happens if you do the same thing in both places? Why?

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex objectType attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    //meshes[name]->objectType = objectType;

    return meshes[name];
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    if (inchidere_joc < 90.f) {
        cout << inchidere_joc<< endl;
        inchidere_joc += deltaTimeSeconds;
        {
            
            //camera->Set(glm::vec3(t.position.x, t.position.y, t.position.z), glm::vec3(t.position.x, t.position.y, t.position.z), glm::vec3(0, 1, 0));
        }
        {
            for (int i = 0;i < 20;i++) {

                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(t.position.x, 0.25f, t.position.z));
                if (t.position.x != cladire[i].scalex && t.position.z != cladire[i].scalez) {
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(t.position.x, 0, t.position.z - 5 ));
                    modelMatrix = glm::rotate(modelMatrix, t.rotation, glm::vec3(0, 1, 0));
                }


                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));

                // esti in spatele tancului
                modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));


                RenderMesh(meshes["corp"], shaders["TemaShader"], modelMatrix, glm::vec3(1, 0.5f, 0.5f));
                RenderMesh(meshes["roti"], shaders["TemaShader"], modelMatrix, glm::vec3(1, 0, 0));
                RenderMesh(meshes["turela"], shaders["TemaShader"], modelMatrix, glm::vec3(1, 1, 1));
                RenderMesh(meshes["tun"], shaders["TemaShader"], modelMatrix, glm::vec3(1, 0, 0));

                
            }
        }
        {
            for (int i = 0;i < 11;i++) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(cladire[i].px, 0.25f, cladire[i].pz));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(cladire[i].scalex, cladire[i].scaley, cladire[i].scalez));

                RenderMesh(meshes["cladire"], shaders["TemaShader"], modelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
            }
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
            //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
            RenderMesh(meshes["plane"], shaders["TemaShader"], modelMatrix, glm::vec3(0, 0.5f, 0));
        }

        if (renderCameraTarget)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
            RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix, glm::vec3(1, 0, 0.2f));
        }

        if (trage) {
           glm::mat4 modelMatrix = glm::mat4(1);
           modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 4));
           modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
           RenderSimpleMesh(meshes["sphere"], shaders["Simple"], modelMatrix, glm::vec3(0, 0, 0));     
        }
    }
    else {
        exit(0);
    }
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

    mesh->Render();
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);


    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));


    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);


    
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 2.0f;
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            camera->TranslateForward(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-deltaTime * cameraSpeed);
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->TranslateForward(-deltaTime * cameraSpeed);
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(deltaTime * cameraSpeed);
        }
        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(deltaTime * cameraSpeed);
        }
    }
    else {
        float speed = 1.5f;
        if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT))
        {
            trage = true;
        }

        // miscare tanc

        if (window->KeyHold(GLFW_KEY_W)) {
            t.position.z += -speed * deltaTime;
            camera->MoveForward(deltaTime * speed);
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            t.position.z += +speed * deltaTime;
            camera->MoveForward(-deltaTime * speed);

        }
        if (window->KeyHold(GLFW_KEY_A)) {
            t.rotation += speed * deltaTime;
            camera->RotateFirstPerson_OY(speed * deltaTime);
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            t.rotation += -speed * deltaTime;
            camera->RotateFirstPerson_OY(-speed * deltaTime);

        }

    }

    

    if (window->KeyHold(GLFW_KEY_1)) {
        //if (projection) 
        {
            fov += deltaTime * cameraSpeed;
            projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
        }
    }
    if (window->KeyHold(GLFW_KEY_0)) {
        //if (projection) 
        {
            fov -= deltaTime * cameraSpeed;
            //cout << fov << endl;
            projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
        }
    }

}


void Tema2::OnKeyPress(int key, int mods)
{
    //

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
            camera->RotateFirstPerson_OY(sensivityOY * -deltaX);

        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.

            camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
            camera->RotateThirdPerson_OY(sensivityOY * -deltaX);

        }
    }
    
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
