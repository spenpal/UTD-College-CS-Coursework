# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

from util import Stack, Queue, PriorityQueue, PriorityQueueWithFunction

class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return  [s, s, w, s, w, w, s, w]

def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print("Start:", problem.getStartState())
    print("Is the start a goal?", problem.isGoalState(problem.getStartState()))
    print("Start's successors:", problem.getSuccessors(problem.getStartState()))
    """
    "*** YOUR CODE HERE ***"
    # Starter Variables
    start = {
        'state': problem.getStartState(),
        'action': None,
        'stepCost': 0,
        'path': []
    }
    
    fringe, visited = Stack(), set()
    fringe.push(start)
    
    while not fringe.isEmpty():
        # Unpack elements from fringe
        node = fringe.pop()
        
        if node.get('state') in visited:
            continue
        visited.add(node.get('state'))  # Add state to visited set
        
        if problem.isGoalState(node.get('state')):
            return node.get('path') # Remove first element in path, which is 'None' (from start state)
        
        # Add non-visited successors to the fringe
        successors = problem.getSuccessors(node.get('state'))
        for succ_state, succ_action, succ_stepCost in successors:
            succ_node = {
                'state': succ_state,
                'action': succ_action,
                'stepCost': succ_stepCost,
                'path': node.get('path') + [succ_action]
            }
            fringe.push(succ_node)
                
    return [] # Return empty list if fringe is empty to begin with

def breadthFirstSearch(problem):
    """Search the shallowest nodes in the search tree first."""
    "*** YOUR CODE HERE ***"
    # Starter Variables
    start = {
        'state': problem.getStartState(),
        'action': None,
        'stepCost': 0,
        'path': []
    }
    
    fringe, visited = Queue(), set()
    fringe.push(start)
    flag = False
    
    while not fringe.isEmpty():
        # Unpack elements from fringe
        node = fringe.pop()
            
        if node.get('state') in visited:
            continue
        visited.add(node.get('state'))  # Add state to visited set
        
        if problem.isGoalState(node.get('state')):
            return node.get('path') # Remove first element in path, which is 'None' (from start state)
        
        # Add non-visited successors to the fringe
        successors = problem.getSuccessors(node.get('state'))
        for succ_state, succ_action, succ_stepCost in successors:
            succ_node = {
                'state': succ_state,
                'action': succ_action,
                'stepCost': succ_stepCost,
                'path': node.get('path') + [succ_action]
            }
            fringe.push(succ_node)
    
    return [] # Return empty list if fringe is empty to begin with

def uniformCostSearch(problem):
    """Search the node of least total cost first."""
    "*** YOUR CODE HERE ***"
    
    start = {
        'state': problem.getStartState(),
        'action': None,
        'stepCost': 0,
        'path': [],
        'gCost': 0,
    }
    
    fringe, visited = PriorityQueue(), set()
    fringe.push(start, start.get('gCost'))
    
    while not fringe.isEmpty():
        # Unpack elements from fringe
        node = fringe.pop()
        
        if node.get('state') in visited:
            continue
        visited.add(node.get('state'))  # Add state to visited set
        
        if problem.isGoalState(node.get('state')):
            return node.get('path') # Remove first element in path, which is 'None' (from start state)
        
        # Add non-visited successors to the fringe
        successors = problem.getSuccessors(node.get('state'))
        for succ_state, succ_action, succ_stepCost in successors:
            succ_node = {
                'state': succ_state,
                'action': succ_action,
                'stepCost': succ_stepCost,
                'path': node.get('path') + [succ_action],
                'gCost': node.get('gCost') + succ_stepCost,
            }
            fringe.push(succ_node, succ_node.get('gCost'))
                
    return [] # Return empty list if fringe is empty to begin with

def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0

def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""
    "*** YOUR CODE HERE ***"
    
    def priorityFunction(node):
        """Calculates f(n) by adding g(n) [total cost] and h(n) [heuristic cost]"""
        node['fCost'] = node.get('gCost') + node.get('hCost')
        return node.get('fCost')
    
    start = {
        'state': problem.getStartState(),
        'action': None,
        'stepCost': 0,
        'path': [],
        'gCost': 0,
        'hCost': heuristic(problem.getStartState(), problem)
    }
    
    fringe, visited = PriorityQueueWithFunction(priorityFunction), set()
    fringe.push(start)
    
    while not fringe.isEmpty():
        # Unpack elements from fringe
        node = fringe.pop()
        
        if node.get('state') in visited:
            continue
        visited.add(node.get('state'))  # Add state to visited set
        
        if problem.isGoalState(node.get('state')):
            return node.get('path') # Remove first element in path, which is 'None' (from start state)
        
        # Add non-visited successors to the fringe
        successors = problem.getSuccessors(node.get('state'))
        for succ_state, succ_action, succ_stepCost in successors:
            succ_node = {
                'state': succ_state,
                'action': succ_action,
                'stepCost': succ_stepCost,
                'path': node.get('path') + [succ_action],
                'gCost': node.get('gCost') + succ_stepCost,
                'hCost': heuristic(succ_state, problem)
            }
            fringe.push(succ_node)
            
    return [] # Return empty list if fringe is empty to begin with


# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
