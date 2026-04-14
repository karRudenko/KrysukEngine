#include "BasicGeometry.h"


Vector2D::Vector2D(float u, float v)
    : m_u(u), m_v(v) {}

Vector2D Vector2D::operator+(const Vector2D& n) const {
    return Vector2D(m_u + n.m_u, m_v + n.m_v);
}

Vector2D Vector2D::operator-(const Vector2D& n) const {
    return Vector2D(m_u - n.m_u, m_v - n.m_v);
}

Vector2D Vector2D::operator-(float n) const {
    return Vector2D(m_u - n, m_v - n);
}

Vector2D Vector2D::operator*(float n) const {
    return Vector2D(m_u * n, m_v * n);
}

Vector2D Vector2D::operator/(float n) const {
    return Vector2D(m_u / n, m_v / n);
}

void Vector2D::normalize() {
    float len = std::sqrt(m_u * m_u + m_v * m_v);
    if (len != 0.0f) {
        m_u /= len;
        m_v /= len;
    }
}

float Vector2D::length() const {
    return std::sqrt(m_u * m_u + m_v * m_v);
}







Triangle2D::Triangle2D(const Vector2D& a, const Vector2D& b, const Vector2D& c): m_a(a), m_b(b), m_c(c){
    computeNormal();
}


Triangle2D::Triangle2D(const Vector2D& a, const Vector2D& b, const Vector2D& c,const Vector2D& N): m_a(a), m_b(b), m_c(c), m_N(N){
    m_N.normalize();
}


void Triangle2D::computeNormal() {

    Vector2D edge = m_b - m_a;

    m_N = Vector2D(-edge.m_v, edge.m_u);

    m_N.normalize();
}

















Vector3D::Vector3D(float x, float y, float z): m_x(x),m_y(y), m_z(z){
}

Vector3D Vector3D::operator+(const Vector3D &n)const{
    return Vector3D(m_x+n.m_x,m_y+n.m_y,m_z+n.m_z);
}
Vector3D Vector3D::operator-(const Vector3D &n)const{
    return Vector3D(m_x-n.m_x,m_y-n.m_y,m_z-n.m_z);
}
Vector3D Vector3D::operator-(float &n){
    return Vector3D(m_x-n,m_y-n,m_z-n);
}



Vector3D Vector3D::operator*(float n){
    return Vector3D(m_x*n,m_y*n,m_z*n);
}
Vector3D Vector3D::operator/(float n){
    return Vector3D(m_x/n,m_y/n,m_z/n);
}


void Vector3D::normalize(){
    float length=std::sqrt(m_x*m_x+m_y*m_y+m_z*m_z);
    m_x/=length;
    m_y/=length;
    m_z/=length;
}
float Vector3D::length() const{
    return std::sqrt(m_x*m_x+m_y*m_y+m_z*m_z);
}

Triangle3D::Triangle3D(Vector3D const& a, Vector3D const& b, Vector3D const& c){
    m_a=a;
    m_b=b;
    m_c=c;


    m_N=crossProduct(a,b,c);
    m_N.normalize();
}

Triangle3D::Triangle3D(Vector3D const& a, Vector3D const& b,Vector3D const& c, Vector3D const& N){
    m_a=a;
    m_b=b;
    m_c=c;
    m_N=N;
    m_N.normalize();

}







Vector3D crossProduct(Vector3D const& a, Vector3D const& b, Vector3D const& c){
    Vector3D first={b.m_x-a.m_x,b.m_y-a.m_y,b.m_z-a.m_z};
    Vector3D second={c.m_x-a.m_x,c.m_y-a.m_y,c.m_z-a.m_z};

    return {first.m_y*second.m_z-first.m_z*second.m_y,
        first.m_z*second.m_x-first.m_x*second.m_z,
        first.m_x*second.m_y-first.m_y*second.m_x,
    };

}

Vector3D crossProduct(Vector3D const& first, Vector3D const& second){

    return {first.m_y*second.m_z-first.m_z*second.m_y,
        first.m_z*second.m_x-first.m_x*second.m_z,
        first.m_x*second.m_y-first.m_y*second.m_x,
    };

}

void Vector3D::rotate(Vector3D const& vec){
    float const f=M_PI/180;
    
    *this=vectorXmatrix4x4(*this,getRotMatrix(vec));

}



Matrix4x4 Matrix4x4::operator*(Matrix4x4 m){
    return matrixXmatrix4x4(*this,m);
}

Matrix4x4 getRotMatrix(Vector3D const& vec){
    float const f=M_PI/180;
    float const dX=vec.m_x;
    float const dY=vec.m_y;
    float const dZ=vec.m_z;
    Matrix4x4 rotX  = {{
        { 1,  0,                 0,                0 },
        { 0,  std::cosf(dX*f),  std::sinf(dX*f),  0 },
        { 0,  std::sinf(dX*f),   std::cosf(dX*f),  0 },
        { 0,  0,                 0,                1 }
    }};



    Matrix4x4 rotY = {{
    {  std::cosf(dY*f), 0, std::sinf(dY*f), 0 },
    {  0,               1, 0,               0 },
    { -std::sinf(dY*f), 0, std::cosf(dY*f), 0 },
    {  0,               0, 0,               1 }
    }};


    Matrix4x4 rotZ = {{
    { std::cosf(dZ*f), -std::sinf(dZ*f), 0, 0 },
    { std::sinf(dZ*f),  std::cosf(dZ*f), 0, 0 },
    { 0,                0,               1, 0 },
    { 0,                0,               0, 1 }
    }};
    return  matrixXmatrix4x4(rotX,matrixXmatrix4x4(rotY,rotZ));
}


Vector3D vectorXmatrix4x4(Vector3D const& v, Matrix4x4 const& m){
    Vector3D result;
    

    result.m_x = v.m_x * m.data[0][0] +
              v.m_y * m.data[1][0] +
              v.m_z * m.data[2][0] +
              v.m_w  * m.data[3][0];

    result.m_y = v.m_x * m.data[0][1] +
              v.m_y * m.data[1][1] +
              v.m_z * m.data[2][1] +
              v.m_w  * m.data[3][1];

    result.m_z = v.m_x * m.data[0][2] +
              v.m_y * m.data[1][2] +
              v.m_z * m.data[2][2] +
              v.m_w  * m.data[3][2];

    result.m_w = v.m_x * m.data[0][3] +
              v.m_y * m.data[1][3] +
              v.m_z * m.data[2][3] +
              v.m_w  * m.data[3][3];

    

    return result;



}



Vector3D vectorXvectorPart(Vector3D const& a, Vector3D const& b){
    return Vector3D(a.m_x*b.m_x,a.m_y*b.m_y,a.m_z*b.m_z);

}

float dotProduct(Vector3D const& a, Vector3D const& b){
    return a.m_x*b.m_x+a.m_y*b.m_y+a.m_z*b.m_z;

}


Matrix4x4 matrixXmatrix4x4(Matrix4x4 const& a, Matrix4x4 const& b){
    Matrix4x4 result={0};
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            for(int k=0; k<4; k++){
                result.data[i][j]+=a.data[i][k]*b.data[k][j];
            }
        }
    }
    return result;

}



Matrix4x4 getTransMatrix(Vector3D const& vec){
    Matrix4x4 result;
    result.data[0][0]=1;
    result.data[1][1]=1;
    result.data[2][2]=1;
    result.data[3][3]=1;
    result.data[3][0]=vec.m_x;
    result.data[3][1]=vec.m_y;
    result.data[3][2]=vec.m_z;
    return result;


}



 Matrix4x4 getPointAtMatrix(Vector3D const &pos, Vector3D const &target, Vector3D const &up){

    Vector3D newForward = target- pos;
    newForward.normalize();


    Vector3D a = newForward * dotProduct(up, newForward);
    Vector3D newUp = up-a;
    newUp.normalize();


    Vector3D newRight = crossProduct(newUp, newForward);


    Matrix4x4 matrix;
    matrix.data[0][0] = newRight.m_x;     matrix.data[0][1] = newRight.m_y;         matrix.data[0][2] = newRight.m_z;         matrix.data[0][3] = 0.0f;
    matrix.data[1][0] = newUp.m_x;            matrix.data[1][1] = newUp.m_y;            matrix.data[1][2] = newUp.m_z;            matrix.data[1][3] = 0.0f;
    matrix.data[2][0] = newForward.m_x;       matrix.data[2][1] = newForward.m_y;       matrix.data[2][2] = newForward.m_z;       matrix.data[2][3] = 0.0f;
    matrix.data[3][0] = pos.m_x;                      matrix.data[3][1] = pos.m_y;                      matrix.data[3][2] = pos.m_z;                      matrix.data[3][3] = 1.0f;

    return matrix;

}



Matrix4x4 quickInverse(Matrix4x4 const &m){
	Matrix4x4 matrix;
	matrix.data[0][0] = m.data[0][0]; matrix.data[0][1] = m.data[1][0]; matrix.data[0][2] = m.data[2][0]; matrix.data[0][3] = 0.0f;
	matrix.data[1][0] = m.data[0][1]; matrix.data[1][1] = m.data[1][1]; matrix.data[1][2] = m.data[2][1]; matrix.data[1][3] = 0.0f;
	matrix.data[2][0] = m.data[0][2]; matrix.data[2][1] = m.data[1][2]; matrix.data[2][2] = m.data[2][2]; matrix.data[2][3] = 0.0f;
	matrix.data[3][0] = -(m.data[3][0] * matrix.data[0][0] + m.data[3][1] * matrix.data[1][0] + m.data[3][2] * matrix.data[2][0]);
	matrix.data[3][1] = -(m.data[3][0] * matrix.data[0][1] + m.data[3][1] * matrix.data[1][1] + m.data[3][2] * matrix.data[2][1]);
	matrix.data[3][2] = -(m.data[3][0] * matrix.data[0][2] + m.data[3][1] * matrix.data[1][2] + m.data[3][2] * matrix.data[2][2]);
	matrix.data[3][3] = 1.0f;
	return matrix;
}



Vector3D vectorXplainIntersec(Vector3D const& planeP,Vector3D const& N,Vector3D const& lineStart,Vector3D const& lineEnd, float& t){
    Vector3D planeN = N;
    planeN.normalize();
	float plane_d = -dotProduct(planeN, planeP);
	float ad = dotProduct(lineStart, planeN);
	float bd = dotProduct(lineEnd, planeN);
	t = (-plane_d - ad) / (bd - ad);
	Vector3D lineStartToEnd = lineEnd-lineStart;
	Vector3D lineToIntersect = lineStartToEnd*t;
	return lineStart+lineToIntersect;



}





Matrix4x4 getIdentMatrix(){
    return {{{1,0,0,0},
            {0,1,0,0},
            {0,0,1,0},
            {0,0,0,1}}};
}