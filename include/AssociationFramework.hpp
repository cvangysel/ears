/*==========================================================================
 * Copyright (c) 2009, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     AssociationFramework.hpp
 * \brief    Association finding framework
 * \date     2009-09-09
 * \version  0.9
 */

#ifndef _ASSOCIATIONFRAMEWORK_HPP
#define _ASSOCIATIONFRAMEWORK_HPP

// EARS
#include "DataTypes.hpp"
#include "EntityRep.hpp"
#include "QueryRep.hpp"

// Lemur
#include "Index.hpp"


namespace ears {

  /// Association finding framework
  class AssociationFramework
  {
  public:
    /// 
    AssociationFramework() {};
    
    /// 
    ~AssociationFramework() {};
    
    /// Perform a selected task
    /*!
     @param task name of the task. Possible values: ef, ep
     */
    void run( std::string task );
  
  private:
        
    /// Lemur index
    const lemur::api::Index* index_;
        
    /// Entity representation
    EntityRep* entities_;
    
    /// Query representation
    QueryRep* queries_;
            
  };
}

#endif /* _ASSOCIATIONFRAMEWORK_HPP */
