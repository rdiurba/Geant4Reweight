#ifndef G4Reweighter_h
#define G4Reweighter_h

#include <string>
#include <utility>

#include "TFile.h"
#include "TH1D.h"
#include "TGraph.h"

#include "G4ReweightTraj.hh"

#include <cmath>
#include <map>
#include "TTree.h"

#include <iostream>

class G4ReweightTraj;
//class G4ReweightStep;

class G4Reweighter{
  public:

    G4Reweighter(){};
    G4Reweighter(TFile *, TFile *, std::map< std::string, TGraph*> &);
    G4Reweighter(TFile *, TFile *, const std::map< std::string, TH1D*> &, TH1D * inputElasticBiasHist=0x0, bool fix=false);
    virtual ~G4Reweighter();

    void Initialize(TFile *, TFile *, std::map< std::string, TGraph*> &);
    void Initialize(TFile *, TFile *, const std::map< std::string, TH1D*> &, TH1D * inputElasticBiasHist=0x0, bool fix=false);

    double GetWeight( std::string, double );
    double GetWeightFromGraph( std::string, double );

    double GetWeight( const G4ReweightTraj * theTraj );
    virtual std::string GetInteractionSubtype( const G4ReweightTraj & );


    double GetElasticWeight( const G4ReweightTraj * );
    double GetDecayMFP( double );
    //double GetCoulMFP( double );
    double GetNominalMFP( double );
    double GetBiasedMFP( double );
    double GetNominalElasticMFP( double );
    double GetBiasedElasticMFP( double );
    double GetInelasticBias(double p);
    double GetElasticBias(double p);

    void SetTotalGraph( TFile * );

    void SetNewHists( const std::map< std::string, TH1D* > &FSScales );
    void SetNewElasticHists(TH1D * inputElasticBiasHist);
    void SetBaseHists( const std::map< std::string, TH1D* > &FSScales );

    TH1D * GetTotalVariation(){ return totalVariation; };
    TGraph * GetTotalVariationGraph(){ return totalVariationGraph; };
    TH1D * GetExclusiveVariation( std::string );
    TGraph * GetExclusiveVariationGraph( std::string );
    TH1D * GetOldHist( std::string cut ){ return oldHists[cut]; };
    TH1D * GetNewHist( std::string cut ){ return newHists[cut]; };
    TGraph * GetOldGraph( std::string cut ){ return oldGraphs[cut]; };
    TGraph * GetNewGraph( std::string cut ){ return newGraphs[cut]; };

    void AddGraphs(TGraph*, TGraph*);
    void DivideGraphs(TGraph*, TGraph*);
    bool AsGraph(){return as_graphs; };

    TGraph * GetTotalGraph(){ return totalGraph; };

  protected:

    bool as_graphs = false;
    bool fix_total = false;

    std::map< std::string, TH1D* > exclusiveVariations;
    TH1D * totalVariation;

    std::map< std::string, TGraph* > exclusiveVariationGraphs;
    TGraph * totalVariationGraph;

    TGraph * totalGraph;
    TGraph * elasticGraph;
    TGraph * decayGraph;
    //TGraph * coulGraph;

    TH1D * elasticBias;

    double Maximum;
    double Minimum;

    // These should be set in the constructor of the actual reweighter you use (e.g. G4PiPlusReweighter/G4PiMinusReweighter/G4ProtonReweighter)
    std::string fInelastic;
    std::vector< std::string > theInts;

    std::map< std::string, TH1D* > oldHists;
    std::map< std::string, TH1D* > newHists;
    std::map< std::string, TGraph* > oldGraphs;
    std::map< std::string, TGraph* > newGraphs;

    double Mass;
    double Density;
};

#endif
