from scene_helper import *
import sys
print('=== Colorful Box Scene ===',file=sys.stderr)
w=800
h=800
obj=[]
light=[]
wall_mat=Material((1,1,1),(0.7,0.7,0.7),0.3)
blue_wall_mat=Material((0,0,0.85),(0,0,0.7),0.3)
red_wall_mat=Material((0.85,0,0),(0.7,0,0),0.3)
red_light_mat=Material((1,1,1),(0.7,0.7,0.7),0.3,(10,3,0.1))
green_light_mat=Material((1,1,1),(0.7,0.7,0.7),0.3,(0.1,10,3))
blue_light_mat=Material((1,1,1),(0.7,0.7,0.7),0.3,(3,0.1,10))
ball_mat=Material((0.85,0.85,0.85),(0.9,0.9,0.9),0.05)
light_mat=Material((1,1,1),(0.7,0.7,0.7),0.3,(10,10,10))
obj.append(Plane((1,0,0),-100,wall_mat))
obj.append(Plane((-1,0,0),-100,wall_mat))
obj.append(Plane((0,1,0),-100,wall_mat))
obj.append(Triangle((-55,100,55),(0,100,0),(-55,100,-55),red_light_mat))
obj.append(Triangle((-55,100,55),(0,100,0),(55,100,55),green_light_mat))
obj.append(Triangle((55,100,-55),(0,100,0),(55,100,55),blue_light_mat))
obj.append(Sphere((-100+40,-100+25,-60),25,ball_mat))
cam=PerspectiveCamera((0,0,379.54),(0,0,-1),(0,1,0),w,h,1286.1582)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)