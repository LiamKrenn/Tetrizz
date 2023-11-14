from PIL import Image

img = Image.open("images/background_flashing.gif")
img = img.convert("RGB")
pixels = img.load()

colors: dict = {}

output = []

for i in range(img.height):
    output.append([])
    for j in range(img.width):
        pixel = pixels[j, i]

        rgb = f"{pixel[0]};{pixel[1]};{pixel[2]}"
        if rgb not in colors.keys():
            colors[rgb] = input(f"\x1b[48;2;{rgb}m   \x1b[0m ").lower()
        output[i].append(colors[rgb])

compr = []
ff = open("assets/flashing.bin", "wb")
for line in output:
    compr.append(line[0])
    for c in line[1:]:
        if compr[-1][-1] == c and not (len(compr[-1]) >= 4 and compr[-1][-3] == "e"):
            compr[-1] += "e01"
        elif len(compr[-1]) >= 4 and compr[-1][-3] == "e" and compr[-1][-4] == c:
            count = int(compr[-1][-2:]) + 1
            if count < 100:
                compr[-1] = (
                    compr[-1][:-2] + ("0" if len(str(count)) == 1 else "") + str(count)
                )
            else:
                compr[-1] += c
        else:
            compr[-1] += c
    compr[-1] += "f"

joined = f"{img.width}f{img.height}f" + "".join(compr)
if len(joined) % 2 == 1:
    joined += "f"

ff.write(bytes.fromhex(joined))
