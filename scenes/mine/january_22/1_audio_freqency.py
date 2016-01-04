import numpy
import scipy
import scipy.fftpack

f = open("/home/tushar/manta_mikey_1/manta/scenes/mine/january_22/audio.dat")
d = f.readlines()

#Take the first slice from 3 to 1668
#chunk = d[166*audio_part_index+2:166*(audio_part_index+1)+2]
for j in range(300):

	chunk = d[1666*j+2:1666*(j+1)+2]
	chunk_left = []
	chunk_right = []

	for i in chunk:
		chunk_left.append(float(i.split()[0]))
		chunk_right.append(float(i.split()[1]))

	FFT = abs(scipy.fft(chunk_right))
	freqs = scipy.fftpack.fftfreq(1666, chunk_left[1]-chunk_left[0])

	print FFT.max()
	print freqs[FFT.argmax()]
	print
