//
//! \file
//
// Created by Sander van Woensel / Umut Uyumaz
// Copyright (c) 2018 ASML Netherlands B.V. All rights reserved.
//
//! Example Task

#ifndef __Tasks_HardcodedDisplayTask__
#define __Tasks_HardcodedDisplayTask__

#include "Facilities_MeshNetwork.hpp"

#include <painlessMesh.h>
#include <LEDMatrixDriver.hpp>

#include <functional>
#include <string>
#include <vector>


// Forward declarations.
namespace Facilities { class MeshNetwork; }


namespace Tasks {

class HardcodedDisplayTask : public Task
{
public:
   explicit HardcodedDisplayTask(Facilities::MeshNetwork& mesh);
   ~HardcodedDisplayTask() {};

   // Disallow copy-ing
	HardcodedDisplayTask(const HardcodedDisplayTask& other) = delete;
	HardcodedDisplayTask(HardcodedDisplayTask&& other) = delete;
	HardcodedDisplayTask& operator=(const HardcodedDisplayTask& other) = delete;

   void execute();

private:
   static const int LEDMATRIX_WIDTH;
   static const int LEDMATRIX_HEIGHT;
   static const int LEDMATRIX_SEGMENTS;
   static const int LEDMATRIX_INTENSITY;
   static const int LEDMATRIX_CS_PIN;
   static const unsigned long POLL_DELAY_MS;

   Facilities::MeshNetwork& m_mesh;
   LEDMatrixDriver m_lmd;

   const std::vector<long long> nodeids = {978880179,
                                            3954383551, 
                                           3954352616,
                                           978879453,
                                           };

   int m_x;

   void receivedCb(Facilities::MeshNetwork::NodeId nodeId, String& msg);
   void loop();
   void drawMessage(int matrix[32][8]);
   void decodeMatrix(const String& msg, int matrix[32][8]);


};

} // namespace Tasks

#endif //  __Tasks_ExampleDisplayTask__
