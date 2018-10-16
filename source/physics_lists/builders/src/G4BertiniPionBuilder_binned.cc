//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: G4BertiniPionBuilder_binned.cc 83699 2014-09-10 07:18:25Z gcosmo $
//
//---------------------------------------------------------------------------
//
// ClassName:   G4BertiniPionBuilder_binned
//
// Author: 2010 G.Folger
//  devired from G4BertiniPiKBuilder_binned
//
// Modified:
// 02.04.2009 V.Ivanchenko remove add cross section, string builderis reponsible 
//
//----------------------------------------------------------------------------
//
#include "G4BertiniPionBuilder_binned.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4CrossSectionDataSetRegistry.hh"


/*G4BertiniPionBuilder_binned::
G4BertiniPionBuilder_binned() 
 {
   thePiData = (G4PiNuclearCrossSection_binned*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4PiNuclearCrossSection_binned::Default_Name());
   theMin = 0*GeV;
   theMax = 9.9*GeV;
   theModel = new G4CascadeInterface;
   theModel->SetMinEnergy(theMin);
   theModel->SetMaxEnergy(theMax); 
 }*/

G4BertiniPionBuilder_binned::
G4BertiniPionBuilder_binned(G4ReweightHist * bias_hist) 
 {
   thePiData = (G4PiNuclearCrossSection_binned*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4PiNuclearCrossSection_binned::Default_Name());
   thePiData->SetBias(bias_hist);
   std::cout << "SetBias" << std::endl;
   theMin = 0*GeV;
   theMax = 9.9*GeV;
   theModel = new G4CascadeInterface;
   theModel->SetMinEnergy(theMin);
   theModel->SetMaxEnergy(theMax); 
 }

G4BertiniPionBuilder_binned::~G4BertiniPionBuilder_binned() 
{
}

void G4BertiniPionBuilder_binned::
Build(G4PionPlusInelasticProcess * aP)
 {
   aP->RegisterMe(theModel);
   theModel->SetMinEnergy(theMin);
   theModel->SetMaxEnergy(theMax);
 }

void G4BertiniPionBuilder_binned::
Build(G4PionMinusInelasticProcess * aP)
 {
   aP->RegisterMe(theModel);
   theModel->SetMinEnergy(theMin);
   theModel->SetMaxEnergy(theMax);
 }

void G4BertiniPionBuilder_binned::
Build(G4HadronElasticProcess * ) {}

         