
#include "Shape.hpp"

void Square::fillMatrix(int a, int b, int & node[32][8])
{
    for(int i = a; i<b; i++)
    {
        for( int k = 0; k<8: k++)
        {
            node[i][k] = 1;
        }
    }
}


void Square::scale(const unsigned short noOfNodes,
                           int & node1[32][8],
                           int & node2[32][8],
                           int & node3[32][8],
                           int & node4[32][8]) 
{
    if(noOfNodes == 1)
    {
        fillMatrix(12,20,node1);
    }
    else if(noOfNodes == 2)
    {
        fillMatrix(8,24,node1);
        node2 = node1;
    }
    else if(noOfNodes == 3)
    {
        fillMatrix(4, 28, node1);
        node2 = node1;
        node3 = node1;
    }
    else
    {
        fillMatrix(0, 32, node1);
        node2 = node1;
        node3 = node1;
        node4 = node1;
    }
}

