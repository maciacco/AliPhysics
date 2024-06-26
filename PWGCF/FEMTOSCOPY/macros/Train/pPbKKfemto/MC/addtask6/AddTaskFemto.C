#if !defined(__CINT__) && !defined(__CLING__)
#include <TROOT.h>
#include <TString.h>
#include <TList.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <AliAnalysisManager.h>
#include "AliAnalysisTaskFemto.h"

#endif
//=============================================================================
//this line for local:
//AliAnalysisTaskFemto *AddTaskFemtoKchHBT(const char *configMacroName="ConfigFemtoAnalysis.C", const char *configMacroParameters="" )
AliAnalysisTaskFemto *AddTaskFemto(TString configMacroName, const char *containerName="femtolist", const char *configMacroParameters="" )
{
// Creates a proton analysis task and adds it to the analysis manager.

  // A. Get the pointer to the existing analysis manager via the static access method.
  //==============================================================================
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    Error("AddTaskFemto", "No analysis manager to connect to.");
    return NULL;
  }

  // B. Check the analysis type using the event handlers connected to the analysis
  //    manager. The availability of MC handler cann also be checked here.
  //==============================================================================
  if (!mgr->GetInputEventHandler()) {
    ::Error("AddTaskFemto", "This task requires an input event handler");
    return NULL;
  }
  TString type = mgr->GetInputEventHandler()->GetDataType(); // can be "ESD" or "AOD"
  cout << "Found " <<type << " event handler" << endl;

  // C. Create the task, add it to manager.
  //===========================================================================
//  gSystem->SetIncludePath("-I$ROOTSYS/include  -I./PWG2AOD/AOD -I./PWG2femtoscopy/FEMTOSCOPY/AliFemto -I./PWG2femtoscopyUser/FEMTOSCOPY/AliFemtoUser -I$ALICE_PHYSICS/include");

  if (TProofMgr::GetListOfManagers()->GetEntries()) {
//     if (dynamic_cast<TProofLite *> gProof) {
//       char *macrocommand[10000];
//       sprintf(macrocommand, ".L %s", configMacroName);
//       gProof->Exec(macrocommand);
//     }
//     else
    gProof->Load(configMacroName);
  }
  //  gROOT->LoadMacro("ConfigFemtoAnalysis.C++");
//===========================================================================
  //Train:
  AliAnalysisTaskFemto *taskfemto = new AliAnalysisTaskFemto("TaskFemto","$ALICE_PHYSICS/"+configMacroName,configMacroParameters,kFALSE);
  //Local:
  //AliAnalysisTaskFemto *taskfemto = new AliAnalysisTaskFemto("TaskFemto",configMacroName,configMacroParameters,kFALSE);
  //taskfemto->SelectCollisionCandidates(AliVEvent::kINT7);
  //AliAnalysisTaskFemto *taskfemto = new AliAnalysisTaskFemto("TaskFemto",configMacroName);
  mgr->AddTask(taskfemto);

  //MC PID (not needed for data):
  AliAnalysisTaskSE *pidresponse = AddTaskPIDResponse(kTRUE);

  // D. Configure the analysis task. Extra parameters can be used via optional
  // arguments of the AddTaskXXX() function.
  //===========================================================================

  // E. Create ONLY the output containers for the data produced by the task.
  // Get and connect other common input/output containers via the manager as below
  //==============================================================================
  TString outputfile = AliAnalysisManager::GetCommonFileName();
  outputfile += ":PWG2FEMTO";
  AliAnalysisDataContainer *cout_femto  = mgr->CreateContainer("KpKp_MC",  TList::Class(),AliAnalysisManager::kOutputContainer,outputfile);
  //AliAnalysisDataContainer *cout_femto  = mgr->CreateContainer(containerName,TList::Class(),AliAnalysisManager::kOutputContainer,outputfile);


   mgr->ConnectInput(taskfemto, 0, mgr->GetCommonInputContainer());
   mgr->ConnectOutput(taskfemto, 0, cout_femto);

   // Return task pointer at the end
   return taskfemto;
}
