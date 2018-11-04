import tkinter as tk
from winclass import Main


def main():
	parent = tk.Tk()
	win = Main(parent)
	win.mainloop()


if __name__ == '__main__':
	main()
