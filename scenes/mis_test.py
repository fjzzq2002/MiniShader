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
light_mat=Material((1,1,1),(0.7,0.7,0.7),0.3,(4,3,4))
ball_mat=Material((0.7,0,0),(0,0.9,0),0.4)
obj.append(Triangle((0,0,0),(100,0,0),(0,0,100),light_mat))
obj.append(Sphere((30,61,30),30,ball_mat))
#{"camera_center":[3.91923405612551,32.58482882895819,23.499783244909946],"camera_direction":[0.6766846705160343,0.7181262873250804,0.1624576053758331],"camera_dist":284.68750160323594,"camera_height":800,"camera_horizontal":[-0.23344535877074823,1.884169308075112e-8,0.9723698794348058],"camera_type":0,"camera_up":[-0.6982843960061436,0.695912762845304,-0.16764326817341885],"camera_width":800,"client_speed":0.6,"client_width":800,"client_height":800,"client_sensitivity":50}
cam=PerspectiveCamera((3.91923405612551,32.58482882895819,23.499783244909946),(0.6766846705160343,0.7181262873250804,0.1624576053758331),(-0.6982843960061436,0.695912762845304,-0.16764326817341885),w,h,284.68750160323594)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)