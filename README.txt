Ethan Park
CS 482
Fall 2019
Homework Assignment 3

This directory contains the following files:
+--	Belief-Network
|	+--	belief_network
|	+--	belief_network.cpp
|	+--	input.txt
|	+--	makefile
|	+--	README.txt

This directory contains a program, belief_network.cpp which takes input from an input text file and outputs a specified belief network. It also takes in probabilities from the text file which can be calculated. A sample input text file is provided. The probabilities for each node as well as the names for each node can be changed (names of nodes must be a single character). NOTE: changing the structure of the network or the given probabilites (aside from their names) may cause problems (see assumptions). The correct structure and CPT will be generated but the calculated probabilities may not generate.

To compile this code change to the directory to HW3, and simply type make. To run this code enter ./belief_network <input_file> where input_file is the file containing the specifications for the belief network and probabilities to calculate.

Additional libraries added include iostream (needed for the cout function), fstream (used for file i/o), and math (needed for exponent function).

Assumpition: Since the prompt says to "implement a belief network which has the above structure and uses the provided probabilities" to "to write generalized functions that can be used to calculate various probabilities of the same format," I interpreted this to mean that the probabilities for each nodes would change but the structure and the probabilities to calculate would not change. Currently, I have made a program that can read in any structure but I only set up cases for the probability calculations listed in the prompt (with a network of the same structure) due to time constraints.
