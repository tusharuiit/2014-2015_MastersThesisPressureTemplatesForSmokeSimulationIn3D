import Image

img = Image.open('/home/tushar/manta_mikey_1/manta/build/pictures/february_20/templates/1/phi_128.png').convert('L')

img.save('/home/tushar/manta_mikey_1/manta/build/pictures/february_20/templates/1/phi_128_grayscale.png')
