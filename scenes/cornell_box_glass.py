from scene_helper import *
import sys
print('=== Cornell Box Scene ===',file=sys.stderr)
w=800
h=800
obj=[]
light=[]
wall_mat=Material((1,1,1),(0.7,0.7,0.7),0.3)
blue_wall_mat=Material((0,0,0.85),(0,0,0.7),0.3)
red_wall_mat=Material((0.85,0,0),(0.7,0,0),0.3)
light_mat=Material((1,1,1),(0.7,0.7,0.7),0.3,(30,30,30))
glass_mat=SolidGlassMaterial(1.55,(0.7,0.7,0.7),(0.7,0.7,0.7))
ball_mat=Material((0.85,0.85,0.85),(0.9,0.9,0.9),0.05)
obj.append(Plane((1,0,0),-100,red_wall_mat))
obj.append(Plane((-1,0,0),-100,blue_wall_mat))
obj.append(Plane((0,1,0),-100,wall_mat))
obj.append(Plane((0,-1,0),-100,wall_mat))
obj.append(Plane((0,0,1),-100,wall_mat))
ParseObj("cube.obj",obj,lambda x:(x[0]*50,x[1]/50+100,x[2]*50-30),light_mat)
obj.append(Sphere((-100+40,-100+25,-60),25,ball_mat))
#ParseObj("cube.obj",obj,lambda x:(x[0]*25+100-60,x[1]*25-100+25,x[2]*25-35),glass_mat)
obj.append(Sphere((100-60,-100+25,0),25,ball_mat))
obj.append(Plane((0,0,-1),-30,glass_mat))
cam=PerspectiveCamera((0,0,379.54),(0,0,-1),(0,1,0),w,h,1286.1582)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)