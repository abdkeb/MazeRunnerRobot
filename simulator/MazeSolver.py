import MazeEnv
from Algorithms import *
import time
from IPython.display import clear_output

# S = start
# G = goal
# W = wall
# F = free to move

boards = {
    "Map":
    ["WGFFFW",
     "WFWWWW",
     "WFFFFW",
     "WFWWSW",
     "WFFFWW",
     ]
}


def print_solution(actions, env: MazeEnv) -> None:
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

def SolveMaze():
    for i, board in enumerate(boards.values()):
        env = MazeEnv(board)
        # try:
        WAstar_agent = WeightedAStarAgent()
        actions, total_cost, expanded = WAstar_agent.search(env, h_weight=0.5)
        print_solution(actions, env)

            # assert total_cost <= opt_results[i], "incorrect solution cost"
        # except Exception as e:
        #     f.write("WAstar{}: FAIL - {}\n".format(i, e))
        # else:
        #     f.write("WAstar{}: PASS\n".format(i))


# opt_results = [235, 30, 52, 52, 76, 76, 87, 97, 106, 148]
# f = open("feedback.txt", "w")

print("testing...")
SolveMaze()
