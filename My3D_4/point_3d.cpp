#include "point_3d.h"

Point_3D::Point_3D()
{
    m_x = 0.0;
        m_y = 0.0;
        m_z = 0.0;
}

Point_3D::Point_3D(double x, double y, double z)
{
    m_x=x;
    m_y=y;
    m_z=z;
}

double Point_3D::x() const
{
    return m_x;
}

double Point_3D::y() const
{
    return m_y;
}

double Point_3D::z() const
{
    return m_z;
}

Point_3D Point_3D::operator +(const Point_3D &a)
{
    return Point_3D(m_x + a.m_x, m_y + a.m_y, m_z + a.m_z);
}

Point_3D Point_3D::operator *(double c)
{
    return Point_3D(m_x * c, m_y * c, m_z * c);
}
