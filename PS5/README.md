# PS5 - The Sudi Assistant
## Hidden Markov Models and Viterbi decoding

The program is the basis of voice assistants and is fed a set of training
sentences and a set of each word's respective tags, creating a hidden Markov
Model of the frequency of tags following other tags, so that it may infer tags
from further example sentences. When fed further example sentences, the program
uses the Viterbi decoding to infer tags following tags following tags, and returns
the set of guessed tags. After, the program is fed the correct tags and matches each
to return the percentage the algorithm got right.

Fed .txt files.


