import numpy
import scipy
import scipy.fftpack

f = open("/home/tushar/temporary/audio/audacity/beethoven10_mono.dat")
d = f.readlines()

#Take the first slice from 3 to 168
#for audio_part_index in range(300):
#chunk = d[166*audio_part_index+2:166*(audio_part_index+1)+2]
big_chunk = []
for j in range(3000):

	chunk = d[147*j+2:147*(j+1)+2]
	chunk_left = []
	chunk_right = []

	for i in chunk:
		chunk_left.append(float(i.split()[0]))
		chunk_right.append(float(i.split()[1]))

	FFT = abs(scipy.fft(chunk_right))
	freqs = scipy.fftpack.fftfreq(147, chunk_left[1]-chunk_left[0])
	freqs_abs = abs(freqs)

#print "freqs = ", freqs
#print "---------------------------------------------------------------------------------------"
#print "freqs_abs = ", freqs_abs

#	print "freqs_abs.max() = ", freqs_abs.max()
#	print "freqs_abs.min() = ", freqs_abs.min()
#	print "FFT.max() = ", FFT.max()
	print "freqs[FFT.argmax()] = ", freqs[FFT.argmax()]
	big_chunk.append(freqs_abs[FFT.argmax()])
#	print

print " max = " , max(big_chunk)
print " min = " , min(big_chunk)
