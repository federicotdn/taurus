import subprocess
from PIL import Image

eyes = ['open', 'closed']

for eye in eyes:
    im = Image.open(eye + '.png')
   
    im = im.resize((64, 64))
    im = im.transpose(Image.TRANSPOSE)
    pix = im.load()
    out = open('../eye-' + eye + '.cpp', 'w')

    outH = open('../include/eye-' + eye + '.h', 'w')
    outH.write('#ifndef ' + eye.upper() + '_H\n')
    outH.write('#define ' + eye.upper() + '_H\n\n')
    outH.write('#include <avr/pgmspace.h>\n\n')
    outH.write('extern const unsigned char ' + eye + '_eye[];\n\n')   
    outH.write('#endif')
    outH.close()
    
    print('size:', im.size)

    out.write('#include "include/eye-' + eye + '.h"\n\n')
    out.write('PROGMEM const unsigned char ' + eye + '_eye[] = {\n')

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

            if any(v < 255 for v in value):
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

