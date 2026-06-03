#ifndef SETTINGS_H
#define SETTINGS_H
#include <yaml-cpp/yaml.h>
#include <string>

    class Settings{
        public:
            Settings();
            int m_windowWidth;
            int m_windowHeight;
            int m_pointOfViewAngle;
            float m_maxDistance;
            float m_screenDistance;
            std::string m_windowName;
    };    
#endif