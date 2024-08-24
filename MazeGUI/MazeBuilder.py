import tkinter as tk
import tkinter.messagebox as messagebox
import defs
from collections import deque

class MazeBuilder:
    def __init__(self, root_builder, rows_builder, cols_builder):
        self.root = root_builder
        self.rows = rows_builder
        self.cols = cols_builder
        self.grid = [['F' for _ in range(cols_builder)] for _ in range(rows_builder)]
        self.buttons = [[None for _ in range(cols_builder)] for _ in range(rows_builder)]
        self.selected_type = 'F'  # Default type
        self.start_position = None
        self.goal_position = None

        self.create_grid()
        self.create_controls()
        self.set_edge_walls()  # Set edges to be walls by default

    def create_grid(self):
        for r in range(self.rows):
            for c in range(self.cols):
                btn = tk.Button(self.root, width=2, height=1, bg="white",
                                command=lambda r=r, c=c: self.toggle_cell(r, c))
                btn.grid(row=r, column=c)
                self.buttons[r][c] = btn

    def create_controls(self):
        control_frame = tk.Frame(self.root)
        control_frame.grid(row=self.rows, column=0, columnspan=self.cols)

        # Buttons for selecting types of cells
        btn_types = ['Start (S)', 'Goal (G)', 'Free (F)', 'Wall (W)']
        for i, btn_type in enumerate(btn_types):
            btn = tk.Button(control_frame, text=btn_type, command=lambda t=btn_type[0]: self.set_type(t))
            btn.grid(row=0, column=i)

        # Submit and reset buttons
        submit_button = tk.Button(control_frame, text="Submit", command=self.submit)
        submit_button.grid(row=0, column=len(btn_types))

        reset_button = tk.Button(control_frame, text="Reset", command=self.reset_grid)
        reset_button.grid(row=0, column=len(btn_types) + 1)

        # Add buttons to increase/decrease grid size on a new line
        size_control_frame = tk.Frame(self.root)
        size_control_frame.grid(row=self.rows + 1, column=0, columnspan=self.cols)

        increase_button = tk.Button(size_control_frame, text="Increase Size", command=self.increase_size)
        increase_button.grid(row=0, column=0)

        decrease_button = tk.Button(size_control_frame, text="Decrease Size", command=self.decrease_size)
        decrease_button.grid(row=0, column=1)

    def set_edge_walls(self):
        """Set the edges of the grid to always be walls ('W')."""
        for r in range(self.rows):
            for c in range(self.cols):
                if r == 0 or r == self.rows - 1 or c == 0 or c == self.cols - 1:
                    self.grid[r][c] = 'W'
                    self.buttons[r][c].config(bg="black", state="disabled")  # Disable buttons on the edge
                else:
                    self.buttons[r][c].config(state="normal")  # Enable buttons inside the grid

    def set_type(self, t):
        self.selected_type = t

    def toggle_cell(self, r, c):
        if not (r == 0 or r == self.rows - 1 or c == 0 or c == self.cols - 1):  # Prevent changing edges
            # Update grid
            self.grid[r][c] = self.selected_type
            color = {'S': 'green', 'G': 'red', 'F': 'white', 'W': 'black'}
            self.buttons[r][c].config(bg=color[self.selected_type])

            # Update the start and goal positions
            if self.selected_type == 'S':
                self.update_start_position(r, c)
            elif self.selected_type == 'G':
                self.update_goal_position(r, c)

    def update_start_position(self, r, c):
        """Update the start position and remove any previous start."""
        if self.start_position:
            prev_r, prev_c = self.start_position
            self.grid[prev_r][prev_c] = 'F'
            self.buttons[prev_r][prev_c].config(bg='white')

        self.start_position = (r, c)

    def update_goal_position(self, r, c):
        """Update the goal position and remove any previous goal."""
        if self.goal_position:
            prev_r, prev_c = self.goal_position
            self.grid[prev_r][prev_c] = 'F'
            self.buttons[prev_r][prev_c].config(bg='white')

        self.goal_position = (r, c)

    def reset_grid(self):
        for r in range(self.rows):
            for c in range(self.cols):
                if not (r == 0 or r == self.rows - 1 or c == 0 or c == self.cols - 1):
                    self.grid[r][c] = 'F'
                    self.buttons[r][c].config(bg="white")
        self.start_position = None
        self.goal_position = None
        self.set_edge_walls()  # Ensure edges stay as walls after reset

    def count_walls_around(self, r, c):
        """Count the number of walls around a given cell."""
        wall_count = 0
        directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]  # Up, Down, Left, Right

        for dr, dc in directions:
            nr, nc = r + dr, c + dc
            if 0 <= nr < self.rows and 0 <= nc < self.cols and self.grid[nr][nc] == 'W':
                wall_count += 1

        return wall_count

    def validate_maze(self):
        """Validate that both the start and goal exist, have exactly 3 walls around them, and there is a solution."""
        if self.start_position is None or self.goal_position is None:
            return False, "There must be exactly one Start (S) and one Goal (G)!"

        start_walls = self.count_walls_around(*self.start_position)
        goal_walls = self.count_walls_around(*self.goal_position)

        if start_walls != 3:
            return False, "Start must be surrounded by exactly 3 walls!"

        if goal_walls != 3:
            return False, "Goal must be surrounded by exactly 3 walls!"

        if not self.is_solution():
            return False, "No valid path from Start to Goal!"

        if self.has_2x2_free_block():
            return False, "Path width shoud not be exactly one block, no 2x2 Free blocks allowed!"

        return True, ""

    def has_2x2_free_block(self):
        """Check if there is any 2x2 block of free cells ('F') in the maze."""
        for r in range(self.rows - 1):
            for c in range(self.cols - 1):
                if (self.grid[r][c] == 'F' and self.grid[r][c + 1] == 'F' and
                        self.grid[r + 1][c] == 'F' and self.grid[r + 1][c + 1] == 'F'):
                    return True
        return False

    # ... your other methods ...

    def submit(self):
        # Validate the maze
        is_valid, message = self.validate_maze()
        if not is_valid:
            # Show an error message if validation fails
            messagebox.showwarning("Invalid Maze", message)
            return

        # Proceed to submit if validation is successful
        maze_as_strings = [''.join(row) for row in self.grid]
        maze_output = {
            "Map": maze_as_strings
        }
        defs.boards = maze_output
        self.root.quit()

        return True, ""

    def is_solution(self):
        """Check if there is at least one solution from the Start to the Goal using BFS."""
        if self.start_position is None or self.goal_position is None:
            return False

        # Perform BFS from Start to Goal
        start_r, start_c = self.start_position
        goal_r, goal_c = self.goal_position

        queue = deque([(start_r, start_c)])
        visited = set()
        visited.add((start_r, start_c))

        directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]  # Up, Down, Left, Right

        while queue:
            r, c = queue.popleft()

            if (r, c) == (goal_r, goal_c):
                return True

            for dr, dc in directions:
                nr, nc = r + dr, c + dc

                if 0 <= nr < self.rows and 0 <= nc < self.cols and (nr, nc) not in visited:
                    if self.grid[nr][nc] in ('F', 'G'):  # Move through free space or goal
                        visited.add((nr, nc))
                        queue.append((nr, nc))

        return False

    def resize_grid(self, new_rows, new_cols):
        """Resize the grid, preserving as much of the existing maze as possible."""
        # Save the current grid's contents
        old_grid = self.grid
        old_buttons = self.buttons
        old_rows = self.rows
        old_cols = self.cols

        # Update the grid size
        self.rows = new_rows
        self.cols = new_cols

        # Create a new grid with the new size
        self.grid = [['F' for _ in range(new_cols)] for _ in range(new_rows)]
        self.buttons = [[None for _ in range(new_cols)] for _ in range(new_rows)]

        # Copy over the old grid's contents into the new grid within bounds
        min_rows = min(old_rows, new_rows)
        min_cols = min(old_cols, new_cols)
        # for r in range(min_rows):
        #     for c in range(min_cols):
        #         self.grid[r][c] = old_grid[r][c]

        # Clear the GUI grid and re-create buttons for the new grid size
        for widget in self.root.grid_slaves():
            widget.grid_forget()

        self.create_grid()
        self.create_controls()
        self.set_edge_walls()  # Ensure edges are properly set as walls

        # Reapply the button states and colors based on the new grid
        for r in range(min_rows):
            for c in range(min_cols):
                cell_type = self.grid[r][c]
                color = {'S': 'green', 'G': 'red', 'F': 'white', 'W': 'black'}
                self.buttons[r][c].config(bg=color[cell_type])

    def increase_size(self):
        """Increase grid size by 1 row and 1 column."""
        new_rows = self.rows + 1
        new_cols = self.cols + 1
        self.resize_grid(new_rows, new_cols)

    def decrease_size(self):
        """Decrease grid size by 1 row and 1 column, if possible."""
        if self.rows > 5 and self.cols > 5:  # Prevent reducing below 5x5 (or some minimum size)
            new_rows = self.rows - 1
            new_cols = self.cols - 1
            self.resize_grid(new_rows, new_cols)

    def submit(self):
        # Validate the maze
        is_valid, message = self.validate_maze()
        if not is_valid:
            # Show an error message if validation fails
            messagebox.showwarning("Invalid Maze", message)
            return

        # Proceed to submit if validation is successful
        maze_as_strings = [''.join(row) for row in self.grid]
        maze_output = {
            "Map": maze_as_strings
        }
        defs.boards = maze_output
        self.root.quit()

# if __name__ == "__main__":
#     root_builder = tk.Tk()
#     app = MazeBuilder(root_builder, rows_builder=12, cols_builder=12)  # Adjust the size here as needed
