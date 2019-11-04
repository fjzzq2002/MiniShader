from scene_helper import *
import sys
print('=== Many Balls Scene ===',file=sys.stderr)
w=800
h=800
obj=[]
light=[]
mat=Material((1.0,0.76,0.33),(1,0.76,0.33),0.1)
for x in range(0,20):
    for y in range(0,20):
        for z in range(0,20):
            obj.append(Sphere((x/4.0+0.5/4.0,y/4.0+0.5/4.0,z/4.0+0.5/4.0),0.5/4.0,mat))
obj.append(Plane((0,1,0),10,Material((1,1,1),(0.7,0.7,0.7),0.3,(3,3,3))))
obj.append(Plane((0,1,0),0,Material((0,0,0),(0,0,0),0.7)))
cam=PerspectiveCamera((3.2042, 3.7431, 9.3591),(-0.2364, -0.2259, -0.945),(-0.116, 0.9722, -0.2034),w,h,828.2397)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)