from scene_helper import *
import sys
print('=== Rabbit Test Scene ===',file=sys.stderr)
w=800
h=800
obj=[]
light=[]
wall_mat=Material((1,1,1),(1,1,1),0.5)
steel_mat=Material((0.85,0.85,0.85),(0.9,0.9,0.9),0.05)
gold_mat=Material((1.0,0.76,0.33),(1,0.76,0.33),0.1)
obj.append(Plane((0,1,0),-100,wall_mat))
obj.append(Plane((0,0,1),-100,wall_mat))
ParseObj("bunny_48k.obj",obj,lambda x:(x[0]*130-100+40,x[1]*130-100-10,x[2]*130-60),steel_mat)
#obj.append(Sphere((-100+40,-100+25,-60),25,steel_mat))
obj.append(Sphere((100-60,-100+20,70),20,gold_mat))
light.append(PointLight((200,300,400),(0.7,0.5,0.3)))

#{"camera_center":[-68.81999999999908,-45.884264671206466,173.86747835099692],"camera_direction":[0,-0.17192909938120776,-0.9851093074314335],"camera_dist":3776.419436538416,"camera_height":800,"camera_horizontal":[0.9999999628020718,0,0],"camera_type":0,"camera_up":[0,0.9851093074314335,-0.17192909938120773],"camera_width":800,"client_speed":0.6,"client_width":800,"client_height":800,"client_sensitivity":50}

cam=PerspectiveCamera((-68.81999999999908,-45.884264671206466,173.86747835099692),(0,-0.17192909938120776,-0.9851093074314335),(0,0.9851093074314335,-0.17192909938120773),w,h,3776)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)
