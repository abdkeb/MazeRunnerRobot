import MazeEnv
from AlgorithmAgents import *
import time
from IPython.display import clear_output
import defs
import TranslateToRobotActions
import tkinter as tk
import MazeBuilder
import gspread
from oauth2client.service_account import ServiceAccountCredentials


# S = start
# G = goal
# W = wall
# F = free to move

def convertTranslationToRobotFormat(robot_action_list):
    # maze_solution = "{ "
    # for turn, turn_cnt in robot_action_list:
    #     maze_solution += "{ " + f"{turn}, {turn_cnt}" + " },"
    # maze_solution += " }"
    maze_solution = []
    for turn, turn_cnt in robot_action_list:
        maze_solution.append([turn, turn_cnt])
    return maze_solution

def printSolution(actions, env: MazeEnv) -> None:
    env.reset()
    total_cost = 0
    print(env.render())
    print(f"Timestep: {1}")
    print(f"State: {env.get_state()}")
    print(f"Action: {None}")
    # print(f"Cost: {0}")
    time.sleep(1)
    for i, action in enumerate(actions):
        state, cost, terminated = env.step(action)
        total_cost += cost
        clear_output(wait=True)
        print(env.render())
        print(f"Timestep: {i + 2}")
        print(f"State: {state}")
        print(f"Action: {action}")
        # print(f"Cost: {cost}")
        print(f"Total cost: {total_cost}")
        time.sleep(1)
        if terminated is True:
            break

def buildMaze(size, print_maze=0):
    root_builder = tk.Tk()
    app = MazeBuilder.MazeBuilder(root_builder, rows_builder=size, cols_builder=size)  # Adjust the size here as needed
    root_builder.mainloop()
    if print_maze:
        env = MazeEnv(defs.boards['Map'])
        env.reset()
        print(env.render())
        clear_output(wait=True)

def solveMaze( print_solution=0):
    board = defs.boards['Map']
    env = MazeEnv(board)
    # try:
    WAstar_agent = WeightedAStarAgent()
    agent_action_list, total_cost, expanded = WAstar_agent.search(env, h_weight=0.5)

    robot_action_list = TranslateToRobotActions.TTRA(agent_action_list, board).translateSolution()
    maze_solution = convertTranslationToRobotFormat(robot_action_list)
    if print_solution:
        print(maze_solution)
        # printSolution(agent_action_list, env)
    return agent_action_list, robot_action_list, maze_solution

def transmitSolution(maze_solution):
    scope = ["https://spreadsheets.google.com/feeds", "https://www.googleapis.com/auth/drive"]
    creds = ServiceAccountCredentials.from_json_keyfile_name("JSONkey.json", scope)
    client = gspread.authorize(creds)
    sheet = client.open("Transmit" ).sheet1  # Open the first sheet
    # Clear the existing data in the sheet
    sheet.clear()
    # Insert the data into the sheet starting from the first row
    for row in maze_solution:
        sheet.append_row(row)

buildMaze(10,print_maze=1)
agent_actions, robot_actions, solution = solveMaze(print_solution=1)
transmitSolution(solution)
# print(solution)
