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


ExampleTransmitTask::ExampleTransmitTask(Facilities::MeshNetwork& mesh) :
   Task(TASK_SECOND * 1 , TASK_FOREVER, std::bind(&ExampleTransmitTask::execute, this)),
   m_mesh(mesh)
{

}


String ExampleTransmitTask::encodeMatrix(int component, int matrix[32][8])
{
   char ch = (int) component;
   String str = "";
   str += ch;
  
    for(int i=0 ; i<32 ; i++)
    {
        char row = 0;
        for(int j=0; j<8; j++)
        {
            row = row << 1 ;
            if(matrix[i][j] == 1)
            {
                row = row | 1;
            }

        }

        str +=  row; 
    }

    return str;
}

void ExampleTransmitTask::sendMatrix()
{
    int  matrix[32][8];
    memset(matrix, 0, sizeof(matrix));

    //mSquare.scale(1, matrix, nullptr, nullptr, nullptr);
#if 0  // a wide \/\/
      for(int i=0; i<2; i++)
      {
          for(int j=0;j<8;j++)  matrix[i*16+j][j] = 1;
          for(int j=0;j<8;j++)  matrix[i*16+j+8][7-j] = 1;
      }
#else  // four X's:  XXXX
      for(int i=0; i<4; i++)
      {
          for(int j=0;j<8;j++)  matrix[i*8+j][j] = 1;
          for(int j=0;j<8;j++)  matrix[i*8+j][7-j] = 1;
      }
#endif

    String msg = encodeMatrix( 7 ,  matrix);
    MY_DEBUG_PRINTLN("Data sent: "+msg);
    m_mesh.sendBroadcast( msg );
}

void ExampleTransmitTask::execute()
{
   String msg = F("Hello from Alex's node ");
   msg += m_mesh.getMyNodeId();
   m_mesh.sendBroadcast( msg );
   sendMatrix();
}

} // namespace Tasks
