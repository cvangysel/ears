/*==========================================================================
 * Copyright (c) 2009-2010, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     Model2.hpp
 * \brief    The "Model 2" association finding model
 * \date     2010-01-18
 * \version  1.05
 */

#ifndef _MODEL2_HPP
#define _MODEL2_HPP

// EARS
#include "AssociationModel.hpp"

// Lemur
#include "IndexedReal.hpp"


namespace ears {
  
  /// The "Model 2" association finding model
  class Model2 : public AssociationModel 
  {
  public:
    ///
    Model2( const lemur::api::Index& index,
            const EntityRep& entities,
            const QueryRep& queries,
            Stat& stat
          ) : AssociationModel( index, entities, queries, stat ) {}
    
    /// 
    ~Model2() {}

    ///
    void init();
    
    /// Set smoothing parameter based on average document length.
    /*! 
     * \f$ \beta = \frac{\sum_d n(d)}{|d|} \f$,
     * where \f$ n(d) \f$ is the length of document \f$ d \f$,
     * and \f$ |d| \f$ is the total number of documents.
     */
    void setSmoothingAuto();
    
    /// Implements Model 2 of Balog. et. al.
    /*!
     * Calculates \f$ p(q|e) \f$ for each query \f$ q \f$ and 
     * entity \f$ e \f$ as follows:
     * <ul> 
     * <li>For each query \f$ q \f$ 
     *   <ol>
     *    <li>Init \f$ p(q|e) \f$ with value 0 for all entities.
     *    <li>Determine a set of relevant documents \f$ D_q \f$ 
     *     by scoring documents using a standard language modeling approach.
     *     That is, calculate the query likelihood \f$ p(q|d) \f$
     *     (using either Jelinek-Mercer or Dirichlet smoothing).
     *    <li>Optionally, limit the number of documents taken into account
     *     (i.e., limit the size of \f$ D_q \f$).
     *    <li>For each of the documents \f$ d \in D_q \f$:
     *      <ul>
     *      <li>For each entity \f$ e \f$ associated with document \f$ d \f$
     *         (that is, if \f$ p(d|e) > 0 \f$): 
     *         \f$ p(q|e) += p(q|d) \cdot p(d|e) \f$ 
     *        </li>
     *      </ul>
     *   </ol>
     * </li>
     * </ul>
     */
    ASSOCRES_T scoreAll();  
        
  private:
    /// Scoring all documents in the collection for a given query
    /*!
     * @param qID Query ID
     * @param pqd_all Stores p(q|d), sorted by value desc
     */
    void scoreCollection( const int& qID, lemur::api::IndexedRealVector& pqd_all );
    
    /// Smooth support file
    std::string smoothSupportFile_;

    /// Number of top documents considered per query
    int topDocNum_;
        
  };
}

#endif /* _MODEL2_HPP */
