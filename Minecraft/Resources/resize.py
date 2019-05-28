from PIL import Image
import os

for x in range(14):
	img = Image.open(str(x) + '.png')
	img = img.resize((48, 48))
	os.remove(str(x) + '.png')
	img.save(str(x) + '.png', "PNG")