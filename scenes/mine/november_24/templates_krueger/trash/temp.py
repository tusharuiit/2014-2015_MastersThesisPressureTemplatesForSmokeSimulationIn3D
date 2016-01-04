import Image
import os

im = Image.open("/home/tushar/manta_mikey_1/manta/scenes/mine/november_3/templates_krueger/trash/star4.png")
#Can be many different formats.
extreme_values = im.getextrema()
max_pixel_value = extreme_values[1]
min_pixel_value = extreme_values[0]

print extreme_values
print max_pixel_value
print min_pixel_value
