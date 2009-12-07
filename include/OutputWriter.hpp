/*==========================================================================
 * Copyright (c) 2009, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     OutputWriter.hpp
 * \brief    Writing results to output
 * \date     2009-09-09
 * \version  0.9
 */

#ifndef _OUTPUTWRITER_HPP
#define _OUTPUTWRITER_HPP

// EARS
#include "DataTypes.hpp"
#include "EntityRep.hpp"
#include "QueryRep.hpp"

// Lemur
#include "IndexedReal.hpp"


namespace ears {
  
  /// Writing results to output
  class OutputWriter
    {
    public:
      /// 
      /*!
       @param entities Entities 
       @param queries Queries
       */
      OutputWriter( const EntityRep& entities, const QueryRep& queries )
                  : entities_(entities), queries_(queries) {}
      
      /// 
      ~OutputWriter() {}
      
      /// Output topic-entity associations
      /*!
       @param task Name of the task; possible values: ef, ep
       @param pqe Topic-entity associations (pqe[e][q] = p(q|e))
       */
      void outputAssocRes( const std::string& task, 
                           const ASSOCRES_T& pqe );
      
    private:

      /// Write p(q|e) and (internal) entityID, for a given query 
      /// to result vector. Results are sorted by p(q|e) desc.
      /*!
       @param pqe Topic-entity associations
       @param qID Query ID 
       @param res Result vector (to be filled with values)
       */
      void resultsFinding( const ASSOCRES_T& pqe, 
                           const ID_T qID,
                           lemur::api::IndexedRealVector& res);

      /// Write p(q|e) and (internal) queryID, for a given entity 
      /// to result vector. Results are sorted by p(q|e) desc.
      /*!
       @param pqe Topic-entity associations
       @param eID Entity ID 
       @param res Result vector (to be filled with values)
       */
      void resultsProfiling( const ASSOCRES_T& pqe, 
                             const ID_T eID,
                             lemur::api::IndexedRealVector& res);
      
      /// Write a result record into file
      /*!
       @param outFile Output stream
       @param task Task ("ef" or "ep")
       @param outFileFormat Output file format (0 or 1)
       @param ID QueryID for entity finding, entityID for entity profiling
       @param res Result vector 
       @param runID RunID (used only for output format 0)
       */
      void writeRecord( fstream& outFile, 
                        const std::string& task,
                        const int& outFileFormat,
                        const std::string& ID, 
                        const lemur::api::IndexedRealVector& res,
                        const std::string& runID );
      
      /// Entity representation
      const EntityRep& entities_;
      
      /// Query representation
      const QueryRep& queries_;
      
    };
}

#endif /* _OUTPUTWRITER_HPP */
