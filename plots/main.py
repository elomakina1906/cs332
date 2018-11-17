import tkinter as tk
from winclass import MainWindow


def main():
	parent = tk.Tk()
	wi = MainWindow(parent)
	wi.mainloop()


if __name__ == '__main__':
	main()
