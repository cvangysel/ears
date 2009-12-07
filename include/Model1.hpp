/*==========================================================================
 * Copyright (c) 2009, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     Model1.hpp
 * \brief    The "Model 1" association finding model
 * \date     2009-09-09
 * \version  0.9
 */

#ifndef _MODEL1_HPP
#define _MODEL1_HPP

// EARS
#include "AssociationModel.hpp"

// Lemur
#include "DocUnigramCounter.hpp"


namespace ears {
  
  /// The "Model 1" association finding model
  class Model1 : public AssociationModel 
  {
  public:
    ///
    Model1( const lemur::api::Index& index,
            const EntityRep& entities,
            const QueryRep& queries
          ) : AssociationModel( index, entities, queries ) {}
    
    /// 
    ~Model1() {}

    ///
    void init();
    
    /// Set smoothing parameter based on average candidate representation length.
    /*! 
     * \f$ \beta = \frac{\sum_e n(e)}{|e|} \f$,
     * where \f$ n(e) \f$ is the number of terms associated with 
     * entity \f$ e \f$, and \f$ |e| \f$ is the total number of entities.
     */
    void setSmoothingAuto();

    /// Implements Model 1 of Balog et. al.
    /*!
     * Calculates \f$ p(q|\theta_e) \f$ for each query \f$ q \f$ and 
     * entity \f$ e \f$ as follows:
     * <ul> 
     * <li>For each entity \f$ e \f$ 
     *   <ol>
     *    <li>Construct the entity's language model \f$ p(t|\theta_e) \f$</li>
     *    <li>Score each query \f$ q \f$ against the entity model:
     *      \f$ \log p(q|\theta_e) 
     *          = \sum_{t \in q} p(t|q) \cdot \log p(t|\theta_e) \f$,
     *      where \f$ p(t|q) \f$ is the weight of term \f$ t \f$ in query.</li>
     *   </ol>
     * </li>
     * </ul>
     * 
     * The entity language model is estimated as follows:
     * <ul>
     * <li>using Jelinek-Mercer smoothing: 
     *   \f$ p(t|\theta_e) 
     *       = (1-\lambda) \cdot (n(t,e)/n(e)) + \lambda \cdot p(t) \f$ </li>
     * <li>using Dirichlet smoothing: 
     *   \f$ p(t|\theta_e) 
     *       = (n(t,e) + \beta \cdot p(t)) / (n(e) + \beta) \f$ </li>
     * </ul>
     *
     * Here, \f$ n(t,e) \f$ is the number of times term \f$ t \f$ occurs
     * in the entity's textual representation and \f$ n(e) = \sum_t n(t,e) \f$
     * (i.e., the total number of terms associated with the entity).
     * The collection language model is denoted by \f$ p(t) \f$.
     * <br>
     * The raw term counts in the entity's representation are calculated using 
     * the following equation: \f$ n(t,e) = \sum_{D_e} n(t,d) \cdot p(d|e) \f$, 
     * where \f$ {D_e} \f$ is the set of documents associated with the entity
     * and \f$ p(d|e) \f$ expresses the strength of the association between
     * document \f$ d \f$ and entity \f$ e \f$.
     */
    ASSOCRES_T scoreAll();  
        
  private:
        
    /// Term counter for an entity 
    /// (caller is responsible for the deletion of the generated new instance) 
    /*!
     * @param entityID Entity ID
     */
    lemur::langmod::DocUnigramCounter* entityCounter( const ID_T& entityID );
    
  };
}

#endif /* _MODEL1_HPP */
