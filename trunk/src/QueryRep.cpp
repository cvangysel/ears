/*==========================================================================
 * Copyright (c) 2009, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     QueryRep.cpp
 * \brief    Query representation
 * \date     2009-09-09
 * \version  0.9
 */

// EARS
#include "QueryRep.hpp"
#include "Logger.hpp"

// Lemur
#include "BasicDocStream.hpp"
#include "Param.hpp"
#include "TextQuery.hpp"

/// String used for unknown queries
#define UNKNOWN_QUERY "[UNKNOWN_QUERY]"


///
void 
ears::QueryRep::loadQueries( const lemur::api::Index& index )
{
  //
  // parameters
  //
  std::string queryFile = lemur::api::ParamGetString( "queryFile" );
  if ( queryFile.length() == 0 ) {
    LOG( logERROR ) << "Parameter 'queryFile' is not set";
    exit( EXIT_FAILURE );    
  }
  
  //
  // loading queries from file
  //
  double weight;
  double sumWeight;
  lemur::api::TERM_T term;
  lemur::api::TERMID_T termID;
  EXID_T queryID;
  
  LOG( logINFO ) << "Loading queries '" << queryFile << "'";
  
  try {
    // use Lemur's docStream object to parse queries
    lemur::api::DocStream* qryStream = new lemur::parse::BasicDocStream( queryFile );
    qryStream->startDocIteration();
    
    while ( qryStream->hasMore() ) {      
      lemur::api::Document* d = qryStream->nextDoc();
      queryID = d->getID();      
      LOG( logDEBUG ) << "Query '" << queryID << "':";
      
      // query terms
      DISTR_T queryTerms;
      sumWeight = 0.0;
      lemur::api::TextQuery* q = new lemur::api::TextQuery( *d );      
      
      LOG( logDEBUG2 ) << "Data read from query file:";
      
      q->startTermIteration();
      while ( q->hasMore() ) {	
        const lemur::api::Term* t = q->nextTerm();
        std::string line = t->spelling();
        int pos = line.find(";");
                
        // query format 1: "term;weight\n"
        if ( pos != string::npos) {          
					term = line.substr( 0, pos );
					char *pEnd;
					weight = strtod( line.substr( pos+1, line.length() ).c_str(), &pEnd );          
        }
        // query format 0: "term\n"
        else {
          term = line;
          weight = 1.0;
        }
        LOG( logDEBUG2 ) << "  TERM: '" << term << "'   WEIGHT: " << weight;
        
        termID = index.term( term );
        if ( termID > 0 ) {
          queryTerms[termID] = weight; 
          sumWeight += weight;
        }
        else {
          LOG( logWARNING ) << "Term '" << term << "' does not exist in index"
          << " (query term ignored)";          
        }
      }
      
      if ( sumWeight > 0.0 ) {              
        // normalizing term distribution
        DISTR_T queryDistr;
        DISTR_T::const_iterator it;
        
        LOG( logDEBUG2 ) << "Internal query representation:";
        
        for ( it = queryTerms.begin(); it != queryTerms.end(); it++ ) {
          weight = (double) it->second / sumWeight;
          queryDistr[it->first] = weight;
          
          LOG( logDEBUG ) << "    TERM: '" << index.term( it->first ) << "'"
          << " (ID " << it->first << ")   WEIGHT: " << weight;
        }      
        
        // recording query
        queryID_.push_back( queryID );			
        ptq_.push_back( queryDistr );
      }
      else {
        LOG( logWARNING ) << "Query '" << queryID << "' is empty; ignored";
      }
      
      delete q;
    }
    
    delete qryStream;
  } 
  catch ( const lemur::api::Exception& e ) {
    LOG( logERROR ) << "Can't open query file, check parameter queryFile";
    exit( EXIT_FAILURE );
  } 
  
}



///
const int 
ears::QueryRep::queryNum() const
{
  return queryID_.size();
}



///
const ears::EXID_T 
ears::QueryRep::queryID( const int& qID ) const
{
  if ( qID >= 0 && qID < queryID_.size() )
    return queryID_[qID];
  else
    return UNKNOWN_QUERY;
}



///
const ears::DISTR_T 
ears::QueryRep::query( const int& qID ) const
{
  if ( qID >= 0 && qID < ptq_.size() ) 
    return ptq_[qID];
  else {
    LOG( logERROR ) << "Reference to non-existing query";
    exit( EXIT_FAILURE );        
  }  
}



///
lemur::retrieval::SimpleKLQueryModel* 
ears::QueryRep::query( const lemur::api::Index& index, const int& qID ) const
{
  lemur::retrieval::SimpleKLQueryModel* qModel 
  = new lemur::retrieval::SimpleKLQueryModel( index );
  
  const DISTR_T& q = this->query( qID );
  for ( DISTR_T::const_iterator term_it = q.begin();
       term_it != q.end(); term_it++ ) {    
    const lemur::api::TERMID_T& termID = term_it->first; // t
    const double& ptq = term_it->second; // p(t|q)    
    qModel->setCount( termID, ptq );
  }

  return qModel;
}
