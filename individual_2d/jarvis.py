class Jarvis():
	def __init__(self, points, height):
		self.height = height
		self.points = points
		self.result = []


	def find_first(self):
		by_x = lambda x: x[0]
		by_y = lambda x: x[1]
		self.points = sorted(self.points, key = by_x)
		self.points = sorted(self.points, key = by_y, reverse = True)
		first = self.points[0]
		return first


	def scalar_mult(self, point):
		really_y = lambda y: self.height - y
		n = len(self.result)
		if n < 2:
			b = self.result[0]
			a = (b[0] - 1, b[1])
		else:
			a = self.result[n - 2]
			b = self.result[n - 1]
		vec_ab = (b[0] - a[0], really_y(b[1]) - really_y(a[1]))
		vec_bp = (point[0] - b[0], really_y(point[1]) - really_y(b[1]))
		res = vec_ab[0] * vec_bp[0] + vec_ab[1] * vec_bp[1]
		return res


	def id_next_point(self):
		n = len(self.points)
		idm, mc = 0, -1000 * 1000
		for i in range(0, n):
			c = self.scalar_mult(self.points[i])
			if c > mc:
				idm = i
				mc = c
		return idm


	def solver(self):
		first = self.find_first()
		self.result.append(first)
		while True:
			id = self.id_next_point()
			if id == 0:
				break
			else:
				self.result.append(self.points[id])
				del self.points[id]
		self.result.append(first)
		return self.result
