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

   m_mesh.onReceive(std::bind(&HardcodedDisplayTask::receivedCb, this, std::placeholders::_1, std::placeholders::_2));
}

//! Update display

void HardcodedDisplayTask::loop() {
  // Show runtime in seconds for one second
  unsigned long now = millis();
  int digit;
  do {
    // My display has the digits in reverse order
    // Ie digit 0 is the one on the far right.
    // This matches 10^0 :-)
    unsigned long rest = millis();
    for (digit = 0; digit < 8; digit++) {
      m_lmd.setDigit(digit, rest % 10, digit == 3);
      rest /= 10;
    }
    m_lmd.display(); 
    delay(10);
  }
  while (millis() - now < 1000);

  // Show "- HELP -" for one second
  m_lmd.setDigit(7, LEDMatrixDriver::BCD_DASH);
  m_lmd.setDigit(6, LEDMatrixDriver::BCD_BLANK);
  m_lmd.setDigit(5, LEDMatrixDriver::BCD_H);
  m_lmd.setDigit(4, LEDMatrixDriver::BCD_E);
  m_lmd.setDigit(3, LEDMatrixDriver::BCD_L);
  m_lmd.setDigit(2, LEDMatrixDriver::BCD_P);
  m_lmd.setDigit(1, LEDMatrixDriver::BCD_BLANK);
  m_lmd.setDigit(0, LEDMatrixDriver::BCD_DASH);
  m_lmd.display(); 
  delay(1000);
}
void HardcodedDisplayTask::execute()
{
   m_lmd.clear();

    //loop();
   /*m_lmd.setPixel(m_x, 0, true);

   m_lmd.setPixel(m_x, 2, true);

    m_lmd.setPixel(1, 0, true);

   // m_lmd.setDigit(3, 3, true);
   
   m_lmd.setPixel(9, 4, true);*/

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

/*
      for(int i=0;i<2;i++)
      {
          for(int j=0;j<8;j++)
          {
            matrix[i*16+j][j] = 1;
          }

          for(int j=0;j<8;j++)
          {
            matrix[i*16+j+8][7-j] = 1;
          }
      }
*/

   
   Circle circle;
   int noOfNodes = 4;
   circle.scale(noOfNodes, 1, p);

   
/*
   for (int i=0; i<32; i++)
   {
       MY_DEBUG_PRINT(i);
       MY_DEBUG_PRINT(" ");
       for (int k=0; k<8; k++)
       {
           
                MY_DEBUG_PRINT(matrix[i][k]);
                MY_DEBUG_PRINT(" ");

                /*
                if(i<15)
                {
                    matrix[i][k] = 0;
                }

                if( i == 1)
                matrix[i][k] = 1;
                if(k == 0)
                matrix[i][k] = 0;
               
       }
       MY_DEBUG_PRINTLN(" ");

       

   }
    */

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

void HardcodedDisplayTask::receivedCb(Facilities::MeshNetwork::NodeId nodeId, String& msg)
{
   MY_DEBUG_PRINTLN("Received data in ExampleDisplayTask"+msg);

   if(++m_x>LEDMATRIX_WIDTH)
   {
      m_x=0;
   }

    if (msg[0] != 7)
        return ;

    MY_DEBUG_PRINTLN("Data received:"+msg);

   /*int matrix[32][8];
   decodeMatrix( msg, matrix);*/
}

} // namespace Tasks
