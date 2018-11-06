import tkinter as tk
from jarvis import Jarvis

WIDTH = 1000
HEIGHT = 500


class Main(tk.Frame):
	def __init__(self, parent):
		super().__init__()
		self.__parent = parent
		self.__init_ui()

	def __init_ui(self):
		self.__parent.title("Jarvis")
		self.__points = []
		self.__vertexes = []

		self.__doit = tk.Button(text="Jarvis")
		self.__doit.bind("<Button-1>", self.__do_js)
		self.__doit.grid(row=1, column=0)
		
		self.__clear = tk.Button(text="Clear")
		self.__clear.bind("<Button-1>", self.__clear_canvas)
		self.__clear.grid(row=1, column=1)

		self.__canvas = tk.Canvas(width=WIDTH, height=HEIGHT, bg='white')
		self.__canvas.bind("<Button-1>", self.__add_point)
		self.__canvas.grid(row=0, column=0, columnspan=2)		

	def __draw_points(self):
		for point in self.__points:
			x1, y1 = point[0] - 1, point[1] - 1
			x2, y2 = point[0] + 1, point[1] + 1
			self.__canvas.create_oval(x1, y1, x2, y2)

	def __draw_lines(self):
		self.__canvas.create_line(self.__vertexes)

	def __add_point(self, event):
		self.__points.append((event.x, event.y))
		self.__draw_points()

	def __do_js(self, event):
		self.__vertexes.clear()
		self.__canvas.delete("all")
		self.__draw_points()
		if len(self.__points) < 3:
			return
		js = Jarvis(self.__points, self.__canvas.winfo_height())
		self.__vertexes = js.solver()
		self.__draw_lines()

	def __clear_canvas(self, event):
		self.__points.clear()
		self.__vertexes.clear()
		self.__canvas.delete("all")
