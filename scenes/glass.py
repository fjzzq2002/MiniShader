from scene_helper import *
import sys
print('=== Classroom Scene ===',file=sys.stderr)
w=800
h=800
obj=[]
light=[]
light_mat=Material((1,1,1),(0.7,0.7,0.7),0.3,(10,10,10))
ball_mat=Material((0.85,0.85,0.85),(0.9,0.9,0.9),0.05)
glass_mat=SolidGlassMaterial(1.55,(0.8,0.8,0.8),(0.8,0.8,0.8))
wall_mat=Material(("chessboard.jpg",(0.02,0.02)),(0.04,0.04,0.04),0.3)
obj.append(Plane((0,1,0),0,wall_mat))
obj.append(Sphere((1,1,0),1,light_mat))
obj.append(Sphere((9,3,3),2,glass_mat))
obj.append(Sphere((5,3,4),3,ball_mat))
cam=PerspectiveCamera((-2,1,0),(1,0,0),(0,1,0),w,h,300)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)