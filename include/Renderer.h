#ifndef RENDERER_H
#define RENDERER_H
#define GLM_ENABLE_EXPERIMENTAL
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <gl\glu.h>
#include <string>
#include <vector>
#include <cmath>
#include <queue>
#include "Settings.h"
#include "Window.h"
#include "Transform.h"
#include "Scene.h"
#include "BasicGeometry.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

struct GPUContext{
    GLuint m_vertices =0;
    GLuint m_texture =0;
    GLuint m_UV=0;
    int m_vertexCount;
    glm::mat4 m_MVP;
    bool m_init=false;
};

class Light;
class Camera;
class Renderer{
    public:
        Renderer(Window const& window, Settings const& settings);
        ~Renderer();
        void render(Scene const& scsene);
    private:
        Settings m_settings;
        const Window& m_window;
        void initGL();
        void loadShaders(const char * vertex_file_path,const char * fragment_file_path);
        void loadTransform(Transform& t);
        void calculateMVP(Transform const& t);
        glm::mat4 getProjectionMatrix() const;
        glm::mat4 getModelMatrix(Transform const& t);
        Matrix4x4 glmToMatrix4x4(const glm::mat4& mat4);
        glm::mat4 Matrix4x4toglm(const Matrix4x4& m);
        glm::mat4 m_camera;
        glm::mat4 m_projection;
        std::unordered_map<const Transform*,GPUContext> m_cache;
        GLuint m_programID = 0;
        GLuint m_VBO = 0;
        GLuint m_VAO = 0;
        GLuint m_MVPID = 0;
        SDL_GLContext m_Context;
        

        
};




#endif