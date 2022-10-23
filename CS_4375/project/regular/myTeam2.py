# myTeam.py
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


from captureAgents import CaptureAgent
import random, util
from game import Directions
from util import nearestPoint

#################
# Team creation #
#################

def createTeam(firstIndex, secondIndex, isRed,
               first='OffensiveReflexAgent', second='OffensiveReflexAgent'):
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

  # The following line is an example only; feel free to change it.
  return [eval(first)(firstIndex), eval(second)(secondIndex)]

class OffensiveReflexAgent(CaptureAgent):
  def registerInitialState(self, gameState):
    self.start, self.border = gameState.getAgentPosition(self.index), []
    CaptureAgent.registerInitialState(self, gameState)

    if self.red:
      self.b = (gameState.data.layout.width / 2) - 1
    else:
      self.b = gameState.data.layout.width / 2

    for i in range(gameState.data.layout.height):
      if not gameState.data.layout.walls[self.b][i]:
        self.border.append(((self.b), i))

  def chooseAction(self, gameState):
    actions = gameState.getLegalActions(self.index)
    vals = [self.evaluate(gameState, a) for a in actions]
    maxval = max(vals)
    bact = [a for a, v in zip(actions, vals) if v == maxval]
    return random.choice(bact)

  def getSuccessor(self, gameState, action):
    successor = gameState.generateSuccessor(self.index, action)
    pos = successor.getAgentState(self.index).getPosition()
    if pos != nearestPoint(pos):
      return successor.generateSuccessor(self.index, action)
    else:
      return successor

  def evaluate(self, gameState, action):
    return self.getFeatures(gameState, action) * self.getWeights(gameState, action)

  def getFeatures(self, gameState, action):
    feat, successor = util.Counter(), self.getSuccessor(gameState, action)
    foodList = self.getFood(successor).asList()
    feat['score'] = -len(foodList)
    teampos = successor.getAgentState(self.index).getPosition()
    minDistance = min([self.getMazeDistance(teampos, food) for food in foodList], 0)
    feat['fooddist'] = minDistance
    feat['stopping'] = 0
    if action == Directions.STOP:
      feat['stopping'] = 1
    rev = Directions.REVERSE[gameState.getAgentState(self.index).configuration.direction]
    feat['backwards'] = 0
    if action == rev:
      feat['backwards'] = 1
    feat['enear'], feat['ernear'] = 0, 0
    enemy = [successor.getAgentState(i) for i in self.getOpponents(successor)]
    threat = [a for a in enemy if not a.isPacman and a.getPosition() != None]
    if len(threat) > 0:
      distance = [self.getMazeDistance(teampos, a.getPosition()) for a in threat]
      feat['enemdist'] = min(distance)
      for dist in distance:
        if dist <= 1:
          feat['enear'] = 1
    cap = self.getCapsules(successor)
    if len(cap) > 0:
      capdistmin = min([self.getMazeDistance(teampos, cap) for cap in cap])
      if feat['enear'] == 0:
        feat['capdist'] = capdistmin
      else:
        feat['capdist'] = 5 * capdistmin
    disttoscore = min([self.getMazeDistance(teampos, scores) for scores in self.border])
    feat['scoredist'] = disttoscore

    return feat

  def getWeights(self, gameState, action):
    enemy = []
    threat = []
    scoredist = 0
    successor = self.getSuccessor(gameState, action)
    teampos = successor.getAgentState(self.index).getPosition()
    if gameState.getAgentState(self.index).numCarrying != 0:
      for i in self.getOpponents(successor):
        enemy = [successor.getAgentState(i)]
      for a in enemy:
        if not a.isPacman and a.getPosition() != None:
          threat = [a]
      if len(threat) > 0:
        close = min(
          [self.getMazeDistance(teampos, a.getPosition()) for a in threat])
        if close >= 10:
          scoredist = 0
        else:
          scoredist = -5 * gameState.getAgentState(self.index).numCarrying
    else:
      scoredist = 0

    return {'score': 100, 'fooddist': -2, 'enemdist': 1,
            'enear': -100, 'stopping': -300,
            'capdist': -2, 'ernear': -100, 'backwards': -20,
            'scoredist': scoredist}


class DefensiveReflexAgent(CaptureAgent):
  def registerInitialState(self, gameState):
    CaptureAgent.registerInitialState(self, gameState)

  def getSuccessor(self, gameState, action):
    successor = gameState.generateSuccessor(self.index, action)
    pos = successor.getAgentState(self.index).getPosition()
    if pos != nearestPoint(pos):
      return successor.generateSuccessor(self.index, action)
    else:
      return successor

  def getFeatures(self, gameState, action):
    features, successor = util.Counter(), self.getSuccessor(gameState, action)

    state = successor.getAgentState(self.index)
    pos = state.getPosition()
    features['onDefense'] = 1
    if state.isPacman:
      features['onDefense'] = 0
    enemies = [successor.getAgentState(i) for i in self.getOpponents(successor)]
    invaders = [a for a in enemies if a.isPacman and a.getPosition() != None]
    features['numInvaders'] = len(invaders)
    if len(invaders) > 0:
      dists = [self.getMazeDistance(pos, a.getPosition()) for a in invaders]
      features['invaderDistance'] = min(dists)

    if action == Directions.STOP:
      features['stop'] = 1
    rev = Directions.REVERSE[
      gameState.getAgentState(self.index).configuration.direction]
    if action == rev:
      features['reverse'] = 1

    return features

  def getWeights(self, gameState, action):
    return {'numInvaders': -1000, 'onDefense': 100, 'invaderDistance': -5,
            'stop': -100, 'reverse': -2}

  def evaluate(self, gameState, action):
    return self.getFeatures(gameState, action) * self.getWeights(gameState, action)

  def chooseAction(self, gameState):
    bestact = []
    actions = gameState.getLegalActions(self.index)
    val = [self.evaluate(gameState, a) for a in actions]
    maxval = max(val)
    for a, v in zip(actions,val):
      if v == maxval:
        bestact = [a]
    return random.choice(bestact)

