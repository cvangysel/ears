/*==========================================================================
 * Copyright (c) 2009, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     DataTypes.hpp
 * \brief    EARS data types
 * \date     2009-09-09
 * \version  0.9
 */

#ifndef _DATATYPES_HPP
#define _DATATYPES_HPP

// stl
#include <map>
#include <string>
#include <utility>
#include <vector>


namespace ears
{
  /// Internal ID encoding
  typedef int ID_T;
  
  /// External ID encoding
  typedef std::string EXID_T;
  
  /// Probability distribution
  typedef std::map< ID_T, double > DISTR_T;
  
  /// Association results
  typedef std::vector< std::vector< double > > ASSOCRES_T;  
}

#endif /* _DATATYPES_HPP */
