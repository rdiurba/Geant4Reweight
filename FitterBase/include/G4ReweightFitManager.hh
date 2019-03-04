#ifndef G4ReweightFitManager_h
#define G4ReweightFitManager_h


#include "TGraph.h"
#include "TGraphErrors.h"
#include "TTree.h"
#include "TFile.h"
#include "TDirectory.h"
#include "FitSample.hh"
#include "FitParameter.hh"
#include "G4ReweightFitter.hh"
#include "G4ReweightHandler.hh"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include "tinyxml2.h"

#include "fhiclcpp/ParameterSet.h"

class G4ReweightFitManager{
  public:
    G4ReweightFitManager(std::string &);


    void MakeFitParameters( std::vector< fhicl::ParameterSet > &);

    bool CheckIsDummy( std::string theCut ){
      //Maybe throw an exception if not in the map?
      return CutIsDummy.at( theCut );
    };

    void DefineMCSets( std::vector< fhicl::ParameterSet > &);
    void DefineExperiments( fhicl::ParameterSet &);
    void GetAllSamples( std::vector< fhicl::ParameterSet > & );
    void GetAllData();
    void RunFitAndSave();
    
    bool IsSetActive( std::string theSetName ){
      return ( std::find( sets.begin(), sets.end(), theSetName ) != sets.end() );
    };


  protected:
    TFile      * out;
    TDirectory * data_dir;

    std::map< std::string, std::vector< G4ReweightFitter* > > mapSetsToFitters;
    std::vector< G4ReweightFitter* > allFitters;

    std::map< std::string, std::vector< FitParameter > > FullParameterSet;
    std::map< std::string, bool > CutIsDummy;

    std::vector< std::string > sets;

    size_t nSamples;
    
    G4ReweightHandler handler;

};

#endif
