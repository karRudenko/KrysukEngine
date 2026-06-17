#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
int main(){
    std::string type;
    std::cin>>type;

    std::string newType;
    std::cin>>newType;

    std::fstream include("../include/" + type + ".h");

    if(!include.is_open()) return -1;
    
    std::string line;
    std::vector<std::string> newIncludeData;

    newIncludeData.push_back("#pragma once");
    newIncludeData.push_back("#include \"" + type +".h\"");

    while(std::getline(include, line)){
        if(line.empty()) continue;

        if(line.find("public:") !=std::string::npos || line.find("privat:") !=std::string::npos){
            newIncludeData.push_back(line);
        }
        if(line.find("class") !=std::string::npos && line.find(type)!=std::string::npos){
            int pos=line.find(type);
            while(pos!=std::string::npos){
                line.replace(pos,type.size(),newType);
                line=line.substr(pos+1);
                pos=line.find(type);
            }
            if(line.find("{") !=std::string::npos) 
                line.clear();
            
            line+= "class " +newType +" : public " +type+"{";
            newIncludeData.push_back(line);
        }
        

        if(line.find("virtual")!=std::string::npos){
            int pos=line.find(type);
            while(pos!=std::string::npos){
                line.replace(pos,type.size(),newType);
                line=line.substr(pos+1);
                line=line.substr(pos);
                pos=line.find(type);
            }
            if(line.find(")") !=std::string::npos)
                line.erase(line.begin()+line.find(")")+1, line.end());
            if(line.find("~") ==std::string::npos)    
            line+=" override";
            line+=";";
            newIncludeData.push_back(line);

        }

       
    }
    newIncludeData.push_back("};");

    include.close();



    std::ofstream newInclude("bin/components/h/"+newType+".h");
    if(!newInclude.is_open()) return -1;
    for( auto& str:newIncludeData){
        newInclude<<str<<std::endl;
    }
    newInclude.close();

    std::ofstream newcpp("bin/components/cpp/"+newType+".cpp");

    if(!newcpp.is_open()) return -1;
    newcpp<<"#include \"" + newType +".h\""<<std::endl;

    newcpp<<"#include \"GameObject.h\""<<std::endl;
    newcpp<<"#include \"ComponentFactory.h\""<<std::endl;



    newcpp<<"void register_" +newType+"()"<<std::endl<<
    "{ComponentFactory::init().registerComponent(\""+newType+"\", []() {return std::make_unique<"+newType+">();   });}";
    newcpp.close();
    return 0;
    

}
