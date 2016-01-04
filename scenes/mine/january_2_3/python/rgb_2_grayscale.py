import Image

img = Image.open('/home/tushar/manta_mikey_1/manta/scenes/mine/january_2_3/templates/rbg/portrait_1.jpeg').convert('L')

img.save('/home/tushar/manta_mikey_1/manta/scenes/mine/january_2_3/templates/grayscale/portrait_1.png')
