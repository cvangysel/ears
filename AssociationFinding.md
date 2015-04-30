# Tasks #
EARS focuses on two entity-related information access tasks:
  * **finding entities** ("Which entities are associated with topic X?"), and
  * **profiling entities** ("What topics is an entity associated with?").


# Models #

EARS implements a generative probabilistic modeling framework for capturing associations between entities and topics. We employ two main families of models, both based on generative language modeling techniques, for calculating the probability of a query topic (q) being associated with an entity (e), P(q|e). According to one family of models (Model 1) we build a textual representation (i.e., language model) for each entity, according to the documents associated with that entity. From this representation we then estimate the probability of the query topic given the entity's language model. In our second group of models (Model 2) we first identify important documents for a given a topic, and then determine which entities are most closely associated with these documents.

For a formal definition of the models, check the [research papers](Papers.md) published on the subject. The actual computation of the models is explained in the doxygen documentation of Model 1 and Model 2.