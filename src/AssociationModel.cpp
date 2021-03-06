/*==========================================================================
 * Copyright (c) 2009, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     AssociationModel.cpp
 * \brief    Abstract interface for an association finding model
 * \date     2009-12-04
 * \version  1.0
 */

// EARS
#include "AssociationModel.hpp"
#include "Logger.hpp"

// Lemur
#include "Param.hpp"


///
void ears::AssociationModel::initSmoothing()
{
  std::string smethod;
  std::string sparam;
  bool autoSmoothing = false;
  
  smoothingMethod_ = lemur::api::ParamGetString( "smoothingMethod" );
  smoothingParam_ = lemur::api::ParamGetDouble( "smoothingParam", 0.0 );
  
  // default is Dirichlet smoothing
  if ( smoothingMethod_ == "" ) 
     smoothingMethod_ = "dirichlet";
  
  // setting smoothing parameter automatically
  // (if auto smoothing then it's always Dirichlet smoothing)
  if ( smoothingParam_ == 0.0 ) {
    LOG( logINFO ) << "Estimating smoothing parameter (automatically)";
    autoSmoothing = true;
    smoothingMethod_ = "dirichlet";
    this->setSmoothingAuto();
  }
  
  // display smoothing method and parameter  
  if ( smoothingMethod_ == "dirichlet" ) { // Dirichlet
    smethod = "Dirichlet";
    sparam = "beta";
  }
  else if ( smoothingMethod_ == "jm" ) { // Jelinek-Mercer
    smethod = "Jelinek-Mercer";
    sparam = "lambda";
  }  
  else {
    LOG( logERROR ) << "Unknown smoothing method '" << smoothingMethod_ << "'";
    exit( EXIT_FAILURE );
  }
  
  if ( autoSmoothing )
    sparam += ", auto";
  
  LOG( logDEBUG ) << "Smoothing method: " << smethod; 
  LOG( logDEBUG ) << "Smoothing parameter (" << sparam << "): " << smoothingParam_;
}
