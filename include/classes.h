#pragma once

#include <concepts>
#include <memory>
#include <type_traits>
#include <stdexcept>
#include <iostream>

#define EPSILON 1e-9

bool eq(double a, double b)
{
    return std::abs(a - b) < EPSILON;
}

bool less(double a, double b)
{
    return (b - a) > EPSILON;
}

bool greater(double a, double b)
{
    return (a - b) > EPSILON;
}

template <typename T>
concept IsScalar = std::is_scalar_v<T>;

template <IsScalar T>
class Point
{
private:
    T x{0};
    T y{0};

public:
    Point() = default;

    Point(T x, T y) : x(x), y(y)
    {
    }

    T get_x() const
    {
        return x;
    }

    T get_y() const
    {
        return y;
    }

    template <IsScalar U>
    friend std::istream& operator>>(std::istream& is, Point<U>& p);
};

template <IsScalar T>
std::istream& operator>>(std::istream& is, Point<T>& p)
{
    is >> p.x >> p.y;
    return is;
}

template <typename T>
class Figure
{
protected:
    std::unique_ptr<Point<T>[]> points;
    size_t num_points;

protected:
    double find_signed_area() const
    {
        if (this->num_points < 3)
        {
            return 0.0;
        }

        double area = 0.0;
        for (size_t i = 0; i < num_points; ++i)
        {
            size_t j = (i + 1) % num_points;
            area += points[i].get_x() * points[j].get_y();
            area -= points[j].get_x() * points[i].get_y();
        }
        return area / 2.0;
    }

public:
    Figure() : points(nullptr), num_points(0)
    {
    }

    virtual ~Figure() = default;

    virtual Point<T> find_center() const
    {
        if (num_points == 0) return Point<T>(0, 0);
        if (num_points == 1) return points[0];
        if (num_points == 2)
        {
            T x = (points[0].get_x() + points[1].get_x()) / 2;
            T y = (points[0].get_y() + points[1].get_y()) / 2;
            return Point<T>(x, y);
        }

        T c_x = 0;
        T c_y = 0;
        double signedArea = find_signed_area();

        if (eq(signedArea, 0.0))
        {
            T total_x = 0, total_y = 0;
            for (size_t i = 0; i < num_points; ++i)
            {
                total_x += points[i].get_x();
                total_y += points[i].get_y();
            }
            return Point<T>(total_x / num_points, total_y / num_points);
        }

        for (size_t i = 0; i < num_points; ++i)
        {
            size_t j = (i + 1) % num_points;
            double cross_product = (points[i].get_x() * points[j].get_y()) -
                (points[j].get_x() * points[i].get_y());
            c_x += (points[i].get_x() + points[j].get_x()) * cross_product;
            c_y += (points[i].get_y() + points[j].get_y()) * cross_product;
        }
        return Point<T>(c_x / (6.0 * signedArea), c_y / (6.0 * signedArea));
    }

    virtual operator double() const
    {
        return std::abs(find_signed_area());
    }

    virtual void print(std::ostream& os) const
    {
        if (num_points == 0)
        {
            os << "Figure is empty" << std::endl;
            return;
        }
        os << "Vertex coordinates:" << std::endl;
        for (size_t i = 0; i < num_points; ++i)
        {
            os << "  Point " << i + 1 << ": ("
                << points[i].get_x() << ", "
                << points[i].get_y() << ")" << std::endl;
        }
    }

    virtual void read(std::istream& is) = 0;
};


template <typename T>
bool check_rhombus(Point<T> a, Point<T> b, Point<T> c, Point<T> d)
{
    T mid1_x = (a.get_x() + c.get_x()) / 2;
    T mid1_y = (a.get_y() + c.get_y()) / 2;
    T mid2_x = (b.get_x() + d.get_x()) / 2;
    T mid2_y = (b.get_y() + d.get_y()) / 2;
    if (!eq(mid1_x, mid2_x) || !eq(mid1_y, mid2_y)) return true;

    T vec1_x = c.get_x() - a.get_x();
    T vec1_y = c.get_y() - a.get_y();
    T vec2_x = d.get_x() - b.get_x();
    T vec2_y = d.get_y() - b.get_y();
    T dot_product = vec1_x * vec2_x + vec1_y * vec2_y;
    if (!eq(dot_product, 0)) return true;

    T len_vec1_sq = vec1_x * vec1_x + vec1_y * vec1_y;
    T len_vec2_sq = vec2_x * vec2_x + vec2_y * vec2_y;
    if (eq(len_vec1_sq, 0) || eq(len_vec2_sq, 0)) return true;

    return false;
}

template <IsScalar T>
double distance_sq(const Point<T>& a, const Point<T>& b)
{
    double dx = a.get_x() - b.get_x();
    double dy = a.get_y() - b.get_y();
    return dx * dx + dy * dy;
}

template <IsScalar T>
bool check_regular_ngon(Point<T> points[], size_t n)
{
    if (n < 3) return false;

    T center_x = 0;
    T center_y = 0;
    for (size_t i = 0; i < n; ++i)
    {
        center_x += points[i].get_x();
        center_y += points[i].get_y();
    }
    Point<T> center(center_x / n, center_y / n);

    double dist_sq_center = distance_sq(center, points[0]);
    for (size_t i = 1; i < n; ++i)
    {
        if (!eq(distance_sq(center, points[i]), dist_sq_center))
        {
            return false;
        }
    }

    double side_len_sq = distance_sq(points[0], points[1]);
    for (size_t i = 1; i < n; ++i)
    {
        size_t j = (i + 1) % n;
        if (!eq(distance_sq(points[i], points[j]), side_len_sq))
        {
            return false;
        }
    }

    return true;
}

template <typename T>
class Rhombus : public Figure<T>
{
public:
    Rhombus() : Figure<T>()
    {
    }

    Rhombus(Point<T> a, Point<T> b, Point<T> c, Point<T> d) : Figure<T>()
    {
        if (check_rhombus(a, b, c, d))
        {
            throw std::invalid_argument("Error: Rhombus is not defined");
        }
        this->num_points = 4;
        this->points = std::make_unique<Point<T>[]>(this->num_points);
        this->points[0] = a;
        this->points[1] = b;
        this->points[2] = c;
        this->points[3] = d;
    }

    Rhombus(const Rhombus& other)
    {
        this->num_points = other.num_points;
        this->points = std::make_unique<Point<T>[]>(this->num_points);
        for (size_t i = 0; i < this->num_points; ++i)
        {
            this->points[i] = other.points[i];
        }
    }

    Rhombus(Rhombus&& other) noexcept
    {
        this->num_points = other.num_points;
        this->points = std::move(other.points);
    }

    void read(std::istream& is) override
    {
        Point<T> a, b, c, d;
        std::cout << "Enter 4 points (x y) for the Rhombus (A, B, C, D):" << std::endl;
        std::cout << "Point A: ";
        is >> a;
        std::cout << "Point B: ";
        is >> b;
        std::cout << "Point C: ";
        is >> c;
        std::cout << "Point D: ";
        is >> d;

        if (check_rhombus(a, b, c, d))
        {
            throw std::invalid_argument("Error: Input points do not form a rhombus");
        }

        this->num_points = 4;
        this->points = std::make_unique<Point<T>[]>(this->num_points);
        this->points[0] = a;
        this->points[1] = b;
        this->points[2] = c;
        this->points[3] = d;
    }
};

template <typename T>
class Pentagon : public Figure<T>
{
public:
    Pentagon() : Figure<T>()
    {
    }

    Pentagon(Point<T> p1, Point<T> p2, Point<T> p3, Point<T> p4, Point<T> p5) : Figure<T>()
    {
        Point<T> input_points[] = {p1, p2, p3, p4, p5};
        if (!check_regular_ngon(input_points, 5))
        {
            throw std::invalid_argument("Error: Points do not form a regular pentagon");
        }

        this->num_points = 5;
        this->points = std::make_unique<Point<T>[]>(this->num_points);
        for (size_t i = 0; i < 5; ++i) this->points[i] = input_points[i];
    }

    Pentagon(const Pentagon& other)
    {
        this->num_points = other.num_points;
        this->points = std::make_unique<Point<T>[]>(this->num_points);
        for (size_t i = 0; i < this->num_points; ++i)
        {
            this->points[i] = other.points[i];
        }
    }

    Pentagon(Pentagon&& other) noexcept
    {
        this->num_points = other.num_points;
        this->points = std::move(other.points);
    }

    void read(std::istream& is) override
    {
        Point<T> input_points[5];
        std::cout << "Enter 5 points (x y) for the Regular Pentagon:" << std::endl;
        for (size_t i = 0; i < 5; ++i)
        {
            std::cout << "Point " << i + 1 << ": ";
            is >> input_points[i];
        }

        if (!check_regular_ngon(input_points, 5))
        {
            throw std::invalid_argument("Error: Input points do not form a regular pentagon");
        }

        this->num_points = 5;
        this->points = std::make_unique<Point<T>[]>(this->num_points);
        for (size_t i = 0; i < 5; ++i) this->points[i] = input_points[i];
    }
};

template <typename T>
class Hexagon : public Figure<T>
{
public:
    Hexagon() : Figure<T>()
    {
    }

    Hexagon(Point<T> p1, Point<T> p2, Point<T> p3, Point<T> p4, Point<T> p5, Point<T> p6) : Figure<T>()
    {
        Point<T> input_points[] = {p1, p2, p3, p4, p5, p6};
        if (!check_regular_ngon(input_points, 6))
        {
            throw std::invalid_argument("Error: Points do not form a regular hexagon");
        }

        this->num_points = 6;
        this->points = std::make_unique<Point<T>[]>(this->num_points);
        for (size_t i = 0; i < 6; ++i) this->points[i] = input_points[i];
    }

    Hexagon(const Hexagon& other)
    {
        this->num_points = other.num_points;
        this->points = std::make_unique<Point<T>[]>(this->num_points);
        for (size_t i = 0; i < this->num_points; ++i)
        {
            this->points[i] = other.points[i];
        }
    }

    Hexagon(Hexagon&& other) noexcept
    {
        this->num_points = other.num_points;
        this->points = std::move(other.points);
    }

    void read(std::istream& is) override
    {
        Point<T> input_points[6];
        std::cout << "Enter 6 points (x y) for the Regular Hexagon:" << std::endl;
        for (size_t i = 0; i < 6; ++i)
        {
            std::cout << "Point " << i + 1 << ": ";
            is >> input_points[i];
        }

        if (!check_regular_ngon(input_points, 6))
        {
            throw std::invalid_argument("Error: Input points do not form a regular hexagon");
        }

        this->num_points = 6;
        this->points = std::make_unique<Point<T>[]>(this->num_points);
        for (size_t i = 0; i < 6; ++i) this->points[i] = input_points[i];
    }
};
