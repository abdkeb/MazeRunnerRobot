import tkinter as tk
import defs
class MazeBuilder:
    def __init__(self, root_builder, rows_builder, cols_builder):
        self.root = root_builder
        self.rows = rows_builder
        self.cols = cols_builder
        self.grid = [['F' for _ in range(cols_builder)] for _ in range(rows_builder)]
        self.buttons = [[None for _ in range(cols_builder)] for _ in range(rows_builder)]
        self.selected_type = 'F'  # Default type

        self.create_grid()
        self.create_controls()

    def create_grid(self):
        for r in range(self.rows):
            for c in range(self.cols):
                btn = tk.Button(self.root, width=2, height=1, bg="white", command=lambda r=r, c=c: self.toggle_cell(r, c))
                btn.grid(row=r, column=c)
                self.buttons[r][c] = btn

    def create_controls(self):
        control_frame = tk.Frame(self.root)
        control_frame.grid(row=self.rows, column=0, columnspan=self.cols)

        btn_types = ['Start (S)', 'Goal (G)', 'Free (F)', 'Wall (W)']
        for i, btn_type in enumerate(btn_types):
            btn = tk.Button(control_frame, text=btn_type, command=lambda t=btn_type[0]: self.set_type(t))
            btn.grid(row=0, column=i)

        submit_button = tk.Button(control_frame, text="Submit", command=self.submit)
        submit_button.grid(row=0, column=len(btn_types))

    def set_type(self, t):
        self.selected_type = t

    def toggle_cell(self, r, c):
        self.grid[r][c] = self.selected_type
        color = {'S': 'green', 'G': 'red', 'F': 'white', 'W': 'black'}
        self.buttons[r][c].config(bg=color[self.selected_type])

    def submit(self):
        maze_as_strings = [''.join(row) for row in self.grid]
        maze_output = {
            "Map": maze_as_strings
        }
        defs.boards = maze_output
        self.root.quit()

# if __name__ == "__main__":
#     root_builder = tk.Tk()
#     app = MazeBuilder(root_builder, rows_builder=12, cols_builder=12)  # Adjust the size here as needed
#     root_builder.mainloop()
