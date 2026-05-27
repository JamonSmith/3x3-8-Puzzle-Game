Developed using C++ 6.3.0-1

-----------------------------------------------------------------------------
CODE STRUCTURE:

For my code, I used a struct, Node, to store all of the essential variables
and values that I need. There are three more structs that I implemented: 
CompareNodeCost (used to order the fifo fringe in ascending order by cost of 
each move in UCS), CompareNodeHeuristic (same as CompareNodeCost, but uses 
the heuristic value instead of the cost in Greedy search), and CompareNodeFN 
(combines CompareNodeCost and CompareNodeHeuristic to use for A* search). But 
those are only "Comparators" used to create my priority queue structures for
a few of the search strategies: UCS, Greedy, and A*.

There are also a few functions declared before the main function: 
FindZeroTile (keeps track of where the "blank space" on the game board is), 
GameComplete (the goal evaluation function), Heuristic (calculates the 
heuristic value for each state of the game that the algorithms examine, 
CreateStateIDs (makes an "ID number" of sorts so that my visited list can 
keep track of game states that have already been examined to avoid adding 
duplicates, and GetChildren (creates all the possible moves that can be made
from the current state of the game to store in the fringe and wait to be 
examined).

Then I have a function for each of the search algorithms: BFS, DFS, DLDFS, 
IDDFS, UCS, Greedy Search, and A* Search. Each of which has their own 
implementation of the fringe (because different search strategies call for 
different requirements to organize the fringe and which nodes to pop before
others. Each of these functions implements the search strategy, outputs the 
cost of the path to the goal that was found, how many steps it took, the 
amount of nodes added to the fringe, amount generated, amount expanded, 
amount popped from the fringe, and the sequence of moves from the initial 
state that was taken to reach the goal state.  

Then I have the main function which reads the startfile.txt and adds its 
contents to a 3x3 array to be used as the game board that is manipulated over
each step in the game. Then it reads the goalfile.txt and adds its contents 
to another array which is then used as the the state which the game is won.
the first array is then compared to the second array to see if the game must
continue or can stop due to being in the goal state. Finally I used a 
conditional statement that makes the use choose which algorithm to use.

-----------------------------------------------------------------------------
HOW TO RUN:

This code can be run using an IDE or the command line, I used command line, 
but I did not notice any part that may make running using an IDE difficult.

1. Open the zipped file that contains the contents of the project and 
   extract the contents of the file to a folder that you can open using the 
   command line. 
   
   * For me, I used a folder I labeled C++ that I kept in another folder I 
     labeled CMD_programs that I kept in my Downloads folder (the downloads 
	   folder was the only folder that I could access the contents of through
     my command line for some reason).


1. Navigate to the folder that the program is in on your computer using the 
   command line. For me it was using the command: 
   cd Downloads/CMD_programs/C++			
   
   * Yes, I used forward slashes because the back slashes did notwork for
     me, they might work for you however, I am not sure. You are welcome to
     try it yourself.
			
			
2. If using the same MinGW method as I did, compile the code using the
   command:
   g++ expense_8_puzzle.cpp -o expense_8_puzzle    
   
   * Or another name for the .exe that you prefer, or even leave off the 
	 "-o expense_8_puzzle" and the .exe should be named "a". For safety, I 
	 recommend just copy and pasting the command I gave to compile the code.
   
   
3. Run the .exe file using the command:
   <name_of_exe> startFile.txt goalFile.txt <search_strategy>   
													
   * This zip file should come with two text files, one named startFile.txt 
     and the other named goalFile.txt. They each contain the initial and
     goal states respectively, so you must put the startFile in the argument
     before the goalFile so the code runs properly.
												 
	Note: The arguments to pass in place of 
		  <search_strategy> include:
		  bfs or BFS
		  dfs or DFS
		  dldfs or DLDFS
		  iddfs or IDDFS
		  ucs or UCS
		  greedy, GREEDY or Greedy
		  astar, ASTAR or AStar			
		  
		  * Any others will be considered invalid
		    and not work.
										 
