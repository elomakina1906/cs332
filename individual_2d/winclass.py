import tkinter as tk
from jarvis import Jarvis

WIDTH = 1000
HEIGHT = 500


class Main(tk.Frame):
	def __init__(self, parent):
		super().__init__()
		self.parent = parent
		self.__init_ui()


	def __init_ui(self):
		self.parent.title("Jarvis")
		self.points = []
		self.vertexes = []

		self.doit = tk.Button(text="Jarvis")
		self.doit.bind("<Button-1>", self.__do_js)
		self.doit.grid(row=1, column=0)
		
		self.clear = tk.Button(text="Clear")
		self.clear.bind("<Button-1>", self.__clear_canvas)
		self.clear.grid(row=1, column=1)

		self.canvas = tk.Canvas(width=WIDTH, height=HEIGHT, bg='white')
		self.canvas.bind("<Button-1>", self.__add_point)
		self.canvas.grid(row=0, column=0, columnspan=2)
		

	def __draw_points(self):
		for point in self.points:
			x1, y1 = point[0] - 1, point[1] - 1
			x2, y2 = point[0] + 1, point[1] + 1
			self.canvas.create_oval(x1, y1, x2, y2)


	def __draw_lines(self):
		self.canvas.create_line(self.vertexes)


	def __add_point(self, event):
		self.points.append((event.x, event.y))
		self.__draw_points()


	def __do_js(self, event):
		self.vertexes.clear()
		self.canvas.delete("all")
		self.__draw_points()
		if len(self.points) < 3:
			return
		js = Jarvis(self.points, self.canvas.winfo_height())
		self.vertexes = js.solver()
		self.__draw_lines()


	def __clear_canvas(self, event):
		self.points.clear()
		self.vertexes.clear()
		self.canvas.delete("all")
