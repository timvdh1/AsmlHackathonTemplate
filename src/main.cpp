#include <Arduino.h>

#include "Debug.hpp"
#include "painlessMesh.h"
#include "Facilities_MeshNetwork.hpp"
#include "Tasks_ExampleTransmitTask.hpp"
#include "Tasks_ExampleDisplayTask.hpp"
#include "Tasks_IdentifyMasterTask.hpp"
#include "Tasks_HardcodedDisplay.hpp"


//HTTP SERVER
#include "IPAddress.h"
#include "Hash.h"
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define   STATION_SSID     "Mesh_Black"
#define   STATION_PASSWORD "blueblue"

#define HOSTNAME "HTTP_BRIDGE"
IPAddress getlocalIP();
AsyncWebServer server(80);
IPAddress myIP(0,0,0,0);
IPAddress myAPIP(0,0,0,0);
//END HTTP SERVER

// Translation unit local variables
namespace {

Scheduler                  taskScheduler;

Facilities::MeshNetwork    meshNetwork;
Tasks::IdentifyMasterTask  identifyMasterTask(meshNetwork);
Tasks::ExampleTransmitTask exampleTransmitTask(meshNetwork,identifyMasterTask);
Tasks::HardcodedDisplayTask  hardcodedDisplayTask(meshNetwork);
Tasks::ExampleDisplayTask  exampleDisplayTask(meshNetwork);
}

//! Called once at board startup.
void setup()
{
   MY_DEBUG_BEGIN(115200);

   // Create MeshNetwork
   meshNetwork.initialize(F(STATION_SSID), F(STATION_PASSWORD), taskScheduler);

    //meshNetwork.m_mesh.stationManual(STATION_SSID, STATION_PASSWORD);
    meshNetwork.m_mesh.setHostname(HOSTNAME);
    myAPIP = IPAddress(meshNetwork.m_mesh.getAPIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
     request->send(200, "text/html", "<form>Text to Broadcast<br><input type='text' name='BROADCAST'><br><br><input type='submit' value='Submit'></form>");
     exampleTransmitTask.nextShape();
     //if (request->hasArg("BROADCAST")){
     //  String msg = request->arg("BROADCAST");
     //  meshNetwork.m_mesh.sendBroadcast(msg);
     //}
   });
   server.begin();

   // Create and add tasks.
   bool hardcoded = false;
   taskScheduler.addTask( identifyMasterTask );
   identifyMasterTask.enable();
   if(hardcoded)
   {
   taskScheduler.addTask( hardcodedDisplayTask);
   hardcodedDisplayTask.enable();
   }
   else
   {
   taskScheduler.addTask( exampleTransmitTask );
   taskScheduler.addTask( exampleDisplayTask );
   exampleTransmitTask.enable();
   exampleDisplayTask.enable();

   }


   MY_DEBUG_PRINTLN(F("Setup completed"));
}

//! Called repeatedly after setup().
void loop()
{
   taskScheduler.execute();
   meshNetwork.update();

    if(myIP != getlocalIP()) {
     myIP = getlocalIP();
     Serial.println("My IP is " + myIP.toString());
   }
}

IPAddress getlocalIP() {
  return IPAddress(meshNetwork.m_mesh.getStationIP());
}

