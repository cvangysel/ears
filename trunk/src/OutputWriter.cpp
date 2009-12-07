/*==========================================================================
 * Copyright (c) 2009, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     OutputWriter.cpp
 * \brief    Writing results output
 * \date     2009-09-09
 * \version  0.9
 */

// EARS
#include "OutputWriter.hpp"
#include "Logger.hpp"

// Lemur
#include "Param.hpp"


///
void 
ears::OutputWriter::outputAssocRes( const std::string& task, 
                                    const ASSOCRES_T& pqe )
{  
  //
  // parameters
  //
  std::string outputFile = lemur::api::ParamGetString( "outputFile" );
  int outputFileFormat = lemur::api::ParamGetInt( "outputFileFormat", 0 );
  int outputNum = lemur::api::ParamGetInt( "outputNum", 0 );
  std::string runID = lemur::api::ParamGetString( "runID", "EARS" );
  
  if ( outputFile.length() == 0 ) {
    LOG( logERROR ) << "Parameter 'outputFile' is not set";
    exit( EXIT_FAILURE );    
  }
  
  //
  // writing output to file
  //
  LOG( logINFO ) << "Writing output to '" << outputFile << "'";
  fstream outFile ( outputFile.c_str(), ios::out | ios::trunc );
  lemur::api::IndexedRealVector res;
  
  if ( outFile.is_open() ) {
    
    // Output for entity finding
    if ( task == "ef" ) {      
      for ( ID_T qID = 0; qID < queries_.queryNum(); qID++ ) { // for each query        
        const EXID_T queryID = queries_.queryID( qID );
        this->resultsFinding( pqe, qID, res );
        
        if ( outputNum > 0 ) // limit the number of results
          res.resize( outputNum );
        
        LOG( logDEBUG2 ) << "Query #" << (qID+1) << " ('" << queryID << "')"
          << " top "<< res.size() << " results";
        
        this->writeRecord( outFile, task, outputFileFormat, queryID, res, runID );        
      } // qID
    } // ef

    // Output for entity profiling
    if ( task == "ep" ) {      
      for ( ID_T eID = 0; eID < entities_.entityNum(); eID++ ) { // for each entity
        const EXID_T entityID = entities_.entityID( eID );
        this->resultsProfiling( pqe, eID, res );
        
        if ( outputNum > 0 ) // limit the number of results
          res.resize( outputNum );
        
        LOG( logDEBUG2 ) << "Entity #" << (eID+1) << " ('" << entityID 
          << "') top "<< res.size() << " results";
        
        this->writeRecord( outFile, task, outputFileFormat, entityID, res, runID );        
      } // entityID
    } // ep
    
    outFile.close();
  }
  else {
    LOG( logERROR ) << "Can't open '" << outputFile << "', check path and permissions!";
    exit( EXIT_FAILURE );
  }
  
}



///
void 
ears::OutputWriter::resultsFinding( const ASSOCRES_T& pqe, 
                                    const ID_T qID,
                                    lemur::api::IndexedRealVector& res )
{
  res.clear();
  for ( ID_T eID = 0; eID < entities_.entityNum(); eID++ ) {
    res.PushValue( eID, pqe[eID][qID] );      
  }
  res.Sort();
}



///
void 
ears::OutputWriter::resultsProfiling( const ASSOCRES_T& pqe, 
                                      const ID_T eID,
                                      lemur::api::IndexedRealVector& res )
{
  res.clear();
  for ( ID_T qID = 0; qID < queries_.queryNum(); qID++ ) {
    res.PushValue( qID, pqe[eID][qID] );      
  }
  res.Sort();
}



///
void
ears::OutputWriter::writeRecord( fstream& outFile, 
                                 const std::string& task,
                                 const int& outFileFormat,
                                 const std::string& ID,
                                 const lemur::api::IndexedRealVector& res,
                                 const std::string& runID
                                )
{
  for ( int i = 0; i < res.size(); i++ ) {
    const double& score = res[i].val;
    
    EXID_T resID;
    if ( task == "ef" )
      resID = entities_.entityID( res[i].ind );
    if ( task == "ep" )
      resID = queries_.queryID( res[i].ind );
            
    // output format 
    switch ( outFileFormat ) {
      case 1:
        // ID resID score
        outFile << ID << " " << resID << " " << score << endl;
        break;
      default: // 0
        // ID Q0 resID rank score runID
        outFile << ID << " Q0 " << resID << " " << (i+1) << " " 
          << score << " " << runID << endl;              
        break;
    } // outputFileFormat             
  } // i
}
