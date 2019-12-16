from scene_helper import *
import sys
print('=== Inside Test Scene ===',file=sys.stderr)
w=800
h=800
obj=[]
light=[]
light_mat=Material((1,1,1),(0.1,0.1,0.1),0.8,(0.4,0.4,0.4))
wall_mat=Material((1,1,1),(0.9,0.9,0.9),0.8)
obj.append(Sphere((0,0,0),20,light_mat))
obj.append(Sphere((0,0,0),5,wall_mat))
cam=PerspectiveCamera((0,-8.8,-0.3),(0,1,0),(1,0,0),w,h,300)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)
