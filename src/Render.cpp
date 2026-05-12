#include "Renderer.h"
#include "Camera.h"
#include "Light.h"
#include <float.h>
Renderer::Renderer(Window const& window, Settings const& settings):m_settings(settings){
        m_renderer = SDL_CreateRenderer( window.gWindow,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        m_projMatrix=getProjectionMatrix();

        m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, m_settings.m_windowWidth, m_settings.m_windowHeight);
        m_frameBuffer.resize(settings.m_windowWidth * settings.m_windowHeight);
        m_dephBuffer.resize(settings.m_windowWidth * settings.m_windowHeight,0);

        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, 0x0, 0x0, 0x0, 0x0 );     
}
Renderer::~Renderer(){
    SDL_DestroyRenderer(m_renderer);
    m_renderer=NULL;
}


void Renderer::render(Scene const& scene){
    SDL_RenderClear(m_renderer);
    std::fill(m_frameBuffer.begin(), m_frameBuffer.end(), 0);
    std::fill(m_dephBuffer.begin(), m_dephBuffer.end(), 0 );


    auto cam=scene.findComponents<Camera>("Camera")[0];
    m_camMatrix=cam->getCameraMatrix();
    m_camPos=cam->owner->m_transform.m_position;

    m_lightSources=scene.findComponents<Light>("Light");


    for(auto& obj :scene.m_objects){
        
        projectMesh(obj.second->m_transform);
    }
    clipPolygons();





    projectToScreen();

    rasterPolygons();
    for(auto & tr: m_collected){
        
        drawPolygon(tr);
        //drawTriangle(tr);
    }


    m_collected.clear();
    
    SDL_UpdateTexture(m_texture, NULL, m_frameBuffer.data(), m_settings.m_windowWidth * sizeof(Uint32));
    
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);

    
    
    
}



void Renderer::projectMesh(Transform& transform){
    for(auto& tr : transform.m_mesh.data){
        
        projectToCameraView(transform, tr);

    }


}

    void Renderer::calculateLight(Polygon& pol){
        float lum=0;
        for(auto& l: m_lightSources){
        lum=std::max(lum,l->getLuminocity(pol.tri));
        }
        pol.m_lum=lum;
    }

void Renderer::clipPolygons(){
    std::vector<Polygon> result;
    Vector3D plain[2]={{0,0,0.1},{0,0, 1}};
    for(auto &pol :m_collected){
        polygonClipAgainstPlane(plain[0], plain[1],pol,result);
    }
    m_collected=std::move(result);
}

void Renderer::rasterPolygons(){
    std::vector<Polygon> result=m_collected;
    Vector3D plains[][2]={{{0,0,0},{0,1,0}},
                         {{0,0,0},{1,0,0}},
                         {{(float)m_settings.m_windowWidth,0,0},{-1,0,0}},
                         {{0,(float)m_settings.m_windowHeight,0},{0,-1, 0}}};
    int head=0;
    for(int p=0;p<4;p++){
        int sz=result.size()-head;
        for(int i=head; i<sz; i++){
            polygonClipAgainstPlane(plains[p][0], plains[p][1],result[i],result);
            head++;
        }
    }
    m_collected.assign(std::make_move_iterator(result.begin()+head),std::make_move_iterator(result.end()));  


}


void Renderer::projectToScreen(){
    for(auto& pol : m_collected){
        pol.tri.m_a=vectorXmatrix4x4(pol.tri.m_a,m_projMatrix);
        pol.tri.m_b=vectorXmatrix4x4(pol.tri.m_b,m_projMatrix);
        pol.tri.m_c=vectorXmatrix4x4(pol.tri.m_c,m_projMatrix);

        float wA=pol.tri.m_a.m_w;
        float wB=pol.tri.m_b.m_w;
        float wC=pol.tri.m_c.m_w;

        pol.text.m_a= pol.text.m_a/wA;
        pol.text.m_b= pol.text.m_b/wB;
        pol.text.m_c= pol.text.m_c/wC;

        pol.text.m_a.m_w=1.0f/wA;
        pol.text.m_b.m_w=1.0f/wB;
        pol.text.m_c.m_w=1.0f/wC;

        pol.tri.m_a=pol.tri.m_a/wA;
        pol.tri.m_b=pol.tri.m_b/wB;
        pol.tri.m_c=pol.tri.m_c/wC;






        pol.tri.m_a.m_x = (pol.tri.m_a.m_x+1)*m_settings.m_windowWidth/2;
        pol.tri.m_a.m_y = (1-pol.tri.m_a.m_y)*m_settings.m_windowHeight/2;

        pol.tri.m_b.m_x = (pol.tri.m_b.m_x+1)*m_settings.m_windowWidth/2;
        pol.tri.m_b.m_y = (1- pol.tri.m_b.m_y)*m_settings.m_windowHeight/2;

        pol.tri.m_c.m_x = (pol.tri.m_c.m_x+1)*m_settings.m_windowWidth/2;
        pol.tri.m_c.m_y = (1-pol.tri.m_c.m_y)*m_settings.m_windowHeight/2;
    }

    



}
    void Renderer::projectToCameraView(Transform const& transform, Polygon const& tri){
        
        


        Matrix4x4 worldMatrix=getRotMatrix(transform.m_rotation)*getTransMatrix(transform.m_position);


        Vector3D a=vectorXmatrix4x4(tri.tri.m_a,worldMatrix);
        Vector3D b=vectorXmatrix4x4(tri.tri.m_b,worldMatrix);
        Vector3D c=vectorXmatrix4x4(tri.tri.m_c,worldMatrix);

        Polygon result(Triangle3D{a,b,c}, tri.text);
        result.sprite=tri.sprite;

        if(dotProduct(result.tri.m_N,a-m_camPos)>0) return;

        if(transform.m_light)
        calculateLight(result);



        result.tri.m_a=vectorXmatrix4x4(result.tri.m_a,m_camMatrix);
        result.tri.m_b=vectorXmatrix4x4(result.tri.m_b,m_camMatrix);
        result.tri.m_c=vectorXmatrix4x4(result.tri.m_c,m_camMatrix);

        m_collected.emplace_back(std::move(result));
        





    } 





void Renderer::drawTriangle(Polygon const& t) const{
    
    

   /*
    float m=t.m_lum;
    const std::vector< SDL_Vertex > verts ={
        {SDL_FPoint{t.tri.m_a.m_x,t.tri.m_a.m_y}, SDL_Colour{255*m,255*m,255*m,255},SDL_FPoint{0}},
        {SDL_FPoint{t.tri.m_b.m_x,t.tri.m_b.m_y}, SDL_Colour{255*m,255*m,255*m,255},SDL_FPoint{0}},
        {SDL_FPoint{t.tri.m_c.m_x,t.tri.m_c.m_y}, SDL_Colour{255*m,255*m,255*m,255},SDL_FPoint{0}}
    };
    SDL_RenderGeometry(m_renderer,NULL,verts.data(),verts.size(),nullptr,0);
    */
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255 ); 
    SDL_RenderDrawLine(m_renderer,t.tri.m_a.m_x,t.tri.m_a.m_y,t.tri.m_b.m_x,t.tri.m_b.m_y);
    SDL_RenderDrawLine(m_renderer,t.tri.m_a.m_x,t.tri.m_a.m_y,t.tri.m_c.m_x,t.tri.m_c.m_y);
    SDL_RenderDrawLine(m_renderer,t.tri.m_b.m_x,t.tri.m_b.m_y,t.tri.m_c.m_x,t.tri.m_c.m_y);
    SDL_SetRenderDrawColor(m_renderer, 0x0, 0x0, 0x0, 0x0 ); 
}





Matrix4x4 Renderer::getProjectionMatrix() const{
    float h=m_settings.m_windowHeight;
    float w=m_settings.m_windowWidth;

    float pov2=m_settings.m_pointOfViewAngle/2;

    float zfar=m_settings.m_maxDistance;
    float znear=m_settings.m_screenDistance;

    float tan= std::tan(3.14*pov2/180);

    
    Matrix4x4 restult={{{h/(w*tan),0,0,0},
                        {0,1/tan,0,0},
                        {0,0,zfar/(zfar-znear),1},
                        {0,0,-zfar*znear/(zfar-znear),0}}};

    return restult;
}
















void Renderer::polygonClipAgainstPlane(Vector3D& plane_p, Vector3D plane_n, Polygon& in, std::vector<Polygon>& result){

	plane_n.normalize();


	auto dist = [&](Vector3D const &p){
		return (dotProduct(plane_n,p) - dotProduct(plane_n, plane_p));
	};


	Vector3D* inside_points[3];  int nInsidePointCount = 0;
	Vector3D* outside_points[3]; int nOutsidePointCount = 0;
    Vector2D* inside_tex[3]; int nInsideTexCount = 0;
	Vector2D* outside_tex[3]; int nOutsideTexCount = 0;






	float d0 = dist(in.tri.m_a);
	float d1 = dist(in.tri.m_b);
	float d2 = dist(in.tri.m_c);

	if (d0 >= 0) { inside_points[nInsidePointCount++] = &in.tri.m_a; inside_tex[nInsideTexCount++] = &in.text.m_a; }
	else { outside_points[nOutsidePointCount++] = &in.tri.m_a; outside_tex[nOutsideTexCount++] = &in.text.m_a; }

	if (d1 >= 0) { inside_points[nInsidePointCount++] = &in.tri.m_b; inside_tex[nInsideTexCount++] = &in.text.m_b; }
    else { outside_points[nOutsidePointCount++] = &in.tri.m_b;outside_tex[nOutsideTexCount++] = &in.text.m_b; }

	if (d2 >= 0) { inside_points[nInsidePointCount++] = &in.tri.m_c;inside_tex[nInsideTexCount++] = &in.text.m_c; }
	else { outside_points[nOutsidePointCount++] = &in.tri.m_c; outside_tex[nOutsideTexCount++] = &in.text.m_c; }


	if (nInsidePointCount == 0){


		return; 
	}

	if (nInsidePointCount == 3){

		result.push_back(in);

		return; 
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2){

        Polygon out = in;

        float f;

		out.tri.m_a = *inside_points[0];
        out.text.m_a = *inside_tex[0];

		out.tri.m_b = vectorXplainIntersec(plane_p, plane_n, *inside_points[0], *outside_points[0],f);
        out.text.m_b = (*outside_tex[0]-*inside_tex[0])*f+*inside_tex[0];
        out.text.m_b.m_w = (outside_tex[0]->m_w-inside_tex[0]->m_w)*f+inside_tex[0]->m_w;



		out.tri.m_c = vectorXplainIntersec(plane_p, plane_n, *inside_points[0], *outside_points[1],f);
        out.text.m_c = (*outside_tex[1]-*inside_tex[0])*f+*inside_tex[0];
        out.text.m_c.m_w = (outside_tex[1]->m_w-inside_tex[0]->m_w)*f+inside_tex[0]->m_w;


        result.push_back(out);

		return; 
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1){

        Polygon out1=in;

        Polygon out2=in;
        float f;

		out1.tri.m_a = *inside_points[0];
        out1.text.m_a = *inside_tex[0];

		out1.tri.m_b = *inside_points[1];
        out1.text.m_b = *inside_tex[1];

		out1.tri.m_c = vectorXplainIntersec(plane_p, plane_n, *inside_points[0], *outside_points[0],f);
        out1.text.m_c = (*outside_tex[0]-*inside_tex[0])*f+*inside_tex[0];
        out1.text.m_c.m_w = (outside_tex[0]->m_w-inside_tex[0]->m_w)*f+inside_tex[0]->m_w;




		out2.tri.m_a = *inside_points[1];
        out2.text.m_a = *inside_tex[1];



		out2.tri.m_b = out1.tri.m_c;
        out2.text.m_b = out1.text.m_c;

		out2.tri.m_c = vectorXplainIntersec(plane_p, plane_n, *inside_points[1], *outside_points[0],f);
        out2.text.m_c = (*outside_tex[0]-*inside_tex[1])*f+*inside_tex[1];
        out2.text.m_c.m_w = (outside_tex[0]->m_w-inside_tex[1]->m_w)*f+inside_tex[1]->m_w;
        

        result.push_back(out1);
        result.push_back(out2);
		return; 
	}
}






void Renderer::drawPolygon(Polygon& pol) {

    if (pol.tri.m_b.m_y < pol.tri.m_a.m_y) {
        std::swap(pol.tri.m_b, pol.tri.m_a);
        std::swap(pol.text.m_b, pol.text.m_a);
    }
    if (pol.tri.m_c.m_y < pol.tri.m_a.m_y) {
        std::swap(pol.tri.m_c, pol.tri.m_a);
        std::swap(pol.text.m_c, pol.text.m_a);
    }
    if (pol.tri.m_c.m_y < pol.tri.m_b.m_y) {
        std::swap(pol.tri.m_c, pol.tri.m_b);
        std::swap(pol.text.m_c, pol.text.m_b);
    }

    int x1 = pol.tri.m_a.m_x;
    int y1 = pol.tri.m_a.m_y;
    int x2 = pol.tri.m_b.m_x;
    int y2 = pol.tri.m_b.m_y;
    int x3 = pol.tri.m_c.m_x;
    int y3 = pol.tri.m_c.m_y;

    float u1 = pol.text.m_a.m_u;
    float v1 = pol.text.m_a.m_v;
    float u2 = pol.text.m_b.m_u;
    float v2 = pol.text.m_b.m_v;
    float u3 = pol.text.m_c.m_u;
    float v3 = pol.text.m_c.m_v;

    float w1=pol.text.m_a.m_w;
    float w2=pol.text.m_b.m_w;
    float w3=pol.text.m_c.m_w;


    if (y1 != y2) {

        float dxdy1 = (x2 - x1) / (float)(y2 - y1);
        float dxdy2 = (x3 - x1) / (float)(y3 - y1);
        

        float dudy1 = (u2 - u1) / (float)(y2 - y1);
        float dvdy1 = (v2 - v1) / (float)(y2 - y1);
        float dwdy1 = (w2 - w1) / (float)(y2 - y1);

        float dudy2 = (u3 - u1) / (float)(y3 - y1);
        float dvdy2 = (v3 - v1) / (float)(y3 - y1);
        float dwdy2 = (w3 - w1) / (float)(y3 - y1);

        for (int y = y1; y <= y2 && y < m_settings.m_windowHeight; y++) {
            if (y < 0) continue;
            
            int y_offset = y - y1;
            

            float x_start = x1 + dxdy1 * y_offset;
            float x_end = x1 + dxdy2 * y_offset;
            

            float u_start = u1 + dudy1 * y_offset;
            float v_start = v1 + dvdy1 * y_offset;
            float w_start = w1 + dwdy1 * y_offset;

            float u_end = u1 + dudy2 * y_offset;
            float v_end = v1 + dvdy2 * y_offset;
            float w_end = w1 + dwdy2 * y_offset;

            if (x_start > x_end) {
                std::swap(x_start, x_end);
                std::swap(u_start, u_end);
                std::swap(v_start, v_end);
                std::swap(w_start, w_end);
            }
            

            int ix_start = std::max(0, (int)x_start);
            int ix_end = std::min(m_settings.m_windowWidth, (int)x_end);
            

            

            float dx = x_end - x_start;
            float du_dx = (u_end - u_start) / dx;
            float dv_dx = (v_end - v_start) / dx;
            float dw_dx = (w_end - w_start) / dx;





            float u = u_start + (ix_start - x_start) * du_dx;
            float v = v_start + (ix_start - x_start) * dv_dx;
            float w = w_start + (ix_start - x_start) * dw_dx;
            
            for (int x = ix_start; x < ix_end; x++) {

                float m = pol.m_lum;
                int r, g, b;
                
                if (pol.sprite) {
                    Uint32 pixel = getPixel(pol.sprite, u/w, v/w);
                    r = ((pixel >> 16) & 0xFF)*m;
                    g = ((pixel >> 8) & 0xFF)*m;
                    b = (pixel & 0xFF)*m;
                } else {
                    r = 255 * m;
                    g = 255 * m;
                    b = 255 * m;
                }
                
                if(w > m_dephBuffer[y*m_settings.m_windowWidth+x]){
                        m_frameBuffer[y * m_settings.m_windowWidth + x] = 
                            (255 << 24) | (r << 16) | (g << 8) | b;
                        m_dephBuffer[y*m_settings.m_windowWidth+x]=w;

                }
                

                u += du_dx;
                v += dv_dx;
                w += dw_dx;
            }
        }
    }




    if (y2 != y3) {

        float dxdy1 = (x3 - x2) / (float)(y3 - y2);
        float dxdy2 = (x3 - x1) / (float)(y3 - y1);
        

        float dudy1 = (u3 - u2) / (float)(y3 - y2);
        float dvdy1 = (v3 - v2) / (float)(y3 - y2);
        float dwdy1 = (w3 - w2) / (float)(y3 - y2);

        float dudy2 = (u3 - u1) / (float)(y3 - y1);
        float dvdy2 = (v3 - v1) / (float)(y3 - y1);
        float dwdy2 = (w3 - w1) / (float)(y3 - y1);

        for (int y = y2; y <= y3 && y < m_settings.m_windowHeight; y++) {
            if (y < 0) continue;
            int y_offset1 = y - y2;  
            int y_offset2 = y - y1;
            

            float x_start = x2 + dxdy1 * y_offset1;
            float x_end = x1 + dxdy2 * y_offset2;
            

            float u_start = u2 + dudy1 * y_offset1;
            float v_start = v2 + dvdy1 * y_offset1;
            float w_start = w2 + dwdy1 * y_offset1;

            float u_end = u1 + dudy2 * y_offset2;
            float v_end = v1 + dvdy2 * y_offset2;
            float w_end = w1 + dwdy2 * y_offset2;

            if (x_start > x_end) {
                std::swap(x_start, x_end);
                std::swap(u_start, u_end);
                std::swap(v_start, v_end);
                std::swap(w_start, w_end);
            }
            

            int ix_start = std::max(0, (int)x_start);
            int ix_end = std::min(m_settings.m_windowWidth, (int)x_end);
            

            

            float dx = x_end - x_start;
            float du_dx = (u_end - u_start) / dx;
            float dv_dx = (v_end - v_start) / dx;
            float dw_dx = (w_end - w_start) / dx;
            

            float u = u_start + (ix_start - x_start) * du_dx;
            float v = v_start + (ix_start - x_start) * dv_dx;
            float w = w_start + (ix_start - x_start) * dw_dx;
            
            for (int x = ix_start; x < ix_end; x++) {

                float m = pol.m_lum;
                int r, g, b;
                
                if (pol.sprite) {
                    Uint32 pixel = getPixel(pol.sprite, u/w, v/w);
                    r = ((pixel >> 16) & 0xFF)*m;
                    g = ((pixel >> 8) & 0xFF)*m;
                    b = (pixel & 0xFF)*m;
                } else {
                    r = 255 * m;
                    g = 255 * m;
                    b = 255 * m;
                }
                
                if(w > m_dephBuffer[y*m_settings.m_windowWidth+x]){
                        m_frameBuffer[y * m_settings.m_windowWidth + x] = 
                            (255 << 24) | (r << 16) | (g << 8) | b;
                        m_dephBuffer[y*m_settings.m_windowWidth+x]=w;

                }
                

                u += du_dx;
                v += dv_dx;
                w += dw_dx;
            }
        }
    }
}


Uint32 Renderer::getPixel(SDL_Surface *surface, float u, float v) {
    u = std::clamp(u, 0.0f, 1.0f);
    v = std::clamp(v, 0.0f, 1.0f);
    
    int x = static_cast<int>(u * (surface->w - 1));
    int y = static_cast<int>(v * (surface->h - 1));
    
    x = std::max(0, std::min(x, surface->w - 1));
    y = std::max(0, std::min(y, surface->h - 1));
    
    Uint32 pixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch;
    
    switch(surface->format->BytesPerPixel) {
        case 1:
            pixel = p[x];
            break;
        case 2:
            pixel = ((Uint16 *)p)[x];
            break;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                pixel = (p[x*3] << 16) | (p[x*3+1] << 8) | p[x*3+2];
            } else {
                pixel = p[x*3] | (p[x*3+1] << 8) | (p[x*3+2] << 16);
            }
            break;
        case 4:
            pixel = ((Uint32 *)p)[x];
            break;
        default:
            return 0xFF000000;  
    }
    
    Uint8 r = ((pixel & surface->format->Rmask) >> surface->format->Rshift) << surface->format->Rloss;
    Uint8 g = ((pixel & surface->format->Gmask) >> surface->format->Gshift) << surface->format->Gloss;
    Uint8 b = ((pixel & surface->format->Bmask) >> surface->format->Bshift) << surface->format->Bloss;
    Uint8 a = surface->format->Amask ? 
        (((pixel & surface->format->Amask) >> surface->format->Ashift) << surface->format->Aloss) : 255;
    
    return (a << 24) | (r << 16) | (g << 8) | b;
}