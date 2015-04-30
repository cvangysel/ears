# Installing EARS #

## Requirements and prerequisites ##

The sources have only two hard dependencies: the [Lemur toolkit](http://sourceforge.net/projects/lemur/) and the STL (which comes with all sane C++ compilers).

### Installing the Lemur toolkit ###
See the [Compiling and Installing](http://sourceforge.net/apps/trac/lemur/wiki/Compiling%20and%20Installing) section on the [Lemur Toolkit and Indri Search Engine Wiki](http://sourceforge.net/apps/trac/lemur/wiki).

Note: EARS has been tested with the following versions of the Lemur toolkit: 4.3, 4.4, 4.7, 4.8, 4.9, 4.10

You might want to check the [known issues](KnownIssues.md) page, even if using the most recent version of the Lemur toolkit.


## Installing and compiling EARS ##
  1. download and extract EARS
  1. open `Makefile` and set the value of the `Lemur_dir` variable according to your local settings
  1. The build is fairly conventional. Go to the EARS directory, and type `make`
    * If you need to re-compile it for any reason, you may want to issue `make clean` and then `make`

For using EARS on Windows with Visual Studio 2008, [consult this page](CompilingWithVisualStudio.md).

# Using EARS #

Usage will be demonstrated on the ["ILPS Abstracts" data set](ExampleDataSet.md), shipped with the EARS distribution.

## Indexing the collection ##
The document collection needs to be indexed using the Lemur toolkit.

  1. Change to the `EARS` directory<br><i>This step is crucial as paths are set relative to this directory. Alternatively, you may edit the config files and change the paths to absolute ones.</i>
<ol><li>Execute either of the two:<br>
<ul><li><code>{Lemur_dir}/bin/IndriBuildIndex examples/ilps_abstracts/conf/IndriBuildIndex.conf examples/ilps_abstracts/stopwords/stopwords.indri</code> for building an Indri index (preferred)<br>
</li><li><code>{Lemur_dir}/bin/BuildIndex examples/ilps_abstracts/conf/BuildIndex.conf</code> for building a KeyFile index,<br>
where <code>{Lemur_dir}</code> is the location of your Lemur installation.</li></ul></li></ol>

More details about the indexing process and the differences between index types are given on <a href='http://sourceforge.net/apps/trac/lemur/wiki/Building%20Indexes'>this page</a>.<br>
<br>
<h2>Finding and profiling entities</h2>
EARS can help you in two entity related tasks:<br>
<ul><li><b>finding entities</b> is about retrieving entities for a query topic (<i>"Which entities are associated with the topic?"</i>). In our example, we rank people with respect to their expertise given a particular topical area. Therefore, for each query, a ranked list of entities (names) is returned.<br>
</li><li><b>entity profiling</b> is the other way around: for each entity we return topics (<i>"Which topics are associated with the entity?"</i>) In our example, topics are ranked according to the person's proficiency in each.</li></ul>

Learn more about the tasks and the association finding models <a href='AssociationFinding.md'>here</a>.<br>
<br>
<h3>Syntax</h3>
The syntax of the <code>ears</code> program is the following:<br>
<code>ears &lt;command&gt; &lt;paramfile&gt; [debug_level]</code>

Where<br>
<ul><li><code>&lt;command&gt;</code> is either <code>ef</code> (for entity finding) or <code>ep</code> (for entity profiling)<br>
</li><li><code>&lt;paramfile&gt;</code> is a parameter file (see <a href='Parameters.md'>this page</a> for the description of possible parameters)<br>
</li><li><code>debug_level</code> (optionally) is the reporting level (see below and <a href='Logging.md'>this page</a>)</li></ul>

<h3>Parameter files</h3>
There are sample parameter files for the two baseline models in the <code>examples/ilps_abstracts/conf</code> directory: <code>ears_model1.conf</code> and <code>ears_model2.conf</code> for Models 1 and 2, respectively.<br>
<ul><li>If you indexed the collection using a KeyFile index, you need to edit these files, and<br>replace the line <code>&lt;index&gt;examples/ilps_abstracts/index&lt;/index&gt;</code> <br>with <code>&lt;index&gt;examples/ilps_abstracts/index/index.key&lt;/index&gt;</code></li></ul>

<h3>Example usage</h3>
<i>All examples are executed from the EARS directory</i>

Finding experts using Model 1:<br>
<pre><code>ears ef examples/ilps_abstracts/conf/ears_model1.conf<br>
</code></pre>

The output will look like:<br>
<pre><code>Entity and Association Retrieval System (EARS) v0.9 (2009-09-09)<br>
Copyright (c) 2005-2009, Krisztian Balog<br>
<br>
Loading index 'examples/ilps_abstracts/index'<br>
Loading Entity-Document associations 'examples/ilps_abstracts/data/associations.list'<br>
Loading queries 'examples/ilps_abstracts/data/queries.ldf'<br>
Initializing Model 1<br>
Estimating smoothing parameter<br>
Calculating topic-entity associations<br>
Model 1 completed<br>
Writing output to 'examples/ilps_abstracts/output/model1.out'<br>
Over and out<br>
</code></pre>

The debug levels can be used to see what is actually happening.<br>
E.g., using <code>debug</code>
<pre><code>ears ef examples/ilps_abstracts/conf/ears_model1.conf debug<br>
</code></pre>
will help you to follow the process of the computation:<br>
<pre><code>Estimating smoothing parameter<br>
  Parameter beta is set to 426.557<br>
Calculating topic-entity associations<br>
  Constructing collection Language Model<br>
  Caching collection probabilities<br>
  Entity #1/61 (K_Balog)<br>
  Entity #2/61 (M_Rijke)<br>
  Entity #3/61 (W_Weerkamp)<br>
</code></pre>

Using <code>debugall</code> will include even more details, e.g., how the scores are computed:<br>
<pre><code>  Entity #1/61 (K_Balog)<br>
    Creating model based on 9 associated documents<br>
    Scoring queries<br>
      Query #1/10 ('q-01')<br>
        log p(q|e) = <br>
          + p('information'|q) [0.333333] * log( p(t|e) [0.00336702] )<br>
          + p('model'|q) [0.333333] * log( p(t|e) [0.00554641] )<br>
          + p('retrieval'|q) [0.333333] * log( p(t|e) [0.0102995] )<br>
          = -5.15466<br>
        p(q|e) = 0.00577242<br>
      Query #2/10 ('q-02')<br>
        log p(q|e) = <br>
          + p('enterprise'|q) [0.333333] * log( p(t|e) [0.00507137] )<br>
          + p('expert'|q) [0.333333] * log( p(t|e) [0.00764664] )<br>
          + p('finding'|q) [0.333333] * log( p(t|e) [0.00974633] )<br>
          = -4.9295<br>
</code></pre>

On the other end of the spectrum, you may use <code>warning</code> or <code>error</code> as the <code>debug_level</code> to report only warnings and errors, respectively.