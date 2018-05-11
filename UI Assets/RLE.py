# This script converts given image into constant c array
# The format of the RLE is a 16-bit array
# The first 3 items are the width, height and number of (colour-number) pairs in the compressed image

from PIL import Image

# convert colour from 24 bit format to 8 bit format
def RGB888_to_RGB565(rgb_tuple):
	red = rgb_tuple[0]
	green = rgb_tuple[1]
	blue = rgb_tuple[2]

	rgb = ((red & 0b11111000) << 8) | ((green & 0b11111100) << 3) | (blue >> 3);
	return hex(rgb)

class ImageObject(object):

	def __init__(self, img_file):
		im = Image.open(img_file)
		self.width = im.size[0]
		self.height = im.size[1]
		self.rgb_im = im.convert('RGB')	# convert image to RGB
		self.image_array = self.convert_to_RGB565_array()		

	def get_pixel_value(self, x, y):
		r,g,b = self.rgb_im.getpixel((x,y))
		return (r,g,b)	

	def convert_to_RGB565_array(self):
		image_array = []
		for row in range(0, self.height):
			for col in range(0, self.width):			
				rgb = self.get_pixel_value(col, row)
				rgb565 = RGB888_to_RGB565(rgb)
				image_array.append(rgb565)
		return image_array

	def convert_to_RLE(self):
		RLE_array = []
		RLE_array.append(hex(self.width))
		RLE_array.append(hex(self.height))
		RLE_array.append(hex(0))	# Just a dummy value to hold the number of colour-number pairs

		colour_pair_array = []

		total_count = 0

		current_colour = self.image_array[0]
		count = 0
		for colour in self.image_array:
			if colour == current_colour:
				count += 1			
			else: 
				colour_pair_array.append([current_colour, count])
				total_count += count
				count = 1
				current_colour = colour

		colour_pair_array.append([current_colour, count])

		print total_count

		# update number of colour pairs
		number_of_colour_pairs = len(colour_pair_array)
		RLE_array[2] = hex(number_of_colour_pairs)

		for item in colour_pair_array:
			RLE_array.append(item[0])
			RLE_array.append(hex(item[1]))

		return RLE_array


def convert_to_1bit(imageObject, file_name):
	image_array1bit = []
	index = 0
	word = ""
	for pixel in imageObject.image_array:
		if pixel == "0x0":
			word+= "0"
		else:
			word+="1"

		index+=1
		if index >= 16:

			print word
			index = 0
			# appends the hex value
			image_array1bit.append(hex(int(word, 2)))
			word = ""


	with open("single_bit.txt", "a") as file:
		first_line = "const uint16_t " +  str(file_name[:-4]) + "_1bit[] = { "
		file.write(first_line)

		file.write(str(hex(imageObject.width)) + ", ")
		file.write(str(hex(imageObject.height)) + ", ")


		index = 1
		for item in image_array1bit:
			if index < len(image_array1bit):
				file.write(str(item) + ", ")
			else:
				file.write(str(item))
			index+=1

		file.write(" };\n")


#file_names = ["five_25.png", "six_25.png", "seven_25.png", "eight_25.png", "nine_25.png"]
file_names = ["one_25.png"]

for file in file_names:
	img = ImageObject(file)	
	convert_to_1bit(img, file)






