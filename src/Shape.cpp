
#include "Shape.hpp"
#include <cstring>

void Square::fillMatrix(int a, int b, unsigned short intensity, int * node)
{
    for(int i = 0; i<32; i++)
    {
        for( int k = 0; k<8; k++)
        {
            if(i >= a && i < b)
            {
                node[unwrapX(i)*8 + unwrapY(k)] = intensity;
            }
            else
            {
                node[unwrapX(i)*8 + unwrapY(k)] = 0;
            }
        }
    }
}


void Square::scale(const unsigned short noOfNodes, unsigned short intensity, int * (nodes[4])) 
{

    if(noOfNodes == 1)
    {
        fillMatrix(12,20,intensity, (int*)nodes[0]);
    }
    else if(noOfNodes == 2)
    {
        fillMatrix(8,24,intensity, (int*)nodes[0]);
        nodes[1] = nodes[0];
    }
    else if(noOfNodes == 3)
    {
        fillMatrix(4, 28,intensity, (int*)nodes[0]);
        nodes[1] = nodes[0];
        nodes[2] = nodes[0];
    }
    else
    {
        fillMatrix(0, 32,intensity, (int*)nodes[0]);
        nodes[1] = nodes[0];
        nodes[2] = nodes[0];
        nodes[3] = nodes[0];
    }
}

void Circle::scale(const unsigned short noOfNodes, unsigned short intensity, int * (nodes[4]))
{
 

    center_ = 4*noOfNodes + 0.5;

    for (int i = 0; i<noOfNodes*8; i++)
    {
        for(int k = 0; k<32; k++)
        {
            if(isInsideCircle(k+1, i+1))
            {
                nodes[i/8][unwrapX(k)*8 + unwrapY(i%8)] = intensity;
            }
            else
            {
                nodes[i/8][unwrapX(k)*8 + unwrapY(i%8)] = 0;
            }
        }
    }
}

bool Circle::isInsideCircle (float x, float y)
{
    float distance = (x-16.5)*(x-16.5) + (y-center_)*(y-center_);
    return (center_)*(center_) > distance ? true : false;
}


void Triangle::scale(const unsigned short noOfNodes, unsigned short intensity, int * (nodes[4])) 
{
    
    if(noOfNodes == 1)
    {
        Coordinate v1 {-1.0, 3.5};
        Coordinate v2 { 31.0, 0.0};
        Coordinate v3 { 31.0, 7.0};
        traverse(intensity, nodes, v1, v2, v3);

    }
    if (noOfNodes == 2)
    {
        Coordinate v1 {-1.0, 7,5};
        Coordinate v2 { 31.0, 0.0};
        Coordinate v3 { 31.0, 15.0};
        traverse(intensity, nodes, v1, v2, v3);
    }
    if (noOfNodes == 3)
    {
        Coordinate v1 {-1.0, 11,5};
        Coordinate v2 { 31.0, 0.0};
        Coordinate v3 { 31.0, 23.0};
        traverse(intensity, nodes, v1, v2, v3);
    }
    if (noOfNodes == 4)
    {
        Coordinate v1 {-1.0, 15,5};
        Coordinate v2 { 31.0, 0.0};
        Coordinate v3 { 31.0, 31.0};
        traverse(intensity, nodes, v1, v2, v3);
    }
    
}


void Triangle::traverse(unsigned short intensity, int * (p[4]) , 
                                       Coordinate v1, 
                                       Coordinate v2, 
                                       Coordinate v3)
{
    for (int i = 0; i<32; i++)
    {
        for (int k = 0; k<32; k++)
        {
            if(isInside({(float)i, (float)k}, v1, v2, v3))
                p[k/8][unwrapX(i)*8 + unwrapY(k%8)] = intensity;
            else
                p[k/8][unwrapX(i)*8 + unwrapY(k%8)] = 0;
        }
    }

}


bool Triangle::isInside(Coordinate pt, Coordinate v1, Coordinate v2, Coordinate v3)
{
    bool b1, b2, b3;

    b1 = sign(pt, v1, v2) < 0.0f;
    b2 = sign(pt, v2, v3) < 0.0f;
    b3 = sign(pt, v3, v1) < 0.0f;

    return ((b1 == b2) && (b2 == b3));
}