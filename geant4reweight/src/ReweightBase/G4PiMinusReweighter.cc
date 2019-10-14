#include "G4PiMinusReweighter.hh"

std::string G4PiMinusReweighter::GetInteractionSubtype( G4ReweightTraj & theTraj ){
    int nPi0     = theTraj.HasChild(111).size();  
    int nPiPlus  = theTraj.HasChild(211).size();
    int nPiMinus = theTraj.HasChild(-211).size();

    if( (nPi0 + nPiPlus + nPiMinus) == 0){
      return "abs";
    }
    else if( (nPiPlus + nPiMinus) == 0 && nPi0 == 1 ){
      return "cex";
    }
    else if( (nPiPlus + nPiMinus + nPi0) > 1 ){
      return "prod";
    }
    else if( (nPi0 + nPiMinus) == 0 && nPiPlus == 1 ){
      return "dcex";
    }
    else if( (nPi0 + nPiPlus) == 0 && nPiMinus == 1 ){
      return "inel"; 
    }

    return "default";
}

G4PiMinusReweighter::~G4PiMinusReweighter(){}
