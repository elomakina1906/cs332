class Jarvis():
	def __init__(self, points):
		self.points = points
		self.result = []


	def find_first_second(self):
		first = (-1, -1)
		second = (-1, -1)
		by_x = lambda x: x[0]
		by_y = lambda x: x[1]
		self.points = sorted(self.points, key = by_x)
		self.points = sorted(self.points, key = by_y, reverse = True)
		first = self.points[0]
		second = self.points[1]
		return first, second


	def scalar_mult(self, point):
		if len(self.result) < 2:
			return 100000	
		n = len(self.result)
		a = self.result[n-2]
		b = self.result[n-1]
		vec_a = (b[0] - a[0], b[1] - a[1])
		vec_b = (point[0] - b[0], point[1] - b[1])
		return vec_a[0] * vec_b[0] + vec_a[1] * vec_b[1]


	def id_next_point(self):
		n = len(self.points)
		idm, mc = 0, 2
		for i in range(1, n):
			c = self.scalar_mult(self.points[i])
			if c < mc:
				idm = i
				mc = c
		
		return idm


	def solver(self):
		first, second = self.find_first_second()
		self.result.append(first)
		self.result.append(second)
		while True:
			id = self.id_next_point()
			if id == 0:
				break
			else:
				self.result.append(self.points[id])
				del self.points[id]
		self.result.append(first)

		return self.result
