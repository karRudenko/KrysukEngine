#define GLM_ENABLE_EXPERIMENTAL
#include "Renderer.h"
#include "Camera.h"
#include "Light.h"
#include <float.h>
#include <fstream>


Renderer::Renderer(Window const& window, Settings const& settings):m_settings(settings), m_window(window){
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE ); 
    m_Context = SDL_GL_CreateContext( window.gWindow );
	glewInit();
	glViewport(0, 0, m_settings.m_windowWidth, m_settings.m_windowHeight);

    


    initGL();
	
	m_projection=getProjectionMatrix();


}
Renderer::~Renderer(){
 
}


void Renderer::render(Scene const& scene)
{
	glBindVertexArray(m_VAO);

    auto cam=scene.findComponents<Camera>("Camera")[0];
    m_camera = glm::lookAt(
        glm::vec3(cam->m_position.m_x, cam->m_position.m_y, cam->m_position.m_z),
        glm::vec3(cam->m_dir.m_x, cam->m_dir.m_y, cam->m_dir.m_z),
        glm::vec3(cam->m_up.m_x, cam->m_up.m_y, cam->m_up.m_z)
    ); 
	

    
    

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram( m_programID );



	for(auto& t: scene.m_objects){
		loadTransform(t.second->m_transform);
		calculateMVP(t.second->m_transform);

	}

	for(auto p : m_cache){
		GPUContext& data =p.second;

		glBindBuffer(GL_ARRAY_BUFFER,data.m_vertices);

        glVertexAttribPointer(
            0,                 
            3,                  
            GL_FLOAT,           
            GL_FALSE,           
            0,                  
            (void*)0            
        );
        glEnableVertexAttribArray(0);  

		
		
		if(data.m_UV!=0){
			glBindBuffer(GL_ARRAY_BUFFER, data.m_UV);

			glVertexAttribPointer(
			1,                                
			2,                                
			GL_FLOAT,                         
			GL_FALSE,                         
			0,                                
			(void*)0                          
			);
			glEnableVertexAttribArray(1);




		}
		if(data.m_UV!=0){
			glBindTexture(GL_TEXTURE_2D, data.m_texture);
		}



		glUniformMatrix4fv(m_MVPID, 1, GL_FALSE, &data.m_MVP[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, data.m_vertexCount);
	}
    
    
    SDL_GL_SwapWindow(m_window.gWindow);
}


void Renderer::loadTransform(Transform& t){
	if(m_cache.count(&t)) return;
	GPUContext data;


	glCreateBuffers(1, &data.m_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, data.m_vertices);



	std::vector<GLfloat> vertices;
	for(auto& pol :t.m_mesh.data){

		std::vector<GLfloat> temp = {pol.tri.m_a.m_x,pol.tri.m_a.m_y,pol.tri.m_a.m_z,
									pol.tri.m_b.m_x,pol.tri.m_b.m_y,pol.tri.m_b.m_z,
									pol.tri.m_c.m_x,pol.tri.m_c.m_y,pol.tri.m_c.m_z,};

		vertices.insert(vertices.end(),std::make_move_iterator(temp.begin()),std::make_move_iterator(temp.end()));
	}

	

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	




	if(t.m_mesh.sprite){

		std::vector<GLfloat> UV;
	    for(auto& pol :t.m_mesh.data){

		std::vector<GLfloat> temp = {pol.text.m_a.m_u,pol.text.m_a.m_v,
									pol.text.m_b.m_u,pol.text.m_b.m_v,
									pol.text.m_c.m_u,pol.text.m_c.m_v};

		UV.insert(UV.end(),std::make_move_iterator(temp.begin()),std::make_move_iterator(temp.end()));
	    }




		
		glGenBuffers(1, &data.m_UV);
		glBindBuffer(GL_ARRAY_BUFFER, data.m_UV);
		glBufferData(GL_ARRAY_BUFFER, UV.size() * sizeof(GLfloat), UV.data(), GL_STATIC_DRAW);
		






		int format;
		if(t.m_mesh.sprite->format->Amask!=0){
			format=0x1908;
		}else{
			format=0x1907;
		}


		
		glGenTextures(1,&data.m_texture);
		glBindTexture(GL_TEXTURE_2D, data.m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format,t.m_mesh.sprite->w, t.m_mesh.sprite->h, 0, format, GL_UNSIGNED_BYTE, t.m_mesh.sprite->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}











	data.m_vertexCount=vertices.size();

	m_cache[&t]=std::move(data);
	
}










void Renderer::initGL(){
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    loadShaders("shaders/VertexShader.glsl","shaders/FragmentShader.glsl");

    glGenVertexArrays(1, &m_VAO);
    
    glBindVertexArray(m_VAO);

	glEnableVertexAttribArray(0);

	glEnableVertexAttribArray(1);
    

    glGenBuffers(1, &m_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	m_MVPID = glGetUniformLocation(m_programID, "MVP");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);





    

}




























void Renderer::loadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint programID = glCreateProgram(); 
	glAttachShader(programID, VertexShaderID);
	glAttachShader(programID, FragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(programID, VertexShaderID);
	glDetachShader(programID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	
	m_programID = programID;
}



glm::mat4 Renderer::getProjectionMatrix() const{

	glm::mat4 gl=glm::perspective(glm::radians(static_cast<float>(m_settings.m_pointOfViewAngle)),(float)m_settings.m_windowWidth/(float)m_settings.m_windowHeight, m_settings.m_screenDistance,m_settings.m_maxDistance);

    return gl;
}


Matrix4x4 Renderer::glmToMatrix4x4(const glm::mat4& m)
{
    Matrix4x4 result{};
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col)
            result.data[row][col] = m[col][row];

    return result;
}

glm::mat4 Renderer::Matrix4x4toglm(const Matrix4x4& m)
{
    glm::mat4 result(1.0f);
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col)
            result[col][row] = m.data[row][col];

    return result;
}

glm::mat4 Renderer::getModelMatrix(const Transform& t){
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(t.m_position.m_x,t.m_position.m_y,t.m_position.m_z));
	glm::mat4 rotate = Matrix4x4toglm(getRotMatrix(t.m_rotation));
	return translate*rotate;
}



void Renderer::calculateMVP(Transform const& t){
	GPUContext& d=m_cache[&t];
	d.m_MVP=  m_projection * m_camera* getModelMatrix(t);
}