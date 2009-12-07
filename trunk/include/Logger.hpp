/*==========================================================================
 * Copyright (c) 2009, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     Logger.hpp
 * \brief    Event logger
 * \date     2009-09-09
 * \version  0.9
 */

#ifndef _LOGGER_HPP
#define _LOGGER_HPP

#include <sstream>

namespace ears {

  /// logging levels
  enum TLogLevel {logERROR, logWARNING, logINFO, logDEBUG, logDEBUG2};
  
  /// Event logger
  class Logger
  {
  public:
    ///
    Logger();
    
    ///
    virtual ~Logger();
    
    ///
    std::ostringstream& log( TLogLevel level = logINFO );
    
    ///
    static TLogLevel& reportingLevel();
    
    ///
    static TLogLevel fromString( const std::string& level );
    
  protected:
    ///
    std::ostringstream os;
    
  private:
    ///
    Logger( const Logger& );
    
    ///
    Logger& operator = ( const Logger& );
  };

  
  inline Logger::Logger()
  {    
  }
    
  
  inline std::ostringstream& Logger::log( TLogLevel level )
  {
    if ( level == logERROR )
      os << "ERROR: ";
    if ( level == logWARNING )
      os << "WARNING: ";
    if ( level == logDEBUG )
      os << "  ";
    if ( level == logDEBUG2 )
      os << "    ";
    return os;
  }
    
  
  inline Logger::~Logger()
  {
    os << std::endl;
    fprintf( stderr, "%s", os.str().c_str() );
    fflush( stderr );
  }
    
  
  inline TLogLevel& Logger::reportingLevel()
  {
    static TLogLevel rlevel = logINFO;
    return rlevel;
  }

    
  inline TLogLevel Logger::fromString( const std::string& level )
  {
    if (level == "debugall")
      return logDEBUG2;
    if (level == "debug")
      return logDEBUG;
    if (level == "info")
      return logINFO;
    if (level == "warning")
      return logWARNING;
    if (level == "error")
      return logERROR;
    Logger().log(logWARNING) << "Unknown logging level '" << level << "'. "
      << "Using INFO level as default.";
    return logINFO;
  }
    
  
  typedef Logger EARSLog;
  
  #define LOG(level) \
    if (level > EARSLog::reportingLevel()) ; \
    else Logger().log(level)
  
}

#endif
