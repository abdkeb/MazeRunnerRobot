


class Block:
    def __init__(self, right=False, left=False, top=False, bottom=False):
        self.top = top
        self.right = right
        self.bottom = bottom
        self.left = left


class Matrix:

    def __init__(self, size):
        self.size = size
        self.grid = [[Block() for _ in range(size)] for _ in range(size)]
        self.empty_block = Block()

    def set_block(self, row, col, right, left, top, bottom):
        self.grid[row][col] = Block(right, left, top, bottom)


    def print_matrix(self):
        to_print = ""
        for col in range(self.size):
            if self.grid[0][col].top:
                to_print += " _"
            else:
                to_print += "  "
        to_print += "\n"

        for row in range(self.size):
            if self.grid[row][0].left:
                to_print += "|"
            else:
                to_print += " "
            for col in range(self.size):
                block = self.grid[row][col]

                if col == self.size-1:
                    right_block = self.empty_block
                else:
                    right_block = self.grid[row][col + 1]

                if row == self.size-1:
                    bottom_block = self.empty_block
                else:
                    bottom_block = self.grid[row + 1][col]

                if block.bottom or bottom_block.top:
                    to_print += "_"
                else:
                    to_print += " "

                if block.right or right_block.left:
                    to_print += "|"
                else:
                    to_print += " "

            to_print += "\n"



                # if row == self.size - 1:

        for row in self.grid:
            for block in row:
                print(f'[{int(block.right)}{int(block.left)}{int(block.top)}{int(block.bottom)}]', end=' ')
            print()

        print(to_print)


class Robot:

    def __init__(self, algorithm, row=0, col=0):
        self.algorithm = algorithm
        self.row = row
        self.col = col

    def step(self, right, left, front):
        action = self.algorithm.step(right, left, front)
        return action
"""
Psudoo code for simulatiion
"""
#
# class Simulation:
#     def __init__(self, matrix, Robot):
#         pass
#
#     def run(self):
#         while not finish:
#             list = measurment()
#             action = Robot.step(list)
#
#
#     def doAction(self, action):
#

# sim = Simulation()


# Example usage
maze = Matrix(5)
maze.set_block(0, 0, 1, 1, 0, 0)
maze.set_block(1, 0, 0, 1, 0, 1)
maze.set_block(1, 1, 1, 0, 1, 0)
maze.set_block(2, 1, 0, 1, 0, 1)
maze.set_block(2, 2, 1, 0, 1, 0)
maze.set_block(3, 2, 0, 1, 0, 1)
maze.set_block(3, 3, 1, 0, 1, 0)
maze.set_block(4, 3, 0, 1, 0, 1)
maze.set_block(4, 4, 1, 0, 1, 0)

maze.set_block(0, 1, 1, 1, 1, 1)
maze.set_block(0, 2, 1, 1, 1, 1)
maze.set_block(0, 3, 1, 1, 1, 1)
maze.set_block(0, 4, 1, 1, 1, 1)







maze.print_matrix()
