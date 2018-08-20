//
//! \file
//
// Created by Sander van Woensel / Umut Uyumaz
// Copyright (c) 2018 ASML Netherlands B.V. All rights reserved.
//
//! Mesh Network wrapper class.

#include "Facilities_MeshNetwork.hpp"

#include "Debug.hpp"
#include "painlessMesh.h"


namespace Facilities {

const uint16_t MeshNetwork::PORT = 5555;

//! Construct only.
//! \note Does not construct and initialize in one go to be able to initialize after serial debug port has been opened.
MeshNetwork::MeshNetwork()
{
   // Set debug messages before init() so that you can see startup messages.
   //m_mesh.setDebugMsgTypes( ERROR | STARTUP );  // To enable all: ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE
   //m_mesh.init( prefix, password, &m_userTaskScheduler, MeshNetwork::PORT );

   m_mesh.onReceive(std::bind(&MeshNetwork::receivedCb, this, std::placeholders::_1, std::placeholders::_2));
   //m_mesh.onNewConnection();
   //m_mesh.onChangedConnections();
   //m_mesh.onNodeTimeAdjusted();

}

// Initialize mesh network.
void MeshNetwork::initialize(const __FlashStringHelper *prefix, const __FlashStringHelper *password, Scheduler& taskScheduler)
{
   // Set debug messages before init() so that you can see startup messages.
   m_mesh.setDebugMsgTypes( ERROR | STARTUP );  // To enable all: ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE
   m_mesh.init( prefix, password, &taskScheduler, MeshNetwork::PORT );
}

//! Update mesh; forward call to painlessMesh.
void MeshNetwork::update()
{
   m_mesh.update();
}

void MeshNetwork::receivedCb(uint32_t from, String& msg)
{
   MY_DEBUG_PRINTF("Data received from node: %u; msg: %s\n", from, msg.c_str());
}

// void newConnectionCallback(uint32_t nodeId)
// {
//     Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
// }
//
// void changedConnectionCallback()
// {
//     Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str());
// }
//
// void nodeTimeAdjustedCallback(int32_t offset)
// {
//     Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
// }


} // namespace Facilities
