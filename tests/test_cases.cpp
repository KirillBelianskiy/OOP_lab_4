#include "gtest/gtest.h"
#include "../include/classes.h"
#include <sstream>
#include <cmath>

TEST(PointTest, ConstructorAndGetters)
{
    Point<int> p1(10, 20);
    EXPECT_EQ(p1.get_x(), 10);
    EXPECT_EQ(p1.get_y(), 20);

    Point<double> p2(1.5, -2.5);
    EXPECT_DOUBLE_EQ(p2.get_x(), 1.5);
    EXPECT_DOUBLE_EQ(p2.get_y(), -2.5);

    Point<int> p_default;
    EXPECT_EQ(p_default.get_x(), 0);
    EXPECT_EQ(p_default.get_y(), 0);
}

TEST(PointTest, StreamInput)
{
    std::istringstream iss("123 456");
    Point<int> p;
    iss >> p;
    EXPECT_EQ(p.get_x(), 123);
    EXPECT_EQ(p.get_y(), 456);
}

// --- Тесты для класса Rhombus ---

class RhombusTest : public ::testing::Test
{
protected:
    Point<double> p1{1.0, 0.0};
    Point<double> p2{0.0, 2.0};
    Point<double> p3{-1.0, 0.0};
    Point<double> p4{0.0, -2.0};
    Point<double> invalid_p{10.0, 10.0};
};

TEST_F(RhombusTest, ValidConstructor)
{
    EXPECT_NO_THROW(Rhombus<double> r(p1, p2, p3, p4));
}

TEST_F(RhombusTest, InvalidConstructorThrows)
{
    EXPECT_THROW(Rhombus<double> r(p1, p2, p3, invalid_p), std::invalid_argument);
    EXPECT_THROW(Rhombus<double> r(p1, p1, p1, p1), std::invalid_argument);
}

TEST_F(RhombusTest, FindCenter)
{
    Rhombus<double> r(p1, p2, p3, p4);
    Point<double> center = r.find_center();
    EXPECT_NEAR(center.get_x(), 0.0, EPSILON);
    EXPECT_NEAR(center.get_y(), 0.0, EPSILON);
}

TEST_F(RhombusTest, AreaCalculation)
{
    Rhombus<double> r(p1, p2, p3, p4);
    double area = static_cast<double>(r);
    EXPECT_NEAR(area, 4.0, EPSILON);
}

TEST_F(RhombusTest, ReadValid)
{
    std::istringstream iss("1 0 0 2 -1 0 0 -2");
    Rhombus<double> r;
    EXPECT_NO_THROW(r.read(iss));
    double area = static_cast<double>(r);
    EXPECT_NEAR(area, 4.0, EPSILON);
}

TEST_F(RhombusTest, ReadInvalidThrows)
{
    std::istringstream iss("1 1 2 2 3 3 4 4");
    Rhombus<double> r;
    EXPECT_THROW(r.read(iss), std::invalid_argument);
}

TEST_F(RhombusTest, CopyConstructor)
{
    Rhombus<double> r1(p1, p2, p3, p4);
    Rhombus<double> r2 = r1;
    EXPECT_NEAR(static_cast<double>(r1), 4.0, EPSILON);
    EXPECT_NEAR(static_cast<double>(r2), 4.0, EPSILON);
    Point<double> center = r2.find_center();
    EXPECT_NEAR(center.get_x(), 0.0, EPSILON);
}

TEST_F(RhombusTest, MoveConstructor)
{
    Rhombus<double> r1(p1, p2, p3, p4);
    Rhombus<double> r2 = std::move(r1);
    EXPECT_NEAR(static_cast<double>(r2), 4.0, EPSILON);
    Point<double> center = r2.find_center();
    EXPECT_NEAR(center.get_x(), 0.0, EPSILON);
}

// --- Тесты для класса Pentagon ---

class PentagonTest : public ::testing::Test
{
protected:
    Point<double> p1{1.0, 0.0};
    Point<double> p2{std::cos(1 * 2 * M_PI / 5), std::sin(1 * 2 * M_PI / 5)};
    Point<double> p3{std::cos(2 * 2 * M_PI / 5), std::sin(2 * 2 * M_PI / 5)};
    Point<double> p4{std::cos(3 * 2 * M_PI / 5), std::sin(3 * 2 * M_PI / 5)};
    Point<double> p5{std::cos(4 * 2 * M_PI / 5), std::sin(4 * 2 * M_PI / 5)};
    Point<double> invalid_p{10.0, 10.0};
};

TEST_F(PentagonTest, ValidConstructor)
{
    EXPECT_NO_THROW(Pentagon<double> p(p1, p2, p3, p4, p5));
}

TEST_F(PentagonTest, InvalidConstructorThrows)
{
    EXPECT_THROW(Pentagon<double> p(p1, p2, p3, p4, invalid_p), std::invalid_argument);
}

TEST_F(PentagonTest, FindCenter)
{
    Pentagon<double> p(p1, p2, p3, p4, p5);
    Point<double> center = p.find_center();
    EXPECT_NEAR(center.get_x(), 0.0, EPSILON);
    EXPECT_NEAR(center.get_y(), 0.0, EPSILON);
}

TEST_F(PentagonTest, AreaCalculation)
{
    Pentagon<double> p(p1, p2, p3, p4, p5);
    double area = static_cast<double>(p);
    double expected_area = (5.0 / 2.0) * 1.0 * 1.0 * std::sin(2 * M_PI / 5);
    EXPECT_NEAR(area, expected_area, EPSILON);
}

// --- Тесты для класса Hexagon ---

class HexagonTest : public ::testing::Test
{
protected:
    Point<double> p1{1.0, 0.0};
    Point<double> p2{0.5, 0.86602540378};
    Point<double> p3{-0.5, 0.86602540378};
    Point<double> p4{-1.0, 0.0};
    Point<double> p5{-0.5, -0.86602540378};
    Point<double> p6{0.5, -0.86602540378};
    Point<double> invalid_p{10.0, 10.0};
};

TEST_F(HexagonTest, ValidConstructor)
{
    EXPECT_NO_THROW(Hexagon<double> h(p1, p2, p3, p4, p5, p6));
}

TEST_F(HexagonTest, InvalidConstructorThrows)
{
    EXPECT_THROW(Hexagon<double> h(p1, p2, p3, p4, p5, invalid_p), std::invalid_argument);
}

TEST_F(HexagonTest, FindCenter)
{
    Hexagon<double> h(p1, p2, p3, p4, p5, p6);
    Point<double> center = h.find_center();
    EXPECT_NEAR(center.get_x(), 0.0, EPSILON);
    EXPECT_NEAR(center.get_y(), 0.0, EPSILON);
}

TEST_F(HexagonTest, AreaCalculation)
{
    Hexagon<double> h(p1, p2, p3, p4, p5, p6);
    double area = static_cast<double>(h);
    double expected_area = (3.0 * std::sqrt(3.0) / 2.0) * 1.0 * 1.0;
    EXPECT_NEAR(area, expected_area, EPSILON);
}

// --- Тесты полиморфизма ---

TEST(FigurePolymorphismTest, PolymorphicCalls)
{
    std::unique_ptr<Figure<double>> fig1 = std::make_unique<Rhombus<double>>(
        Point<double>{2.0, 0.0},
        Point<double>{0.0, 1.0},
        Point<double>{-2.0, 0.0},
        Point<double>{0.0, -1.0}
    );

    double area1 = static_cast<double>(*fig1);
    Point<double> center1 = fig1->find_center();

    EXPECT_NEAR(area1, 4.0, EPSILON);
    EXPECT_NEAR(center1.get_x(), 0.0, EPSILON);
    EXPECT_NEAR(center1.get_y(), 0.0, EPSILON);

    std::unique_ptr<Figure<double>> fig2 = std::make_unique<Hexagon<double>>(
        Point<double>{1.0, 0.0},
        Point<double>{0.5, 0.86602540378},
        Point<double>{-0.5, 0.86602540378},
        Point<double>{-1.0, 0.0},
        Point<double>{-0.5, -0.86602540378},
        Point<double>{0.5, -0.86602540378}
    );

    double area2 = static_cast<double>(*fig2);
    Point<double> center2 = fig2->find_center();
    double expected_area_hex = (3.0 * std::sqrt(3.0) / 2.0);

    EXPECT_NEAR(area2, expected_area_hex, EPSILON);
    EXPECT_NEAR(center2.get_x(), 0.0, EPSILON);
    EXPECT_NEAR(center2.get_y(), 0.0, EPSILON);
}
