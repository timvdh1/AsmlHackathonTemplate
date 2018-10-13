//
//! \file
//
// Created by Sander van Woensel / Umut Uyumaz
// Copyright (c) 2018 ASML Netherlands B.V. All rights reserved.
//
//! Mesh Network wrapper class to provide a container to add specific
//! mesh network behaviour.

#include "Facilities_MeshNetwork.hpp"

#include "Debug.hpp"
#include "painlessMesh.h"


namespace Facilities {

const uint16_t MeshNetwork::PORT = 5555;

//! Construct only.
//! \note Does not construct and initialize in one go to be able to initialize after serial debug port has been opened.
MeshNetwork::MeshNetwork() :
    receivedCallbacks(std::vector<receivedCallback_t>())
{
   m_mesh.onReceive(std::bind(&MeshNetwork::receivedCb, this, std::placeholders::_1, std::placeholders::_2));
   //m_mesh.onNewConnection(...);
   //m_mesh.onChangedConnections(...);
   //m_mesh.onNodeTimeAdjusted(...);
}

// Initialize mesh network.
void MeshNetwork::initialize(const __FlashStringHelper *prefix, const __FlashStringHelper *password, Scheduler& taskScheduler)
{
   // Set debug messages before init() so that you can see startup messages.
   m_mesh.setDebugMsgTypes( ERROR | STARTUP );  // To enable all: ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE
   m_mesh.init( prefix, password, &taskScheduler, MeshNetwork::PORT);
   m_mesh.onReceive(std::bind(&MeshNetwork::receivedCb, this, std::placeholders::_1, std::placeholders::_2));
}

//! Update mesh; forward call to painlessMesh.
void MeshNetwork::update()
{
   m_mesh.update();
}

void MeshNetwork::sendBroadcast(String &message)
{
  // MY_DEBUG_PRINT("Broadcasting message: "); MY_DEBUG_PRINTLN(message);
   m_mesh.sendBroadcast(message, false); // false: Do not include self.
}

void MeshNetwork::sendBroadcastAll(String &message)
{
  // MY_DEBUG_PRINT("Broadcasting message: "); MY_DEBUG_PRINTLN(message);
   m_mesh.sendBroadcast(message, true);
}

void MeshNetwork::sendSingle(MeshNetwork::NodeId node, String &message)
{
   MY_DEBUG_PRINT("Send message: "); MY_DEBUG_PRINTLN(message);
   if(node != m_mesh.getNodeId())
   {
       this->receivedCb(node,message);
   }
   else
   {       
     m_mesh.sendSingle(node, message);
   }
}

MeshNetwork::NodeId MeshNetwork::getMyNodeId()
{
    //MY_DEBUG_PRINT("My node id is: ");
    //MY_DEBUG_PRINTLN(m_mesh.getNodeId() );
   return m_mesh.getNodeId();
}


void MeshNetwork::onReceive(receivedCallback_t receivedCallback)
{
    receivedCallbacks.push_back(receivedCallback);
}

void MeshNetwork::receivedCb(NodeId transmitterNodeId, String& msg)
{
   MY_DEBUG_PRINTF("Data received from node: %u; msg: %s\n", transmitterNodeId, msg.c_str());
   
    for(size_t i = 0; i < receivedCallbacks.size(); i++)
    {
        receivedCallback_t callback = receivedCallbacks[i];
        callback(transmitterNodeId, msg);
    }
}


} // namespace Facilities
