//
//! \file
//
// Created by Sander van Woensel / Umut Uyumaz
// Copyright (c) 2018 ASML Netherlands B.V. All rights reserved.
//
//! Example Task

#ifndef __Tasks_ExampleTransmitTask__
#define __Tasks_ExampleTransmitTask__

#include <painlessMesh.h>
#include "Facilities_MeshNetwork.hpp"
#include "Tasks_IdentifyMasterTask.hpp"
#include <functional>
#include "Shape.hpp"

// Forward declarations.
namespace Facilities { class MeshNetwork; }

namespace Tasks {

class ExampleTransmitTask : public Task
{
public:
   explicit ExampleTransmitTask(Facilities::MeshNetwork& mesh, IdentifyMasterTask& identifyMasterTask);
   ~ExampleTransmitTask() {};

   // Disallow copy-ing
	ExampleTransmitTask(const ExampleTransmitTask& other) = delete;
	ExampleTransmitTask(ExampleTransmitTask&& other) = delete;
	ExampleTransmitTask& operator=(const ExampleTransmitTask& other) = delete;

    String  encodeMatrix( int * matrix);
    void  sendMatrix();
   void execute();
   
   void nextShape();
private:
   IdentifyMasterTask& m_identifyMasterTask;
   Facilities::MeshNetwork& m_mesh;
   Square mSquare;
   int currentShape;

};

} // namespace Tasks

#endif //  __Tasks_ExampleTransmitTask__
