from scene_helper import *
import sys
from PIL import Image
import random
print('=== Classroom Scene ===',file=sys.stderr)
w=800
h=800
img = Image.open("alg.bmp")
obj=[]
light=[]
#light.append(PointLight((0,300,0),(3,3,3)))
wall_mat=Material((0.9,0.9,0.9),(0.05,0.05,0.05),0.8)
for x in range(0,150):
    for y in range(0,150):
        if img.getpixel((x,y))==0:
            he=-1+random.uniform(0,10)
            a=(x-75)*2
            b=(y-75)*2
            m=BRDFMaterial((random.uniform(0,1),random.uniform(0,1),random.uniform(0,1)),(random.uniform(0,1),random.uniform(0,1),random.uniform(0,1)),random.uniform(0,1),(random.uniform(0,0.7),random.uniform(0,0.7),random.uniform(0,0.7)))
            ParseObj("cube.obj",obj,lambda x:(x[0]+a,x[1]*100-100+he,x[2]+b),m)
#{"camera_center":[0,100,0],"camera_direction":[-0.1719117262213672,-0.9851098721565386,0.0022131803596698776],"camera_dist":305,"camera_height":800,"camera_horizontal":[0.01569956669891066,-0.00049337875984531,0.9998766324816564],"camera_type":0,"camera_up":[0.9849872496601473,-0.17192526387100296,-0.015550615737963111],"camera_width":800,"client_speed":0.6,"client_width":800,"client_height":800,"client_sensitivity":50}
obj.append(Plane((0,1,0),-2,wall_mat))
cam=PerspectiveCamera((0,100,0),(0,-1,0),(0,0,-1),w,h,200)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)