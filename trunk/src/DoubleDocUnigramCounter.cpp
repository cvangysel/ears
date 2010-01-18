/*==========================================================================
 * Copyright (c) 2009, Wouter Weerkamp and Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     DoubleDocUnigramCounter.cpp
 * \brief    DocUnigramCounter able to deal with doubles
 * \date     2009-12-04
 * \version  1.0
 */

// EARS
#include "DoubleDocUnigramCounter.hpp"


///
ears::DoubleDocUnigramCounter::DoubleDocUnigramCounter(
          const lemur::utility::WeightedIDSet &docSet, 
          const lemur::api::Index &homeIndex )
        : ind(homeIndex), 
          lemur::utility::ArrayCounter<double>(homeIndex.termCountUnique()+1 )
{
  docSet.startIteration();
  while ( docSet.hasMore() ) {
    int docID;
    double wt;
    docSet.nextIDInfo( docID, wt );
    countDocUnigram( docID, wt );
  }
}



///
void 
ears::DoubleDocUnigramCounter::countDocUnigram( lemur::api::DOCID_T docID, 
                                                double weight )
{
  lemur::api::TermInfoList *tList = ind.termInfoList( docID );
  const lemur::api::TermInfo *info;
  tList->startIteration();
  while ( tList->hasMore() ) {
    info = tList->nextEntry();
    incCount( info->termID(), weight*info->count() );
  }
  delete tList;
}
