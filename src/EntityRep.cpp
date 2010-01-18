/*==========================================================================
 * Copyright (c) 2009, Krisztian Balog and Wouter Weerkamp. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     EntityRep.cpp
 * \brief    Entity representation
 * \date     2009-12-04
 * \version  1.0
 */

// EARS
#include "EntityRep.hpp"
#include "Logger.hpp"

// Lemur
#include "Param.hpp"

/// String used for unknown entities
#define UNKNOWN_ENTITY "[UNKNOWN_ENTITY]"


//------------------------------------------------------------
// Document-entity associations
//------------------------------------------------------------

///
void
ears::EntityRep::loadAssociations( const lemur::api::Index& index )
{
  //
  // Parameters
  //
  std::string associationFile = lemur::api::ParamGetString( "associationFile" );
  int associationFileFormat = lemur::api::ParamGetInt( "associationFileFormat", 0 );
  std::string entityAliasMapping = lemur::api::ParamGetString( "entityAliasMapping" );

  if ( associationFile.length() == 0 ) {
    LOG( logERROR ) << "Parameter 'associationFile' is not set";
    exit( EXIT_FAILURE );    
  }
  
  if ( associationFileFormat == 2 || associationFileFormat == 3 ) {
    if ( entityAliasMapping.length() == 0 ) {
      LOG( logERROR ) << "Parameter 'entityAliasMapping' is not set";
      exit( EXIT_FAILURE );    
    }    
  }
  
  
  // +++
  // Loading data from file(s)
  // +++
  ifstream inFile;
  EXID_T entityStr;
  EXID_T extEntityID;
  EXID_T entityAlias;
  lemur::api::EXDOCID_T docNO;
  ID_T entityID;
  lemur::api::DOCID_T docID;
  double weight;
  int assocNum = 0;
  int entityNum = 0;
  int docNum = 0;
  lemur::api::IndexedRealVector pde_sum;
  vector< DISTR_T > pde_tmp;

  //
  // Loading EntityAlias-EntityID mapping
  //
  if ( associationFileFormat % 4 >= 2 ) { // format: 2, 3, 6, 7
    LOG( logINFO ) << "Loading EntityAlias-EntityID mapping '" 
    << entityAliasMapping << "'";
    
    inFile.open( entityAliasMapping.c_str() );
    
    if ( inFile.is_open() ) {
      
      inFile >> entityAlias;
      while ( !inFile.eof() ) {
        inFile >> extEntityID;
        
        if ( entityAlias.length() > 0 && extEntityID.length() > 0 ) {
          aliasEntityID_[entityAlias] = extEntityID;
        }
        
        inFile >> entityAlias;
      }
      inFile.close();
      LOG( logDEBUG ) << aliasEntityID_.size() << " Alias-ID pairs loaded";
    }
    else {
      LOG( logERROR ) << "EntityAlias mapping file '" << entityAliasMapping 
      << "' does not exist";
      exit( EXIT_FAILURE );    
    }
  }
  
  //
  // Loading Document-Entity associations
  //
  LOG( logINFO ) << "Loading Entity-Document associations '" 
  << associationFile << "'";
  
  inFile.open( associationFile.c_str() );
  
  if ( inFile.is_open() ) {
    
    if ( associationFileFormat % 2 == 0 ) // format: 0, 2, 4, 6
      inFile >> docNO;
    else // format: 1, 3, 5, 7 
      inFile >> entityStr;
    
    while ( !inFile.eof() ) {

      if ( associationFileFormat % 2 == 0 ) // format: 0, 2, 4, 6
        inFile >> entityStr;
      else // 1, 3, 5, 7 
        inFile >> docNO;

      if ( associationFileFormat >= 4 ) // format: 4, 5, 6, 7
        inFile >> weight;
      else
        weight = 1.0;
                                
      docID = index.document( docNO );
      if ( docID > 0 && weight > 0.0 ) {
        if ( associationFileFormat % 4 >= 2 ) { // format: 2, 3, 6, 7
          extEntityID = this->entityIDfromAlias( entityStr );          
          LOG( logDEBUG2 ) << "ENTITY ALIAS '" << entityStr << "' => ENTITY '"
          << extEntityID << "'";
        }        
        else { // format: 0, 1, 4, 5
          extEntityID = entityStr;          
        } 
        
        // get internal entityID
        entityID = this->entityID( extEntityID );
        // new (unseen) entityID
        if ( entityID < 0 ) {          
          entityID = entityNum;
          intEntityID_[extEntityID] = entityID;
          extEntityID_.push_back( extEntityID );
          entityNum++;
        }
        
        //
        // store document-entity association
        //
        bool wasAssoc = false; // docID-entityID association already existed
        if( pde_sum.size() <= entityID )
          pde_sum.resize( entityID + 1 );

        if ( assocIndex_ == aiENTITY ) {
          if ( pde_tmp.size() <= entityID )
            pde_tmp.resize( entityID + 1 );
          if ( pde_tmp[entityID][docID] > 0 ) {
            wasAssoc = true;
            LOG( logWARNING ) << "ENTITY '" << extEntityID 
              << "' and DOC '" << docNO 
              << "' already have an association; using first occurrence.";
          } else {
            pde_tmp[entityID][docID] = weight;
            pde_sum.IncreaseValueFor( entityID, weight );
          }
        }
        if ( assocIndex_ == aiDOCUMENT ) {
          if ( pde_tmp.size() <= docID ) {
            pde_tmp.resize( docID + 1 );
            docNum = docID + 1;
          }
          if ( pde_tmp[docID][entityID] > 0 ) {
            wasAssoc = true;
            LOG( logWARNING ) << "ENTITY '" << extEntityID 
              << "' and DOC '" << docNO 
              << "' already have an association; using first occurrence.";
          } else {
            pde_tmp[docID][entityID] = weight;
            pde_sum.IncreaseValueFor( entityID, weight );
          }
        }

        if ( !wasAssoc ) {
          assocNum++;
          
          // number of documents associated with the entity
          if ( nd_.size() <= entityID )
            nd_.resize( entityID + 1, 0 );
          nd_[entityID]++;
          
          // number of entities associated with the document
          if ( ne_.size() <= docID )
            ne_.resize( docID + 1, 0 );
          ne_[docID]++;
        } // !wasAssoc
                
        LOG( logDEBUG2 ) << "ENTITY: '" << extEntityID << "' (ID " << entityID
          << ")   DOC: '" << docNO << "' (ID " << docID << ")   WEIGHT: "
          << weight;      
      } // docID > 0 && weight > 0.0
      else {
        if ( !(docID > 0) )
          LOG( logWARNING ) << "Document '" << docNO << "' does not exist "
            << "in index (association ignored)";
        if ( !(weight > 0.0) )
          LOG( logWARNING ) << "Association (" << docNO << "-" << entityStr 
            << ") with 0.0 weight (ignored)";
      }
      
      if ( associationFileFormat % 2 == 0 ) // format: 0, 2, 4, 6
        inFile >> docNO;
      else // format: 1, 3, 5, 7 
        inFile >> entityStr;
    
    } // !inFile.eof()
    inFile.close();    

    LOG( logDEBUG ) << entityNum << " unique entities";
    LOG( logDEBUG ) << assocNum << " document-entity associations";
    
    //
    // normalize associations
    // p(d|e) = assoc(d|e) / \sum_d'[ assoc(d'|e) ]
    // 
    LOG( logINFO ) << "Normalizing Entity-Document associations";
    int nAssocNum = 0;
    
    // document-oriented storage
    if ( assocIndex_ == aiDOCUMENT ) {
      pde_.resize( docNum );

      for ( ID_T nDocID = 1; nDocID < pde_tmp.size(); nDocID++ ) {
        DISTR_T npde = pde_tmp[nDocID];
        if( npde.size() > 0 ) {
          for ( DISTR_T::const_iterator nit = npde.begin(); nit != npde.end(); nit++ ) {
            int nEntityID = nit->first;
            double nWeight = ((double) nit->second) 
                             / ((double) pde_sum.FindByIndex(nEntityID)->val);
            pde_[nDocID][nEntityID] = nWeight;
            nAssocNum++;
          }
          pde_tmp[nDocID].clear();
        }
      }
    }
        
    // entity-oriented storage
    if ( assocIndex_ == aiENTITY ) {
      pde_.resize( entityNum );

      for ( ID_T nEntityID; nEntityID < pde_tmp.size(); nEntityID++ ) {
        DISTR_T npde = pde_tmp[nEntityID];
        for ( DISTR_T::const_iterator nit = npde.begin(); nit != npde.end(); nit++ ) {
          int nDocID = nit->first;
          double nWeight = ((double) nit->second)
                           / ((double) pde_sum.FindByIndex(nEntityID)->val);
          pde_[nEntityID][nDocID] = nWeight;
          nAssocNum++;
        }
        pde_tmp[nEntityID].clear();
      }
    }

    pde_tmp.clear();
    pde_sum.clear();
    
    LOG( logDEBUG ) << nAssocNum << " associations after normalization";
    
  } // inFile.open()
  else {
    LOG( logERROR ) << "Association file '" << associationFile << "' does not exist";
    exit( EXIT_FAILURE );    
  }
  
}



///
const ears::DISTR_T& 
ears::EntityRep::entityDocs( const ID_T& entityID ) const
{
  // entity-oriented storage
  if ( assocIndex_ == aiENTITY ) {  
    if ( entityID >= 0 && entityID < pde_.size() ) 
      return pde_[entityID];
    else {
      LOG( logERROR ) << "Reference to non-existing entity";
      exit( EXIT_FAILURE );        
    }  
  }

  // document-oriented storage
  if ( assocIndex_ == aiDOCUMENT ) {
    LOG( logERROR ) << "This operation is not supported "
      << "in document-oriented storage mode";
    exit( EXIT_FAILURE );        

    /*
    // TODO 
    // associations could be returned by iterating over documents
    // and assessing for each whether the entity is associated with it
     
    DISTR_T pde_temp;
    DISTR_T::iterator pde_it;
     
    for ( ID_T docID = 0; docID < pde_.size(); docID++ ) {
      pde_it = pde_[docID].find( entityID );
      if ( pde_it != pde_[docID].end() )               
        pde_temp[docID] = pde_it->second;
    }

    return pde_temp;
    */    
  }
}



///
const ears::DISTR_T& 
ears::EntityRep::docEntities( const ID_T& docID ) const
{
  // document-oriented storage
  if ( assocIndex_ == aiDOCUMENT ) {  
    if ( docID > 0 && docID < pde_.size() ) 
      return pde_[docID];
    else {
      // document might exist but without associatons;
      // return docID=0, i.e., no associations
      return pde_[0];
    }  
  }
  else {
    LOG( logERROR ) << "This operation is not supported "
      << "in entity-oriented storage mode";
    exit( EXIT_FAILURE );        

    /*
    // TODO 
    // associations could be returned by iterating over entities
    // and assessing for each whether the document is associated with it
     
    DISTR_T pde_temp;
    DISTR_T::iterator pde_it;
     
    for ( ID_T entityID = 0; entityID < pde_.size(); entityID++ ) {
      pde_it = pde_[entityID].find( docID );
      if ( pde_it != pde_[entityID].end() )               
        pde_temp[entityID] = pde_it->second;
    }
     
    return pde_temp;
    */        
  }  
}



//------------------------------------------------------------
// Summary counts
//------------------------------------------------------------

///
const int 
ears::EntityRep::entityNum() const
{
  return nd_.size();
}



///
const int 
ears::EntityRep::entityDocNum( const ID_T& entityID ) const
{
  if ( entityID >= 0 && entityID < nd_.size() ) 
    return nd_[entityID];
  else {
    return 0;
  }
}



///
const int 
ears::EntityRep::docEntityNum( const lemur::api::DOCID_T& docID ) const
{
  if ( docID >= 0 && docID < ne_.size() ) 
    return ne_[docID];
  else {
    return 0;
  }
}


//------------------------------------------------------------
// Conversion between entity IDs
//------------------------------------------------------------

///
const ears::ID_T 
ears::EntityRep::entityID( const EXID_T& extEntityID ) const
{
  std::map< EXID_T, ID_T >::const_iterator it = intEntityID_.find( extEntityID );
  if ( it != intEntityID_.end() ) 
    return it->second;
  else 
    return -1;
}



///
const ears::EXID_T 
ears::EntityRep::entityID( const ID_T& intEntityID ) const
{
  if ( intEntityID < extEntityID_.size() ) 
    return extEntityID_[intEntityID];
  else 
    return UNKNOWN_ENTITY;
}



///
const ears::EXID_T  
ears::EntityRep::entityIDfromAlias( const EXID_T& entityAlias ) const
{
  std::map< EXID_T, EXID_T >::const_iterator it = aliasEntityID_.find( entityAlias );
  
  if ( it != aliasEntityID_.end() ) 
    return it->second;
  else 
    return UNKNOWN_ENTITY;
}

