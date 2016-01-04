import Image

img = Image.open('./star3.png').convert('L')

img.save('star5.png')
