from RLE import ImageObject


img = ImageObject("V_18.png")	
RLE = img.convert_to_RLE()

with open("image.txt", "w") as file:
	first_line = "const uint16_t V_18[] = { "
	file.write(first_line)

	index = 1
	for item in RLE:
		if index < len(RLE):
			file.write(str(item) + ", ")
		else:
			file.write(str(item))
		index+=1

	file.write(" };")

