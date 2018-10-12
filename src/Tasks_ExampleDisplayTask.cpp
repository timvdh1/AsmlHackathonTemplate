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
const unsigned long ExampleDisplayTask::POLL_DELAY_MS = 50;  // was: 100

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

//! Update display

void ExampleDisplayTask::loop() {
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
     //MY_DEBUG_PRINTLN("Inside loop()");  // never
 delay(1000);
}

int step, offsets[8];

void ExampleDisplayTask::execute()
{
   if (step==0) { m_lmd.clear(); for (int i=0; i<8; ++i)  offsets[i] = rand() % 3; }
 #if 0
   int Y = step & 15;
   bool dot = (step&16? false: true);

   for (int i=0; i < 8; ++i) {
        m_lmd.setPixel(Y+i, i, dot);
    }
    ++step;
#elif 0
   int Y = step & 31;
   for (int i=0; i < 8; ++i)  m_lmd.setPixel((Y+offsets[i]) ^ 0x18, i, false);
   ++step;
   Y = step & 31;
   for (int i=0; i < 8; ++i)  m_lmd.setPixel((Y+offsets[i]) ^ 0x18, i, true);
#elif 0
    ++step;
    m_lmd.setIntensity(step%10);
    int Y = (gN ^ 0x18) & 31;
    if (gN & 32)
    for (int i=0; i < 8; ++i)  m_lmd.setPixel(Y, i, true);
    else
    for (int i=0; i < 8; ++i)  m_lmd.setPixel(Y, i, false);
#else
    ++step;
#endif
   m_lmd.display();
}


void ExampleDisplayTask::decodeMatrix(const String& msg, int matrix[32][8])
{
   int counter  = 1;
    for(int i=1 ; i<33 ; i++)
    {
        char ch = msg[i];
        for(int j=0; j<8; j++)
        {
            matrix[i-1][j] = ch & 1;
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
   MY_DEBUG_PRINTLN("Received data in ExampleDisplayTask: "+msg);

    if (msg[0] != 7)
        return ;

    MY_DEBUG_PRINTLN("Data received:"+msg);

   int matrix[32][8];
   decodeMatrix( msg, matrix);
   drawMessage(matrix);
}

} // namespace Tasks
