from scene_helper import *
import sys
print('=== Classroom Scene ===',file=sys.stderr)
w=800
h=800
obj=[]
light=[]
black_mat=Material((0.1,0.1,0.1),(0,0,0),0.8)
wall_mat=Material((1,1,1),(0.04,0.04,0.04),0.3)
dim_light_mat=Material((1,1,1),(0.04,0.04,0.04),0.9,(30,30,25))
light_mat=Material((1,1,1),(0.04,0.04,0.04),0.9,(2,10,10))
wood_mat=Material((139/255.0,115/255.0,111/255.0),(0.04,0.04,0.04),0.2)
ParseObj("cube.obj",obj,lambda x:(x[0]/60-500,x[1]*200+30,x[2]*200+30),dim_light_mat)
obj.append(Plane((0,0,1),-100,wall_mat))
obj.append(Plane((0,1,0),-100,wood_mat))
ParseObj("cube.obj",obj,lambda x:(x[0]*82,x[1]*52+30,x[2]/60-100),black_mat)
ParseObj("cube.obj",obj,lambda x:(x[0]*80,x[1]*50+30,x[2]/50-100),light_mat)
def dz(d):
    ParseObj("cube.obj",obj,lambda x:(x[0]*70,x[1]*3-70,x[2]*20-30+d),wood_mat)
    ParseObj("cube.obj",obj,lambda x:(x[0]*5+65,x[1]*16-85,x[2]/60+20-30+d),wood_mat)
    ParseObj("cube.obj",obj,lambda x:(x[0]*5+65,x[1]*16-85,x[2]/60-20-30+d),wood_mat)
    ParseObj("cube.obj",obj,lambda x:(x[0]*5-65,x[1]*16-85,x[2]/60+20-30+d),wood_mat)
    ParseObj("cube.obj",obj,lambda x:(x[0]*5-65,x[1]*16-85,x[2]/60-20-30+d),wood_mat)
dz(0)
dz(60)
dz(60*2)
cam=PerspectiveCamera((33.3845,-17.6846,85.1639),(-0.1684,-0.4357,-0.8842),(-0.0319,0.8989,-0.4369),w,h,305.5742)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)