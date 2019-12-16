from scene_helper import *
import sys
print('=== Classroom Scene ===',file=sys.stderr)
w=800
h=800
obj=[]
light=[]
black_mat=Material((0.1,0.1,0.1),(0,0,0),0.8)
dim_light_mat=Material((1,1,1),(0.04,0.04,0.04),0.9,(30,30,25))
wall_mat=Material((0,0,0),(0,0,0),0.3,("chessboard.jpg",(0.02,0.02)))
light_mat=Material((1,1,1),(0.04,0.04,0.04),0.9,(2,10,10))
#wood_mat=Material((139/255.0,115/255.0,111/255.0),(0.04,0.04,0.04),0.2)
obj.append(Plane((0,1,0),0,wall_mat))
cam=PerspectiveCamera((0,1,0),(1,0,0),(0,1,0),w,h,305)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)