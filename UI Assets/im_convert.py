# This script converts given image into constant c array
# The format of the RLE is a 16-bit array
# The first 3 items are the width, height and number of (colour-number) pairs in the compressed image

from PIL import Image

im = Image.open("V.png")
im_array = im.convert("RGB")

width = im.size[0]
height = im.size[1]


def get_pixel_value(image, x, y):
	r,g,b = image.getpixel((x,y))
	return (r,g,b)	

def convert_to_array(image):
	image_array = []
	height = image.size[1]
	width = image.size[0]
	for row in range(0, height):
		for col in range(0, width):			
			rgb = get_pixel_value(image, col, row)
			#rgb565 = RGB888_to_RGB565(rgb)
			image_array.append(rgb)
	return image_array


image_array = convert_to_array(im_array)

array = []
for pixel in image_array:
	array.append((0, pixel[1] ,0))

print array

img = Image.new('RGB', (width, height))
img.putdata(array)
img.save('V_green.png')
img.show()