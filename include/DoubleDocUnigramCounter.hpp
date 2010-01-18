/*==========================================================================
 * Copyright (c) 2009, Wouter Weerkamp and Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     DoubleDocUnigramCounter.hpp
 * \brief    DocUnigramCounter able to deal with doubles
 * \date     2009-12-04
 * \version  1.0
 */


#ifndef _DOUBLEDOCUNIGRAMCOUNTER_HPP
#define _DOUBLEDOCUNIGRAMCOUNTER_HPP

// Lemur
#include "common_headers.hpp"
#include "WeightedIDSet.hpp"
#include "Counter.hpp"
#include "Index.hpp"

namespace ears {

  /// DocUnigramCounter for doubles (to use with WeightedIDSets).
  /// Similar to RelDocUnigram, but without length normalization.
  class DoubleDocUnigramCounter : public lemur::utility::ArrayCounter <double>
  {
  public:
    ///
    DoubleDocUnigramCounter( const lemur::utility::WeightedIDSet &docSet, 
                             const lemur::api::Index &homeIndex );

    ///
    virtual ~DoubleDocUnigramCounter() {}
    
    ///
    const std::string lexiconID() const {
      return ( ind.termLexiconID() );
    }

  protected:
    ///
    void countDocUnigram( lemur::api::DOCID_T docID, double weight = 1 );
    
    ///
    const lemur::api::Index &ind;
  };
}

#endif /* _DOUBLEDOCUNIGRAMCOUNTER_HPP */
