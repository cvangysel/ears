/*==========================================================================
 * Copyright (c) 2009-2010, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     Stat.cpp
 * \brief    Collecting and calculating statistics
 * \date     2010-01-18
 * \version  1.05
 */

// EARS
#include "Stat.hpp"
#include "Logger.hpp"


///
void 
ears::Stat::incOpCount( const std::string& operation )
{
  this->incOpCount( operation, 1 );
}  



///
void 
ears::Stat::incOpCount( const std::string& operation, const int& count )
{
  std::map< std::string, int >::iterator it = opCount_.find( operation );
  if ( it != opCount_.end() ) 
    opCount_[operation] += count;
  else
    opCount_[operation] = count;
}



///
void 
ears::Stat::outputStat()
{  
  // we get the length of the longest operation name, for fancy displaying
  int maxlength = 0;
  std::map< std::string, int >::iterator it;
  for ( it = opCount_.begin(); it != opCount_.end(); it++ ) 
    if ( ((std::string) it->first).length() > maxlength )
      maxlength = ((std::string) it->first).length();
  
  std::string spaces = "";
  std::string sep = "----------";
  for ( int i = 0; i < maxlength; i++ ) {
    spaces += " ";
    sep += "-";
  }
  
  
  LOG( logDEBUG ) << "Statistics:" << std::endl
    << "    Operation" << spaces.substr( 0, maxlength - 9 ) 
    << "    Count " << std::endl 
    << "    " << sep;

  for ( it = opCount_.begin(); it != opCount_.end(); it++ ) {
    LOG( logDEBUG ) << "  " << it->first 
      << spaces.substr( 0, maxlength - ((std::string) it->first).length() ) 
      << "    " << it->second;
  }
  
  LOG( logDEBUG) << "  " << sep; 
}
