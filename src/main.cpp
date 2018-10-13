#include <Arduino.h>

#include "Debug.hpp"
#include "painlessMesh.h"
#include "Facilities_MeshNetwork.hpp"
#include "Tasks_ExampleTransmitTask.hpp"
#include "Tasks_ExampleDisplayTask.hpp"
#include "Tasks_HardcodedDisplay.hpp"
#include "Tasks_IdentifyMasterTask.hpp"

// Translation unit local variables
namespace {

Scheduler                  taskScheduler;

Facilities::MeshNetwork    meshNetwork;
Tasks::ExampleTransmitTask exampleTransmitTask(meshNetwork);
Tasks::ExampleDisplayTask  exampleDisplayTask(meshNetwork);
Tasks::HardcodedDisplayTask  hardcodedDisplayTask(meshNetwork);
Tasks::IdentifyMasterTask  identifyMasterTask(meshNetwork);
}

//! Called once at board startup.
void setup()
{
   MY_DEBUG_BEGIN(115200);

   // Create MeshNetwork
   meshNetwork.initialize(F("Mesh_Black_ugurFix"), F("blueblue_ugurFix"), taskScheduler);

   configTime(0, 0, "pool.ntp.org", "time.nist.gov");

   // Create and add tasks.
   taskScheduler.addTask( exampleTransmitTask );
   taskScheduler.addTask( exampleDisplayTask );
   taskScheduler.addTask( identifyMasterTask );
   taskScheduler.addTask( hardcodedDisplayTask);

   //exampleTransmitTask.enable();
   //exampleDisplayTask.enable();
   hardcodedDisplayTask.enable();
   identifyMasterTask.enable();

   MY_DEBUG_PRINTLN(F("Setup completed"));
}

//! Called repeatedly after setup().
void loop()
{
   taskScheduler.execute();
   meshNetwork.update();

}
