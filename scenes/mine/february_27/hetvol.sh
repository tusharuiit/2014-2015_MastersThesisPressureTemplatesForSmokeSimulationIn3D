#!/bin/bash

cd /home/tushar/manta_mikey_1/manta/build/pictures/february_27/5

montage /home/tushar/manta_mikey_1/manta/build/pictures/february_27/1/112_density.png /home/tushar/manta_mikey_1/manta/build/pictures/february_27/2/113_density.png /home/tushar/manta_mikey_1/manta/build/pictures/february_27/3/114_density.png /home/tushar/manta_mikey_1/manta/build/pictures/february_27/4/115_density.png -mode concatenate -tile 2x2  montage_array.png
