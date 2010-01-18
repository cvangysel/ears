/*==========================================================================
 * Copyright (c) 2009, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     Model1.cpp
 * \brief    The "Model 1" association finding model
 * \date     2009-12-04
 * \version  1.0
 */

// EARS
#include "Model1.hpp"
#include "Logger.hpp"

// Lemur
#include "IndexedReal.hpp"
#include "TextQueryRep.hpp"
#include "UnigramLM.hpp"
#include "DocUnigramCounter.hpp"

#include <math.h> // C


///
void
ears::Model1::init()
{
  LOG( logINFO ) << "Initializing Model 1";
  LOG( logDEBUG ) << "version 1.0 (last mod. 2009-12-04)";
  
  // smoothing 
  initSmoothing();
  
  init_ = true;
}



///
void
ears::Model1::setSmoothingAuto()
{
  double sum_ne;
  int entityNum = 0;
  int progress = 0;
  int lastProgress = 0;
  for ( ID_T entityID = 0; entityID < entities_.entityNum(); entityID++ ) {    
    // indicate progress    
    progress = 100 * ( entityID + 1 ) / entities_.entityNum();
    if ( progress % 10 == 0 && progress > lastProgress ) {
      LOG( logDEBUG ) << progress << "% completed";
      lastProgress = progress;
    }
    // we ignore entities without any associated documents
    if ( entities_.entityDocNum( entityID ) > 0 ) {
      double ne = 0.0;
      const DISTR_T& pde = entities_.entityDocs( entityID );
      
      // n(e) = \sum_d (n(d) * p(d|e))
      for ( DISTR_T::const_iterator it=pde.begin(); it != pde.end(); it++ ) 
        ne += (double) index_.docLength( it->first ) * it->second;
      
      sum_ne += ne;
      entityNum++;
    }
  }
  
  // smoothingParam_ holds Dirichlet's beta, where \beta = (\sum_e n(e)) / |e|
  smoothingParam_ = (double) sum_ne / entityNum;  
}



///
ears::ASSOCRES_T
ears::Model1::scoreAll()
{
  ASSOCRES_T pqe_all;
  double lambda = smoothingParam_; // for J-M smoothing
  double beta = smoothingParam_; // for Dirichlet smoothing
  int entityNum = entities_.entityNum();
  int queryNum = queries_.queryNum();
  
  if ( !init_ ) {
    LOG( logERROR ) << "Model needs to be initialized first";
    exit( EXIT_FAILURE );                
  }
    
  LOG( logINFO ) << "Calculating topic-entity associations";

  LOG( logDEBUG ) << "Constructing collection Language Model";
  lemur::langmod::DocUnigramCounter* collCounter 
    = new lemur::langmod::DocUnigramCounter( index_ ); 
  lemur::langmod::UnigramLM *collModel 
    = new lemur::langmod::MLUnigramLM( *collCounter, index_.termLexiconID() );
  
  // caching
  LOG( logDEBUG ) << "Caching collection probabilities"; 

  // store p(t) for all query terms in a map
  std::map< lemur::api::TERMID_T, double > pt_cache;
  for ( ID_T qID = 0; qID < queryNum; qID++ ) {
    const DISTR_T& ptq = queries_.query( qID );
    for ( DISTR_T::const_iterator term_it = ptq.begin();
         term_it != ptq.end(); term_it++ ) {
      
      const lemur::api::TERMID_T& termID = term_it->first; // t
      pt_cache[termID] = collModel->prob( termID ); // p(t)
    }
  }
  
  // +++
  // calculate p(q|e) for all entities and queries
  // +++ 
  for ( ID_T entityID = 0; entityID < entityNum; entityID++ ) {
    LOG( logDEBUG ) << "Entity #" << (entityID+1) << "/" << entityNum
      << " (" << entities_.entityID( entityID ) << ")";
    
    std::vector< double > pqe; // p(q|e) for entity e
    pqe.resize( queryNum, 0.0 );
    
    // build entity language model
    int entityDocNum = entities_.entityDocNum( entityID );
    
    if ( entityDocNum > 0 ) {
      LOG( logDEBUG2 ) << "Creating model based on " 
        << entityDocNum << " associated documents";
      
      ears::DoubleDocUnigramCounter* entityCounter 
        = this->entityCounter( entityID );
      double ne = (double) entityCounter->sum(); // n(e)
      
      LOG( logDEBUG2 ) << "Scoring queries";
      for ( ID_T qID = 0; qID < queryNum; qID++ ) {
        LOG( logDEBUG2 ) << "  Query #" << (qID+1) << "/" << queryNum
          << " ('" << queries_.queryID( qID ) << "')";

        double score = 0.0;
        const DISTR_T& q = queries_.query( qID ); // q
        
        LOG( logDEBUG2 ) << "    log p(q|e) = ";

        for ( DISTR_T::const_iterator term_it = q.begin();
             term_it != q.end(); term_it++ ) {
          
          const lemur::api::TERMID_T& termID = term_it->first; // t
          const double& ptq = term_it->second; // p(t|q)
          double& pt = pt_cache[termID]; // p(t)                    
          double nte = entityCounter->count( termID ); // n(t,e)
          double pte; // p(t|\theta_e)
          
          // Jelinek-Mercer smoothing
          // p(t|\theta_e) = (1-\lambda) * (n(t,e)/n(e)) + \lambda * p(t)
          if ( smoothingMethod_ == "jm" ) 
            pte = (double) (1.0 - lambda) * (nte / ne) + lambda * pt;           
          // Dirichlet smoothing
          // p(t|\theta_e) = (n(t,e) + \beta * p(t)) / (n(e) + \beta)
          if ( smoothingMethod_ == "dirichlet" ) 
            pte = ((double) nte + beta * pt) / ((double) ne + beta);
          
          // log p(q|e) += p(t|q) * log ( p(t|\theta_e) )
          score += ptq * log( pte );
          
          LOG( logDEBUG2 ) << "      + p('" << index_.term( termID ) << "'|q) "
            << "[" << ptq << "] * log( p(t|e) [" << pte << "] )";
          
        } // query term t (termID)
        
        // record p(q|e)
        pqe[qID] = exp( score );
        
        LOG( logDEBUG2 ) << "      = " << score;
        LOG( logDEBUG2 ) << "    p(q|e) = " << pqe[qID];
        
      } // query q (qID)

      delete entityCounter;
    }
    // no associated documents -- entity scores 0 on all queries
    else {      
      LOG( logDEBUG2 ) << "No associated documents";
    }
    
    // record scores
    pqe_all.push_back( pqe );    
    
  } // entity e (entityID)
  
  delete collCounter;
  delete collModel;
  
  LOG( logINFO ) << "Model 1 completed";

  return pqe_all;
}



///
ears::DoubleDocUnigramCounter* 
ears::Model1::entityCounter( const ID_T& entityID )
{
  lemur::api::IndexedRealVector irv;
  const DISTR_T& pde = entities_.entityDocs( entityID );

  // consider all documents associated with the entity
  for ( DISTR_T::const_iterator it=pde.begin(); it != pde.end(); it++ ) 
    irv.PushValue( it->first, it->second );
  
  lemur::api::PseudoFBDocs* entityDocs = 
    new lemur::api::PseudoFBDocs( irv, -1, false );
  
  ears::DoubleDocUnigramCounter* counter
    = new ears::DoubleDocUnigramCounter( *entityDocs, index_ );
  
  irv.clear();
  delete entityDocs;
  
  return counter;
}


