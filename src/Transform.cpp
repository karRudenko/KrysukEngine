#include "Transform.h"
#include <fstream>
#include <sstream>

Polygon::Polygon(Triangle3D const& tr):tri(tr){};
Polygon::Polygon(Triangle3D const& tr,float lum):tri(tr), m_lum(lum){}
Polygon::Polygon(Triangle3D const& tr,Triangle2D const& textCor):tri(tr), text(textCor){};


Transform::Transform(YAML::Node const& obj){
    if(obj[".obj"]){
        m_mesh.LoadObjFile(obj[".obj"].as<std::string>());
    }
    if(obj["texture"]){
        m_mesh.sprite=IMG_Load(obj["texture"].as<std::string>().c_str());
        for(auto& pol:m_mesh.data){

         pol.sprite=m_mesh.sprite;
        }
    }    
    if(auto transf=obj["transformation"]){
        if(transf["position"]){
            std::vector<float> position=transf["position"].as<std::vector<float>>();
            m_position={position[0],position[1],position[2]};
        }
        if(transf["rotation"]){
            std::vector<float> rotation=transf["rotation"].as<std::vector<float>>();
            m_rotation={rotation[0],rotation[1],rotation[2]};
        }
        if(transf["light"]){
            m_light=transf["light"].as<bool>();
        }
        if(transf["size"]){
            std::vector<float> size=transf["size"].as<std::vector<float>>();
            m_size={size[0],size[1],size[2]};
            for(auto &pol : m_mesh.data){
                pol.tri.m_a.m_x*=m_size.m_x;
                pol.tri.m_a.m_y*=m_size.m_y;
                pol.tri.m_a.m_z*=m_size.m_z;
                pol.tri.m_b.m_x*=m_size.m_x;
                pol.tri.m_b.m_y*=m_size.m_y;
                pol.tri.m_b.m_z*=m_size.m_z;
                pol.tri.m_c.m_x*=m_size.m_x;
                pol.tri.m_c.m_y*=m_size.m_y;
                pol.tri.m_c.m_z*=m_size.m_z;

            }
        }
    }
    
}



Mesh::~Mesh(){
    if(sprite){
        SDL_FreeSurface(sprite);
        sprite=nullptr;
    }
}





























std::vector<int> parseOBJIndex(const std::string& vertexStr) {
    std::vector<int> indices(3, -1);
    
    std::vector<std::string> parts;
    size_t start = 0;
    size_t end = 0;
    
    while ((end = vertexStr.find('/', start)) != std::string::npos) {
        if (end > start) {
            parts.push_back(vertexStr.substr(start, end - start));
        } else {
            parts.push_back(""); 
        }
        start = end + 1;
    }
    
    if (start < vertexStr.length()) {
        parts.push_back(vertexStr.substr(start));
    }
    
    // Заполняем индексы
    int partIndex = 0;
    for (size_t i = 0; i < parts.size() && partIndex < 3; i++) {
        if (!parts[i].empty()) {
            try {
                indices[partIndex] = std::stoi(parts[i]);
            } catch (...) {
                indices[partIndex] = -1;
            }
        }
        
        // Переходим к следующему индексу
        // В формате OBJ порядок всегда: вершина/текстура/нормаль
        if (i == 0) partIndex = 1; // После вершины -> текстура
        else if (i == 1) partIndex = 2; // После текстуры -> нормаль
    }
    
    return indices;
}



bool Mesh::LoadObjFile(std::string const & path){
    std::ifstream file(path);
    if(!file.is_open()) return false;

    std::vector<Vector3D> verts;
    std::vector<Vector2D> texCoords;
    std::vector<Vector3D> normals;
    std::string line;

    while(std::getline(file, line)){
        if(line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if(type == "v"){  
            Vector3D v;
            ss >> v.m_x >> v.m_y >> v.m_z;
            
            
            float dummy;
            while(ss >> dummy) {} 
            
            verts.push_back(v);
        }
        else if(type == "vn"){  
            Vector3D vn;
            ss >> vn.m_x >> vn.m_y >> vn.m_z;
            
            
            float dummy;
            while(ss >> dummy) {} 
            
            normals.push_back(vn);
        }
        else if(type == "vt"){  
            Vector2D vt;
            ss >> vt.m_u >> vt.m_v;
            vt.m_v=-vt.m_v;
           
            float w;
            if (ss >> w) {
                
            }
            
            texCoords.push_back(vt);
        }
        else if (type == "f") {
            std::vector<std::string> faceTokens;
            std::string token;
            std::vector<MeshPoint> points;
            while (ss >> token) {
                faceTokens.push_back(token);
            }
            proccesFace(faceTokens,points,verts,texCoords,normals);
            if(points.size()==3){

                Triangle3D tri3d={points[0].verts,points[1].verts,points[2].verts,points[0].normal};
                Triangle2D tri2d={points[0].textCor,points[1].textCor,points[2].textCor};



                Polygon polygon(tri3d, tri2d);
                polygon.sprite = sprite;
                data.push_back(std::move(polygon));
            }
            if(points.size()==4){
                Triangle3D tri3d={points[0].verts,points[1].verts,points[2].verts,points[0].normal};
                Triangle2D tri2d={points[0].textCor,points[1].textCor,points[2].textCor};
                Polygon polygon1(tri3d, tri2d);
                polygon1.sprite = sprite;

                tri3d={points[0].verts,points[2].verts,points[3].verts,points[0].normal};
                tri2d={points[0].textCor,points[2].textCor,points[3].textCor};
                Polygon polygon2(tri3d, tri2d);
                polygon2.sprite = sprite;

                data.push_back(std::move(polygon1));
                data.push_back(std::move(polygon2));

            }

        }
    }
    

    return true;
}
void Mesh::proccesFace(std::vector<std::string>& face, std::vector<MeshPoint>& points, std::vector<Vector3D>& verts, std::vector<Vector2D>& texCoords, std::vector<Vector3D>& normals){
    std::string part;
    for (const auto& tuple : face) {

        MeshPoint point;
        std::stringstream ss(tuple);

        std::getline(ss,part,'/');
        point.verts=verts[std::stoi(part)-1];

        std::getline(ss,part,'/');
        point.textCor=texCoords[std::stoi(part)-1];

        std::getline(ss,part,'/');
        point.normal=normals[std::stoi(part)-1];
    
        points.push_back(point);

        
        

	}
    
}