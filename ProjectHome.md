| ![http://staff.science.uva.nl/~kbalog/ears/EARS_logo_medium.jpg](http://staff.science.uva.nl/~kbalog/ears/EARS_logo_medium.jpg) |
|:--------------------------------------------------------------------------------------------------------------------------------|

EARS is an open source toolkit for entity-oriented search and discovery in large text collections. Currently, EARS supports two main tasks: finding entities (_"Which entities are associated with topic X?"_) and profiling entities (_"What topics is an entity associated with?"_).

## Background ##
The association finding framework and models implemented in EARS were originally developed for expertise retrieval in an organizational setting, over a period of more than three years. These models are able to incorporate and exploit special characteristics and features of test collections and/or the organizational settings that they represent, and are considered the current state-of-the-art in expertise retrieval. The baseline models are also robust and generic, and can be applied to finding associations between topics and entities other than people. <br />
[Read further here about academic research supported by EARS](Papers.md).

## Features ##
  * Free and open-source software, even for commercial use
  * Cross-platform, fast and modular code written in C++
  * Built on top of the [Lemur toolkit](http://www.lemurproject.org/) for Language Modeling and Information Retrieval (open-source)

## Getting started ##
Check out the [Crash course on installing and using EARS](HowTo.md) page to get right started.

## More information ##
  * To learn more about the association finding framework and about the models implemented in EARS, [click here](AssociationFinding.md).
  * Information about features coming in future releases can be found on [this page](FutureReleases.md).
  * Some general conventions followed during the development [are here](Conventions.md).
  * EARS has extensive doxygen documentation; check the `doc` directory.
  * [List of known issues](KnownIssues.md)

## News ##
  * **11 May, 2010** EARS 1.05 released (see [Changelog](Changelog.md))
  * **18 Jan, 2010** The latest source code is [available on svn](http://code.google.com/p/ears/source/checkout)
  * **11 Dec, 2009** EARS 1.0 released (see [Changelog](Changelog.md))
  * **14 Sept, 2009** EARS 0.9 released

## Credits and Acknowledgments ##
The EARS toolkit was created by [Dr. Krisztian Balog](http://staff.science.uva.nl/~kbalog/).

The author wishes to express his gratitude to [Prof.dr. Maarten de Rijke](http://staff.science.uva.nl/~mdr) and [Dr. Leif Azzopardi](http://www.dcs.gla.ac.uk/~leif/) for the years of research work that led to the development of the models implemented in this software.
Special thanks go to [Wouter Weerkamp](http://wouter.weerkamp.com/) for his contributions to the EARS implementation and example data set, and for all the joint work that involved EARS.
The author also thanks to [Katja Hoffmann](http://katja-hofmann.de/) for feedback and suggestions, and to [Allard Dijk](http://www.linkedin.com/in/allarddijk) for creating a project file and a how-to for using EARS under Windows with Visual Studio.