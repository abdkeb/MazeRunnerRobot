import MazeEnv
from AlgorithmAgents import *
import time
from IPython.display import clear_output
import defs
import TranslateToRobotActions

# S = start
# G = goal
# W = wall
# F = free to move

boards = {
    "Map":
    ["SFFFWFFFFFWW",
     "WFWFFFWWWFFW",
     "WFWFWWWFWWWW",
     "FFWFFFWFFFFG",
     "WWWFWFFFWFWW",
     ]
}

def convertTranslationToRobotFormat(robot_action_list):
    solution = "{ "
    for turn, turn_cnt  in robot_action_list:
        solution += "{ "+ f"{turn}, {turn_cnt}" + " },"
    solution += " }"
    return solution

def printSolution(actions, env: MazeEnv) -> None:
    env.reset()
    total_cost = 0
    print(env.render())
    print(f"Timestep: {1}")
    print(f"State: {env.get_state()}")
    print(f"Action: {None}")
    print(f"Cost: {0}")
    time.sleep(1)

    for i, action in enumerate(actions):
        state, cost, terminated = env.step(action)
        total_cost += cost
        clear_output(wait=True)

        print(env.render())
        print(f"Timestep: {i + 2}")
        print(f"State: {state}")
        print(f"Action: {action}")
        print(f"Cost: {cost}")
        print(f"Total cost: {total_cost}")

        time.sleep(1)

        if terminated is True:
            break

def solveMaze(boards, print_solution=0):
    for i, board in enumerate(boards.values()):
        env = MazeEnv(board)
        # try:
        WAstar_agent = WeightedAStarAgent()
        agent_action_list, total_cost, expanded = WAstar_agent.search(env, h_weight=0.5)
        robot_action_list = TranslateToRobotActions.TTRA(agent_action_list, board).translateSolution()
        solution = convertTranslationToRobotFormat(robot_action_list)
        if print_solution:
            printSolution(agent_action_list, env)
        return agent_action_list, robot_action_list, solution



# opt_results = [235, 30, 52, 52, 76, 76, 87, 97, 106, 148]
# f = open("feedback.txt", "w")

# print("testing...")
agent_actions, robot_actions, solution = solveMaze(boards, print_solution=0)
print(solution)


