/*==========================================================================
 * Copyright (c) 2009, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     Model2.cpp
 * \brief    The "Model 2" association finding model
 * \date     2009-09-09
 * \version  0.9
 */

// EARS
#include "Model2.hpp"
#include "Logger.hpp"

// Lemur
#include "DocInfoList.hpp"
#include "Param.hpp"
#include "QueryDocument.hpp"
#include "ScoreAccumulator.hpp"
#include "SimpleKLRetMethod.hpp"
#include "TermInfoList.hpp"
#include "TextQuery.hpp"

#include <set> // stl
#include <math.h> // C


///
void
ears::Model2::init()
{
  LOG( logINFO ) << "Initializing Model 2";
  LOG( logDEBUG ) << "version 1.0 (last mod. 2009-09-09)";

  // smoothing  
  initSmoothing();
  smoothSupportFile_ = lemur::api::ParamGetString( "smoothSupportFile" );

  // top documents
  topDocNum_ = lemur::api::ParamGetInt( "topDocNum", 0 );
  
  init_ = true;
}



///
void
ears::Model2::setSmoothingAuto()
{
  // smoothingParam_ holds Dirichlet's beta, where \beta = (\sum_d n(d)) / |d|,
  // i.e., the average document length
  smoothingParam_ = (double) index_.docLengthAvg();  
}



///
ears::ASSOCRES_T
ears::Model2::scoreAll()
{
  lemur::api::DOCID_T docID;
  int entityNum = entities_.entityNum();
  int queryNum = queries_.queryNum();

  // init p(q|e)
  ASSOCRES_T pqe_all;
  pqe_all.resize( entityNum );
  for ( ID_T entityID = 0; entityID < pqe_all.size(); entityID++ )
    pqe_all[entityID].resize( queryNum, 0.0 );
  
  if ( !init_ ) {
    LOG( logERROR ) << "Model needs to be initialized first";
    exit( EXIT_FAILURE );                
  }
      
  // +++
  // calculate p(q|e) for all entities and queries
  // +++
  LOG( logINFO ) << "Calculating topic-entity associations";

  if ( topDocNum_ > 0 )
    LOG( logDEBUG ) << "Considering only top " << topDocNum_ << " documents per query";
  else 
    LOG( logDEBUG ) << "Considering all documents per query";
  
  
  for ( ID_T qID = 0; qID < queryNum; qID++ ) {
    LOG( logDEBUG ) << "Query #" << (qID+1) << "/" << queryNum
    << " ('" << queries_.queryID( qID ) << "')";
    
    //
    // retrieving relevant documents
    //
    LOG( logDEBUG2 ) << "Retrieving documents";
        
    lemur::api::IndexedRealVector pqd_all; // p(q|d) for all documents
    this->scoreCollection( qID, pqd_all );

    // limiting the number of documents considered
    if ( topDocNum_ > 0 ) 
      pqd_all.resize( topDocNum_ );
    
    // 
    // associating relevant documents with entities 
    //    
    LOG( logDEBUG2 ) << "Associating relevant documents with entities";
    
    for ( int i = 0; i < pqd_all.size(); i++ ) {
      const double& pqd = pqd_all[i].val;
      docID = pqd_all[i].ind;
      
      LOG( logDEBUG2 ) << "  DOC '" << index_.document( docID ) << "' "
        << "p(q|d) = " << pqd;
      
      // entities associated with docID
      const DISTR_T& pde_doc = entities_.docEntities( docID );
      
      if ( pde_doc.size() > 0 ) {
        LOG( logDEBUG2 ) << "    " << pde_doc.size() << " associated entities:";
        for ( DISTR_T::const_iterator pde_it = pde_doc.begin(); 
              pde_it != pde_doc.end(); pde_it++ ) {
          const ID_T& entityID = pde_it->first;
          const double& pde = pde_it->second;

          // p(q|e) = \sum_d p(q|d) * p(d|ca)
          pqe_all[entityID][qID] += (double) pqd * pde;
          
          LOG( logDEBUG2 ) << "    ENTITY '" << entities_.entityID( entityID ) 
          << " (ID " << entityID << ") ";
          LOG( logDEBUG2 ) << "      p(q|e) += p(q|d) [" << pqd << "] "
          << "* p(d|e) [" << pde << "]  =>  p(q|e) = " << pqe_all[entityID][qID];
        }
      }
      else {
        LOG( logDEBUG2 ) << "    No associated entities";        
      }
    } // relevant doc at rank i
    
    // clean up
    pqd_all.clear();
  } // qID  
      
  LOG( logINFO ) << "Model 2 completed";
  
  return pqe_all;
}



///
/* -------------------------------------------------------------------------
 *  document retrieval based on Lemur' SimpleKLRetMethod
 * ------------------------------------------------------------------------- */
void 
ears::Model2::scoreCollection( const int& qID, lemur::api::IndexedRealVector& pqd_all ) 
{
  // init retrieval method
  lemur::retrieval::ArrayAccumulator* accumulator 
    = new lemur::retrieval::ArrayAccumulator( index_.docCount() );
  lemur::retrieval::SimpleKLRetMethod* retMethod 
    = new lemur::retrieval::SimpleKLRetMethod (index_, smoothSupportFile_, *accumulator );
  
  SimpleKLParameter::DocSmoothParam docParam;  
  if ( smoothingMethod_ == "dirichlet" ) {
    docParam.smthMethod = SimpleKLParameter::DIRICHLETPRIOR;    
    docParam.DirPrior = smoothingParam_;    
  }
  if ( smoothingMethod_ == "jm" ) {
    docParam.smthMethod = SimpleKLParameter::JELINEKMERCER;    
    docParam.JMLambda = smoothingParam_;    
  }  
  docParam.smthStrategy = SimpleKLParameter::INTERPOLATE;
  retMethod->setDocSmoothParam( docParam );

  SimpleKLParameter::QueryModelParam qryParam;  
  qryParam.adjScoreMethod = SimpleKLParameter::QUERYLIKELIHOOD;
  retMethod->setQueryModelParam( qryParam );

  // query as SimpleKLQueryModel
  lemur::retrieval::SimpleKLQueryModel* qModel = queries_.query( index_, qID );
  
  // score documents
  retMethod->scoreCollection( *qModel, pqd_all );
  
  // map log(x) values to exp(log(x)) and normalize
  pqd_all.LogToPosterior();
  
  // sort 
  pqd_all.Sort();  
    
  // clean up
  delete qModel;
  delete retMethod;
  delete accumulator;
}



/* -------------------------------------------------------------------------
 *  alternative implementation for document retrieval from scratch
 *  delivers the very same scores as the implementation based on 
 *  Lemur' SimpleKLRetMethod
 * ------------------------------------------------------------------------- */
/*
void 
ears::Model2::scoreCollection( const int& qID, lemur::api::IndexedRealVector& pqd_all ) 
{
  double lambda = smoothingParam_;
  double beta = smoothingParam_;
  const DISTR_T& q = queries_.query( qID ); // q

  // caching background term probabilities
  LOG( logDEBUG2 ) << "Caching collection probabilities"; 
  DISTR_T pt_cache;
  for ( DISTR_T::const_iterator term_it = q.begin();
       term_it != q.end(); term_it++ ) {
    
    const lemur::api::TERMID_T& termID = term_it->first; // t
    double pt = (double) index_.termCount( termID ) / index_.termCount(); // p(t)
    pt_cache[termID] = pt;
  }
  
  // construct a pool of documents that need to be scored
  // i.e., documents that contain at least one query term
  LOG( logDEBUG2 ) << "Creating a pool of candidate documents"; 
  std::set< lemur::api::DOCID_T > docPool;
  for ( DISTR_T::const_iterator term_it = q.begin();
       term_it != q.end(); term_it++ ) {
    
    const lemur::api::TERMID_T& termID = term_it->first; 
    lemur::api::DocInfoList* dList = index_.docInfoList( termID );
    dList->startIteration();
    while ( dList->hasMore() ) {
      lemur::api::DocInfo* dInfo = dList->nextEntry();                
      docPool.insert( dInfo->docID() );      
    }
    delete dList;
  }
  
  // +++
  // scoring candidate documents
  // +++
  LOG( logDEBUG2 ) << "Scoring candidate documents"; 
  for ( std::set< lemur::api::DOCID_T >::const_iterator doc_it = docPool.begin(); 
        doc_it != docPool.end(); doc_it++ ) {

    lemur::api::DOCID_T docID = *doc_it;
    
    //
    // get query term frequencies in docID
    //
    std::map< int, int > ntd_cache; // n(t|d) for all query terms in docID

    // init
    for ( DISTR_T::const_iterator term_it = q.begin();
         term_it != q.end(); term_it++ ) {
      const lemur::api::TERMID_T& termID = term_it->first; 
      ntd_cache[termID] = 0;
    }

    // get the frequency of term t in docID
    // NOTE: there is no better way of it than iterating through all terms in the doc
    lemur::api::TermInfoList* tInfoList = index_.termInfoList( docID );
    tInfoList->startIteration();
    int queryTermsFound = 0;
    while ( tInfoList->hasMore() && queryTermsFound < q.size() ) {
      lemur::api::TermInfo* tInfo = tInfoList->nextEntry();
      const lemur::api::TERMID_T& termID = tInfo->termID();      
      if ( pt_cache[termID] > 0 ) { // if query term
        ntd_cache[termID] = tInfo->count();
        queryTermsFound++; 
      }
    }      
    delete tInfoList;

    // compute the score for the document
    // log p(q|\theta_d) = \sum_{t \in q} p(t|q) * log( p(t|\theta_d) )
    double pqd = 0.0;
    int nd = index_.docLength( docID );        
    for ( DISTR_T::const_iterator term_it = q.begin();
         term_it != q.end(); term_it++ ) {
      
      const lemur::api::TERMID_T& termID = term_it->first; 
      const double& ptq = term_it->second;
      const double& pt = pt_cache[termID];
      const int& ntd = ntd_cache[termID];
      double ptd; // p(t|\theta_d)
      
      // Jelinek-Mercer smoothing
      // p(t|\theta_d) = (1-\lambda) * p(t|d) + \lambda * p(t)
      if ( smoothingMethod_ == "jm" ) 
        ptd = (1.0 - lambda) * ((double) ntd / nd) + lambda * pt;         

      // Dirichlet smoothing
      // p(t|\theta_d) = (n(t,d) + \beta * p(t)) / (n(d) + \beta)
      if ( smoothingMethod_ == "dirichlet" ) 
        ptd = ((double) ntd + beta * pt) / ((double) nd + beta);
      
      // log p(q|\theta_d) += p(t|q) * log ( p(t|\theta_d) )
      pqd += (double) ptq * log( ptd );
    }
    
    // record value
    pqd_all.PushValue( docID, pqd );    
  } // docPool

  // map log(x) values to exp(log(x)) and normalize
   pqd_all.LogToPosterior();
 
  // sort 
  pqd_all.Sort();  
}
*/
