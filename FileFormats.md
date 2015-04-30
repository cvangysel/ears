## Query file ##
A query file may contain any number of queries. Query terms (optionally along with the corresponding weights) are enumerated between `<DOC qID>` and `</DOC>` tags, where `qID` is a unique query identifier. The weight of each term can be defined after the character `;` -- if not provided explicitly, the value `1.0` is used. Term weights are normalized automatically, so that they sum up to 1.

| **Syntax** | **Example** |
|:-----------|:------------|
| `<DOC qID>`<br><code>term_1</code><br><code>...</code><br><code>term_n</code><br><code>&lt;/DOC&gt;</code> <table><thead><th> <code>&lt;DOC q-01&gt;</code><br><code>solar</code><br><code>energy</code><br><code>&lt;/DOC&gt;</code> </th></thead><tbody>
<tr><td> <code>&lt;DOC qID&gt;</code><br><code>term_1;weight_1</code><br><code>...</code><br><code>term_n;weight_n</code><br><code>&lt;/DOC&gt;</code> </td><td> <code>&lt;DOC q-01&gt;</code><br><code>solar;0.55</code><br><code>energy;0.45</code><br><code>&lt;/DOC&gt;</code> </td></tr></tbody></table>

<h2>Document-entity association file</h2>
The syntax of the document-entity association file is controlled by the parameter <code>associationFileFormat</code>.<br>
<br>
<table><thead><th> <b>Format</b> </th><th> <b>Syntax</b> </th><th> <b>Example</b> </th></thead><tbody>
<tr><td> <code>0</code> (default) </td><td> <code>DOCNO entityID</code> </td><td> <code>Doc-01 Entity-01</code><br><code>Doc-01 Entity-02</code><br><code>Doc-02 Entity-01</code> </td></tr>
<tr><td> <code>1</code> </td><td> <code>entityID DOCNO</code> </td><td> <code>Entity-01 Doc-01</code><br><code>Entity-02 Doc-01</code><br><code>Entity-01 Doc-02</code> </td></tr>
<tr><td> <code>2</code> </td><td> <code>DOCNO entityAlias</code> </td><td> <code>Doc-01 Person-01-name</code><br><code>Doc-01 Person-01-email</code><br><code>Doc-01 Person-02-name</code> </td></tr>
<tr><td> <code>3</code> </td><td> <code>entityAlias DOCNO</code> </td><td> <code>Person-01-name Doc-01</code><br><code>Person-01-email Doc-01</code><br><code>Person-02-name Doc-01</code> </td></tr>
<tr><td> <code>4</code> </td><td> <code>DOCNO entityID weight</code> </td><td> <code>Doc-01 Entity-01 0.5</code><br><code>Doc-01 Entity-02 0.5</code><br><code>Doc-02 Entity-01 0.2</code> </td></tr>
<tr><td> <code>5</code> </td><td> <code>entityID DOCNO weight</code> </td><td> <code>Entity-01 Doc-01 0.5</code><br><code>Entity-02 Doc-01 0.5</code><br><code>Entity-01 Doc-02 0.2</code> </td></tr>
<tr><td> <code>6</code> </td><td> <code>DOCNO entityAlias weight</code> </td><td> <code>Doc-01 Person-01-name 0.1</code><br><code>Doc-01 Person-01-email 0.3</code><br><code>Doc-01 Person-02-name 0.2</code> </td></tr>
<tr><td> <code>7</code> </td><td> <code>entityAlias DOCNO weight</code> </td><td> <code>Person-01-name Doc-01 0.1</code><br><code>Person-01-email Doc-01 0.3</code><br><code>Person-02-name Doc-01 0.2</code> </td></tr></tbody></table>


Notes:<br>
<ul><li>columns are separated by whitespace<br>
</li><li><code>DOCNO</code> denotes the external document ID<br>
</li><li><code>weight</code> is a non-negative numeric value (integer or real)<br>
</li><li>association weights are NOT re-normalized (unlike query term weights)<br>
</li><li>when <code>EntityAlias</code> is used, an !entityAlias-entityID mapping needs to be provided; see <a href='EntityRepresentation.md'>EntityRepresentation</a> and below</li></ul>

<h2>Entity alias mapping file</h2>
Location defined in parameter <code>entityAliasMapping</code>. See <a href='EntityRepresentation.md'>EntityRepresentation</a> for the explanation of entity aliases.<br>
<br>
<table><thead><th> <b>Syntax</b> </th><th> <b>Example</b> </th></thead><tbody>
<tr><td> <code>entityAlias entityID</code> </td><td> <code>johnsmith John_Smith</code><br><code>john.smith@email.com John_Smith</code><br><code>Person-02-name Person_02</code> </td></tr></tbody></table>


<h2>Output file</h2>
The syntax of the output file is controlled by the parameter <code>OutputFileFormat</code>.<br>
<br>
<table><thead><th> <b>Format</b> </th><th> <b>Task</b> </th><th> <b>Syntax</b> </th><th> <b>Example</b> </th></thead><tbody>
<tr><td> <code>0</code> (default) </td><td> <code>ef</code> </td><td> <code>qID Q0 entityID rank score runID</code> </td><td> <code>q-01 Q0 Entity-03 1 0.891 Model1</code><br><code>q-01 Q0 Entity-19 2 0.750 Model1</code><br><code>q-01 Q0 Entity-01 3 0.749 Model1</code> </td></tr>
<tr><td> <code>1</code> </td><td> <code>ef</code> </td><td> <code>qID entityID score</code> </td><td> <code>q-01 Entity-03 0.891</code><br><code>q-01 Entity-19 0.750</code><br><code>q-01 Entity-01 0.749</code> </td></tr>
<tr><td> <code>0</code> (default) </td><td> <code>ep</code> </td><td> <code>entityID Q0 qID rank score runID</code> </td><td> <code>Entity-01 Q0 q-13 1 0.179 Model1</code><br><code>Entity-01 Q0 q-27 2 0.153 Model1</code><br><code>Entity-01 Q0 q-09 3 0.091 Model1</code><br> </td></tr>
<tr><td> <code>1</code> </td><td> <code>ep</code> </td><td> <code>entityID qID score</code> </td><td> <code>Entity-01 q-13 0.179</code><br><code>Entity-01 q-27 0.153</code><br><code>Entity-01 q-09 0.091</code><br> </td></tr>