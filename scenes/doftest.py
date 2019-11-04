from scene_helper import *
import sys
print('=== DOF Test Scene ===',file=sys.stderr)
w=800
h=800
obj=[]
light=[]
wall_mat=Material((1,1,1),(1,1,1),0.5)
steel_mat=Material((0.85,0.85,0.85),(0.9,0.9,0.9),0.05)
gold_mat=Material((1.0,0.76,0.33),(1,0.76,0.33),0.1)
obj.append(Plane((0,1,0),-100,wall_mat))
obj.append(Plane((0,0,1),-100,wall_mat))
ParseObj("bunny_1k.obj",obj,lambda x:(x[0]*130-100+40,x[1]*130-100-10,x[2]*130-60),steel_mat)
#obj.append(Sphere((-100+40,-100+25,-60),25,steel_mat))
obj.append(Sphere((100-60,-100+20,70),20,gold_mat))
light.append(PointLight((200,300,400),(0.7,0.5,0.3)))
cam=ThinLensCamera((21,-46,175),(0,-0.2181432413965348,-0.9759167619387497),(0,0.9759167619387473,-0.21814324139654206),w,h,786,52,1.3)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)
