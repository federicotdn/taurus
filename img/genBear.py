import subprocess
from PIL import Image

im = Image.open('white_bear.png')

im = im.resize((64, 64))
im = im.transpose(Image.TRANSPOSE)
pix = im.load()
out = open('../white-bear.cpp', 'w')

outH = open('../include/white-bear.h', 'w')
outH.write('#ifndef WHITE_BEAR_H\n')
outH.write('#define WHITE_BEAR_H\n\n')
outH.write('#include <avr/pgmspace.h>\n\n')
outH.write('extern const unsigned char white_bear[];\n\n')   
outH.write('#endif')
outH.close()

print('size:', im.size)

out.write('#include "include/white-bear.h"\n\n')
out.write('PROGMEM const unsigned char white_bear[] = {\n')

out.write(hex(im.size[0]) + ', ')
out.write(hex(im.size[1]) + '\n')

VALS_PER_LINE = 15
current = 0
count = 0
writes = 0

for x in range(im.size[0]):
    for y in range(im.size[1]):
        current <<= 1
        value = pix[x, y]

        if value[3]:
            current += 1
        
        count += 1

        if count == 8:
            out.write(', ' + hex(current))
            writes += 1
            count = 0
            current = 0

            if writes % VALS_PER_LINE == 0:
                out.write('\n')

print('Wrote', writes, 'times. Pixels:', writes * 8)

out.write('\n};\n')

out.close()

