# Filename:     myTeam.py
# Date:         5/13/21
# Authors:      Sanjeev Penupala, Sanjana Sivakumar
# Email:        sanjeev.penupala@utdallas.edu, sxs170002@utdallas.edu
# Course:       CS 4365.001

from captureAgents import CaptureAgent
import distanceCalculator
import random, time, util, sys
from game import Directions
import game
from util import nearestPoint, manhattanDistance

#################
# Team creation #
#################

def createTeam(firstIndex, secondIndex, isRed,
               first = 'MixReflexAgent', second = 'DefensiveReflexAgent'):
  """
  This function should return a list of two agents that will form the
  team, initialized using firstIndex and secondIndex as their agent
  index numbers.  isRed is True if the red team is being created, and
  will be False if the blue team is being created.

  As a potentially helpful development aid, this function can take
  additional string-valued keyword arguments ("first" and "second" are
  such arguments in the case of this function), which will come from
  the --redOpts and --blueOpts command-line arguments to capture.py.
  For the nightly contest, however, your team will be created without
  any extra arguments, so you should make sure that the default
  behavior is what you want for the nightly contest.
  """
  return [eval(first)(firstIndex), eval(second)(secondIndex)]

def getSuccessorsPositions(currentPosition, legalPositions):
    successorsPositions = []
    x, y = currentPosition
    if (x + 1, y) in legalPositions:
        successorsPositions.append((x + 1, y))
    if (x - 1, y) in legalPositions:
        successorsPositions.append((x - 1, y))
    if (x, y + 1) in legalPositions:
        successorsPositions.append((x, y + 1))
    if (x, y - 1) in legalPositions:
        successorsPositions.append((x, y - 1))
    return successorsPositions

##########
# Agents #
##########

class ReflexCaptureAgent(CaptureAgent):
  """
  A base class for reflex agents that chooses score-maximizing actions
  """
 
  def registerInitialState(self, gameState):
    self.start = gameState.getAgentPosition(self.index)
    CaptureAgent.registerInitialState(self, gameState)
    
    self.stuckMoves = 0
    self.nextEntry = None
    self.nextCapsule = None

  def chooseAction(self, gameState):
    """
    Picks among the actions with the highest Q(s,a).
    """
    actions = gameState.getLegalActions(self.index)

    # You can profile your evaluation time by uncommenting these lines
    # start = time.time()
    values = [self.evaluate(gameState, a) for a in actions]
    # print 'eval time for agent %d: %.4f' % (self.index, time.time() - start)

    maxValue = max(values)
    bestActions = [a for a, v in zip(actions, values) if v == maxValue]

    return random.choice(bestActions)

  def getSuccessor(self, gameState, action):
    """
    Finds the next successor which is a grid position (location tuple).
    """
    successor = gameState.generateSuccessor(self.index, action)
    pos = successor.getAgentState(self.index).getPosition()
    if pos != nearestPoint(pos):
      # Only half a grid position was covered
      return successor.generateSuccessor(self.index, action)
    else:
      return successor

  def evaluate(self, gameState, action):
    """
    Computes a linear combination of features and feature weights
    """
    features = self.getFeatures(gameState, action)
    weights = self.getWeights(gameState, action)
    return features * weights
  
  def getTimeLeft(self, gameState):
      return gameState.data.timeleft
    
  def distanceToBoundary(self, gameState):     
    currentPosition = gameState.getAgentState(self.index).getPosition()
    width = gameState.data.layout.width
    legalPositions = [p for p in gameState.getWalls().asList(False)]
    if self.red:
      boundaryPositions = [p for p in legalPositions if p[0] == (width / 2) - 1]
    else:
      boundaryPositions = [p for p in legalPositions if p[0] == width / 2]
      
    return min([self.getMazeDistance(currentPosition, a) for a in boundaryPositions])
  
  def getEntrys(self, gameState):
    width = gameState.data.layout.width
    legalPositions = [p for p in gameState.getWalls().asList(False)]
    redBoundaryPositions = [p for p in legalPositions if p[0] == (width / 2) - 1]
    blueBoundaryPositions = [p for p in legalPositions if p[0] == width / 2]
      
    redEntrys, blueEntrys = [], []
    for redPos in redBoundaryPositions:
      for bluePos in blueBoundaryPositions:
        redX, redY = redPos
        blueX, blueY = bluePos
        if (redX + 1 == blueX) and (redY == blueY):
          redEntrys.append(redPos)
          blueEntrys.append(bluePos)
    
    return redEntrys if self.red else blueEntrys
    
class DefensiveReflexAgent(ReflexCaptureAgent):
  """
  A reflex agent that keeps its side Pacman-free. Again,
  this is to give you an idea of what a defensive agent
  could be like.  It is not the best or only way to make
  such an agent.
  """
  
  def getFeatures(self, gameState, action):
    features = util.Counter()
    successor = self.getSuccessor(gameState, action)

    currentState = gameState.getAgentState(self.index)
    successorState = successor.getAgentState(self.index)
    successorPosition = successorState.getPosition()
    
    currentCapsules = self.getCapsulesYouAreDefending(gameState)

    # Computes whether we're on defense (1) or offense (0)
    features['onDefense'] = 1
    if successorState.isPacman: features['onDefense'] = 0

    # Computes distance to invaders we can see
    enemies = [successor.getAgentState(i) for i in self.getOpponents(successor)]
    invaders = [a for a in enemies if a.isPacman and a.getPosition() != None]
    features['numInvaders'] = len(invaders)
        
    if len(invaders) == 0:
      features['runToMiddle'] = self.distanceToBoundary(successor)
    # Chase the invader
    elif len(invaders) > 0 and currentState.scaredTimer == 0:
      dists = [self.getMazeDistance(successorPosition, a.getPosition()) for a in invaders]
      features['invaderDistance'] = min(dists)
    # If scared, follow the Pac-man, but keep a safe distance
    elif len(invaders) > 0 and currentState.scaredTimer != 0:    
        dists = min([self.getMazeDistance(successorPosition, a.getPosition()) for a in invaders])
        features['follow'] = (dists - 2) ** 2
    
    # Ensure invaders don't get near capsules
    if len(invaders) > 0 and len(currentCapsules) > 0:         
        dists = [self.getMazeDistance(c, successorPosition) for c in currentCapsules]
        features['protectCapsules'] = min(dists)
    
    if action == Directions.STOP: features['stop'] = 1
    rev = Directions.REVERSE[gameState.getAgentState(self.index).configuration.direction]
    if action == rev: features['reverse'] = 1

    return features

  def getWeights(self, gameState, action):
    return {'numInvaders': -100, 'onDefense': 10, 'invaderDistance': -10, 'stop': -100, 'reverse': -2, 'runToMiddle': -2, 'distToMiddle': -3, 'follow': -100, 'protectCapsules': -3}

class MixReflexAgent(ReflexCaptureAgent):
  """
  A reflex agent that will go between offense and defense. 
  It will first be default to an offensive agent, but once it acquired enough pellets to keep a lead
  on the enemy team, then it will switch to a defensive agent.
  """
      
  def getFeaturesOffense(self, gameState, action):
    # Setup Variables
    features = util.Counter()
    successor = self.getSuccessor(gameState, action)
    foodList = self.getFood(successor).asList()
    currentPosition = gameState.getAgentState(self.index).getPosition()
    nextPosition = successor.getAgentState(self.index).getPosition()
    
    legalPositions = [p for p in gameState.getWalls().asList(False)]
    enemies = [successor.getAgentState(i) for i in self.getOpponents(successor)]
    ghosts = [a for a in enemies if not a.isPacman and a.getPosition() and manhattanDistance(currentPosition, a.getPosition()) <= 6]
    scaredGhosts = [a for a in ghosts if a.scaredTimer > 2]
    activeGhosts = [a for a in ghosts if a not in scaredGhosts]
    capsules = self.getCapsules(gameState)
    
    # Get the successor score
    features['successorScore'] = self.getScore(successor)
    
    # Make sure you have enough time to come back to home, before game ends
    if self.getTimeLeft(gameState) / 4 < self.distanceToBoundary(gameState) + 3:
        features['homeDist'] = self.distanceToBoundary(successor)
        return features
    
    # If you have secured the win condition, just return back home
    if len(foodList) < 3:
      features['returnWin'] = self.distanceToBoundary(successor)
    
    # Find next pellet that is not near a ghost
    if len(foodList) >= 3 and len(activeGhosts) == 0:
      features['foodDist'] = min(self.getMazeDistance(nextPosition, food) for food in foodList)
      if nextPosition in self.getFood(gameState).asList():
          features['foodDist'] = -1
    
    # Ghost is near by; it is best to return home
    if len(foodList) >= 3 and len(activeGhosts) > 0:
      ghost_dist = min(self.getMazeDistance(nextPosition, a.getPosition()) for a in activeGhosts)
      features['ghostDist'] = 99 - ghost_dist
      
      # Check if you are going to be walking into a ghost or not
      ghostPositions = [a.getPosition() for a in activeGhosts]
      nextGhostPositions = [getSuccessorsPositions(p, legalPositions) for p in ghostPositions]
      if nextPosition in ghostPositions or nextPosition in nextGhostPositions:
        features['death'] = 1
      
      features['returnWin'] = self.distanceToBoundary(successor)
    
    # Grab nearest capsule if there are nearby ghosts
    if len(ghosts) == 0:
      self.nextCapsule = None
    
    if len(activeGhosts) and len(capsules): # For active ghosts
        for capsule in capsules:
          distToCap = self.getMazeDistance(currentPosition, capsule)
          distOfGhostToCap = min(self.getMazeDistance(a.getPosition(), capsule) for a in activeGhosts)
          if distToCap < distOfGhostToCap:
              self.nextCapsule = capsule
    
    if len(scaredGhosts) and len(capsules): # For scared ghosts
        for capsule in capsules:
          distToCap = self.getMazeDistance(currentPosition, capsule)
          distOfGhostToCap = min(self.getMazeDistance(a.getPosition(), capsule) for a in scaredGhosts)
          ghostScaredTimer = scaredGhosts[0].scaredTimer
          if distToCap >= ghostScaredTimer and distToCap < distOfGhostToCap:
              self.nextCapsule = capsule
              
    if self.nextCapsule:
      features['capsuleDist'] = self.getMazeDistance(nextPosition, self.nextCapsule)
      if nextPosition == self.nextCapsule:
        self.nextCapsule = None
        
    # If there are no nearby active ghosts, leave capsules alone for now
    if len(activeGhosts) == 0 and nextPosition in capsules:
      features['noCapsule'] = 0.1
              
    # If there is a stand-off duel at the border, find a different entrance point into enemy territory
    isPacman = gameState.getAgentState(self.index).isPacman
    self.stuckMoves += ((not isPacman) and (self.stuckMoves != -1) and (len(activeGhosts) > 0))
    
    if self.stuckMoves > 5: # If you have been stuck for a while, manually find a different entry point
        self.stuckMoves, self.nextEntry = -1, random.choice(self.getEntrys(gameState))
    
    if self.nextEntry and not features['foodDist']: # Get the distance to the next entry point
        features['goToEntry'] = self.getMazeDistance(nextPosition, self.nextEntry)

    if isPacman or nextPosition == self.nextEntry: # You have successfully penetrated the enemy defenses. Reset your counters
        self.stuckMoves, self.nextEntry = 0, None
        
    # Ensure the offensive agent doesn't stop for whatever
    if action == Directions.STOP: features['stop'] = 1
    
    return features
  
  def getFeaturesDefense(self, gameState, action):
    features = util.Counter()
    successor = self.getSuccessor(gameState, action)

    currentState = gameState.getAgentState(self.index)
    successorState = successor.getAgentState(self.index)
    successorPosition = successorState.getPosition()
    
    currentCapsules = self.getCapsulesYouAreDefending(gameState)

    # Computes whether we're on defense (1) or offense (0)
    features['onDefense'] = 1
    if successorState.isPacman: features['onDefense'] = 0

    # Computes distance to invaders we can see
    enemies = [successor.getAgentState(i) for i in self.getOpponents(successor)]
    invaders = [a for a in enemies if a.isPacman and a.getPosition() != None]
    features['numInvaders'] = len(invaders)
        
    if len(invaders) == 0:
      features['runToMiddle'] = self.distanceToBoundary(successor)
    # Chase the invader
    elif len(invaders) > 0 and currentState.scaredTimer == 0:
      dists = [self.getMazeDistance(successorPosition, a.getPosition()) for a in invaders]
      features['invaderDistance'] = min(dists)
    # If scared, follow the Pac-man, but keep a safe distance
    elif len(invaders) > 0 and currentState.scaredTimer != 0:    
        dists = min([self.getMazeDistance(successorPosition, a.getPosition()) for a in invaders])
        features['follow'] = (dists - 2) ** 2
    
    # Ensure invaders don't get near capsules
    if len(invaders) > 0 and len(currentCapsules) > 0:         
        dists = [self.getMazeDistance(c, successorPosition) for c in currentCapsules]
        features['protectCapsules'] = min(dists)
    
    if action == Directions.STOP: features['stop'] = 1
    rev = Directions.REVERSE[gameState.getAgentState(self.index).configuration.direction]
    if action == rev: features['reverse'] = 1

    return features
  
  def getWeightsOffense(self):
    return {'successorScore': 1, 'foodDist': -2, 'ghostDist': -10, 'death': -2000, 'homeDist': -100, 'returnWin': -1, 'stop': -50, 'goToEntry': -1000, 'capsuleDist': -1200, 'noCapsule': -1}
  
  def getWeightsDefense(self):
    return {'numInvaders': -100, 'onDefense': 10, 'invaderDistance': -10, 'stop': -100, 'reverse': -2, 'runToMiddle': -2, 'distToMiddle': -3, 'follow': -100, 'protectCapsules': -3}
  
  def getFeatures(self, gameState, action):
    successor = self.getSuccessor(gameState, action)
    successorState = successor.getAgentState(self.index)
    isGhost = not successorState.isPacman
    return self.getFeaturesDefense(gameState, action) if self.getScore(gameState) > 0 else self.getFeaturesOffense(gameState, action)

  def getWeights(self, gameState, action):
    successor = self.getSuccessor(gameState, action)
    successorState = successor.getAgentState(self.index)
    isGhost = not successorState.isPacman
    return self.getWeightsDefense() if self.getScore(gameState) > 0 else self.getWeightsOffense()
