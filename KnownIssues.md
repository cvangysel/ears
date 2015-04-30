# Known issues #

  * Model 1 using Indri indexes might leak memory
    * This is a [bug in the Lemur toolkit](http://sourceforge.net/tracker/?func=detail&aid=2879981&group_id=161383&atid=819615) that has been fixed after Lemur 4.10/Indri 2.10; [download LemurIndriIndex.cpp revision 1.19 or above](http://lemur.cvs.sourceforge.net/viewvc/lemur/lemur/index/src/LemurIndriIndex.cpp)
  * EARS example files cannot be read on Windows
    * Download the newest Lemur MAIN revision from [Lemur CVS](http://sourceforge.net/scm/?type=cvs&group_id=161383) (for _modulename_ use "lemur")