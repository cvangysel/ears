/*==========================================================================
 * Copyright (c) 2009-2010, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     AssociationFramework.cpp
 * \brief    Association finding framework
 * \date     2010-01-18
 * \version  1.05
 */

// EARS
#include "AssociationFramework.hpp"
#include "AssociationModel.hpp"
#include "Logger.hpp"
#include "Model1.hpp"
#include "Model2.hpp"
#include "OutputWriter.hpp"

// Lemur
#include "IndexManager.hpp"
#include "Param.hpp"


///
void 
ears::AssociationFramework::run( std::string task )
{
  //
  // load parameters
  //
  if ( task != "ef" && task != "ep" ) {
    LOG( logERROR ) << "unsupported task: '" << task << "'";
    exit( EXIT_FAILURE );
  }  
  
  std::string modelname = lemur::api::ParamGetString( "model" );
  if ( modelname.length() == 0 ) {
    LOG( logERROR ) << "Parameter 'model' is not set";
    exit( EXIT_FAILURE );    
  }
  if ( modelname != "1" && modelname != "2" ) {
    LOG( logERROR ) << "Unknown model '" << modelname << "'. Valid values: 1 or 2";
    exit( EXIT_FAILURE );            
  }
  
  // init stat collector
  stat_ = new Stat();
  
  //
  // load index
  //
  std::string indexFile = lemur::api::ParamGetString( "index" );
  if ( indexFile.length() == 0 ) {
    LOG( logERROR ) << "Parameter 'index' is not set";
    exit( EXIT_FAILURE );    
  }
  LOG( logINFO ) << "Loading index '" << indexFile << "'";
  try {    
    index_ = lemur::api::IndexManager::openIndex( indexFile );    
  }
  catch ( const lemur::api::Exception& e ) {
    LOG( logERROR ) << "Can't open index, check parameter index";
    exit( EXIT_FAILURE );
  }      
  LOG( logDEBUG ) << index_->docCount() << " documents";

  //
  // load associations; note that they depend on the model
  // 
  if ( modelname == "1" )   
    entities_ = new EntityRep( EntityRep::aiENTITY );
  if ( modelname == "2" )
    entities_ = new EntityRep( EntityRep::aiDOCUMENT );
  entities_->loadAssociations( *index_ );
  
  //
  // load queries
  //
  queries_ = new QueryRep();
  queries_->loadQueries( *index_ );
  
  //
  // Run model, i.e,. calculate p(q|e) for all queries and entities
  //
  AssociationModel* model;
  if ( modelname == "1" ) 
    model = new Model1( *index_, *entities_, *queries_, *stat_ );
  if ( modelname == "2" )
    model = new Model2( *index_, *entities_, *queries_, *stat_ );
  model->init();
  ASSOCRES_T pqe = model->scoreAll();  
  delete model;

  //
  // write output
  //
  OutputWriter* output = new OutputWriter( *entities_, *queries_ );
  output->outputAssocRes( task, pqe );
  delete output;

  //
  // output collected statistics (only in debug mode)
  //
  stat_->outputStat();
  
  //
  // clean up
  //
  pqe.clear();
  delete entities_;
  delete queries_;
  delete index_;  
  
  LOG( logINFO ) << "Over and out";
}
