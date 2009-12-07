/*==========================================================================
 * Copyright (c) 2009, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     EntityRep.hpp
 * \brief    Entity representation
 * \date     2009-09-09
 * \version  0.9
 */

#ifndef _ENTITYREP_HPP
#define _ENTITYREP_HPP

// EARS
#include "DataTypes.hpp"

// Lemur
#include "Index.hpp"


namespace ears {
  
  /// Entity representation
  class EntityRep
  {
  public:    
    /// Types of storing document-entity associations
    /*!
     - aiENTITY : entity oriented
     <br>Complexity of entityDocs( entityID ) : constant
     <br>Complexity of docEntities( docID )   : linear in entityNum
     - aiDOCUMENT : document oriented
     <br>Complexity of docEntities( docID )   : constant
     <br>Complexity of entityDocs( entityID ) : linear in docNum
     */
    enum TAssocIndexing {aiENTITY, aiDOCUMENT};

    /// 
    /*!
     @param assocIndex Type of storing document-entity associations
     */
    EntityRep( const TAssocIndexing& assocIndex ) : assocIndex_( assocIndex ) {};
    
    /// 
    ~EntityRep() {};
    
    /// @name Document-entity associations
    //@{    
    /// Load entities and document-entity associations
    /*!
     * @param index Index
     */
    void loadAssociations( const lemur::api::Index& index );

    /// Documents associated with the entity
    /*!
     * @param entityID Entity ID (internal)
     */
    const DISTR_T& entityDocs( const ID_T& entityID ) const;
    
    /// Entities associated with the document
    /*!
     * @param docID Document ID (internal)
     */
    const DISTR_T& docEntities( const ID_T& docID ) const;
    //@}

    /// @name Summary counts
    //@{    
    /// Number of entities
    const int entityNum() const;

    /// Number of documents associated with the entity
    /*!
     * @param entityID Entity ID (internal)
     */
    const int entityDocNum( const ID_T& entityID ) const;
    
    /// Number of entities associated with the document 
    /*!
     * @param docID Document ID (internal)
     */
    const int docEntityNum( const lemur::api::DOCID_T& docID ) const;
    //@}
            
    /// @name Conversion between entity IDs
    //@{    
    /// Convert external entityID to internal entityID
    /*!
     * @param extEntityID Entity ID (external)
     */
    const ID_T entityID( const EXID_T& extEntityID ) const;
    
    /// Convert internal entityID to external entityID
    /*!
     * @param intEntityID Entity ID (internal)
     */
    const EXID_T entityID( const ID_T& intEntityID ) const;
    
    /// Get external entityID from entityAlias
    /*!
     * @param entityAlias Entity alias
     */
    const EXID_T entityIDfromAlias( const EXID_T& entityAlias ) const;    
    //@}
    
  private:
    
    /// Mapping from external to internal entity IDs
    std::map< EXID_T, ID_T > intEntityID_;
    
    /// Mapping from internal to external entity IDs
    std::vector< EXID_T > extEntityID_;
    
    /// Mapping from entity alias to external entity ID
    std::map< EXID_T, EXID_T > aliasEntityID_;

    /// Indexing of document-entity associations for internal storage
    const TAssocIndexing assocIndex_;
    
    /// Document-entity associations
    /*!
     Depending on how we need to store associations,
     pde_ is indexed either by documents or by entities:
     - assocIndex_ == aiEntity   : pde_[e] holds p(d|e) for entity e
     - assocIndex_ == aiDOCUMENT : pde_[d] holds p(d|e) for entity d
     */
    std::vector< DISTR_T > pde_;
    
    /// Number of documents associated with an entity
    std::vector< int > nd_;
    
    /// Number of entities associated with a document
    std::vector< int > ne_;
    
  };    
}

#endif /* _ENTITYREP_HPP */
