import tkinter as tk


class Main(tk.Frame):
	def __init__(self, parent):
		super().__init__()
		self.__parent = parent
		self.__ui_init()

	def __ui_init(self):
		self.__parent.title("Room")
