#include <Arduino.h>

#include "Debug.hpp"
#include "painlessMesh.h"
#include "Facilities_MeshNetwork.hpp"
#include "Tasks_ExampleTransmitTask.hpp"
#include "Tasks_ExampleDisplayTask.hpp"
#include "Tasks_IdentifyMasterTask.hpp"

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
Tasks::ExampleDisplayTask  exampleDisplayTask(meshNetwork);
}

//! Called once at board startup.
void setup()
{
   MY_DEBUG_BEGIN(115200);

   // Create MeshNetwork
   meshNetwork.initialize(F("Mesh_Black"), F("blueblue"), taskScheduler);

   meshNetwork.m_mesh.stationManual(STATION_SSID, STATION_PASSWORD);
   meshNetwork.m_mesh.setHostname(HOSTNAME);
   myAPIP = IPAddress(meshNetwork.m_mesh.getAPIP());
   MY_DEBUG_PRINTLN("Setup: My IP is " +myAPIP.toString());
    //MY_DEBUG_PRINTLN("My node id ");
   //MY_DEBUG_PRINTLN("My node id "+ meshNetwork.getMyNodeId());

   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", "<form>Shape to Select<br> \
      <select name=\"shape_type\" required> \
    <option value=\"0\">Square</option> \
    <option value=\"1\">Circle</option> \
    <option value=\"2\">Triangle</option> \
  </select><br><br><input type='submit' value='Submit'></form>");
    MY_DEBUG_PRINTLN("Get Request is received");
     if (request->hasArg("shape_type")){
      String shapeType = request->arg("shape_type");
       int shape = shapeType.toInt();
      MY_DEBUG_PRINTLN("Shape Type: "+ shapeType);
      MY_DEBUG_PRINT("Shape Type INT: ");
       MY_DEBUG_PRINTLN(shape);
    }
  });
server.on("/", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", "<form>Text to Broadcast<br><input type='text' name='BROADCAST'><br><br><input type='submit' value='Submit'></form>");
    MY_DEBUG_PRINTLN("Post Request is received");

     if (request->hasArg("shape_type")){
      String shapeType = request->arg("shape_type");
       int shape = shapeType.toInt();
      MY_DEBUG_PRINTLN("Shape Type: "+ shapeType);
      MY_DEBUG_PRINT("Shape Type INT: ");
       MY_DEBUG_PRINTLN(shape);
    }

  });
  server.begin();
   // Create and add tasks.
   taskScheduler.addTask( exampleTransmitTask );
   taskScheduler.addTask( exampleDisplayTask );
   taskScheduler.addTask( identifyMasterTask );
   exampleTransmitTask.enable();
   exampleDisplayTask.enable();
   identifyMasterTask.enable();

   MY_DEBUG_PRINTLN(F("Setup completed"));
}

//! Called repeatedly after setup().
void loop()
{
   taskScheduler.execute();
   meshNetwork.update();

  

   if(myIP != getlocalIP()){
    myIP = getlocalIP();
    Serial.println("My IP is " + myIP.toString());
   
  }
}

IPAddress getlocalIP() {
  return IPAddress(meshNetwork.m_mesh.getStationIP());
}

