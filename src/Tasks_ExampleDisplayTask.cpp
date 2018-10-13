//
//! \file
//
// Created by Sander van Woensel / Umut Uyumaz
// Copyright (c) 2018 ASML Netherlands B.V. All rights reserved.
//
//! Example Task to output something to the LED Matrix.
#include "Tasks_ExampleDisplayTask.hpp"

#include "Debug.hpp"
#include "Facilities_MeshNetwork.hpp"

#include <LEDMatrixDriver.hpp>

#include <functional>
#include "Shape.hpp"

namespace Tasks {


const int ExampleDisplayTask::LEDMATRIX_WIDTH = 31;
const int ExampleDisplayTask::LEDMATRIX_HEIGHT = 7;
const int ExampleDisplayTask::LEDMATRIX_SEGMENTS = 4;
const int ExampleDisplayTask::LEDMATRIX_INTENSITY = 5;
const int ExampleDisplayTask::LEDMATRIX_CS_PIN = 16;
const unsigned long ExampleDisplayTask::POLL_DELAY_MS = 100;

//! Initializes the LED Matrix display.
ExampleDisplayTask::ExampleDisplayTask(Facilities::MeshNetwork& mesh) :
   Task(POLL_DELAY_MS , TASK_FOREVER, std::bind(&ExampleDisplayTask::execute, this)),
   m_mesh(mesh),
   m_lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN)
{
   m_lmd.setEnabled(true);
   m_lmd.setIntensity(LEDMATRIX_INTENSITY);

   m_mesh.onReceive(std::bind(&ExampleDisplayTask::receivedCb, this, std::placeholders::_1, std::placeholders::_2));
}

void ExampleDisplayTask::execute()
{
    this->drawMatrix();
}

int char2int(char input)
{
  if(input >= '0' && input <= '9')
    return input - '0';
  if(input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  return 0;  
}

void ExampleDisplayTask::decodeMatrix(const String& msg, int matrix[32][8])
{
    for(int i=0 ; i<32 ; i++)
    {
        char ch = char2int(msg[i*2])*16 + char2int(msg[i*2+1]);
        for(int j=0; j<8; j++)
        {
            matrix[i][j] = ch & 1;
            ch = ch >> 1;
        }            
    }
}

void ExampleDisplayTask::drawMatrix()
{

    int time_past = millis() - transistion_start;
    if(time_past > 5000) time_past = 5000;
    float trans = time_past / (5000/2.0);

    int (*matrix)[8];    
    if(trans < 1)
    {
        matrix = matrix_src;
        m_lmd.setIntensity((int)((1-trans)*10));
    }
    else
    {
        matrix = matrix_dst;
        m_lmd.setIntensity((int)(((trans-1))*10));
    }
    m_lmd.clear();

    for(int i = 0; i<32; i++)
    {
        for(int j=0;j<8;j++)
        {
            if(matrix[i][j] == 1 )
                m_lmd.setPixel(i, j, true);
        }
    }

    m_lmd.display();
}

void ExampleDisplayTask::receivedCb(Facilities::MeshNetwork::NodeId nodeId, String& msg)
{
    if(msg.startsWith("Draw:"))
    {
        msg = msg.substring(strlen("Draw:"));

        for(int i = 0; i<32; i++)
        {
            for(int j=0;j<8;j++)
            {
                matrix_src[i][j] = matrix_dst[i][j];
            }
        }
        
        decodeMatrix(msg, matrix_dst);   
        transistion_start = millis();   
    }
}

} // namespace Tasks
