//
//! \file
//
// Created by Sander van Woensel / Umut Uyumaz
// Copyright (c) 2018 ASML Netherlands B.V. All rights reserved.
//
//! Example Task

#ifndef __Tasks_IdentifyMasterTask__
#define __Tasks_IdentifyMasterTask__

#include <painlessMesh.h>
#include <functional>
#include <vector>

#include "Facilities_MeshNetwork.hpp"

// Forward declarations.
namespace Facilities { class MeshNetwork; }

namespace Tasks {



typedef std::function<void(bool isMaster)> masterChangedCallback_t;

class IdentifyMasterTask : public Task
{
public:
   explicit IdentifyMasterTask(Facilities::MeshNetwork& mesh);
   ~IdentifyMasterTask() {};

   // Disallow copy-ing
	IdentifyMasterTask(const IdentifyMasterTask& other) = delete;
	IdentifyMasterTask(IdentifyMasterTask&& other) = delete;
	IdentifyMasterTask& operator=(const IdentifyMasterTask& other) = delete;

   void execute();

   bool isMaster();

   void onMasterChanged(masterChangedCallback_t callback);

private:
   Facilities::MeshNetwork& m_mesh;
   
   Facilities::MeshNetwork::NodeId m_master_id;
   Facilities::MeshNetwork::NodeId m_candidate_id;
   unsigned long m_master_refresh_time;
   
   std::vector<masterChangedCallback_t> onMasterChanged_Callbacks;

   void InvokeMasterChanged();
   void receivedCb(Facilities::MeshNetwork::NodeId nodeId, String& msg);
};

} // namespace Tasks

#endif //  __Tasks_IdentifyMasterTask__
