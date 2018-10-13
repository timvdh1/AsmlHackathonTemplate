//
//! \file
//
// Created by Sander van Woensel / Umut Uyumaz
// Copyright (c) 2018 ASML Netherlands B.V. All rights reserved.
//
//! Example Task
#include "Tasks_ExampleTransmitTask.hpp"

#include "Debug.hpp"


#include <functional>
#include <string>
#include <sstream>

using namespace std;
using namespace ArduinoJson::Internals;

namespace Tasks {


ExampleTransmitTask::ExampleTransmitTask(Facilities::MeshNetwork& mesh, IdentifyMasterTask& identifyMasterTask) :
   Task(TASK_SECOND * 5 , TASK_FOREVER, std::bind(&ExampleTransmitTask::execute, this)),
   m_mesh(mesh),
   m_identifyMasterTask(identifyMasterTask)
{

}

   void ExampleTransmitTask::nextShape()
   {
       currentShape++;
       this->execute();
   }

void array_to_string(char array[], unsigned int len, char buffer[])
{
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}

String ExampleTransmitTask::encodeMatrix(int * matrix)
{
    char encodedMatrix[32];
    char encodedMatrixString[65];
   String str = "Draw:";
    for(int i=0 ; i<32 ; i++)
    {
        encodedMatrix[i];
        char row = 0;
        for(int j=0; j<8; j++)
        {
            encodedMatrix[i] = encodedMatrix[i] << 1 ;
            if(matrix[i*8+j] == 1)
            {
                encodedMatrix[i] = encodedMatrix[i] | 1;
            }
        }
    }
    array_to_string(encodedMatrix,32,encodedMatrixString);
        str += encodedMatrixString; 

    return str;
}

void ExampleTransmitTask::sendMatrix()
{
    int  matrix1[32][8];
    int  matrix2[32][8];
    int  matrix3[32][8];
    int  matrix4[32][8];

    int* matrix[4] = {(int*)matrix1,(int*)matrix2,(int*)matrix3,(int*)matrix4};


     std::list<uint32_t> nodes = m_mesh.m_mesh.getNodeList();
     nodes.sort();
    int totalNodes = nodes.size() + 1;
    if(totalNodes > 4)
    totalNodes = 4;

    
    Square square;
    Circle circle;

    LedShape* shape;
    if(currentShape % 2 == 0)
    {
        shape = &square;
    }
    else
    {
        shape = &circle;
    }

    shape->scale(totalNodes,1, matrix);
    int currentNode = 0;

     MY_DEBUG_PRINTLN("Send shape of size: " + String(totalNodes)); 
     for (std::list<uint32_t>::const_iterator iterator = nodes.begin(), end = nodes.end(); iterator != end; ++iterator) {         
         String msg = encodeMatrix(matrix[currentNode]);
         currentNode++;
         uint32_t node = *iterator;
         m_mesh.sendSingle(node, msg);
     }
     if(currentNode <=3)
     {
        String msg = encodeMatrix(matrix[currentNode]);
        m_mesh.sendSingle(m_mesh.getMyNodeId(), msg);
     }
}

void ExampleTransmitTask::execute()
{
    if(m_identifyMasterTask.isMaster())
    {
     sendMatrix();
    }
}

} // namespace Tasks
