#ifndef POINT_3D_H
#define POINT_3D_H

#include <QObject>

class Point_3D
{
public:
    Point_3D();
    Point_3D(double x, double y, double z);

    double x()const;
    double y()const;
    double z()const;

    Point_3D operator +(const Point_3D &a);
    Point_3D operator *(double c);

private:
    double m_x,m_y,m_z;

};

#endif // POINT_3D_H
