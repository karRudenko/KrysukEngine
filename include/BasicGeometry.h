#ifndef VECTOR_H
#define VECTOR_H
#include <vector>
#include <cmath>
#include <iostream>

#ifndef M_PI 
#define M_PI    3.14159265358979323846264338327950288
#endif

class Vector3D{
    public:
        float m_x=0;
        float m_y=0;
        float m_z=0;
        float m_w=1;
        Vector3D(float x, float y, float z);
        Vector3D()=default;
        Vector3D operator+(const Vector3D &n) const;
        Vector3D operator-(const Vector3D &n) const;
        Vector3D operator-(float &n);
        Vector3D operator*(float n);
        Vector3D operator/(float n);
        void normalize();
        void rotate(Vector3D const& vec);
        float length() const;


};



class Vector2D{
    public:
        float m_u=0;
        float m_v=0;
        float m_w=1;
        Vector2D(float u, float v);
        Vector2D()=default;
        Vector2D operator+(const Vector2D &n) const;
        Vector2D operator-(const Vector2D &n) const;
        Vector2D operator-(float n) const;
        Vector2D operator*(float n) const;
        Vector2D operator/(float n) const;
        void normalize();
        float length() const;


};


class Triangle2D {
public:
    Triangle2D() =default;
    
    Triangle2D(const Vector2D& a, const Vector2D& b, const Vector2D& c);


    Triangle2D(const Vector2D& a, const Vector2D& b, const Vector2D& c,const Vector2D& N);


    void computeNormal();

public:
    Vector2D m_a;
    Vector2D m_b;
    Vector2D m_c;

    Vector2D m_N;

};








class Triangle3D{
    public:
        Triangle3D() =default;
        Triangle3D(Vector3D const& a, Vector3D const& b, Vector3D const& c);
        Triangle3D(Vector3D const& a, Vector3D const& b,Vector3D const& c, Vector3D const& N);
        Vector3D m_a;
        Vector3D m_b;
        Vector3D m_c;
        Vector3D m_N;
    private:
};




Vector3D crossProduct(Vector3D const& a, Vector3D const& b, Vector3D const& c);

Vector3D crossProduct(Vector3D const& first, Vector3D const& second);

Vector3D vectorXvectorPart(Vector3D const& a, Vector3D const& b);

struct Matrix4x4{
    Matrix4x4 operator*(Matrix4x4 m);
    float data[4][4]={0};
};
Vector3D vectorXmatrix4x4(Vector3D const& v, Matrix4x4 const& m);

Matrix4x4 matrixXmatrix4x4(Matrix4x4 const& a, Matrix4x4 const& b);

Matrix4x4 getTransMatrix(Vector3D const& vec);

Matrix4x4 getRotMatrix(Vector3D const& vec);

Matrix4x4 quickInverse(Matrix4x4 const &m);

Matrix4x4 getPointAtMatrix(Vector3D const &pos, Vector3D  const &target, Vector3D const &up);


Matrix4x4 getPointAtMatrix(Vector3D const &pos, Vector3D  const &target, Vector3D const &up);

Matrix4x4 getIdentMatrix();

Vector3D vectorXplainIntersec(Vector3D const& planeP,Vector3D const& N,Vector3D const& lineStart,Vector3D const& lineEnd, float& t);

float dotProduct(Vector3D const& a, Vector3D const& b);


#endif