//C++ Version 6.3.0-1

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <algorithm>
#include <iterator>
#include <array>
#include <set>
#include <map>

#define rows 3			// Dimensions of the tile board
#define cols 3

using namespace std;

struct Node
{
	int gameState[rows][cols];			// state of the game
	int goalArr[rows][cols];
	
	int hVal;
	int cost = 0;
	int level= 0;
	int statesExpanded = 0;
	string pathToGoal;
	
	vector<array<array<int, cols>, rows>> sequence;
	vector<string> moveMade;
}; 

struct CompareNodeCost
{
	bool operator()(const Node& a, const Node& b)
	{
		if (a.cost == b.cost)
		{
			return a.level > b.level;
		}	
		
		return a.cost > b.cost;
	}	
};

int Heuristic(const int currState[rows][cols], const int goal[rows][cols])
{
	int heuristic = 0;
	
	for (int i = 0; i < rows; i ++)
	{
		for (int j = 0; j < cols; j ++)
		{
			int tile = currState[i][j];
			
			if (tile != 0)
			{
				int xGoal;
				int yGoal;
				
				for (int k = 0; k < rows; k ++)
				{
					for (int l = 0; l < cols; l++)
					{
						if (goal[k][l] == tile)
						{
							xGoal = k;
							yGoal = l;
						}	
					}	
				}	
				
				int mDistance = abs(xGoal - i) + abs(yGoal - j);
				heuristic = heuristic + (mDistance * tile);
			}	
		}	
	}	
	
	return heuristic;
}	

struct CompareNodeHeuristic
{
	bool operator()(const Node& a, const Node& b)
	{
		return a.hVal > b.hVal;
	}	
};	

struct CompareNodeFN
{
	bool operator()(const Node& a, Node& b)
	{
		return (a.cost + a.hVal) > (b.cost + b.hVal);
	}
};	

bool GameComplete(int (&arr)[rows][cols], int (&arr2)[rows][cols])
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (arr[i][j] != arr2[i][j])
			{
				return false;
			}	
		}	
	}	
	return true;
}	
	
pair<int, int> FindZeroTile(const int arr[3][3])						//Find the blank space in the game board
{																		//Store the coordinates in a pair structure
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (arr[i][j] == 0)
			{
				return {i, j};
			}	
		}	
	}	
	return {-1, -1};
}

string CreateStateIDs(int arr[rows][cols])								//Use to create a sort of ID number for each
{																		//  state of the game visited. This will be 
	string str;															//  used to check if the current state will
																		//  go into the visited list.
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			str.append(to_string(arr[i][j]));
		}	
	}	
	
	return str;
}	

vector<Node> GetChildren(Node curr)
{
	vector<Node> childNodes;
	
	//pair <int, int> location = find
	
	int blankX = FindZeroTile(curr.gameState).first;					//Store coordinates of blank in X var
	int blankY = FindZeroTile(curr.gameState).second;					//Store coordinates of blank in Y var
	
	int up[2] = {-1, 0};												//Ways that blank can move
	int down[2] = {1, 0};
	int left[2] = {0, -1};
	int right[2] = {0, 1};
	
	int newX = blankX + up[0];											//Move blank up if it can
	int newY = blankY + up[1];
	
	if (  (newX >= 0 && newX < rows) && (newY >= 0 && newY < cols)  )
	{
		Node next = curr;
		int moveCost = curr.gameState[newX][newY];
		swap(next.gameState[blankX][blankY], next.gameState[newX][newY]);
		next.cost = curr.cost + moveCost;
		next.level = curr.level + 1;
		next.pathToGoal = curr.pathToGoal.append("Up -> ");
		childNodes.push_back(next);
	}	
	
	newX = blankX + down[0];											//Move blank down if it can
	newY = blankY + down[1];
	
	if (  (newX >= 0 && newX < rows) && (newY >= 0 && newY < cols)  )
	{
		Node next = curr;
		int moveCost = curr.gameState[newX][newY];
		swap(next.gameState[blankX][blankY], next.gameState[newX][newY]);
		next.cost = curr.cost + curr.gameState[newX][newY];
		next.level = curr.level + 1;
		next.pathToGoal = curr.pathToGoal.append("Down -> ");
		childNodes.push_back(next);
	}	
	
	newX = blankX + left[0];											//Move blank left if it can
	newY = blankY + left[1];
	
	if (  (newX >= 0 && newX < rows) && (newY >= 0 && newY < cols)  )
	{
		Node next = curr;
		int moveCost = curr.gameState[newX][newY];
		swap(next.gameState[blankX][blankY], next.gameState[newX][newY]);
		next.cost = curr.cost + curr.gameState[newX][newY];
		next.level = curr.level + 1;
		next.pathToGoal = curr.pathToGoal.append("Left -> ");
		childNodes.push_back(next);
	}	
	
	newX = blankX + right[0];											//Move blank right if it can
	newY = blankY + right[1];
	
	if (  (newX >= 0 && newX < rows) && (newY >= 0 && newY < cols)  )
	{
		Node next = curr;
		int moveCost = curr.gameState[newX][newY];
		swap(next.gameState[blankX][blankY], next.gameState[newX][newY]);
		next.cost = curr.cost + curr.gameState[newX][newY];
		next.level = curr.level + 1;
		next.pathToGoal = curr.pathToGoal.append("Right -> ");
		childNodes.push_back(next);
	}	
	
	return childNodes;
}	

void BFS(Node initial, Node end)
{
	vector<Node> fringe;
	set<string> visitedStates;											//Set to avoid adding repeats of visited states
	
	array<array<int, cols>, rows> tmpState;								//Because if I dont convert the array format like
																		//  like this then the code doesnt compile
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			tmpState[i][j] = initial.gameState[i][j];
		}	
	}	
	
	initial.sequence.clear();
	initial.sequence.push_back(tmpState);					
	initial.cost = 0;
	initial.level = 0;
	
	fringe.push_back(initial);
	visitedStates.insert(CreateStateIDs(initial.gameState));	//Add initial state to fringe and visited list
	
	int poppedNodes = 0;
	int fringeMax = 0;
	int genNodes = 0;
	int expNodes = 0; 
	
	while (fringe.size() > 0)
	{
		Node curr = fringe.front();										//Pop current state from fringe (first node added)
		fringe.erase(fringe.begin());
		poppedNodes = poppedNodes + 1;
		//expNodes = expNodes + 1;
		
		if (GameComplete(curr.gameState, end.gameState))				//Exit function if game is won
		{
			for (int i = 0; i < curr.sequence.size(); i++)
			{
				for (int j = 0; j < rows; j++)
				{
					for (int k = 0; k < cols; k++)
					{
						cout << curr.sequence[i][j][k] << " ";
					}	
					cout << endl;
				}	
				cout << "---------" << endl;
			}	
			
			cout << "Game won after a cost of " << curr.cost << endl;
			cout << "Game won at a depth of " << curr.level << endl;
			cout << "Nodes created: " << genNodes << endl;
			cout << "Nodes popped: " << poppedNodes << endl;
			cout << "Nodes expanded: " << expNodes << endl;
			cout << "Fringe size at largest: " << fringeMax << endl;
			
			return;													
		}
		
		vector<Node> childNodes = GetChildren(curr);
		expNodes = expNodes + 1;
		
		for (int i = 0; i < childNodes.size(); i ++)
		{
			string tmp = CreateStateIDs(childNodes[i].gameState);
			
			if (visitedStates.count(tmp) == 0)
			{
				visitedStates.insert(tmp);
				
				childNodes[i].sequence = curr.sequence;
				
				array<array<int, cols>, rows> tmpChild;
				
				for (int j = 0; j < rows; j++)
				{
					for (int k = 0; k < cols; k++)
					{
						tmpChild[j][k] = childNodes[i].gameState[j][k];
					}	
				}	
				
				childNodes[i].sequence.push_back(tmpChild);
				
				fringe.push_back(childNodes[i]);
				genNodes = genNodes + 1;
			}
		}

		if (fringe.size() > fringeMax)
		{
			fringeMax = fringe.size();
		}	
	}	
	
	cout << "Game cannot be won" << endl;
}	

void DFS(Node initial, Node end)
{
	vector<Node> fringe;
	set<string> visitedStates;											//Set to avoid adding repeats of visited states
	
	array<array<int, cols>, rows> tmpState;								//Because if I dont convert the array format like
																		//  like this then the code doesnt compile
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			tmpState[i][j] = initial.gameState[i][j];
		}	
	}	
	
	initial.sequence.clear();
	initial.sequence.push_back(tmpState);					
	initial.cost = 0;
	initial.level = 0;
	
	fringe.push_back(initial);
	visitedStates.insert(CreateStateIDs(initial.gameState));	//Add initial state to fringe and visited list
	
	int poppedNodes = 0;
	int fringeMax = 0;
	int genNodes = 0;
	int expNodes = 0; 
	
	while (fringe.size() > 0)
	{
		Node curr = fringe.back();										//Pop current state from fringe (first node added)
		fringe.pop_back();
		poppedNodes = poppedNodes + 1;
		//expNodes = expNodes + 1;
		
		if (GameComplete(curr.gameState, end.gameState))				//Exit function if game is won
		{
			for (int i = 0; i < curr.sequence.size(); i++)
			{
				for (int j = 0; j < rows; j++)
				{
					for (int k = 0; k < cols; k++)
					{
						cout << curr.sequence[i][j][k] << " ";
					}	
					cout << endl;
				}	
				cout << "---------" << endl;
			}	
			
			cout << "Game won after a cost of " << curr.cost << endl;
			cout << "Game won at a depth of " << curr.level << endl;
			cout << "Nodes created: " << genNodes << endl;
			cout << "Nodes popped: " << poppedNodes << endl;
			cout << "Nodes expanded: " << expNodes << endl;
			cout << "Fringe size at largest: " << fringeMax << endl;
			
			return;													
		}
		
		vector<Node> childNodes = GetChildren(curr);
		expNodes = expNodes + childNodes.size();
		
		for (int i = 0; i < childNodes.size(); i ++)
		{
			string tmp = CreateStateIDs(childNodes[i].gameState);
			
			if (visitedStates.count(tmp) == 0)
			{
				visitedStates.insert(tmp);
				
				childNodes[i].sequence = curr.sequence;
				
				array<array<int, cols>, rows> tmpChild;
				
				for (int j = 0; j < rows; j++)
				{
					for (int k = 0; k < cols; k++)
					{
						tmpChild[j][k] = childNodes[i].gameState[j][k];
					}	
				}	
				
				childNodes[i].sequence.push_back(tmpChild);
				
				fringe.push_back(childNodes[i]);
				genNodes = genNodes + 1;
			}
		}

		if (fringe.size() > fringeMax)
		{
			fringeMax = fringe.size();
		}	
	}	
	
	cout << "Game cannot be won" << endl;
}	

void DLDFS(Node initial, Node end, int limit)
{
	vector<Node> fringe;
	set<string> visitedStates;											//Set to avoid adding repeats of visited states
	
	array<array<int, cols>, rows> tmpState;								//Because if I dont convert the array format like
																		//  like this then the code doesnt compile
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			tmpState[i][j] = initial.gameState[i][j];
		}	
	}	
	
	initial.sequence.clear();
	initial.sequence.push_back(tmpState);					
	initial.cost = 0;
	initial.level = 0;
	
	fringe.push_back(initial);
	visitedStates.insert(CreateStateIDs(initial.gameState));	//Add initial state to fringe and visited list
	
	int poppedNodes = 0;
	int fringeMax = 0;
	int genNodes = 0;
	int expNodes = 0; 
	
	while (fringe.size() > 0)
	{
		Node curr = fringe.back();										//Pop current state from fringe (first node added)
		fringe.pop_back();
		poppedNodes = poppedNodes + 1;
		//expNodes = expNodes + 1;
		
		if (GameComplete(curr.gameState, end.gameState))				//Exit function if game is won
		{
			for (int i = 0; i < curr.sequence.size(); i++)
			{
				for (int j = 0; j < rows; j++)
				{
					for (int k = 0; k < cols; k++)
					{
						cout << curr.sequence[i][j][k] << " ";
					}	
					cout << endl;
				}	
				cout << "---------" << endl;
			}	
			
			cout << "Game won after a cost of " << curr.cost << endl;
			cout << "Game won at a depth of " << curr.level << endl;
			cout << "Nodes created: " << genNodes << endl;
			cout << "Nodes popped: " << poppedNodes << endl;
			cout << "Nodes expanded: " << expNodes << endl;
			cout << "Fringe size at largest: " << fringeMax << endl;
			
			return;													
		}
		
		if (curr.level > limit)
		{
			continue;
		}	
		
		vector<Node> childNodes = GetChildren(curr);
		expNodes = expNodes + childNodes.size();
		
		for (int i = 0; i < childNodes.size(); i ++)
		{
			string tmp = CreateStateIDs(childNodes[i].gameState);
			
			if (visitedStates.count(tmp) == 0)
			{
				visitedStates.insert(tmp);
				
				childNodes[i].sequence = curr.sequence;
				
				array<array<int, cols>, rows> tmpChild;
				
				for (int j = 0; j < rows; j++)
				{
					for (int k = 0; k < cols; k++)
					{
						tmpChild[j][k] = childNodes[i].gameState[j][k];
					}	
				}	
				
				childNodes[i].sequence.push_back(tmpChild);
				
				fringe.push_back(childNodes[i]);
				genNodes = genNodes + 1;
			}
		}

		if (fringe.size() > fringeMax)
		{
			fringeMax = fringe.size();
		}	
	}	
	
	cout << "Game cannot be won in " << limit << " moves." << endl;
}	

void IDDFS(Node initial, Node end, int limit)
{
	for (int currLimit = 1; currLimit < limit; currLimit++)
	{
		vector<Node> fringe;
		set<string> visitedStates;											//Set to avoid adding repeats of visited states
		
		array<array<int, cols>, rows> tmpState;								//Because if I dont convert the array format like
																			//  like this then the code doesnt compile
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				tmpState[i][j] = initial.gameState[i][j];
			}	
		}	
		
		initial.sequence.clear();
		initial.sequence.push_back(tmpState);					
		initial.cost = 0;
		initial.level = 0;
		
		fringe.push_back(initial);
		visitedStates.insert(CreateStateIDs(initial.gameState));	//Add initial state to fringe and visited list
		
		int poppedNodes = 0;
		int fringeMax = 0;
		int genNodes = 0;
		int expNodes = 0; 
		
		while (fringe.size() > 0)
		{
			Node curr = fringe.back();										//Pop current state from fringe (first node added)
			fringe.pop_back();
			poppedNodes = poppedNodes + 1;
			//expNodes = expNodes + 1;
			
			if (GameComplete(curr.gameState, end.gameState))				//Exit function if game is won
			{
				for (int i = 0; i < curr.sequence.size(); i++)
				{
					for (int j = 0; j < rows; j++)
					{
						for (int k = 0; k < cols; k++)
						{
							cout << curr.sequence[i][j][k] << " ";
						}	
						cout << endl;
					}	
					cout << "---------" << endl;
				}	
				
				cout << "Game won after a cost of " << curr.cost << endl;
				cout << "Game won at a depth of " << curr.level << endl;
				cout << "Nodes created: " << genNodes << endl;
				cout << "Nodes popped: " << poppedNodes << endl;
				cout << "Nodes expanded: " << expNodes << endl;
				cout << "Fringe size at largest: " << fringeMax << endl;
				
				return;													
			}
			
			if (curr.level > currLimit)
			{
				continue;
			}	
			
			vector<Node> childNodes = GetChildren(curr);
			expNodes = expNodes + 1;
			
			for (int i = 0; i < childNodes.size(); i ++)
			{
				string tmp = CreateStateIDs(childNodes[i].gameState);
				
				if (visitedStates.count(tmp) == 0)
				{
					visitedStates.insert(tmp);
					
					childNodes[i].sequence = curr.sequence;
					
					array<array<int, cols>, rows> tmpChild;
					
					for (int j = 0; j < rows; j++)
					{
						for (int k = 0; k < cols; k++)
						{
							tmpChild[j][k] = childNodes[i].gameState[j][k];
						}	
					}	
					
					childNodes[i].sequence.push_back(tmpChild);
					
					fringe.push_back(childNodes[i]);
					genNodes = genNodes + 1;
				}
			}
	
			if (fringe.size() > fringeMax)
			{
				fringeMax = fringe.size();
			}	
		}	
	}	
	
	cout << "Game cannot be won in " << limit << " moves." << endl;
}	

void UCS(Node initial, Node end)
{
	priority_queue<Node, vector<Node>, CompareNodeCost> fringe;
	map<string, int> visitedStates;											//Map 
	
	array<array<int, cols>, rows> tmpState;								//Because if I dont convert the array format like
																		//  like this then the code doesnt compile for 
	for (int i = 0; i < rows; i++)										//  some reason.
	{
		for (int j = 0; j < cols; j++)
		{
			tmpState[i][j] = initial.gameState[i][j];
		}	
	}	
	
	initial.sequence.clear();
	initial.sequence.push_back(tmpState);					
	initial.cost = 0;
	initial.level = 0;
	
	fringe.push(initial);
	visitedStates[CreateStateIDs(initial.gameState)] = 0;	//Add initial state to fringe and visited list
	
	int poppedNodes = 0;
	int fringeMax = 0;
	int genNodes = 0;
	int expNodes = 0; 
	
	while (fringe.size() > 0)
	{
		Node curr = fringe.top();										//Pop current state from fringe (first node added)
		fringe.pop();
		poppedNodes = poppedNodes + 1;
		
		if (GameComplete(curr.gameState, end.gameState))				//Exit function if game is won
		{
			for (int i = 0; i < curr.sequence.size(); i++)
			{
				for (int j = 0; j < rows; j++)
				{
					for (int k = 0; k < cols; k++)
					{
						cout << curr.sequence[i][j][k] << " ";
					}	
					cout << endl;
				}	
				cout << "---------" << endl;
			}	
			
			cout << "Game won after a cost of " << curr.cost << endl;
			cout << "Game won at a depth of " << curr.level << endl;
			cout << "Nodes created: " << genNodes << endl;
			cout << "Nodes popped: " << poppedNodes << endl;
			cout << "Nodes expanded: " << expNodes << endl;
			cout << "Fringe size at largest: " << fringeMax << endl;
			
			return;													
		}
		
		vector<Node> childNodes = GetChildren(curr);
		expNodes = expNodes + 1;
		
		for (int i = 0; i < childNodes.size(); i ++)
		{
			string tmp = CreateStateIDs(childNodes[i].gameState);
			
			if (visitedStates.count(tmp) == 0 || childNodes[i].cost < visitedStates[tmp])
			{
				visitedStates[tmp] = childNodes[i].cost;
				
				childNodes[i].sequence = curr.sequence;
				
				array<array<int, cols>, rows> tmpChild;
				
				for (int j = 0; j < rows; j++)
				{
					for (int k = 0; k < cols; k++)
					{
						tmpChild[j][k] = childNodes[i].gameState[j][k];
					}	
				}	
				
				childNodes[i].sequence.push_back(tmpChild);
				
				fringe.push(childNodes[i]);
				genNodes = genNodes + 1;
			}
		}

		if (fringe.size() > fringeMax)
		{
			fringeMax = fringe.size();
		}	
	}	
	
	cout << "Game cannot be won" << endl;
}	

void Greedy(Node initial, Node end)
{
	priority_queue<Node, vector<Node>, CompareNodeHeuristic> fringe;
	set<string> visitedStates;											//Set to avoid repeated states
	
	array<array<int, cols>, rows> tmpState;								//Because if I dont convert the array format like
																		//  like this then the code doesnt compile for 
	for (int i = 0; i < rows; i++)										//  some reason.
	{
		for (int j = 0; j < cols; j++)
		{
			tmpState[i][j] = initial.gameState[i][j];
		}	
	}	
	
	initial.sequence.clear();
	initial.sequence.push_back(tmpState);					
	initial.cost = 0;
	initial.level = 0;
	
	fringe.push(initial);
	visitedStates.insert(CreateStateIDs(initial.gameState));				//Add initial state to fringe and visited list
	
	int poppedNodes = 0;
	int fringeMax = 0;
	int genNodes = 0;
	int expNodes = 0; 
	
	while (fringe.size() > 0)
	{
		Node curr = fringe.top();										//Pop current state from fringe (first node added)
		fringe.pop();
		poppedNodes = poppedNodes + 1;
		
		if (GameComplete(curr.gameState, end.gameState))				//Exit function if game is won
		{
			for (int i = 0; i < curr.sequence.size(); i++)
			{
				for (int j = 0; j < rows; j++)
				{
					for (int k = 0; k < cols; k++)
					{
						cout << curr.sequence[i][j][k] << " ";
					}	
					cout << endl;
				}	
				cout << "---------" << endl;
			}	
			
			cout << "Game won after a cost of " << curr.cost << endl;
			cout << "Game won at a depth of " << curr.level << endl;
			cout << "Nodes created: " << genNodes << endl;
			cout << "Nodes popped: " << poppedNodes << endl;
			cout << "Nodes expanded: " << expNodes << endl;
			cout << "Fringe size at largest: " << fringeMax << endl;
			
			return;													
		}
		
		vector<Node> childNodes = GetChildren(curr);
		expNodes = expNodes + childNodes.size();
		
		for (int i = 0; i < childNodes.size(); i ++)
		{
			string tmp = CreateStateIDs(childNodes[i].gameState);
			
			if (visitedStates.count(tmp) == 0)
			{
				visitedStates.insert(tmp);
				
				childNodes[i].sequence = curr.sequence;
				
				array<array<int, cols>, rows> tmpChild;
				
				for (int j = 0; j < rows; j++)
				{
					for (int k = 0; k < cols; k++)
					{
						tmpChild[j][k] = childNodes[i].gameState[j][k];
					}	
				}	
				
				childNodes[i].sequence.push_back(tmpChild);
				
				childNodes[i].cost = curr.cost + 1;
				childNodes[i].level = curr.level + 1;
				childNodes[i].hVal = Heuristic(childNodes[i].gameState, end.gameState);
				
				fringe.push(childNodes[i]);
				genNodes = genNodes + 1;
			}
		}

		if (fringe.size() > fringeMax)
		{
			fringeMax = fringe.size();
		}	
	}	
	
	cout << "Game cannot be won" << endl;
}	

void AStar(Node initial, Node end)
{
	priority_queue<Node, vector<Node>, CompareNodeFN> fringe;
	set<string> visitedStates;											//Set to avoid repeated states
	
	array<array<int, cols>, rows> tmpState;								//Because if I dont convert the array format like
																		//  like this then the code doesnt compile for 
	for (int i = 0; i < rows; i++)										//  some reason.
	{
		for (int j = 0; j < cols; j++)
		{
			tmpState[i][j] = initial.gameState[i][j];
		}	
	}	
	
	initial.sequence.clear();
	initial.sequence.push_back(tmpState);					
	initial.cost = 0;
	initial.level = 0;
	initial.hVal = Heuristic(initial.gameState, end.gameState);
	
	fringe.push(initial);
	visitedStates.insert(CreateStateIDs(initial.gameState));				//Add initial state to fringe and visited list
	
	int poppedNodes = 0;
	int fringeMax = 0;
	int genNodes = 0;
	int expNodes = 0; 
	
	while (fringe.size() > 0)
	{
		Node curr = fringe.top();										//Pop current state from fringe (first node added)
		fringe.pop();
		poppedNodes = poppedNodes + 1;
		
		if (GameComplete(curr.gameState, end.gameState))				//Exit function if game is won
		{
			for (int i = 0; i < curr.sequence.size(); i++)
			{
				for (int j = 0; j < rows; j++)
				{
					for (int k = 0; k < cols; k++)
					{
						cout << curr.sequence[i][j][k] << " ";
					}	
					cout << endl;
				}	
				cout << "---------" << endl;
			}	
			
			cout << "Game won after a cost of " << curr.cost << endl;
			cout << "Game won at a depth of " << curr.level << endl;
			cout << "Nodes created: " << genNodes << endl;
			cout << "Nodes popped: " << poppedNodes << endl;
			cout << "Nodes expanded: " << expNodes << endl;
			cout << "Fringe size at largest: " << fringeMax << endl;
			
			return;													
		}
		
		vector<Node> childNodes = GetChildren(curr);
		expNodes = expNodes + childNodes.size();
		
		for (int i = 0; i < childNodes.size(); i ++)
		{
			string tmp = CreateStateIDs(childNodes[i].gameState);
			
			if (visitedStates.count(tmp) == 0)
			{
				visitedStates.insert(tmp);
				
				childNodes[i].sequence = curr.sequence;
				
				array<array<int, cols>, rows> tmpChild;
				
				for (int j = 0; j < rows; j++)
				{
					for (int k = 0; k < cols; k++)
					{
						tmpChild[j][k] = childNodes[i].gameState[j][k];
					}	
				}	
				
				childNodes[i].sequence.push_back(tmpChild);
				
				int move = curr.gameState[FindZeroTile(childNodes[i].gameState).first][FindZeroTile(childNodes[i].gameState).second];
				childNodes[i].cost = curr.cost + move;
				childNodes[i].level = curr.level + 1;
				childNodes[i].hVal = Heuristic(childNodes[i].gameState, end.gameState);
				
				fringe.push(childNodes[i]);
				genNodes = genNodes + 1;
			}
		}

		if (fringe.size() > fringeMax)
		{
			fringeMax = fringe.size();
		}	
	}	
	
	cout << "Game cannot be won" << endl;
}	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Uniform Cost Search
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

int main(int argc, char* argv[])		//argc = number of args  //argv[] array of cmd arguments passed
{
	if (argc < 4)
	{
		cerr << "Remember to include the start text file, goal text file, and a search algorithm!!" << endl;
		return 1;
	}

	cout << endl;
	
	cout << "File to open: " << argv[1] << endl;

	ifstream file(argv[1]);	
	
	Node n;
	
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (!(file >> n.gameState[i][j]))
			{
				cerr << "Invalid value at (" << i << ", " << j << ")" << endl;
				return 1;
			}	
		}	
	}
	cout << endl;
	
	file.close();
	
	cout << "Next file to open: " << argv[2] << endl;
	
	ifstream file2(argv[2]);
	
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (!(file2 >> n.goalArr[i][j]))
			{
				cerr << "Invalid value at (" << i << ", " << j << ")" << endl;
				return 1;
			}	
		}	
	}
	cout << endl;
	
	cout << "/////////////////////////////////////////////////////////////////////////////////////////" << endl;
	cout << endl;
	
	Node goal;
	
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			goal.gameState[i][j] = n.goalArr[i][j];
		}	
	}
	
	file2.close();
	
	string input = argv[3];
	
	if (input == "bfs" || input == "BFS")
	{
		cout << "Breadth First Search" << endl;
		cout << "-----------------------" << endl;
		
		try
		{
			BFS(n, goal);
		}
		catch (const bad_alloc& e)
		{
			cerr << "Unfortunately, this program has run out of memory: " << e.what() << endl;
			return 1;
		}	
	}
	else if (input == "dfs" || input == "DFS")
	{
		cout << "Depth First Search" << endl;
		cout << "-----------------------" << endl;
		
		try
		{
			DFS(n, goal);
		}
		catch (const bad_alloc& e)
		{
			cerr << "Unfortunately, this program has run out of memory: " << e.what() << endl;
			return 1;
		}	
	}	
	else if (input == "dldfs" || input == "DLDFS")
	{
		cout << "Enter a depth limit for this algorithm: " << endl;
		int depthLimit;
		
		cin >> depthLimit;
		
		cout << "Depth Limited Search with a depth limit of " << depthLimit << endl;
		cout << "-----------------------" << endl;
		
		try
		{
			DLDFS(n, goal, depthLimit);
		}
		catch (const bad_alloc& e)
		{
			cerr << "Unfortunately, this program has run out of memory: " << e.what() << endl;
			return 1;
		}	
	}
	else if (input == "iddfs" || input == "IDDFS")
	{
		cout << "Enter a depth limit for this algorithm: " << endl;
		int depthLimit;
		
		cin >> depthLimit;
		
		cout << "Iteratively Deepening Search with depth limit of " << depthLimit << endl;
		cout << "-----------------------" << endl;
		
		try
		{
			IDDFS(n, goal, depthLimit);
		}
		catch (const bad_alloc& e)
		{
			cerr << "Unfortunately, this program has run out of memory: " << e.what() << endl;
			return 1;
		}	
	}
	else if (input == "ucs" || input == "UCS")
	{
		cout << "Uniform Cost Search" << endl;
		cout << "-----------------------" << endl;
		
		try
		{
			UCS(n, goal);
		}
		catch (const bad_alloc& e)
		{
			cerr << "Unfortunately, this program has run out of memory: " << e.what() << endl;
			return 1;
		}	
	}
	else if (input == "greedy" || input == "GREEDY" || input == "Greedy")
	{
		cout << "Greedy Search" << endl;
		cout << "-----------------------" << endl;
		
		try
		{
			Greedy(n, goal);
		}
		catch (const bad_alloc& e)
		{
			cerr << "Unfortunately, this program has run out of memory: " << e.what() << endl;
			return 1;
		}	
	}
	else if (input == "astar" || input == "ASTAR" || input == "AStar")
	{
		cout << "A* Search" << endl;
		cout << "-----------------------" << endl;
		
		try
		{
			BFS(n, goal);
		}
		catch (const bad_alloc& e)
		{
			cerr << "Unfortunately, this program has run out of memory: " << e.what() << endl;
			return 1;
		}	
	}
	else
	{
		cout << "Unfamiliar search algorithm. Please try again" << endl;
	}
	
	return 0;
}