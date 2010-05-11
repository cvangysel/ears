/*==========================================================================
 * Copyright (c) 2009-2010, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     Stat.hpp
 * \brief    Collecting and calculating statistics
 * \date     2010-01-18
 * \version  1.05
 */

#ifndef _STAT_HPP
#define _STAT_HPP

// set 1 for detailed statistics
// note that it slows down computation
#define DETAILED_STAT 1

// EARS
#include "DataTypes.hpp"


namespace ears {
  
  /// Collecting and calculating statistics
  class Stat
    {
    public:
      /// 
      Stat() {}
      
      /// 
      ~Stat() {}
      
      /// Incrase call count for a selected operation by one
      /*!
       @param operation Name or ID of the operation
       */
      void incOpCount( const std::string& operation );
      
      /// Incrase call count for a selected operation by an arbitrary number
      /*!
       @param operation Name or ID of the operation
       @param count How many times it has been called
       */
      void incOpCount( const std::string& operation, const int& count );
      
      /// Output statistics
      void outputStat();
      
    private:

      /// Counter for keeping the number of times a certain operation was called
      std::map< std::string, int > opCount_;            
    };
}

#endif /* _STAT_HPP */
