from scene_helper import *
import sys
import math
print('=== Light Texture Test Scene ===',file=sys.stderr)
w=800
h=800
obj=[]
light=[]
#light.append(PointLight((0,3,0),(0.3,0.3,0.3)))
glass_mat=Material((1,1,1),(0.04,0.04,0.04),0.2)
grass_mat=Material(("grass_2k.jpg",(1,1)),(0.99,0.99,0.99),0.9)
blanket_mat=Material(("picnic_blanket.jpg",(0.6,0.6)),(0.99,0.99,0.99),0.9)
steel_mat=Material((0.85,0.85,0.85),(0.9,0.9,0.9),0.05)
env_light_mat=Material((1,1,1),(0.3,0.3,0.3),0.8,'hdrsky_249.hdr')
light_mat=Material((0,0,0),(0,0,0),0.8,(255/50.0,223/50.0,171/50.0))
light_mat_2=Material((0,0,0),(0,0,0),0.8,(255/200.0,190/200.0,100/200.0))
wall_mat=Material((1,1,1),(0.9,0.9,0.9),0.8)
wood_mat=Material(("wood.jpg",(0.1,0.1)),(0.04,0.04,0.04),0.2)
blue_color_plastic_mat=Material((71/230.0,170/230.0,1),(0.04,0.04,0.04),0.99)
def col_hex(str):
	return (int(str[0:2],16)/255.0,int(str[2:4],16)/255.0,int(str[4:6],16)/255.0)
color_plastic_mat_1=Material(col_hex("29FFE2"),(0.04,0.04,0.04),0.99)
color_plastic_mat_2=Material(col_hex("D3FA55"),(0.04,0.04,0.04),0.99)
color_plastic_mat_3=Material(col_hex("4FC484"),(0.04,0.04,0.04),0.99)
color_plastic_mat_4=Material(col_hex("FE1633"),(0.04,0.04,0.04),0.99)
color_plastic_mat_5=Material(col_hex("C3E9E2"),(0.04,0.04,0.04),0.99)
silver_mat=Material((1,1,1),(0.99,0.99,0.99),0.8)
cake_mat=Material((1,1,1),(0.9,0.9,0.9),0.95)
#light.append(PointLight((0,10,0),(0.3,0.3,0.3)))
obj.append(Plane((0,1,0),0,grass_mat))
obj.append(Sphere((0,0,0),30000,env_light_mat))
obj.append(Triangle((1,0.001,-2),(1,0.001,2),(4,0.001,2),blanket_mat))
obj.append(Triangle((1,0.001,-2),(4,0.001,2),(4,0.001,-2),blanket_mat))
def cirXZ(o,r,m,p=300):
	g=[]
	for s in range(0,p):
		u=math.pi*2/p*s
		g.append((o[0]+math.cos(u)*r,o[1],o[2]+math.sin(u)*r))
	for s in range(1,p-1):
		obj.append(Triangle(g[0],g[s],g[s+1],m))
cirXZ((2.5,0.002,0),1,silver_mat)
def cirhXZ(o,r,m,h,p=300):
	g=[]
	for s in range(0,p):
		u=math.pi*2/p*s
		g.append((o[0]+math.cos(u)*r,o[1],o[2]+math.sin(u)*r))
	for s in range(0,p):
		a=g[s]
		b=g[(s+1)%p]
		obj.append(Triangle(a,b,(a[0],a[1]+h,a[2]),m))
		obj.append(Triangle((a[0],a[1]+h,a[2]),b,(b[0],b[1]+h,b[2]),m))
cirhXZ((2.5,0.002,0),0.8,cake_mat,0.5)
cirXZ((2.5,0.002+0.5,0),0.8,cake_mat)
cirhXZ((2.5,0.002+0.5,0),0.5,cake_mat,0.5)
cirXZ((2.5,0.002+0.5+0.5,0),0.5,cake_mat)
#ParseObj("cube.obj",obj,lambda x:(x[0]/5+2,(x[1]+1)/5,x[2]/5-2),light_mat)
obj.append(Sphere((-3,2,3),2,light_mat))
co=math.cos(-1)
si=math.sin(-1)
cirhXZ((2.5+0.3,0.002+0.5+0.4,-0.2),0.025,color_plastic_mat_1,0.5)
cirhXZ((2.5,0.002+0.5+0.4,0.25),0.025,color_plastic_mat_2,0.46)
cirhXZ((2.5-0.2,0.002+0.5+0.4,0),0.025,color_plastic_mat_3,0.53)
cirhXZ((2.5+0.15,0.002+0.5+0.4,0.1),0.025,color_plastic_mat_4,0.42)
cirhXZ((2.5-0.05,0.002+0.5+0.4,-0.3),0.025,color_plastic_mat_5,0.47)
ParseObj("bunny_1k.obj",obj,lambda x:((x[0]*co+x[2]*si)*2+1.9,x[1]*2-0.15,(x[0]*si-x[2]*co)*2+1.2),blue_color_plastic_mat)
obj.append(Sphere((100,3,8),1,light_mat_2))
obj.append(Sphere((99,2,10),0.5,light_mat_2))
co=math.cos(0.1)
si=math.sin(0.1)
def Tr(a):
    return (a[0]*co+a[2]*si+0.5,a[1],-a[0]*si+a[2]*co+0.4)
def Tru(a,b,c,m):
    return Triangle(Tr(a),Tr(b),Tr(c),m)
obj.append(Tru((2,0.002,-1.3),(2,0.002,-1.5),(3,0.03,-1.4),steel_mat))
obj.append(Tru((2,0.002,-1.3),(2,0.002,-1.5),(1.8,0.002,-1.3),steel_mat))
obj.append(Tru((1.8,0.002,-1.3),(2,0.002,-1.5),(1.8,0.002,-1.5),steel_mat))
obj.append(Tru((1.3,0.002,-1.35),(1.3,0.002,-1.45),(1.8,0.002,-1.35),wood_mat))
obj.append(Tru((1.8,0.002,-1.35),(1.3,0.002,-1.45),(1.8,0.002,-1.45),wood_mat))
wineglass_seq=[[0.0, -0.459543], [0.0, -0.459544], [0.0, -0.459545], [-0.351882, -0.426747], [-0.848656, -0.278898], [-1.112097, 0.084005], [-1.164785, 1.105511], [-0.991667, 2.328629], [-1.029301, 2.50336], [-1.0888, 2.3456], [-1.278, 1.1628], [-1.2148, 0.0552], [-0.9156, -0.3812], [-0.3804, -0.622], [-0.144, -0.9684], [-0.0968, -1.48], [-0.1284, -2.1124], [-0.3172, -2.2028], [-0.9944, -2.2628], [-1.2148, -2.3232], [-1.1992, -2.3984], [-1.0576, -2.4588], [-0.7112, -2.4588], [0.0, -2.4588], [0.0, -2.458801], [0.0, -2.458802]]
obj.append(RotateBSpline([2.3,2.458802/10,-1.7],list(map(lambda x:[x[0]/10,x[1]/10],wineglass_seq)),glass_mat))
glass_seq=[[-0.8, -0.459543], [-0.8, -0.459544], [-0.8, -0.459545], [-0.871882, -0.426747], [-0.948656, -0.278898], [-1.112097, 0.084005], [-1.164785, 1.105511], [-0.991667, 2.328629], [-1.029301, 2.50336], [-1.0888, 2.3456], [-1.278, 1.1628], [-1.2148, 0.0552], [-0.9156, -0.3812], [-0.3804, -0.522], [-0.0, -0.6684]]
obj.append(RotateBSpline([1.9,0.6684/12,-1.35],list(map(lambda x:[x[0]/12,x[1]/12],glass_seq)),glass_mat))
#obj.append(Tru((2,0.002,-1.35),(3.5,0.002,-1.3),(3.5,0.002,-1.35),steel_mat))
#cirhXZ((2.5,0.002+0.5+0.5,0),0.045,light_mat,0.5)
#obj.append(Sphere((2.5,0.002+0.5+0.5+0.5+0.05,0),0.05,))
#{"camera_center":[0,1.3799999999999994,0],"camera_direction":[0.9354440308298679,-0.35347484377925376,0],"camera_dist":300,"camera_height":800,"camera_horizontal":[0,0,0.9999999999999999],"camera_type":0,"camera_up":[0.3534748437792571,0.9354440308298679,0],"camera_width":800,"client_speed":0.6,"client_width":800,"client_height":800,"client_sensitivity":50}
#{"camera_center":[-0.1272509479522705,1.0432401347160338,0],"camera_direction":[0.9876883685707412,-0.15643447113752762,0],"camera_dist":608.8853175215946,"camera_height":800,"camera_horizontal":[0,0,1.0000000571700105],"camera_type":0,"camera_up":[0.15643447113753364,0.9876883685707404,0],"camera_width":800,"client_speed":0.6,"client_width":800,"client_height":800,"client_sensitivity":50}
#cam=PerspectiveCamera((0,3,0),(1,0,0),(0,1,0),w,h,300)
#{"camera_center":[-3.8082636648106254,2.0246259497316914,0],"camera_direction":[0.990023657716558,-0.1409012319375797,0],"camera_dist":880.8978371103966,"camera_height":800,"camera_horizontal":[0,0,0.9999999999999999],"camera_type":0,"camera_up":[0.14090123193758264,0.9900236577165575,0],"camera_width":800,"client_speed":0.6,"client_width":800,"client_height":800,"client_sensitivity":50}
cam=ThinLensCamera((-3.8,2,0),(1,0,0),(0,1,0),w,h,880,6,0.1)
#cam=PerspectiveCamera((0,1,0),(1,0,0),(0,1,0),w,h,300)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)
