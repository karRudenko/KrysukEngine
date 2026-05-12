    #ifndef COMPONENT_FACTORY_h
    #define COMPONENT_FACTORY_h
    #include <functional>
    #include <string>
    #include <unordered_map>
    #include <memory>
    #include "Component.h"

    class ComponentFactory{
        public:
            using Creator=std::function<std::unique_ptr<Component>()>;
            static ComponentFactory& init();
            bool registerComponent(std::string const& name, Creator creator);
            std::unique_ptr<Component> create(std::string const& name);
            std::unordered_map<std::string, Creator> m_creators;  
        private:
            

    };


    #endif