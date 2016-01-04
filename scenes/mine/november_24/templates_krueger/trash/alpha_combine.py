from PIL import Image
im = Image.open("./star8.png") 

pix = im.load()

x,y=im.size

#for i in range(x):
#	for j in range(1,y):
#		val = pix[i,j][0]
#		alpha = pix[i,j][1]
#		pix[i,j] = (  int(((float(alpha))/(255*1.0))*(float(val)))  ,  alpha  )

extreme_values = im.getextrema()
#max_pixel_value = extreme_values[1]
#min_pixel_value = extreme_values[0]

print "extreme_values = "
print extreme_values[0][1]

print " O LIFE "
im.save("star8.png")
