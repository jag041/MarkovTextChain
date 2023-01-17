# recurse
This is a program which analyzes a piece of text written using UTF or ASCII encoding and spits out a Graph where the Nodes are each states in a Markov chain
It's a directed, weighted, and cyclic graph, and I was hoping to use it as a basis for doing some machine learning and frequency analysis on text

I included the original markov class that I stole much of the code from which uses a hash table rather than a graph, so the graph is implicit in the table

The graph is laid out in a vector, and the children of each node are stored as indices into that vector

I'm interested in trying out different kinds of data layouts as well
