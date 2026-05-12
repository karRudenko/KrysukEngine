#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "BasicGeometry.h"
#include <vector>
#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
struct Polygon{
    Polygon() =default;
    Polygon(Triangle3D const& tr);
    Polygon(Triangle3D const& tr,float lum);
    Polygon(Triangle3D const& tr,Triangle2D const& textCor);
    
    Triangle3D tri;
    Triangle2D text;
    SDL_Surface* sprite=nullptr;
    float m_lum=1;
};



struct Mesh{
        ~Mesh();
        std::vector<Polygon> data;
        bool Light=true;
        bool LoadObjFile(std::string const& path);
        SDL_Surface* sprite=nullptr;
    private: 
        
        struct MeshPoint{
            Vector3D verts;
            Vector2D textCor;
            Vector3D normal;
        };
        void proccesFace(std::vector<std::string>& face, std::vector<MeshPoint>& points, std::vector<Vector3D>& verts, std::vector<Vector2D>& texCoords,std::vector<Vector3D>& normals);

};
class Transform{
    public:
        Transform(YAML::Node const& obj);
        bool m_light=true;
        Vector3D m_position={0,0,0};
        Vector3D m_rotation={0,0,0};
        Vector3D m_size={1,1,1};
        Mesh m_mesh;
        

    
};

#endif