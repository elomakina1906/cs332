import tkinter as tk


class WinClass:
    def __init__(self, **kw):
        super().__init__(**kw)
        self.__parent = tk.Tk()
        self.__parent.title("RayTracer")
        self.__parent.resizable(False, False)
        self.__parent.mainloop()

