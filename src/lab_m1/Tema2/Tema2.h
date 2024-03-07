#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema2/tema_camera.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices, const int& objectType);


    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);


        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        // TODO(student): If you need any other class variables, define them here.
        GLfloat fov = 90;
        bool projection = true;
        float cameraSpeed = 2.0f;

        float translateX = 0, translateY = 0, translateZ = 0;
        float rotateOY= 0;

        int objectType;
        struct cladire{
           float px, pz;
           float scalex, scaley, scalez;

        }cladire[20];

        struct Tanc {
            glm::vec3 position = glm::vec3 (0.0f, 2, 3.5f);
            float rotation = 0.f;
        }t;

        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        GLfloat right = 3, left = -3, bottom = -3, top = 3;
        GLfloat zNear = 0.1f, zFar = 50.f;

        float inchidere_joc = 0;
        bool trage = false;
    };
}   // namespace m1
