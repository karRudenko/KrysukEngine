// =============================================================
//  Testy jednostkowe -- KrysukEngine
//  Pokrycie: BasicGeometry (Vector2D, Vector3D, Matrix4x4,
//            Triangle2D, Triangle3D, funkcje wolne)
//            oraz ComponentFactory (ECS)
// =============================================================
#include <gtest/gtest.h>
#include <cmath>
#include "BasicGeometry.h"
#include "ComponentFactory.h"
#include "Component.h"

// ─────────────────────────────────────────────────────────────
// Pomocnik: porównanie float z tolerancją
// ─────────────────────────────────────────────────────────────
static constexpr float EPS = 1e-4f;

static bool nearlyEqual(float a, float b, float eps = EPS)
{
    return std::fabs(a - b) < eps;
}

// =============================================================
// VECTOR2D
// =============================================================
class Vector2DTest : public ::testing::Test {};

TEST_F(Vector2DTest, Konstruktor_ZapisujePola)
{
    Vector2D v(3.f, 4.f);
    EXPECT_FLOAT_EQ(v.m_u, 3.f);
    EXPECT_FLOAT_EQ(v.m_v, 4.f);
}

TEST_F(Vector2DTest, Dodawanie)
{
    Vector2D a(1.f, 2.f);
    Vector2D b(3.f, 4.f);
    Vector2D c = a + b;
    EXPECT_FLOAT_EQ(c.m_u, 4.f);
    EXPECT_FLOAT_EQ(c.m_v, 6.f);
}

TEST_F(Vector2DTest, OdejmowanieWektora)
{
    Vector2D a(5.f, 7.f);
    Vector2D b(2.f, 3.f);
    Vector2D c = a - b;
    EXPECT_FLOAT_EQ(c.m_u, 3.f);
    EXPECT_FLOAT_EQ(c.m_v, 4.f);
}

TEST_F(Vector2DTest, OdejmowanieSkalarU)
{
    Vector2D a(5.f, 7.f);
    Vector2D c = a - 2.f;
    EXPECT_FLOAT_EQ(c.m_u, 3.f);
    EXPECT_FLOAT_EQ(c.m_v, 5.f);
}

TEST_F(Vector2DTest, MnozeniePrzezSkalar)
{
    Vector2D a(2.f, 3.f);
    Vector2D c = a * 4.f;
    EXPECT_FLOAT_EQ(c.m_u, 8.f);
    EXPECT_FLOAT_EQ(c.m_v, 12.f);
}

TEST_F(Vector2DTest, DzieleniePrzezSkalar)
{
    Vector2D a(8.f, 12.f);
    Vector2D c = a / 4.f;
    EXPECT_FLOAT_EQ(c.m_u, 2.f);
    EXPECT_FLOAT_EQ(c.m_v, 3.f);
}

TEST_F(Vector2DTest, Dlugosc_TrojkaPytag)
{
    Vector2D v(3.f, 4.f);
    EXPECT_NEAR(v.length(), 5.f, EPS);
}

TEST_F(Vector2DTest, Dlugosc_ZerowegoWektora)
{
    Vector2D v(0.f, 0.f);
    EXPECT_FLOAT_EQ(v.length(), 0.f);
}

TEST_F(Vector2DTest, Normalize_DajeJednostkowy)
{
    Vector2D v(3.f, 4.f);
    v.normalize();
    EXPECT_NEAR(v.length(), 1.f, EPS);
}

TEST_F(Vector2DTest, Normalize_NieZmieniaKierunkuDodatniego)
{
    Vector2D v(0.f, 5.f);
    v.normalize();
    EXPECT_NEAR(v.m_u, 0.f, EPS);
    EXPECT_NEAR(v.m_v, 1.f, EPS);
}

TEST_F(Vector2DTest, Normalize_WektorZerowy_BezKraksy)
{
    Vector2D v(0.f, 0.f);
    // normalize dzieli przez 0 — sprawdzamy, że nie rzuca wyjątku
    EXPECT_NO_THROW(v.normalize());
}

// =============================================================
// VECTOR3D
// =============================================================
class Vector3DTest : public ::testing::Test {};

TEST_F(Vector3DTest, Konstruktor_ZapisujePola)
{
    Vector3D v(1.f, 2.f, 3.f);
    EXPECT_FLOAT_EQ(v.m_x, 1.f);
    EXPECT_FLOAT_EQ(v.m_y, 2.f);
    EXPECT_FLOAT_EQ(v.m_z, 3.f);
}

TEST_F(Vector3DTest, DomyslnyKonstruktor_DajeZera)
{
    Vector3D v;
    EXPECT_FLOAT_EQ(v.m_x, 0.f);
    EXPECT_FLOAT_EQ(v.m_y, 0.f);
    EXPECT_FLOAT_EQ(v.m_z, 0.f);
}

TEST_F(Vector3DTest, Dodawanie)
{
    Vector3D a(1.f, 2.f, 3.f);
    Vector3D b(4.f, 5.f, 6.f);
    Vector3D c = a + b;
    EXPECT_FLOAT_EQ(c.m_x, 5.f);
    EXPECT_FLOAT_EQ(c.m_y, 7.f);
    EXPECT_FLOAT_EQ(c.m_z, 9.f);
}

TEST_F(Vector3DTest, OdejmowanieWektora)
{
    Vector3D a(5.f, 7.f, 9.f);
    Vector3D b(1.f, 2.f, 3.f);
    Vector3D c = a - b;
    EXPECT_FLOAT_EQ(c.m_x, 4.f);
    EXPECT_FLOAT_EQ(c.m_y, 5.f);
    EXPECT_FLOAT_EQ(c.m_z, 6.f);
}

TEST_F(Vector3DTest, MnozeniePrzezSkalar)
{
    Vector3D a(1.f, 2.f, 3.f);
    Vector3D c = a * 3.f;
    EXPECT_FLOAT_EQ(c.m_x, 3.f);
    EXPECT_FLOAT_EQ(c.m_y, 6.f);
    EXPECT_FLOAT_EQ(c.m_z, 9.f);
}

TEST_F(Vector3DTest, DzieleniePrzezSkalar)
{
    Vector3D a(3.f, 6.f, 9.f);
    Vector3D c = a / 3.f;
    EXPECT_FLOAT_EQ(c.m_x, 1.f);
    EXPECT_FLOAT_EQ(c.m_y, 2.f);
    EXPECT_FLOAT_EQ(c.m_z, 3.f);
}

TEST_F(Vector3DTest, Dlugosc_TrojkaPytag3D)
{
    // sqrt(1^2 + 2^2 + 2^2) = 3
    Vector3D v(1.f, 2.f, 2.f);
    EXPECT_NEAR(v.length(), 3.f, EPS);
}

TEST_F(Vector3DTest, Dlugosc_WektoraJednostkowego)
{
    Vector3D v(1.f, 0.f, 0.f);
    EXPECT_NEAR(v.length(), 1.f, EPS);
}

TEST_F(Vector3DTest, Normalize_DajeJednostkowy)
{
    Vector3D v(3.f, 0.f, 4.f);
    v.normalize();
    EXPECT_NEAR(v.length(), 1.f, EPS);
}

TEST_F(Vector3DTest, Normalize_KierunekX)
{
    Vector3D v(5.f, 0.f, 0.f);
    v.normalize();
    EXPECT_NEAR(v.m_x, 1.f, EPS);
    EXPECT_NEAR(v.m_y, 0.f, EPS);
    EXPECT_NEAR(v.m_z, 0.f, EPS);
}

TEST_F(Vector3DTest, Rotate_Zerowy_BezZmiany)
{
    // Obrót o (0,0,0) nie zmienia wektora
    Vector3D v(1.f, 0.f, 0.f);
    v.rotate(Vector3D(0.f, 0.f, 0.f));
    EXPECT_NEAR(v.m_x, 1.f, EPS);
    EXPECT_NEAR(v.m_y, 0.f, EPS);
    EXPECT_NEAR(v.m_z, 0.f, EPS);
}

TEST_F(Vector3DTest, Rotate_360StopniDajeOryginal)
{
    // Obrót o 360° wokół osi Y powinien zwrócić oryginalny wektor
    Vector3D v(1.f, 0.f, 0.f);
    v.rotate(Vector3D(0.f, 360.f, 0.f));
    EXPECT_NEAR(v.m_x, 1.f, 1e-3f);
    EXPECT_NEAR(v.m_y, 0.f, 1e-3f);
    EXPECT_NEAR(v.m_z, 0.f, 1e-3f);
}

// =============================================================
// FUNKCJE WOLNE -- dotProduct, crossProduct
// =============================================================
class FunkcjeGeometriiTest : public ::testing::Test {};

TEST_F(FunkcjeGeometriiTest, DotProduct_ProstopadleWektory)
{
    Vector3D a(1.f, 0.f, 0.f);
    Vector3D b(0.f, 1.f, 0.f);
    EXPECT_NEAR(dotProduct(a, b), 0.f, EPS);
}

TEST_F(FunkcjeGeometriiTest, DotProduct_RownolegleWektory)
{
    Vector3D a(2.f, 0.f, 0.f);
    Vector3D b(3.f, 0.f, 0.f);
    EXPECT_NEAR(dotProduct(a, b), 6.f, EPS);
}

TEST_F(FunkcjeGeometriiTest, DotProduct_PrzeciwneWektory)
{
    Vector3D a(1.f, 0.f, 0.f);
    Vector3D b(-1.f, 0.f, 0.f);
    EXPECT_NEAR(dotProduct(a, b), -1.f, EPS);
}

TEST_F(FunkcjeGeometriiTest, DotProduct_OgolnyPrzyklad)
{
    // (1,2,3) · (4,5,6) = 4+10+18 = 32
    Vector3D a(1.f, 2.f, 3.f);
    Vector3D b(4.f, 5.f, 6.f);
    EXPECT_NEAR(dotProduct(a, b), 32.f, EPS);
}

TEST_F(FunkcjeGeometriiTest, CrossProduct_DwaWektory_Prostopadlosc)
{
    Vector3D x(1.f, 0.f, 0.f);
    Vector3D y(0.f, 1.f, 0.f);
    Vector3D z = crossProduct(x, y);
    // x × y = (0,0,1)
    EXPECT_NEAR(z.m_x, 0.f, EPS);
    EXPECT_NEAR(z.m_y, 0.f, EPS);
    EXPECT_NEAR(z.m_z, 1.f, EPS);
}

TEST_F(FunkcjeGeometriiTest, CrossProduct_TrzeciBazowy)
{
    // y × x = (0,0,-1)
    Vector3D x(1.f, 0.f, 0.f);
    Vector3D y(0.f, 1.f, 0.f);
    Vector3D res = crossProduct(y, x);
    EXPECT_NEAR(res.m_z, -1.f, EPS);
}

TEST_F(FunkcjeGeometriiTest, CrossProduct_TrzyPunkty_NormalTriangla)
{
    // Trójkąt w płaszczyźnie XY -- normalna powinna być równoległa do Z
    Vector3D a(0.f, 0.f, 0.f);
    Vector3D b(1.f, 0.f, 0.f);
    Vector3D c(0.f, 1.f, 0.f);
    Vector3D n = crossProduct(a, b, c);
    // Normalna jest (0,0,1) lub (0,0,-1)
    EXPECT_NEAR(n.m_x, 0.f, EPS);
    EXPECT_NEAR(n.m_y, 0.f, EPS);
    EXPECT_GT(std::fabs(n.m_z), 0.5f);
}

TEST_F(FunkcjeGeometriiTest, CrossProduct_RownolegleDajaZerowy)
{
    Vector3D a(2.f, 0.f, 0.f);
    Vector3D b(4.f, 0.f, 0.f);
    Vector3D res = crossProduct(a, b);
    EXPECT_NEAR(res.m_x, 0.f, EPS);
    EXPECT_NEAR(res.m_y, 0.f, EPS);
    EXPECT_NEAR(res.m_z, 0.f, EPS);
}

// =============================================================
// MATRIX4x4
// =============================================================
class Matrix4x4Test : public ::testing::Test {};

TEST_F(Matrix4x4Test, GetIdentMatrix_MaJedynkiNaPrzekatnej)
{
    Matrix4x4 m = getIdentMatrix();
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_FLOAT_EQ(m.data[i][j], (i == j) ? 1.f : 0.f)
                << "Blad na pozycji [" << i << "][" << j << "]";
}

TEST_F(Matrix4x4Test, MatrixXMatrix_IdentPrzezIdent_DaIdent)
{
    Matrix4x4 I = getIdentMatrix();
    Matrix4x4 res = matrixXmatrix4x4(I, I);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_NEAR(res.data[i][j], (i == j) ? 1.f : 0.f, EPS);
}

TEST_F(Matrix4x4Test, MatrixXMatrix_IdentPrzezDowolna_DajeTaSama)
{
    Matrix4x4 I = getIdentMatrix();
    Matrix4x4 M;
    M.data[0][0]=2; M.data[0][1]=3;
    M.data[1][2]=5; M.data[2][3]=7;
    M.data[3][3]=1;

    Matrix4x4 res = matrixXmatrix4x4(I, M);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_NEAR(res.data[i][j], M.data[i][j], EPS);
}

TEST_F(Matrix4x4Test, GetTransMatrix_UstawiaTranslacje)
{
    Matrix4x4 T = getTransMatrix(Vector3D(3.f, 5.f, 7.f));
    EXPECT_FLOAT_EQ(T.data[3][0], 3.f);
    EXPECT_FLOAT_EQ(T.data[3][1], 5.f);
    EXPECT_FLOAT_EQ(T.data[3][2], 7.f);
    EXPECT_FLOAT_EQ(T.data[0][0], 1.f);
    EXPECT_FLOAT_EQ(T.data[1][1], 1.f);
    EXPECT_FLOAT_EQ(T.data[2][2], 1.f);
    EXPECT_FLOAT_EQ(T.data[3][3], 1.f);
}

TEST_F(Matrix4x4Test, VectorXMatrix_IdentNieZmieniaWektora)
{
    Matrix4x4 I = getIdentMatrix();
    Vector3D v(2.f, 3.f, 4.f);
    Vector3D res = vectorXmatrix4x4(v, I);
    EXPECT_NEAR(res.m_x, 2.f, EPS);
    EXPECT_NEAR(res.m_y, 3.f, EPS);
    EXPECT_NEAR(res.m_z, 4.f, EPS);
}

TEST_F(Matrix4x4Test, QuickInverse_IdentPrzezIdentDaIdent)
{
    Matrix4x4 I = getIdentMatrix();
    Matrix4x4 inv = quickInverse(I);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_NEAR(inv.data[i][j], (i == j) ? 1.f : 0.f, EPS);
}

TEST_F(Matrix4x4Test, QuickInverse_OdwracaMacierzObrotu)
{
    // Macierz obrotu: R * R^{-1} = I
    Vector3D angles(0.f, 45.f, 0.f);
    Matrix4x4 R = getRotMatrix(angles);
    Matrix4x4 Rinv = quickInverse(R);
    Matrix4x4 product = matrixXmatrix4x4(R, Rinv);
    // Przekątna bliska 1, reszta bliska 0
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            EXPECT_NEAR(product.data[i][j], (i == j) ? 1.f : 0.f, 1e-3f)
                << "Blad na pozycji [" << i << "][" << j << "]";
}

TEST_F(Matrix4x4Test, GetRotMatrix_Zero_DaIdentycznosc)
{
    Matrix4x4 R = getRotMatrix(Vector3D(0.f, 0.f, 0.f));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_NEAR(R.data[i][j], (i == j) ? 1.f : 0.f, EPS);
}

TEST_F(Matrix4x4Test, GetRotMatrix_90Y_ObraCaOsX)
{
    // Obrót o 90° wokół Y: (1,0,0) -> (0,0,-1) (lub (0,0,1) zależnie od konwencji)
    Vector3D v(1.f, 0.f, 0.f);
    v.rotate(Vector3D(0.f, 90.f, 0.f));
    EXPECT_NEAR(v.m_y, 0.f, 1e-3f);
    // składowa x lub z powinna być ~1
    EXPECT_GT(std::fabs(v.m_x) + std::fabs(v.m_z), 0.9f);
}

TEST_F(Matrix4x4Test, OperatorMnozenia_Przemiennosc_IdentPrzezIdent)
{
    Matrix4x4 I = getIdentMatrix();
    Matrix4x4 res = I * I;
    EXPECT_NEAR(res.data[0][0], 1.f, EPS);
    EXPECT_NEAR(res.data[1][1], 1.f, EPS);
}

// =============================================================
// TRIANGLE3D
// =============================================================
class Triangle3DTest : public ::testing::Test {};

TEST_F(Triangle3DTest, Konstruktor_ZapisujePunkty)
{
    Vector3D a(0.f,0.f,0.f), b(1.f,0.f,0.f), c(0.f,1.f,0.f);
    Triangle3D t(a, b, c);
    EXPECT_FLOAT_EQ(t.m_a.m_x, 0.f);
    EXPECT_FLOAT_EQ(t.m_b.m_x, 1.f);
    EXPECT_FLOAT_EQ(t.m_c.m_y, 1.f);
}

TEST_F(Triangle3DTest, Normalna_ProstopadlaDoPlaszczyzny)
{
    // Trójkąt w XY -- normalna równoległa do Z
    Vector3D a(0.f,0.f,0.f), b(1.f,0.f,0.f), c(0.f,1.f,0.f);
    Triangle3D t(a, b, c);
    // Normalna jest znormalizowana
    EXPECT_NEAR(t.m_N.length(), 1.f, EPS);
    // Normalna prostopadła do krawędzi AB i AC
    Vector3D AB = b - a;
    Vector3D AC = c - a;
    EXPECT_NEAR(dotProduct(t.m_N, AB), 0.f, EPS);
    EXPECT_NEAR(dotProduct(t.m_N, AC), 0.f, EPS);
}

TEST_F(Triangle3DTest, KonstruktorZNormalna_UzywaJejPoBezposredniej)
{
    Vector3D a(0.f,0.f,0.f), b(1.f,0.f,0.f), c(0.f,1.f,0.f);
    Vector3D N(0.f, 0.f, 5.f);
    Triangle3D t(a, b, c, N);
    // Normalna zostaje znormalizowana
    EXPECT_NEAR(t.m_N.length(), 1.f, EPS);
    EXPECT_NEAR(t.m_N.m_z, 1.f, EPS);
}

// =============================================================
// TRIANGLE2D
// =============================================================
class Triangle2DTest : public ::testing::Test {};

TEST_F(Triangle2DTest, Konstruktor_ZapisujePunkty)
{
    Vector2D a(0.f,0.f), b(1.f,0.f), c(0.f,1.f);
    Triangle2D t(a, b, c);
    EXPECT_FLOAT_EQ(t.m_a.m_u, 0.f);
    EXPECT_FLOAT_EQ(t.m_b.m_u, 1.f);
    EXPECT_FLOAT_EQ(t.m_c.m_v, 1.f);
}

TEST_F(Triangle2DTest, Normalna_JestZnormalizowana)
{
    Vector2D a(0.f,0.f), b(4.f,0.f), c(0.f,3.f);
    Triangle2D t(a, b, c);
    EXPECT_NEAR(t.m_N.length(), 1.f, EPS);
}

TEST_F(Triangle2DTest, ComputeNormal_ProstopadlaDoKrawedzi)
{
    // krawędź AB = (1,0) -- normalna powinna być (0,1) lub (0,-1)
    Vector2D a(0.f,0.f), b(1.f,0.f), c(0.f,1.f);
    Triangle2D t(a, b, c);
    // Normalna prostopadła do krawędzi AB: dot(N, AB)==0
    Vector2D AB(1.f, 0.f);
    float dot = t.m_N.m_u * AB.m_u + t.m_N.m_v * AB.m_v;
    EXPECT_NEAR(dot, 0.f, EPS);
}

TEST_F(Triangle2DTest, KonstruktorZNormalna_Normalizuje)
{
    Vector2D a(0.f,0.f), b(1.f,0.f), c(0.f,1.f);
    Vector2D N(0.f, 10.f);
    Triangle2D t(a, b, c, N);
    EXPECT_NEAR(t.m_N.length(), 1.f, EPS);
    EXPECT_NEAR(t.m_N.m_v, 1.f, EPS);
}

// =============================================================
// vectorXplainIntersec
// =============================================================
class PrzeciecieZPlanem : public ::testing::Test {};

TEST_F(PrzeciecieZPlanem, SrodekOdcinka_NaPlaszczyznie)
{
    // Płaszczyzna: y = 0, normalna (0,1,0)
    Vector3D planeP(0.f, 0.f, 0.f);
    Vector3D N(0.f, 1.f, 0.f);
    Vector3D start(0.f, -1.f, 0.f);
    Vector3D end(0.f,  1.f, 0.f);
    float t;
    Vector3D inter = vectorXplainIntersec(planeP, N, start, end, t);
    EXPECT_NEAR(inter.m_y, 0.f, EPS);
    EXPECT_NEAR(t, 0.5f, EPS);
}

TEST_F(PrzeciecieZPlanem, PrzeciecieWEkscentrycznym_PunkcieOdcinka)
{
    // Płaszczyzna: x = 3, normalna (1,0,0)
    Vector3D planeP(3.f, 0.f, 0.f);
    Vector3D N(1.f, 0.f, 0.f);
    Vector3D start(0.f, 0.f, 0.f);
    Vector3D end(6.f, 0.f, 0.f);
    float t;
    Vector3D inter = vectorXplainIntersec(planeP, N, start, end, t);
    EXPECT_NEAR(inter.m_x, 3.f, EPS);
    EXPECT_NEAR(t, 0.5f, EPS);
}

TEST_F(PrzeciecieZPlanem, Parametr_t_Rowny1_GdyKoniecNaPlanie)
{
    Vector3D planeP(0.f, 1.f, 0.f);
    Vector3D N(0.f, 1.f, 0.f);
    Vector3D start(0.f, 0.f, 0.f);
    Vector3D end(0.f, 1.f, 0.f);
    float t;
    vectorXplainIntersec(planeP, N, start, end, t);
    EXPECT_NEAR(t, 1.f, EPS);
}

// =============================================================
// getPointAtMatrix
// =============================================================
class PointAtMatrixTest : public ::testing::Test {};

TEST_F(PointAtMatrixTest, MacierzNieJestZerowa)
{
    Vector3D pos(0.f, 0.f, 0.f);
    Vector3D target(0.f, 0.f, 1.f);
    Vector3D up(0.f, 1.f, 0.f);
    Matrix4x4 m = getPointAtMatrix(pos, target, up);
    // Przynajmniej kilka pól musi być niezerowych
    float sum = 0.f;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            sum += std::fabs(m.data[i][j]);
    EXPECT_GT(sum, 0.5f);
}

TEST_F(PointAtMatrixTest, PozycjaPrzechowywanaWOstatanimWierszu)
{
    Vector3D pos(3.f, 5.f, 7.f);
    Vector3D target(3.f, 5.f, 8.f);
    Vector3D up(0.f, 1.f, 0.f);
    Matrix4x4 m = getPointAtMatrix(pos, target, up);
    EXPECT_NEAR(m.data[3][0], 3.f, EPS);
    EXPECT_NEAR(m.data[3][1], 5.f, EPS);
    EXPECT_NEAR(m.data[3][2], 7.f, EPS);
}

// =============================================================
// COMPONENT FACTORY (ECS)
// =============================================================

// Minimalny komponent testowy (nie wymaga SDL)
struct TestComponentA : public Component {
    void update(Uint32) override {}
    void start() override {}
    void init(YAML::Node const&) override {}
    int value = 42;
};

struct TestComponentB : public Component {
    void update(Uint32) override {}
    void start() override {}
    void init(YAML::Node const&) override {}
    float data = 3.14f;
};

class ComponentFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Czyścimy fabrykę przed każdym testem
        ComponentFactory::init().m_creators.clear();
    }
};

TEST_F(ComponentFactoryTest, Rejestracja_ZwracaTrueGdyNowaKlasa)
{
    bool ok = ComponentFactory::init().registerComponent(
        "TestA",
        []() { return std::make_unique<TestComponentA>(); }
    );
    EXPECT_TRUE(ok);
}

TEST_F(ComponentFactoryTest, Rejestracja_ZwracaFalseGdyDuplicat)
{
    ComponentFactory::init().registerComponent(
        "TestA",
        []() { return std::make_unique<TestComponentA>(); }
    );
    bool ok = ComponentFactory::init().registerComponent(
        "TestA",
        []() { return std::make_unique<TestComponentA>(); }
    );
    EXPECT_FALSE(ok);
}

TEST_F(ComponentFactoryTest, Create_ZnanaNazwa_ZwracaNieNullptr)
{
    ComponentFactory::init().registerComponent(
        "TestA",
        []() { return std::make_unique<TestComponentA>(); }
    );
    auto comp = ComponentFactory::init().create("TestA");
    EXPECT_NE(comp, nullptr);
}

TEST_F(ComponentFactoryTest, Create_NieznanaKlasa_ZwracaNullptr)
{
    auto comp = ComponentFactory::init().create("NieIstniejacyKomponent");
    EXPECT_EQ(comp, nullptr);
}

TEST_F(ComponentFactoryTest, Create_ZwracaPoprawnyTypKomponentu)
{
    ComponentFactory::init().registerComponent(
        "TestA",
        []() { return std::make_unique<TestComponentA>(); }
    );
    auto comp = ComponentFactory::init().create("TestA");
    ASSERT_NE(comp, nullptr);
    auto* typed = dynamic_cast<TestComponentA*>(comp.get());
    EXPECT_NE(typed, nullptr);
    EXPECT_EQ(typed->value, 42);
}

TEST_F(ComponentFactoryTest, Create_DwaRozneKomponenty)
{
    ComponentFactory::init().registerComponent(
        "TestA",
        []() { return std::make_unique<TestComponentA>(); }
    );
    ComponentFactory::init().registerComponent(
        "TestB",
        []() { return std::make_unique<TestComponentB>(); }
    );
    auto compA = ComponentFactory::init().create("TestA");
    auto compB = ComponentFactory::init().create("TestB");
    EXPECT_NE(dynamic_cast<TestComponentA*>(compA.get()), nullptr);
    EXPECT_NE(dynamic_cast<TestComponentB*>(compB.get()), nullptr);
}

TEST_F(ComponentFactoryTest, Create_KazdyCallZwracaNowaInstancje)
{
    ComponentFactory::init().registerComponent(
        "TestA",
        []() { return std::make_unique<TestComponentA>(); }
    );
    auto c1 = ComponentFactory::init().create("TestA");
    auto c2 = ComponentFactory::init().create("TestA");
    EXPECT_NE(c1.get(), c2.get());
}

TEST_F(ComponentFactoryTest, Rejestracja_WieluKomponentow_WszystkieDostepne)
{
    for (int i = 0; i < 5; i++) {
        std::string name = "Komp" + std::to_string(i);
        ComponentFactory::init().registerComponent(
            name,
            []() { return std::make_unique<TestComponentA>(); }
        );
    }
    for (int i = 0; i < 5; i++) {
        std::string name = "Komp" + std::to_string(i);
        EXPECT_NE(ComponentFactory::init().create(name), nullptr) << "Brak: " << name;
    }
}

// =============================================================
// main
// =============================================================
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}