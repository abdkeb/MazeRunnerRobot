from MazeEnv import MazeEnv
import defs
import numpy as np


class TTRA:

    def __init__(self, actions, board):
        self.env = MazeEnv(board)
        self.actions = actions
        self.visited_states = self.get_visited_states()
        return

    def get_visited_states(self):
        self.env.reset()
        to_return_states = []
        for action in self.actions:
            state = self.env.step(action)
            to_return_states.append(state)
        return to_return_states

    def get_successors(self, state, front):
        to_return = []
        successors = self.env.succ(state)
        for action, successor in successors.items():
            succ_state, succ_cost, _ = successor
            if not succ_state or succ_cost == np.inf:
                continue
            if succ_state == state + 1:
                to_return.append(defs.EAST)
            if succ_state == state - 1:
                to_return.append(defs.WEST)
            if succ_state > state + 1:
                to_return.append(defs.DOWN)
            if succ_state < state - 1:
                to_return.append(defs.UP)
        return to_return

    def get_left_right(self, action):
        right = (action - 1) % 4
        left = (action + 1) % 4
        return left, right

    def translateSolution(self):
        to_return_solution = []
        action_index = 0
        r_cnt = 0
        l_cnt = 0

        for next_state in self.visited_states:
            if self.env.is_final_state(next_state[0]):
                break
            front = self.actions[action_index]
            left, right = self.get_left_right(front)
            successors = self.get_successors(next_state[0], front)
            if left in successors:
                l_cnt += 1
            if right in successors:
                r_cnt += 1

            if left == self.actions[action_index + 1]:
                to_return_solution.append(["left", l_cnt])
                r_cnt = 0
                l_cnt = 0
            if right == self.actions[action_index + 1]:
                to_return_solution.append(["right", r_cnt])
                r_cnt = 0
                l_cnt = 0
            action_index += 1
        return to_return_solution

        # return to_return_solution
