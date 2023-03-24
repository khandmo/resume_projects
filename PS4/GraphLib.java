import java.io.*;
import java.util.*;
import static java.lang.Integer.parseInt;
public class GraphLib<V, E> {

	public Graph<V,E> tree;

	public GraphLib() {
		tree = new AdjacencyMapGraph<V,E>();
	}
	public static <V,E> Graph<V,E> bfs(Graph<V,E> g, V source){
		// takes a tree and a root actor and does a breadth first search to find the closest paths
		Graph<V,E> tree = new AdjacencyMapGraph<V,E>();
		tree.insertVertex(source);
		Set<V> visited = new HashSet<V>();
		Queue<V> queue = new LinkedList<V>();

		queue.add(source); //enqueue start vertex
		visited.add(source); //add start to visited Set
		while (!queue.isEmpty()) { //loop until no more vertices
			V u = queue.remove(); //dequeue
			for (V v : g.outNeighbors(u)) { //loop over out neighbors
				if (!visited.contains(v)) { //if neighbor not visited, then neighbor is discovered from this vertex
					visited.add(v); //add neighbor to visited Set
					queue.add(v); //enqueue neighbor
					tree.insertVertex(v);
					tree.insertDirected(v, u, g.getLabel(u, v)); //save that this vertex was discovered from prior vertex
				}
			}
		}
		return tree;
	}

	public static <V,E> Set<V> missingVertices(Graph<V,E> graph, Graph<V,E> subgraph){
		Set<V> disconnected = new HashSet<V>();
		graph.vertices().forEach(disconnected::add);
		subgraph.vertices().forEach(disconnected::remove);
		return disconnected;
	}

	public static <V,E> double averageSeparation(Graph<String, String> tree, Set<String> missingVertices, V root){
		Map<String, Integer> pathSet = new HashMap<String, Integer>();
		// create map of string actor to pathSize, so we know if we've visited before
		pathSet = recursiveAvgSepHelp(tree, root, root, pathSet, missingVertices); // call recursive function on root
		int pathSum = 0;
		for (String actor : pathSet.keySet()){ // for each actor
			pathSum += pathSet.get(actor); // enumerate path Sum
		}
		return pathSum/pathSet.size();
	}

	public static <V, E> Map<String, Integer> recursiveAvgSepHelp(Graph<String, String> tree, V root, V prevNode,
																  Map<String, Integer> pathSet, Set<String> missingVertices){
		for (String nextNode : tree.outNeighbors((String) prevNode)) { // for each child
			if (!pathSet.containsKey(nextNode) && !missingVertices.contains(nextNode)) {
				// if we've never seen an actor, and they have a path to our root
				Graph<String, String> BFS = bfs(tree, nextNode); // run BFS
				List<String> path = getPath(BFS, (String) root); // path from current node (nextNode) to root actor
				pathSet.put(nextNode, path.size()); // add to pathSet
				pathSet = recursiveAvgSepHelp(tree, root, nextNode, pathSet, missingVertices); // recurse
			}
		}
		return pathSet;
	}

	public static <V,E> List<V> getPath(Graph<V,E> tree, V v){
		if (!tree.hasVertex(v)){
			System.out.println("\nNo Path Found");
			return new ArrayList<V>();
		}
		List<V> path = new ArrayList<V>();
		V k = v;
		int i=0;
		while (tree.outDegree(k) != 0) { // while the element is not the root
			tree.outNeighbors(k).forEach(path::add); // add element to path
			k = path.get(i); //reset dummy variable
			i++; // increment (depth)
		}
		return path;
	}

	public static void main(String[] args) throws IOException {
		AdjacencyMapGraph<String, String> tree = new AdjacencyMapGraph<>();

		BufferedReader actorsLines = new BufferedReader(new FileReader("PS4/actors.txt"));
		BufferedReader moviesLines = new BufferedReader(new FileReader("PS4/movies.txt"));
		BufferedReader connectorLines = new BufferedReader(new FileReader("PS4/movie-actors.txt"));

		Map<String, String> actorsMap = new HashMap<>(); //initializes the complete map with key actorID and value actor name
		Map<String, String> moviesMap = new HashMap<>(); // initializes the complete map with key movieID and value movie name
		ArrayList<String[]> connectorList = new ArrayList<>(); // initializes the list of connector keys
		// transplant the entire actors doc to the actors map
		while (actorsLines.ready()) {
			String[] d = actorsLines.readLine().split("\\|");
			actorsMap.put(d[0], d[1]);
		}
		actorsLines.close(); // close reader
		// transplant the entire movies doc to the movies map
		while (moviesLines.ready()) {
			String[] d = moviesLines.readLine().split("\\|");
			moviesMap.put(d[0], d[1]);
		}
		moviesLines.close(); // close reader
		// transplant the entire movie-actors doc into the connector map
		while (connectorLines.ready()) {
			connectorList.add(connectorLines.readLine().split("\\|"));
		}
		connectorLines.close(); // close reader

		// TREE CONSTRUCTION
		for (int i = 0; i < connectorList.size(); i++) { // for each movie-actor connection
			for (int j = i + 1; j < (connectorList.size()); j++) { // run through it twice
				if (parseInt(connectorList.get(i)[0]) == parseInt(connectorList.get(j)[0])) { //if two actors are in the same movie
					if (!tree.hasVertex(actorsMap.get(connectorList.get(i)[1]))) { // if the tree does not contain the first actor
						tree.insertVertex(actorsMap.get(connectorList.get(i)[1])); // add to tree
					}
					if (!tree.hasVertex(actorsMap.get(connectorList.get(j)[1]))) { // if the tree does not contain the second actor
						tree.insertVertex(actorsMap.get(connectorList.get(j)[1])); // add to tree
					}
					// link the actors with the movie
					tree.insertUndirected(actorsMap.get(connectorList.get(i)[1]),
							actorsMap.get(connectorList.get(j)[1]), moviesMap.get(connectorList.get(i)[0]));
				}
			}
		}
		// GAME UI BELOW (INITIALIZE CENTER OF UNIVERSE)
		Scanner userInput = new Scanner(System.in);
		System.out.println("Welcome to the 'Kevin Bacon' game! This game allows you to find the closest actor connections.");
		System.out.println("To begin, name an actor below: ");

		String actorName;
		String secondaryActorName;
		actorName = userInput.nextLine(); // get user input
		Graph<String, String> BFS = bfs(tree, actorName); // create BFS tree for actor
		System.out.println(actorName + " (with " + (tree.numVertices() - missingVertices(tree, BFS).size()) +
				"/" + tree.numVertices() + " connections" +

				// ******************* COMMENT OUT THE BELOW LINE TO ELIMINATE AVERAGE SEPARATION **********************
				" and average separation " + averageSeparation(tree, missingVertices(tree, BFS), actorName) +

				") is\n" + "now the center of the universe. Who would you like to connect them with?");
		secondaryActorName = userInput.nextLine(); // get user input for next actor

		// LOOP TO LIST PATHS (SEARCH CONTINUOUSLY FOR SECOND ACTORS TO PAIR)
		while (!Objects.equals(secondaryActorName, "done")) {
			if (Objects.equals(secondaryActorName, "new")){ // if user wants a new main actor, initialize
				System.out.println("Name your new center of the universe below: ");
				actorName = userInput.nextLine();
				BFS = bfs(tree, actorName);
				System.out.println(actorName + " (with " + (tree.numVertices() - missingVertices(tree, BFS).size()) +
						"/" + tree.numVertices() + " connections) is now the center of the universe. " +
						"\nWho would you like to connect them with?");
				secondaryActorName = userInput.nextLine();
			}
			// determine path from main actor to secondary actor
			List<String> pathTree = getPath(BFS, secondaryActorName);
			if (BFS.hasVertex(secondaryActorName)) {
				System.out.println(secondaryActorName + "'s number is " + pathTree.size());
				System.out.println(secondaryActorName + " was in " + BFS.getLabel(secondaryActorName, pathTree.get(0)) +
						" with " + pathTree.get(0));
				for (int i = 0; i < pathTree.size(); i++) { // for each actor in path from main to secondary
					if (i + 1 < pathTree.size()) {
						System.out.println(pathTree.get(i) + " was in "
								+ BFS.getLabel(pathTree.get(i), pathTree.get(i + 1)) + " with " + pathTree.get(i + 1));
					}
				}
				System.out.println("\nWould you like to try another secondary actor? If so, type their name now.");
				System.out.println("If you would like a new center of the universe, type 'new' (will take time).");
				System.out.println("If you would like to end the game, type 'done'.");
				secondaryActorName = userInput.nextLine();
			}
			else if (!BFS.hasVertex(secondaryActorName) && !Objects.equals(secondaryActorName, "N")) {
				System.out.println("\nThis actor is not connected to "  + actorName + ". Please try another name.\n");
				secondaryActorName = userInput.nextLine();
			}
		}
		System.out.println("Goodbye");
	}
}
