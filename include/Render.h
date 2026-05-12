#ifndef RENDERER_H
#define RENDERER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <queue>
#include "Settings.h"
#include "Window.h"
#include "BasicGeometry.h"
#include "Transform.h"
#include "Scene.h"
class Light;
class Camera;
class Renderer{
    public:
        Renderer(Window const& window, Settings const& settings);
        ~Renderer();
        void render(Scene const& scsene);
        void projectMesh(Transform& transform);
    private:
        std::vector<Polygon> m_collected;
        std::vector<Light*> m_lightSources;
        SDL_Renderer* m_renderer;    
        Settings const& m_settings;

        SDL_Texture* m_texture;
        std::vector<Uint32> m_frameBuffer;
        std::vector<float> m_dephBuffer;



        Matrix4x4 m_projMatrix;
        Matrix4x4 m_camMatrix;
        Vector3D m_camPos;
        void drawTriangle(Polygon const& t) const;
        void projectToCameraView(Transform const& transform, Polygon const& tri);
        Matrix4x4 getProjectionMatrix() const;
        void calculateLight(Polygon& pol);
        void polygonClipAgainstPlane(Vector3D& plane_p, Vector3D plane_n, Polygon& in, std::vector<Polygon>& result);
        void projectToScreen();
        void rasterPolygons();
        void clipPolygons();
        void drawPolygon(Polygon& pol);
        Uint32 getPixel(SDL_Surface *surface, float u, float v);



        
};

#endif