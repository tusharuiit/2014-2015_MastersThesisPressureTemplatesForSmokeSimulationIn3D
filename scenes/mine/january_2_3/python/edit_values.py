import Image
im = Image.open("/home/tushar/manta_mikey_1/manta/scenes/mine/january_2_3/templates/grayscale/two-hollow-hearts_0_1.png")

pixels = im.load() # create the pixel map

for i in range(im.size[0]):    # for every pixel:
	for j in range(im.size[1]):
		if(pixels[i,j] == 0):
			pixels[i,j] = 255
		else:
			pixels[i,j] = 0

im.save('/home/tushar/manta_mikey_1/manta/scenes/mine/january_2_3/templates/grayscale/two-hollow-hearts_1_0.png')

#im.show()
