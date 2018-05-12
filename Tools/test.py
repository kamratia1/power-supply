def digits(n):
	ds = []
	while n > 0:
		ds.append(n % 10)
		n /= 10
		print ds
	return ds


print digits(1028)