from scene_helper import *
import sys
print('=== Simple Box Scene ===',file=sys.stderr)
w=800
h=800
obj=[]
light=[]
mat=Material((0.85,0.85,0.85),(0.3,0.3,0.3),0.8)
obj.append(Plane((1,0,0),-2.5,Material((1,1,1),(1,1,1),0.05)))
#obj.append(Sphere((3,1,3),1,Material((0,0.7,0.9),(0,0,0),0.8)))
obj.append(Sphere((3,1,3),1,Material((0,0,0),(0,0.7,0.9),0.8)))
light.append(PointLight((5,6,5),(1,1,1)))
cam=PerspectiveCamera((3.2042, 3.7431, 9.3591),(-0.2364, -0.2259, -0.945),(-0.116, 0.9722, -0.2034),w,h,828.2397)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)