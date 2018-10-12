
#include "Shape.hpp"
#include <cstring>


void Square::fillMatrix(int a, int b, unsigned short intensity, int * node)
{
    for(int i = a; i<b; i++)
    {
        for( int k = 0; k<8; k++)
        {
            node[i*8 + k] = intensity;
        }
    }
}


void Square::scale(const unsigned short noOfNodes, unsigned short intensity, int * (nodes[4])) 
{
    memset((int*)nodes[0],0,32*8*4);
    memset((int*)nodes[1],0,32*8*4);
    memset((int*)nodes[2],0,32*8*4);
    memset((int*)nodes[3],0,32*8*4);
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
                nodes[i/8][k*8 + i%8] = intensity;
            }
        }
    }
}

bool Circle::isInsideCircle (short x, short y)
{
    float distance = (x-16.5)*(x-16.5) + (y-center_)*(y-center_);
    (center_)*(center_) > distance ? true : false;
}


