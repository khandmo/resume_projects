import java.io.*;
import java.util.*;
import java.io.File;

public class SudiLearn {
    Map<String,Double> starts; // set of tags that we train to look for after # (first word of sentence)
    Map<String, Map<String, Double>> transitions; // tag to map of all possible next tags and their weights
    Map<String, Map<String, Double>> observations; // tag to map of all words at tag and their weights

    public SudiLearn(){
        transitions = new HashMap<String, Map<String, Double>>();
        observations = new HashMap<String, Map<String, Double>>();
        starts = new HashMap<String, Double>();
    }

    public void Trainer(String[] tags, String[] words) {
        //takes each line, already split by spaces such that each entry is a separate tag, word, and modifies the transitions and observations

        // Tags - deal with transition training
        for (int t=0; t<tags.length; t++) { // for each tag
            if (t==0){ // first word organization
                if (starts.containsKey(tags[t])){ // if starts contains the first tag
                    starts.put(tags[t], starts.get(tags[t]) + 1.0); // replace tag weight with tag weight + 1
                }
                else starts.put(tags[t], 1.0); // if starts doesn't contain the tag, simply make it weigh 1 to initialize
            }
            if (t == tags.length - 1) { // if tag is a period
                Map<String, Double> placeHolder = new HashMap<>();
                observations.put(tags[t], placeHolder); // place in observations with empty map (periods are static)
            }
            else if (!transitions.containsKey(tags[t])) { // if tag not in transitions yet
                Map<String, Double> newTrans = new HashMap<>(); // make new transition
                newTrans.put(tags[t+1], 1.0); // input next tag with weight 1
                transitions.put(tags[t], newTrans); // add tag to trans with next tag and initial weight

                Map<String, Double> placeHolder = new HashMap<>();
                observations.put(tags[t], placeHolder); // adds tag to obs without any further data
            }
            else if (transitions.get(tags[t]).containsKey(tags[t + 1])) { // if current tag has transitioned to next tag before
                // updates weight with each call
                transitions.get(tags[t]).put(tags[t+1], transitions.get(tags[t]).get(tags[t+1]) + 1.0);
                // update transition with +1 weight everytime it is seen
            }
            else if (!transitions.get(tags[t]).containsKey(tags[t+1])) { // if current tag transitions to a new next tag
                transitions.get(tags[t]).put(tags[t+1], 1.0);
                // if never seen transition before initialize with weight 1
            }
        }
        // Words - deal with observation training
        for (int w=0; w<words.length; w++) { // for each word
            if (!observations.get(tags[w]).containsKey(words[w])){ // if tag matching word does not contain the word yet
                observations.get(tags[w]).put(words[w], 1.0); // add map to tag of word with weight 1
            }
            else if (observations.get(tags[w]).containsKey(words[w])){ // if tag matching word does contain the word already update the weight
                //updates weight with each call
                observations.get(tags[w]).put(words[w], (observations.get(tags[w]).get(words[w]) + 1.0));
                // update observation with +1 weight everytime it is seen
            }
        }
        // The weights so far have only been the frequency of objects, they must be weighted in another method
    }

    public static List<Map<String,String>> Viterbi(Map<String, Map<String, Double>> transMap, Map<String, Map<String, Double>> obsMap, String[] lineList, Map<String,Double> starts) {
        // Returns List of Maps that map Observations in order to their best winning Tags
        String start = "#";
        List<String> currStates = new ArrayList<String>();
        currStates.add(start); // start with #

        Map<String,Double> currScores = new TreeMap<>();
        currScores.put(start, 0.0);
        TreeMap<Double,String> currScoresRev = new TreeMap<>(Double::compareTo); // ordered map with weights as keys and tags as values
        currScoresRev.put(0.0, start);

        List<Map<String,String>> backTrace = new ArrayList<>();
        // list of each observation state i, map of each nextState to it's most probable previousState

        for (String s : lineList) { // for each word in the observation list
            List<String> nextStates = new ArrayList<String>();
            Map<String, Double> nextScores = new TreeMap<>(); //map with tags as keys and weights as values for quick search
            TreeMap<Double, String> nextScoresRev = new TreeMap<>(Double::compareTo); // ordered map with weights as keys and tags as values
            Map<String, String> backTraceEntry = new TreeMap<>();

            if (currStates.contains("#")) { // start case
                nextStates.addAll(starts.keySet());
                double nextScore;
                for (String k : nextStates) { // for each nextState TAG (first word tag)
                    if (obsMap.get(k).containsKey(s)) { // if tag has been observed as our first word
                        nextScore = starts.get(k); // assign tag
                    } else { // if tag has never been observed as our first word
                        nextScore = -10;
                    }
                    if (!nextScores.containsKey(k) || nextScore > nextScores.get(k)) {
                        nextScores.put(k, nextScore);
                        nextScoresRev.put(nextScore, k);
                    }
                }
            } else { // if not start case
                for (int j = 0; j < currStates.size(); j++) { // for each currState after #
                    if (!Objects.equals(currStates.get(j), ".")) { // if current state is not a period
                        for (String k : transMap.get(currStates.get(j)).keySet()) {// each nextState in Map of currState to nextStates
                            nextStates.add(k);
                            double nextScore;
                            if (obsMap.get(k).containsKey(s)) { // if obs is in list of obs for this tag
                                nextScore = currScores.get(currStates.get(j)) + transMap.get(currStates.get(j)).get(k) + obsMap.get(k).get(s);
                                // currScore at currState + nextState weight + obs weight of word at currState tag
                            } else { // if word is NEVER observed (in training data) by current tag
                                nextScore = currScores.get(currStates.get(j)) + transMap.get(currStates.get(j)).get(k) + -10;
                                // currScore at currState + nextState weight + U (log base 10 of 1/10, 1 in 10 probability gives -1)
                            }
                            //System.out.println(nextScore);
                            if (!nextScores.containsKey(k) || nextScore > nextScores.get(k)) { // if nextState not in nextScores Map or nextScore wins over current nextScore
                                //System.out.println("Winner! " + nextScore);
                                nextScores.put(k, nextScore); // insert this score with this nextState in nextScores Map
                                nextScoresRev.put(nextScore, k);
                            }
                        }
                    }
                }
            }
            if (nextScoresRev.lastKey() == -10.0){ // if best guess is unseen state -10, replace with highest frequency first word tag
                String lowestScoreTag = "dud";
                starts.put(lowestScoreTag, 0.0); // insert dud with 0.0 observable
                for (String k : starts.keySet()){ // for each possible start tag
                    if (starts.get(k) < starts.get(lowestScoreTag)){ // if the tag is larger than the current lowestScoreTag
                        lowestScoreTag = k; // get name of tag
                    }
                } // at the end of the for loop, lowestScoreTag will contain the name of correct tag, starts has dud in it still
                nextScores.put(lowestScoreTag, starts.get(lowestScoreTag)); // add lowestScoreTag and associated value nextScores maps
                nextScoresRev.put(starts.get(lowestScoreTag), lowestScoreTag);
                starts.remove("dud"); // remove dud tag from starts list
            }
            else if (currScoresRev.lastKey() - nextScoresRev.lastKey() > 10.0){ // if the chosen observable is the worst case observable
                //************************************************************************************
                nextScoresRev.remove(nextScoresRev.lastKey()); // go with second best (for some reason, this works a lot better)
                //************************************************************************************
            }
            backTraceEntry.put(nextScoresRev.get(nextScoresRev.lastKey()), currScoresRev.get(currScoresRev.lastKey()));
            // enter next State, then current State
            backTrace.add(backTraceEntry);
            currStates = nextStates;
            currScores = nextScores;
            currScoresRev = nextScoresRev;
        }
        return backTrace;
    }

    public void logNormalize(){
        //transMap Normalize
        for(Map.Entry<String, Map<String,Double>> entry : transitions.entrySet()){ // for each tag
            double count=0.0;
            for(Map.Entry<String,Double> select : transitions.get(entry.getKey()).entrySet()){ // for each transition
                count += select.getValue(); // counts the total number of transitions by accumulating them
            }
            for(Map.Entry<String,Double> select : transitions.get(entry.getKey()).entrySet()){ // for each transition
                transitions.get(entry.getKey()).put(select.getKey(), Math.log10(select.getValue() / count));
                //run identical loop now that I have the total number of transitions to divide by
                // replace freq with log of value/total freq
            }
        }
        //obsMap Normalize
        for(Map.Entry<String, Map<String,Double>> entry : observations.entrySet()){ // for each word
            double count=0.0;
            for(Map.Entry<String,Double> select : observations.get(entry.getKey()).entrySet()){ // for each observation
                count += select.getValue(); // counts the total number of observations by accumulating them
            }
            for(Map.Entry<String,Double> select : observations.get(entry.getKey()).entrySet()){ // for each observation
                observations.get(entry.getKey()).put(select.getKey(), Math.log10(select.getValue() / count));
                //run identical loop now that I have the total number of transitions to divide by
                // replace freq with log of value/total freq
            }
        }
        //states Normalize
        double stateCount = 0.0;
        for(Map.Entry<String,Double> entry : starts.entrySet()){ // for each first word
            stateCount += entry.getValue(); // total number of first words
        }
        for(Map.Entry<String,Double> entry : starts.entrySet()) { // for each first word
            starts.put(entry.getKey(), Math.log10(entry.getValue() / stateCount));
            // replace freq with log of value/total freq
        }
    }

    public static void printOutputToFile(List<Map<String, String>> vOutput, BufferedWriter outputFile) throws IOException {
        // takes each line of Viterbi output, parses and prints to output.txt file (made in function)
        for (Map<String, String> map : vOutput){ // for each mapping
            for (String k : map.keySet()){ // for each key (only one)
                outputFile.write(k + " "); // write out to file
            }
        }
        outputFile.write("\n");
    }

    public static void accuracyCalculator(BufferedReader trueSet, BufferedReader vSet) throws IOException {
        // compares each line of true set (provided) and vSet (viterbi output) and prints the accuracy of the algorithm implementation
        Double totalSize = 0.0;
        Double totalRight = 0.0;
        while (trueSet.ready() && vSet.ready()){
            String[] trueSetLine = trueSet.readLine().split(" ");
            String[] vSetLine = vSet.readLine().split(" ");
            totalSize += trueSetLine.length; // sum total words in line
            for (int i=0; i < trueSetLine.length; i++){ // for each tag in line
                if (Objects.equals(trueSetLine[i], vSetLine[i])){ // if tags match
                    totalRight += 1.0; // increase total right counter
                }
            }
        }
        System.out.println("Algorithm implementation accuracy: " + (totalRight/totalSize)*100 + "%");
        System.out.println("(Incorrectly tagged " + (totalSize-totalRight) + " out of " + totalSize + " words.)");
    }

    public static void main(String[] args) throws IOException {
        BufferedReader sentences = new BufferedReader(new FileReader("day20/simple-train-sentences.txt"));
        BufferedReader tagList = new BufferedReader(new FileReader("day20/simple-train-tags.txt"));
        SudiLearn sudi = new SudiLearn();

        //Trainer
        while (sentences.ready() && tagList.ready()) {
            String[] sentenceLine = sentences.readLine().split(" ");
            String[] tagLine = tagList.readLine().split(" ");
            sudi.Trainer(tagLine, sentenceLine);
        }
        sentences.close();
        tagList.close();

        //add # as initializer with the entire start library (first words)
        sudi.transitions.put("#", sudi.starts);

        //Normalize weights ( trainer has trained )
        sudi.logNormalize();

        //Input Trained data into Viterbi decoder
        BufferedReader tester = new BufferedReader(new FileReader("day20/simple-test-sentences.txt"));
        File outputFile = new File("day20/output.txt"); // create output file for accuracy measurement
        BufferedWriter outputFileWrite = new BufferedWriter(new FileWriter(outputFile));
        while (tester.ready()){
            String[] testerLine = tester.readLine().split(" ");
            List<Map<String, String>> vOutput = Viterbi(sudi.transitions, sudi.observations, testerLine, sudi.starts);
            printOutputToFile(vOutput, outputFileWrite);
            System.out.println(vOutput);
        }
        tester.close();
        outputFileWrite.close();

        // Compare output to provided correct tag file
        BufferedReader providedTags = new BufferedReader(new FileReader("day20/simple-test-tags.txt"));
        BufferedReader ourTags = new BufferedReader(new FileReader("day20/output.txt"));
        accuracyCalculator(providedTags, ourTags);
        providedTags.close();
        ourTags.close();
    }
}
