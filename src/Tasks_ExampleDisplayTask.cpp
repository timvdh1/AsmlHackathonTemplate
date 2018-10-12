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
   m_lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN),
   m_x(0)
{
   m_lmd.setEnabled(true);
   m_lmd.setIntensity(LEDMATRIX_INTENSITY);

   m_mesh.onReceive(std::bind(&ExampleDisplayTask::receivedCb, this, std::placeholders::_1, std::placeholders::_2));
}

void ExampleDisplayTask::execute()
{
    
}

void ExampleDisplayTask::decodeMatrix(const String& msg, int matrix[32][8])
{
    for(int i=0 ; i<32 ; i++)
    {
        char ch = msg[i];
        for(int j=0; j<8; j++)
        {
            matrix[i][j] = ch & 1;
            ch = ch >> 1;
        }            
    }
}

void ExampleDisplayTask::drawMessage(int matrix[32][8])
{
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
        int matrix[32][8];
        msg = msg.substring(strlen("Draw:"));
        decodeMatrix(msg, matrix);        
        drawMessage(matrix);
    }
}

} // namespace Tasks
