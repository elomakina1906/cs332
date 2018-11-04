import tkinter as tk
from jarvis import Jarvis

WIDTH = 500
HEIGHT = 300


class Main(tk.Frame):
	def __init__(self, parent):
		super().__init__()
		self.parent = parent
		self.init_ui()


	def init_ui(self):
		self.parent.title("Jarvis")
		self.points = []
		self.vertexes = []

		self.doit = tk.Button(text="Jarvis")
		self.doit.bind("<Button-1>", self.do_js)
		self.doit.grid(row=0, column=0)
		
		self.clear = tk.Button(text="Clear")
		self.clear.bind("<Button-1>", self.clear_canvas)
		self.clear.grid(row=0, column=1)

		self.canvas = tk.Canvas(width=WIDTH, height=HEIGHT, bg='white')
		self.canvas.bind("<Button-1>", self.add_point)
		self.canvas.grid(row=1, column=0, columnspan=2)
		

	def draw_points(self):
		for point in self.points:
			x1, y1 = point[0] - 1, point[1] - 1
			x2, y2 = point[0] + 1, point[1] + 1
			self.canvas.create_oval(x1, y1, x2, y2)


	def draw_lines(self):
		self.canvas.create_line(self.vertexes)


	def add_point(self, event):
		self.points.append((event.x, event.y))
		self.draw_points()


	def do_js(self, event):
		if len(self.points) < 3:
			return
		js = Jarvis(self.points)
		self.vertexes = js.solver()
		self.draw_lines()
		print(self.vertexes)


	def clear_canvas(self, event):
		self.points.clear()
		self.vertexes.clear()
		self.canvas.delete("all")
