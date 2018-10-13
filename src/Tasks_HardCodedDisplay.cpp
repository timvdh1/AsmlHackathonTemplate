//
//! \file
//
// Created by Sander van Woensel / Umut Uyumaz
// Copyright (c) 2018 ASML Netherlands B.V. All rights reserved.
//
//! Example Task to output something to the LED Matrix.
#include "Tasks_HardcodedDisplay.hpp"

#include "Debug.hpp"
#include "Facilities_MeshNetwork.hpp"

#include <LEDMatrixDriver.hpp>

#include <functional>
#include "Shape.hpp"

namespace Tasks {


const int HardcodedDisplayTask::LEDMATRIX_WIDTH = 31;
const int HardcodedDisplayTask::LEDMATRIX_HEIGHT = 7;
const int HardcodedDisplayTask::LEDMATRIX_SEGMENTS = 4;
const int HardcodedDisplayTask::LEDMATRIX_INTENSITY = 5;
const int HardcodedDisplayTask::LEDMATRIX_CS_PIN = 16;
const unsigned long HardcodedDisplayTask::POLL_DELAY_MS = 100;

//! Initializes the LED Matrix display.
HardcodedDisplayTask::HardcodedDisplayTask(Facilities::MeshNetwork& mesh) :
   Task(POLL_DELAY_MS , TASK_FOREVER, std::bind(&HardcodedDisplayTask::execute, this)),
   m_mesh(mesh),
   m_lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN),
   m_x(0)
{
   m_lmd.setEnabled(true);
   m_lmd.setIntensity(LEDMATRIX_INTENSITY);
}

void HardcodedDisplayTask::execute()
{
   m_lmd.clear();


    int  matrix[32][8];
    int  matrix2[32][8];
    int  matrix3[32][8];
    int  matrix4[32][8];

    memset(matrix, 0, sizeof(matrix));
    memset(matrix2, 0, sizeof(matrix));
    memset(matrix3, 0, sizeof(matrix));
    memset(matrix4, 0, sizeof(matrix));

    int * (p[4]);
    p[0] = (int *)matrix;
    p[1] = (int *)matrix2;
    p[2] = (int *)matrix3;
    p[3] = (int *)matrix4;


   
   Circle circle;
   int noOfNodes = 4;
   circle.scale(noOfNodes, 1, p);

   

   for (int i = 0; i < 4; i++)
   {
       if(m_mesh.getMyNodeId() == nodeids.at(i))
       {
           if( i== 0)
           {
                drawMessage(matrix);
           }  
           else if (i == 1)
           {
               drawMessage(matrix2);
           } 
           else if ( i == 2)
           {
               drawMessage(matrix3);
           }
           else 
           {
                drawMessage(matrix4);
           }
           break;
       }
   }
    
   m_lmd.display();
}


void HardcodedDisplayTask::decodeMatrix(const String& msg, int matrix[32][8])
{
   int counter  = 1;
    for(int i=1 ; i<33 ; i++)
    {
        char ch = msg[i];
        for(int j=0; j<8; j++)
        {
            matrix[i][j] = ch & 1;
            ch = ch >> 1;
        }
            
    }
}

void HardcodedDisplayTask::drawMessage(int matrix[32][8])
{
    m_lmd.clear();

    for(int i = 0; i<32; i++)
    {
        for(int j=0;j<8;j++)
        {
            if(matrix[i][j] != 0 )
                m_lmd.setPixel(i, j, true);
        }
    }

    m_lmd.display();
}

} // namespace Tasks
