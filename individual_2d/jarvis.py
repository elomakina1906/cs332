from math import acos, sqrt


class Jarvis():
	def __init__(self, points, height):
		self.height = height
		self.points = points
		self.result = []


	def __find_first(self):
		by_x = lambda x: x[0]
		by_y = lambda x: x[1]
		self.points = sorted(self.points, key = by_x)
		self.points = sorted(self.points, key = by_y, reverse = True)
		first = self.points[0]
		return first


	def __line_lenght(self, a, b):
		really_y = lambda y: self.height - y
		d_x = b[0] - a[0]
		d_y = really_y(b[1]) - really_y(a[1])
		return sqrt(d_x * d_x + d_y * d_y) 


	def __angle_between_lines(self, p):
		n = len(self.result)
		if n < 2:
			b = self.result[0]
			a = (b[0] - 1, b[1])
		else:
			a = self.result[n - 2]
			b = self.result[n - 1]
		ab = self.__line_lenght(a, b)
		bp = self.__line_lenght(b, p)
		ap = self.__line_lenght(a, p)
		cos_value = (bp * bp + ab * ab - ap * ap) / (2 * ab * bp)
		return acos(cos_value)


	def __id_next_point(self):
		n = len(self.points)
		l = len(self.result)
		idm, mc = 0, -180
		for i in range(0, n):
			if (self.result[l-1] != self.points[i]):
				c = self.__angle_between_lines(self.points[i])
				if c > mc:
					idm = i
					mc = c
		return idm


	def solver(self):
		first = self.__find_first()
		self.result.append(first)
		while True:
			id = self.__id_next_point()
			if id == 0:
				break
			else:
				self.result.append(self.points[id])
				del self.points[id]
		self.result.append(first)
		return self.result
