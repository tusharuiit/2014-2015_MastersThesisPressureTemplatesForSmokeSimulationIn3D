from PIL import Image
im = Image.open("./star12.jpg") 

pix = im.load()

x,y=im.size

#for i in range(x):
for j in range(1,y):
	print pix[int(x/2),j]

print " O LIFE "
#im.save("_1ANTIALIAS_reverse.png")
