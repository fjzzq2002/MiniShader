from scene_helper import *
import sys
from PIL import Image
import random
random.seed(12345)
print('=== Classroom Scene ===',file=sys.stderr)
w=1920
h=1080
obj=[]
light=[]
light.append(PointLight((5,100,5),(0.8,0.8,0.8)))
wall_mat=Material((0.9,0.9,0.9),(0.05,0.05,0.05),0.8)
for x in range(0,40):
    for y in range(0,40):
            he=-1+random.uniform(0,6)
            a=(x-20)*2
            b=(y-20)*2
            m=BRDFMaterial((random.uniform(0,1),random.uniform(0,1),random.uniform(0,1)),(random.uniform(0,1),random.uniform(0,1),random.uniform(0,1)),random.uniform(0,1))
            ParseObj("cube.obj",obj,lambda x:(x[0]+a,x[1]*100-100+he,x[2]+b),m)
obj.append(Plane((0,1,0),-2,wall_mat))

#{"camera_center":[0.30000001192092896,10,0.5],"camera_direction":[-0.062017467237646215,-0.8163994999131358,-0.5741478547481172],"camera_dist":200,"camera_height":800,"camera_horizontal":[0.998026797021407,-0.05638762239221356,-0.027624012409194196],"camera_length":20,"camera_radius":0.6000000238418579,"camera_type":1,"camera_up":[0.009822601838779858,0.5747280762311361,-0.8182855392009275],"camera_width":800,"client_speed":0.6,"client_width":800,"client_height":800,"client_sensitivity":50}


cam=ThinLensCamera((0.3,10,0.5),(-0.062017467237646215,-0.8163994999131358,-0.5741478547481172),(0.009822601838779858,0.5747280762311361,-0.8182855392009275),w,h,400,15,0.4)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)