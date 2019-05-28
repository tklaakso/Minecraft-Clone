from PIL import Image
import os

image_size = 48
atlas_width = 10
atlas_height = 10

images = []

iter = 0
while os.path.isfile(str(iter) + '.png'):
	images.append(Image.open(str(iter) + '.png'))
	iter += 1

atlas = Image.new("RGB", (image_size * atlas_width, image_size * atlas_height), "white")

x = 0
y = 0
for img in images:
	atlas.paste(img, (x * image_size, y * image_size, (x + 1) * image_size, (y + 1) * image_size))
	x += 1
	if x % atlas_width == 0:
		y += 1
		x = 0

atlas.save('atlas.png', "PNG")