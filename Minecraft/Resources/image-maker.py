from PIL import Image
import os

name = input("Enter image name: ")

scale = 1

image = Image.new("RGB", (48 * scale, 48 * scale), "white")

def compress(img, out, loc):
	xloc, yloc = loc
	w, h = img.size
	img = img.resize((w, w))
	interval = w / 16
	for x in range(16):
		for y in range(16):
			section = img.crop((x * interval, y * interval, (x + 1) * interval, (y + 1) * interval)).resize((1, 1)).resize((scale, scale))
			out.paste(section, ((xloc + x) * scale, (yloc + y) * scale, (xloc + x + 1) * scale, (yloc + y + 1) * scale))

base_paths = ['-bottom', '-front', '-left', '-top', '-right', '-back']
remaining = []
paths = []
for p in base_paths:
	if os.path.isfile(name + p + '.jpg'):
		paths.append(name + p + '.jpg')
	elif os.path.isfile(name + p + '.png'):
		paths.append(name + p + '.png')
	elif os.path.isfile(name + p + '.bmp'):
		paths.append(name + p + '.bmp')
	elif os.path.isfile(name + p + '.svg'):
		paths.append(name + p + '.svg')
	elif os.path.isfile(name + '-rest.jpg'):
		paths.append(name + '-rest.jpg')
	elif os.path.isfile(name + '-rest.png'):
		paths.append(name + '-rest.png')
	elif os.path.isfile(name + '-rest.bmp'):
		paths.append(name + '-rest.bmp')
	elif os.path.isfile(name + '-rest.svg'):
		paths.append(name + '-rest.svg')
	else:
		print('Error: image files missing')
		os._exit(0)

compress(Image.open(paths[0]), image, (0, 16))
compress(Image.open(paths[1]), image, (0, 32))
compress(Image.open(paths[2]), image, (16, 16))
compress(Image.open(paths[3]), image, (16, 32))
compress(Image.open(paths[4]), image, (32, 16))
compress(Image.open(paths[5]), image, (32, 32))

iter = 0
while os.path.isfile(str(iter) + '.png'):
	iter += 1

for p in list(set(paths)):
	os.remove(p)

image.save(str(iter) + ".png", "PNG")