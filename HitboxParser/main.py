from PIL import Image
from xml.dom import minidom
from xml.etree import ElementTree
import sys

class Rect:
    def __init__(self, x, y, w, h, t):
        self.x = x
        self.y = y
        self.w = w
        self.h = h
        self.t = t

    def contains(self, x, y, t):
        return self.x <= x and x < self.x + self.w and self.y <= y and y < self.y + self.h and self.t == t

def main():
    if len(sys.argv) <= 2:
        print("Use : main.py img nb_frames")
        return

    frames = int(sys.argv[2])
    if (frames <= 0):
        print("nb_frames is incorrect")
        return

    im = Image.open(sys.argv[1])
    width, height = im.size
    width = width // frames
    pix = im.load()

    root = ElementTree.Element("root")

    for f in range(frames):
        frameXML = ElementTree.SubElement(root, "frame_" + str(f))

        rectangles = []
        i = 0
        for y in range(height):
            for x in range(width):
                x_off = f * width
                #if not transparent
                if (pix[x + x_off, y][3] > 0):
                    color = pix[x + x_off, y]
                    t = ""
                    if (color[0] > 0): t = "hurt"       #red : atk box
                    elif (color[1] > 0): t = "hit"   #green : hp box

                    contained = False
                    for rect in rectangles:
                        if rect.contains(x, y, t):
                            contained = True
                            break

                    if contained: continue

                    i += 1
                    rectXML = ElementTree.SubElement(frameXML, "Rect_" + str(i))
                    rectXML.set("x", str(x))
                    rectXML.set("y", str(y))
                    w, h = 0, 0

                    for x2 in range(width - x):
                        if (pix[x + x_off + x2, y][3] > 0): continue
                        rectXML.set("width", str(x2))
                        w = x2
                        break

                    for y2 in range(height - y):
                        if (pix[x + x_off, y + y2][3] > 0): continue
                        rectXML.set("height", str(y2))
                        h = y2
                        break

                    rectXML.set("type", t)

                    rectangles.append(Rect(x, y, w, h, t))

    data = ElementTree.tostring(root, encoding='unicode', method='xml')
    path = sys.argv[1].split(".")[0]
    file = open(path + ".xml", "w")
    file.write(data)

main()
