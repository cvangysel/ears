/*==========================================================================
 * Copyright (c) 2009, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     QueryRep.hpp
 * \brief    Query representation
 * \date     2009-09-09
 * \version  0.9
 */

#ifndef _QUERYREP_HPP
#define _QUERYREP_HPP

// EARS
#include "DataTypes.hpp"

// Lemur
#include "Index.hpp"
#include "SimpleKLRetMethod.hpp"


namespace ears {
  
  /// Query representation
  class QueryRep
    {
    public:
      /// 
      QueryRep() {};
      
      /// 
      ~QueryRep() {};
      
      /// Load queries
      /*!
       * @param index Lemur index
       */
      void loadQueries( const lemur::api::Index& index );
      
      /// Number of queries
      const int queryNum() const; 
      
      /// External query ID
      /*!
       * @param qID Query ID (internal)
       */
      const EXID_T queryID( const int& qID ) const;
      
      /// Query as a term distribution (EARS representation)
      /*!
       * @param qID Query ID (internal)
       */
      const DISTR_T query( const int& qID ) const;
      
      /// Query as a SimpleKLQueryModel 
      /// (caller is responsible for the deletion of the generated new instance) 
      /*!
       * @param index Lemur index
       * @param qID Query ID (internal)
       */
      lemur::retrieval::SimpleKLQueryModel* query( 
                                              const lemur::api::Index& index, 
                                              const int& qID ) const;
      
    private:
      
      /// Queries
      /// (i.e., ptq_[q] holds p(t|q) for query q)
      std::vector< DISTR_T > ptq_;
      
      /// External query IDs
      std::vector< EXID_T > queryID_;
      
    };
}

#endif /* _QUERYREP_HPP */
