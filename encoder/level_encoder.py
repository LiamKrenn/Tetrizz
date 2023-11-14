from PIL import Image
import rgb2short

img = Image.open("images/level_colors.png")
img = img.convert("RGB")
pixels = img.load()

colors: dict = {}

output = []

for i in range(26):
    for j in range(10):
        color1r, color1g, color1b = pixels[j*24+1, i*8]
        color2r, color2g, color2b = pixels[j*24+9, i*8]
        color3r, color3g, color3b = pixels[j*24,i*8]
        
        color1 = int(rgb2short.rgb2short("{:02x}{:02x}{:02x}".format(*pixels[j*24+1, i*8]))[0])
        color2 = int(rgb2short.rgb2short("{:02x}{:02x}{:02x}".format(*pixels[j*24+9, i*8]))[0])
        color3 = int(rgb2short.rgb2short("{:02x}{:02x}{:02x}".format(*pixels[j*24, i*8]))[0])
        if color1 == 16 and color2 == 16 and color3 == 16: continue
        
        output.append([0, color1, color2, color3])

ff = open("assets/level_colors.txt", "w")
for color in output:        
  ff.write(";".join([str(c) for c in color])+"\n")
