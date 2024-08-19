from collections import deque
import os
import time
from typing import List
from typing import Tuple
from MazeEnv import MazeEnv
import heapdict
import defs

class GeneralAgent:
    def __init__(self):
        self.env = None
        self.goals = None
        self.failure = ([], -1, -1)

    def setup_heurisitic(self, env):
        self.env = env
        self.locate_goals()

    def locate_goals(self):
        self.goals = []
        for state in range(self.env.nrow * self.env.ncol):
            if self.env.is_final_state(state):
                self.goals.append(state)

    def calculate_sap_g_heuristic(self, state, goal):
        state_row, state_col = self.env.to_row_col(state)
        goal_row, goal_col = self.env.to_row_col(goal)
        md = abs(state_row - goal_row) + abs(state_col - goal_col)
        return md

    def calculate_msap_heuristic(self, state):
        if self.goals is None:
            self.locate_goals()
        min_sap = "inf"
        for goal in self.goals:
            if min_sap == "inf":
                min_sap = self.calculate_sap_g_heuristic(state, goal)
                continue
            new_sap = self.calculate_sap_g_heuristic(state, goal)
            if min_sap > new_sap:
                min_sap = new_sap
        return min_sap

    def calculate_weighted_f(self, state, path_to_state_cost, h_weight):
        return round((1 - h_weight) * path_to_state_cost + h_weight * self.calculate_msap_heuristic(state), 6)

    def calculate_f(self, state, path_to_state_cost):
        return path_to_state_cost + self.calculate_msap_heuristic(state)

class Node:
    def __init__(self, parent, state: int, cost: int, action: int) -> None:
        self.parent = parent # if parent is None, this is the root
        self.state = state # the new state after reaching the node
        self.cost = cost # the cost to reach the node from the previous node
        self.action = action # action is the action done from parent to reach self
        self.cost_from_root = parent.cost_from_root + self.cost if parent is not None else self.cost
        return

    def getPathFromRoot(self):
        path = deque()
        curr_node = self
        
        while curr_node.parent is not None:
            path.appendleft(curr_node.action)
            curr_node = curr_node.parent

        return list(path)

    def self_print(self):
        if self.parent is None:
            print(f"parent: None, state: {self.state}, cost: {self.cost}, action: {self.action}")
        else:
            print(f"parent: {self.parent.state}, state: {self.state}, cost: {self.cost}, action: {self.action}")

class UniqueStateNode(Node):
    def __init__(self, parent, state: int, cost: int, action: int):
        super().__init__(parent, state, cost, action)

    def __eq__(self, other):
        return self.state == other.state

    def __hash__(self):
        return hash(self.state)


class WeightedAStarAgent(GeneralAgent):
    def __init__(self) -> None:
        self.opened = None
        self.closed = None
        self.env = None
        # self.printer = Printer()
        super().__init__()

    def search(self, env: MazeEnv, h_weight) -> Tuple[List[int], int, float]:
        # Env
        self.env = env
        self.init_state = env.reset()
        self.opened = heapdict.heapdict()
        self.closed = set()
        node = UniqueStateNode(None, self.init_state, 0, -1)
        self.opened[node] = (0, 0)
        self.setup_heurisitic(env)

        while self.opened:
            node, _ = self.opened.popitem()

            if env.is_final_state(node.state):
                path = node.getPathFromRoot()
                return path, node.cost_from_root, len(self.closed)

            self.closed.add(node.state)
            for action, successor in env.succ(node.state).items():
                succ_state, succ_cost, _ = successor
                if not succ_state:
                    break
                child = UniqueStateNode(node, succ_state, succ_cost, action)

                new_weighted_cost = self.calculate_weighted_f(child.state, child.cost_from_root, h_weight)
                if child.state not in self.closed:
                    if child not in self.opened:
                        self.opened[child] = (new_weighted_cost, child.state)
                    elif self.opened[child] > (new_weighted_cost, child.state):
                        del self.opened[child]
                        self.opened[child] = (new_weighted_cost, child.state)
        return self.failure

