#include "Settings.h"
Settings::Settings(){
    YAML::Node config = YAML::LoadFile("../config.yaml");
    if(!config) throw std::runtime_error("no config found");

    if(!config["window width"])  throw std::runtime_error("no window width provided");
    m_windowWidth=config["window width"].as<int>();

    if(!config["window height"])  throw std::runtime_error("no window height provided");
    m_windowHeight=config["window height"].as<int>();

    if(!config["field of view"])  throw std::runtime_error("no field Of view provided");
    m_pointOfViewAngle=config["field of view"].as<int>();

    if(!config["max distance"])  throw std::runtime_error("no max distance provided");
    m_maxDistance=config["max distance"].as<float>();

    if(!config["screen distance"])  throw std::runtime_error("no screen distance provided");
    m_screenDistance=config["screen distance"].as<float>();
    
    if(!config["window name"])  throw std::runtime_error("no window name provided");
    m_windowName=config["window name"].as<std::string>();
}