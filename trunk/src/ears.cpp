/*==========================================================================
 * Copyright (c) 2009, Krisztian Balog. All rights reserved.
 *
 * Use of the Entity and Association Retrieval System (EARS) 
 * is subject to the terms of the software license set forth 
 * in the LICENSE file included with this software.
 *==========================================================================
 */

/*!
 * \file     ears.cpp
 * \brief    This file is the entry point for the console application
 * \date     2009-12-04
 * \version  1.0
 */

/*! \mainpage 
 The Entity and Association Retrieval System (EARS) 
 is an open source toolkit for entity-oriented search and discovery in 
 large text collections. Currently, EARS supports two main tasks: finding 
 entities ("Which entities are associated with topic X?") and profiling 
 entities ("What topics is an entity associated with?"). \n
 More: http://code.google.com/p/ears/
 
 <h1>Usage</h1>
 <h2>Syntax</h2>
 The syntax of the ears program is the following: 
 \code ears <command> <paramfile> [debug_level]
 \endcode
  
 Where
   - \c &lt;command&gt; is either \c ef (for entity finding) or \c ep (for entity profiling)
   - \c &lt;paramfile&gt; is a parameter file (see this page for the description of possible parameters)
   - \c debug_level (optionally) is the reporting level 
 
 <h2>Parameters</h2>
 See also: http://code.google.com/p/ears/wiki/Parameters
 
 <h3>Obligatory parameters</h3>
   - \c index Location of the Lemur/Indri index 
   - \c model Association finding model 
     - \c 1 : Model 1
     - \c 2 : Model 2
   - \c associationFile File with document-entity associations 
   - \c queryFile File with queries (topics) 
   - \c outputFile Output file 
 
 <h3>Optional parameters</h3>
   - \c smoothingMethod Smoothing method 
     - \c dirichlet : Dirichlet (default)
     - \c jm : Jelinek-Mercer 
   - \c smoothingParam Smoothing parameter
     (\f$ \beta \f$ for Dirichlet, \f$ \lambda \f$ for Jelinek-Mercer smoothing) 
   - \c associationFileFormat Association file format 
     (Possible values: 0 (default), 1, ..., 7)
   - \c entityAliasMapping File defining a mapping of EntityAlias values to 
     EntityID-s. (Obligatory, when \c associationFileFormat is set to 2, 3, 6, 
     or 7.)
   - \c outputFileFormat Output file format (Possible values: 0 (defult) or 1)
   - \c outputNum Number of results to output (entities for entity finding, 
     topics for entity profiling). Default: all results.
   - \c runID runID for TREC style output files (\c outputFileFormat=0) 
   - \c topDocNum (Model 2 only) Number of top relevant documents considered 
     per query. Default: consider all documents. 
   - \c smoothSupportFile (Model 2 only) Smoothing support file
 
 <h2>Debug levels</h2>
 See also: http://code.google.com/p/ears/wiki/Logging
 
   - \c debugall Intended to turn on all logging
   - \c debug Fine-grained informational events
   - \c info Informational messages (Default)
   - \c warning Only errors and warnings 
   - \c error Intended to turn off logging
 
 */

// EARS
#include "AssociationFramework.hpp"
#include "DataTypes.hpp"
#include "Logger.hpp"

// Lemur
#include "Param.hpp"


using namespace ears;


///
void info() {
  std::cerr 
    << "Entity and Association Retrieval System (EARS) v1.0 (2009-12-04)" << std::endl
    << "Copyright (c) 2009, Krisztian Balog. All rights reserved." << std::endl 
    << std::endl;
}


///
void usage() {
  std::cerr << "USAGE: ears <command> <paramfile> [debug_level]" << std::endl
    << std::endl
    << "  Command:   Description:" << std::endl
    << "  --------   ------------" << std::endl
    << "  ef         entity finding" << std::endl
    << "  ep         entity profiling" << std::endl
    << std::endl
    << "  Debug levels: error, warning, info (default), debug, debugall" 
    << std::endl
    << std::endl;
}



///
int main(int argc, char* argv[]) {

  info();
  
  if (argc < 3) {
    usage();
    return -1;
  }
  
  try {    
    // log level
    ears::EARSLog::reportingLevel() = ears::EARSLog::fromString( argv[3] ? argv[3] : "info" );
    
    if ( lemur::api::ParamPushFile( argv[2] ) ) {      
      ears::AssociationFramework* framework = new ears::AssociationFramework();      
      framework->run( argv[1] );      
      delete framework;
    }
    else {
      LOG( ears::logERROR ) << "invalid parameter file";
    }
  }
  catch ( const std::exception& e ) {
    LOG( ears::logERROR ) << e.what();
  }
      
	return 0;
}
