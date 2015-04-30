# Statistics #

To be able to measure not only effectiveness, but also efficiency of the implemented models, EARS has been equipped with the ability to count how many times certain operations were called/issued during the computation process.

To display statistics, you need to run EARS with [debug level](Logging.md) `debug` or `debugall`.

**Important note** Statistics are not collected during smoothing parameter estimation, only during the "core" processing steps.

The following operations are recorded:

| **Operation** | **Description** | **Used in** |
|:--------------|:----------------|:------------|
| P(t) | Get collection probability for term t | M1,M2 |
| n(t,d)`*` | Get the frequency of term t from document d | M1,M2 |
| n(t,e) | Get the frequency of term t from the language model of entity e | M1 |
| entityDocNum | Get the number of documents associated with the entity |  M1 |
| entityDocs | Get the list of documents associated with the entity | M1 |
| docEntities | Get the list of entities associated with the document | M2 |
| termInfoList | Get the term list for a document | M1 |
| docInfoList | Get the document list for a term | M2 |


Operations marked with `*` are not computed by default as they slow down computation. If you want these computed change the line `#define DETAILED_STAT 0` to `#define DETAILED_STAT 1` in `includes/Stat.hpp` (and don't forget to recompile)!