/*==========================================================================
 * Copyright (c) 2009-2010, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     AssociationModel.hpp
 * \brief    Abstract interface for an association finding model
 * \date     2010-01-18
 * \version  1.05
 */

#ifndef _ASSOCIATIONMODEL_HPP
#define _ASSOCIATIONMODEL_HPP

// EARS
#include "DataTypes.hpp"
#include "EntityRep.hpp"
#include "QueryRep.hpp"
#include "Stat.hpp"

// Lemur
#include "Index.hpp"


namespace ears {
  
  /// Abstract interface for an association finding model
  class AssociationModel
  {
  public:
    /// 
    /*!
     @param index Lemur index
     @param entities Entities
     @param queries Queries
     */
    AssociationModel( const lemur::api::Index& index,
                      const EntityRep& entities,
                      const QueryRep& queries,
                      Stat& stat
                     ) : index_(index), entities_(entities), queries_(queries),
                         stat_(stat), init_(false) {}
    
    /// 
    virtual ~AssociationModel() {}

    /// Initialization. Must be called before scoreAll()
    virtual void init() = 0;
    
    /// Init smoothing settings
    void initSmoothing();
    
    /// Set smoothing parameter automatically
    virtual void setSmoothingAuto() = 0;
    
    /// Run model to determine p(q|e) for all queries and entities
    /*!
      A subclass must override this method to implement scoring.
     */
    virtual ASSOCRES_T scoreAll() = 0;
    
  protected:
    
    /// Lemur index
    const lemur::api::Index& index_;
    
    /// Entity representation
    const EntityRep& entities_;
    
    /// Query representation
    const QueryRep& queries_;
    
    /// Stat collector
    Stat& stat_;
    
    /// Whether initialization has completed
    bool init_;

    /// Smoothing method
    std::string smoothingMethod_;
    
    /// Smoothing parameter
    double smoothingParam_;

  };
}

#endif /* _ASSOCIATIONMODEL_HPP */
