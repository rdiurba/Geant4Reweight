#include "G4ReweightParameterMaker.hh"

G4ReweightParameterMaker::G4ReweightParameterMaker( const std::map< std::string, std::vector< FitParameter > > & pars ){


  double dummyX = 0.;
  double dummyY = 1.;

/*
  dummyHist  = new TH1D("dummy", "", 1,0,0);
  //Set the over/underflow bins for the dummy 
  dummyHist->SetBinContent(0,1.);
  dummyHist->SetBinContent(1,1.);
  dummyHist->SetBinContent(2,1.);
*/

  std::map< std::string, std::vector< FitParameter > >::iterator itPar;
  std::map< std::string, bool > CutIsDummy;
  for( auto itPar = pars.begin(); itPar != pars.end(); ++itPar ){
    std::string name = itPar->first;  
    if( name == "reac" ) continue;
    std::cout << "Cut: " << name << std::endl;
    
    bool isDummy = false;

    std::vector< std::pair< double, double > > vars;
    std::vector< double > varX, varY; 

    for( size_t i = 0; i < itPar->second.size(); ++i ){
      
      if( itPar->second.at( i ).Dummy ){
        std::cout << "Dummy" << std::endl;
//        FSHists[name] = (TH1D*)dummyHist->Clone();
        FSHists[name]  = new TH1D( ("dummy_" + name).c_str(), "", 1,0,0);
        std::cout << "Made" << std::endl;
        //Set the over/underflow bins for the dummy 
        FSHists[name]->SetBinContent(0,1.);
        FSHists[name]->SetBinContent(1,1.);
        FSHists[name]->SetBinContent(2,1.);
        std::cout << "Dummy N: " << FSHists[name]->GetNbinsX() << std::endl;
        isDummy = true;
        std::cout << "Set isDummy" << std::endl;
        break;
      }
  
      else{        
        
        double value = itPar->second.at( i ).Value;
        std::pair< double, double > range = itPar->second.at( i ).Range;

        vars.push_back( std::make_pair( range.first,  value ) );
        vars.push_back( std::make_pair( range.second, value ) );

        bool addDummyBin = false;
        if( varX.size() ){
          //If the end of last bin == start of this bin
          //don't need to add a dummy
          if( varX.back() < range.first ){
            varX.push_back( range.first );
            addDummyBin = true;
          }
        }
        else
          varX.push_back(range.first);
        
        varX.push_back( range.second );

        if( addDummyBin )
          varY.push_back( 1. );
        varY.push_back( value );
      }
  
    }

    CutIsDummy[ name ] = isDummy;
    
    if( !isDummy ){
      for( size_t i = 0; i < vars.size(); ++i ){
        std::cout << vars.at(i).first << " " << vars.at(i).second << std::endl;
      }

      std::cout << "Sizes: " << varX.size() << " " << varY.size() << std::endl;
      FSHists[name] = new TH1D( ("var"+name).c_str(),"", varX.size()-1, &varX[0]);
      std::cout << "Setting" << std::endl;
      for( size_t i = 0; i < varY.size(); ++i ){
        std::cout << i << " " << varX[i] << " " << varY[i] << std::endl;
        FSHists[name]->SetBinContent(i+1, varY[i]);
        std::cout << "Center,Content: " << FSHists[name]->GetBinCenter(i+1) << " " << FSHists[name]->GetBinContent(i+1) << std::endl;
      }
      //Set under/overflow
      FSHists[name]->SetBinContent( 0, 1. );
      FSHists[name]->SetBinContent( FSHists[name]->GetNbinsX()+1, 1. );
    }
  }

  if( pars.find( "reac" ) != pars.end() ){
    if( !pars.at( "reac" ).at(0).Dummy ){
      //If reac exists and is not a dummy, go through the exclusive channels 
      //and vary each by the reac variations

      //Build the reac graph
      std::vector< double > reac_bins, varY, reacBins;
      for( size_t i = 0; i < pars.at( "reac" ).size(); ++i ){
        double value = pars.at( "reac" ).at( i ).Value;
        std::pair< double, double > range = pars.at( "reac" ).at( i ).Range;
        std::cout << i << " Range: " << range.first << " " << range.second << std::endl;
        std::cout << i << " Value: " << value << std::endl;

        bool addDummyBin = false;
        if( reac_bins.size() ){
          //If the end of last bin == start of this bin
          //don't need to add a dummy
          if( reac_bins.back() < range.first ){
            reac_bins.push_back( range.first );
            addDummyBin = true;
          }
        }
        else
          reac_bins.push_back(range.first);
        
        reac_bins.push_back( range.second );

        if( addDummyBin )
          varY.push_back( 1. );
        varY.push_back( value );

      }

      std::cout << "Sizes: " << reac_bins.size() << " " << varY.size() << std::endl;
      TH1D reac_hist( "var_reac","", reac_bins.size()-1, &reac_bins[0]);
      std::cout << "Setting" << std::endl;
      for( size_t i = 0; i < varY.size(); ++i ){
        std::cout << i << " " << reac_bins[i] << " " << varY[i] << std::endl;
        reac_hist.SetBinContent(i+1, varY[i]);
      }
      //Set under/overflow
      reac_hist.SetBinContent( 0, 1. );
      reac_hist.SetBinContent( reac_hist.GetNbinsX()+1, 1. );      

      std::cout << "reac found. Varying exclusives:" << std::endl;
      for( auto itGr = FSHists.begin(); itGr != FSHists.end(); ++itGr ){
        std::string name = itGr->first;
        std::cout << name << " " << itGr->second << std::endl;

        auto excHist = itGr->second;
        std::vector< double > exc_bins;
        if( !CutIsDummy[name] ){
          for( int i = 1; i <= excHist->GetNbinsX(); ++i ){
            exc_bins.push_back( excHist->GetBinLowEdge(i) );         
          }
          exc_bins.push_back( excHist->GetBinLowEdge(excHist->GetNbinsX()) + excHist->GetBinWidth(excHist->GetNbinsX()) ); 
        }

        std::vector< double > new_bins = exc_bins;
        for( size_t i = 0; i < reac_bins.size(); ++i ){
          if( std::find( new_bins.begin(), new_bins.end(), reac_bins[i] ) 
          == new_bins.end() ){
            new_bins.push_back( reac_bins[i] );  
          }
        }

        std::sort( new_bins.begin(), new_bins.end() );
        for( size_t i = 0; i < new_bins.size(); ++i ){
          std::cout << new_bins[i] << std::endl;
        }

        TH1D new_hist( "new_hist", "", new_bins.size()-1, &new_bins[0] );
        for( int i = 1; i <= new_hist.GetNbinsX(); ++i ){
          double x = new_hist.GetBinCenter( i );
          int reac_bin = reac_hist.FindBin( x );
          int exc_bin = excHist->FindBin( x );

          std::cout << i << " " << x << " " << reac_bin << " " << exc_bin << std::endl;
          std::cout << "\t" << reac_hist.GetBinContent( reac_bin ) << " " << excHist->GetBinContent( exc_bin ) << std::endl;
          double content = reac_hist.GetBinContent( reac_bin );
          content *= excHist->GetBinContent( exc_bin );

          new_hist.SetBinContent(i, content );
        }

        std::string exc_name = excHist->GetName();
        (*excHist) = new_hist;
        excHist->SetName( exc_name.c_str() );

      }

    }   
  }
}

void G4ReweightParameterMaker::SetNewVals( const std::map< std::string, std::vector< FitParameter > > & pars ){

  
  for( auto itPar = pars.begin(); itPar != pars.end(); ++itPar ){

    std::string name = itPar->first;
    if( name == "reac" ) continue;   

    TH1D * excHist = FSHists[name];

    auto thePars = itPar->second;
    for( size_t i = 0; i < thePars.size(); ++i ){

      //First check if it is a dummy
      if( thePars[i].Dummy ){
        std::cout << "Dummy" << std::endl;
        for( int j = 1; j <= excHist->GetNbinsX(); ++j ){
          excHist->SetBinContent(j,1.);
        }
        break;
      }

      double start = thePars[i].Range.first;
      double end   = thePars[i].Range.second;
      
      std::cout << "Bins:" << std::endl;
      for( int j = 1; j <= excHist->GetNbinsX(); ++j ){
        double bin_low = excHist->GetBinLowEdge(j);
        double bin_high = excHist->GetBinLowEdge(j+1);

        std::cout << bin_low << " " << bin_high << std::endl; 

        if( ( start <= excHist->GetBinLowEdge(j) ) && ( end >= excHist->GetBinLowEdge(j+1) ) ){
          std::cout << "Bin is in range " << std::endl;
          excHist->SetBinContent(j, thePars[i].Value);
        }

        
      }
    }
    
  }

  //Now Get the reactive and go back and vary all others
  if( pars.find( "reac" ) != pars.end() ){
   
    auto reacPars = pars.at( "reac" );
    if( !reacPars[0].Dummy ){
      for( size_t i = 0; i < reacPars.size(); ++i ){
        double reac_start = reacPars[i].Range.first; 
        double reac_end   = reacPars[i].Range.second; 
        double reac_val   = reacPars[i].Value;

        for( auto itHist = FSHists.begin(); itHist != FSHists.end(); ++itHist ){ 

          TH1D * excHist = itHist->second; 
          for( int j = 1; j <= excHist->GetNbinsX(); ++j ){
            double bin_low = excHist->GetBinLowEdge(j);
            double bin_high = excHist->GetBinLowEdge(j+1);
            
            std::cout << bin_low << " " << bin_high << std::endl;

            if( ( reac_start <= excHist->GetBinLowEdge(j) ) && ( reac_end >= excHist->GetBinLowEdge(j+1) ) ){
              std::cout << "Bin is in range " << std::endl;
              
              double exc_val = excHist->GetBinContent(j);
              excHist->SetBinContent(j, reac_val * exc_val);
            }
          }
        }
      }
    }
  }
}
