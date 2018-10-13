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

String ExampleTransmitTask::encodeMatrix(int * matrix)
{
   String str = "Draw:";
    for(int i=0 ; i<32 ; i++)
    {
        char row = 0;
        for(int j=0; j<8; j++)
        {
            row = row << 1 ;
            if(matrix[i*8+j] == 1)
            {
                row = row | 1;
            }

        }

        str += row; 
    }

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


    Circle square;
    square.scale(totalNodes,1, matrix);
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
