#  SDL-Based 3D Engine

A lightweight custom **3D game engine** written in **C++** using **SDL2**, featuring a **component-based architecture**, **prefab system**, and **real-time lighting**. The engine is designed for learning purposes and experimentation with low-level rendering pipelines and engine architecture.

---

## ✨ Features

### 🧩 Component System (ECS-like)
- Attach and remove components at runtime  
- Modular behavior architecture  
- Easy extension with custom components  

### 📦 Prefab System
- Save reusable entity templates  
- Instantiate complex objects with one call  
- Supports hierarchical objects  


### 📐 3D Math Core
- Custom Vector and Matrix math  
- Transform system (position, rotation, scale)  
- Camera projection (perspective)  

### 📷 Camera System
- Free-fly camera  
- First-person style movement  
- Configurable FOV and near/far planes  

### ⌨ Input System
- Keyboard and mouse handling  
- Real-time input polling  

### 🌍 Cross-platform
- Windows (MinGW / MSVC)  
- Linux (GCC / Clang)  

---
##  Dependencies

- **C++17 or newer**
- **SDL2**
- **OpenGL 3.3**
- **CMake 3.6+**

---
##  Usage Example
Window`s propertiesc can be changed in config file.
All used files needs to be located in bin directory.
To add scene with objects you need to create yaml file with all objects in the scene:

``` scene1.yaml
gameObjects:
  - name: "camera"
    transformation:
      rotation: [0,0,0]
      position: [0,0,0]
    prefab: "prefab/player.yaml"




  - name: "cube1"
    transformation:
      rotation: [180,180,0]
      position: [0,0,2]
      size: [1,1,1]
      light: false
    prefab: "prefab/key.yaml"
    .obj: "obj/cube.obj"
    texture: "textures/cube.png"

```
    
In the prefab file you can add custom or premade components:
``` prefab1.yaml
components:
  - name: "Camera3D" <-component name
    type: "Camera" <-base type
    
  - name: "rotation"
    type: "rotation"
    data: [0,0,1] <- data which be used in initialization
```

To create component you need  to use basic component class for scrip, or you may create your own type of camera, lighting, etc.
Components file needs be located in bin/components/h and bin/components/cpp.
Component example:
``` rotation.cpp
#include "rotation.h"
#include "GameObject.h"
#include "ComponentFactory.h"
#include "InputHandler.h"
void register_rotation()
{ComponentFactory::init().registerComponent("rotation", []() {return std::make_unique<rotation>();   });} //name has to be the same as the name of component!!!


void rotation::update(Uint32 dT)
{
    //rotating....
}
 void rotation::start(){
    //starting
 };
 void rotation::init(YAML::Node const& data){
    //init with data
 };
### rotation.h
#pragma once
#include "Component.h"
class rotation : public Component{
    public:
        virtual ~rotation() =default;
        virtual void update(Uint32 dT) override;
        virtual void start() override;
        virtual void init(YAML::Node const& data) override;
};
```
