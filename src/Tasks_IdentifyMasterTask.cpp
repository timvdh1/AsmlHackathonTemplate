//
//! \file
//
// Created by Sander van Woensel / Umut Uyumaz
// Copyright (c) 2018 ASML Netherlands B.V. All rights reserved.
//
//! Example Task
#include "Tasks_IdentifyMasterTask.hpp"

#include "Debug.hpp"
#include "Facilities_MeshNetwork.hpp"
#include <time.h>

#include <functional>
#include <vector>

namespace Tasks {

IdentifyMasterTask::IdentifyMasterTask(Facilities::MeshNetwork& mesh) :
   Task(TASK_SECOND * 1 , TASK_FOREVER, std::bind(&IdentifyMasterTask::execute, this)),
   m_mesh(mesh),
    m_master_id(0),
    m_candidate_id(0),
    m_master_refresh_time(0),
    onMasterChanged_Callbacks(std::vector<masterChangedCallback_t>())
{
   m_mesh.onReceive(std::bind(&IdentifyMasterTask::receivedCb, this, std::placeholders::_1, std::placeholders::_2));
}

void IdentifyMasterTask::onMasterChanged(masterChangedCallback_t callback)
{
    onMasterChanged_Callbacks.push_back(callback);
}

void IdentifyMasterTask::InvokeMasterChanged()
{        
    bool isMaster = this->isMaster();
    for(size_t i = 0; i < onMasterChanged_Callbacks.size(); i++)
    {
        onMasterChanged_Callbacks[0](isMaster);
    }
}

bool IdentifyMasterTask::isMaster()
{
    return m_master_id == m_mesh.getMyNodeId();
}

void IdentifyMasterTask::execute()
{
    if(m_master_refresh_time < millis())
    {
        m_master_id = 0;
    }

   String msg = this->isMaster() ? F("Master") : F("Ping");
   m_mesh.sendBroadcastAll( msg );
}

void IdentifyMasterTask::receivedCb(Facilities::MeshNetwork::NodeId nodeId, String& msg)
{
   if(msg == "Master")
   {
       m_master_id = nodeId;
       InvokeMasterChanged();
       m_master_refresh_time = millis() + (5 * 1000);
   }
   else if(!m_master_id)
   {      
        MY_DEBUG_PRINTLN("Probing for master..");
        if(m_master_refresh_time < millis())
        {
            if(m_candidate_id == m_mesh.getMyNodeId())
            {
                m_master_id = m_candidate_id;
            }
            m_candidate_id = 0;
            m_master_refresh_time = millis() + (5 * 1000);
        } 
        if(msg == "Ping")
        {            
            m_candidate_id = max(m_candidate_id,nodeId);
            MY_DEBUG_PRINTLN("Master candiate: " + String(m_candidate_id));          
        }
   }
}

} // namespace Tasks
