from scene_helper import *
import sys
print('=== Wineglass Scene ===',file=sys.stderr)
w=1920
h=1080
obj=[]
light=[]
#wall_mat=Material((0.9,0.9,0.9),(0.07,0.07,0.07),0.3)
blue_wall_mat=Material((0,0,1),(0,0,0.7),0.3)
red_wall_mat=Material((0.85,0,0),(0.07,0,0),0.3)
mr_mat=MirrorMaterial((0.95,0.95,0.95))
gold_metal_mat=Material((0.85,0.85,0.85),(0.03,0.03,0.03),0.1)
bg_glass_mat=GlassMaterial(1.55,(1,1,1),(0.8,0.9,0.9))
yellow_glass_mat=GlassMaterial(1.55,(1,1,1),(0.9,0.9,0.6))
blue_glass_mat=GlassMaterial(1.55,(1,1,1),(0.7,0.7,0.9))
red_glass_mat=GlassMaterial(1.55,(1,1,1),(0.9,0.8,0.7))
#bg_solid_glass_mat=SolidGlassMaterial(1.55,(1,1,1),(0.95,0.95,0.95))
bg_solid_glass_mat=SolidGlassMaterial(1.55,(1,1,1),(0.8,0.9,0.9))

wall_mat=Material(("chessboard.jpg",(0.2,0.2)),(0.04,0.04,0.04),0.3)
env_light_mat=Material((0.01,0.01,0.01),(0.03,0.03,0.03),0.8,'hdrsky_249.hdr')
#light_mat=Material((0.1,0.1,0.1),(0.1,0.1,0.1),0.3,"hdrsky_249.hdr")
wood_mat=Material(("wood.jpg",(0.1,0.1)),(0.04,0.04,0.04),0.2)
#light_mat=Material((1,1,1),(0.7,0.7,0.7),0.3,(100,100,100))
ball_mat=Material((0.85,0.85,0.85),(0.9,0.9,0.9),0.05)
#light.append(PointLightDecay((-15,13,18),(0.7*30,0.7*30,0.7*30)))
#wineglass_seq=[[-0.459543, -0.0], [-0.459544, -0.0], [-0.459545, -0.0], [-0.426747, 0.351882], [-0.278898, 0.848656], [0.084005, 1.112097], [1.105511, 1.164785], [2.328629, 0.991667], [2.50336, 1.029301], [2.3456, 1.0888], [1.1628, 1.278], [0.0552, 1.2148], [-0.3812, 0.9156], [-0.622, 0.3804], [-0.9684, 0.144], [-1.48, 0.0968], [-2.1124, 0.1284], [-2.2028, 0.3172], [-2.2628, 0.9944], [-2.3232, 1.2148], [-2.3984, 1.1992], [-2.4588, 1.0576], [-2.4588, 0.7112], [-2.4588, -0.0], [-2.458801, -0.0], [-2.458802, -0.0]]
glass_seq=[[-0.8, -0.459543], [-0.8, -0.459544], [-0.8, -0.459545], [-0.871882, -0.426747], [-0.948656, -0.278898], [-1.112097, 0.084005], [-1.164785, 1.105511], [-0.991667, 2.328629], [-1.029301, 2.50336], [-1.0888, 2.3456], [-1.278, 1.1628], [-1.2148, 0.0552], [-0.9156, -0.3812], [-0.3804, -0.522], [-0.0, -0.6684]]
obj.append(Sphere((-13,2,9),25,env_light_mat))
#obj.append(Sphere((2,1,0),1,yellow_glass_mat))
obj.append(RotateBSpline([3,0.6684,3],glass_seq,blue_glass_mat))
#obj.append(Sphere((5,2,8),2,bg_solid_glass_mat))
obj.append(Sphere((5,2,8),2,bg_solid_glass_mat))
ParseObj("bunny_1k.obj",obj,lambda x:(x[0]*10,(x[1]-0.07)*10,x[2]*10-3),mr_mat)
#obj.append(Triangle((3,1,0),(3,5,0),(3,1,3),glass_mat))
obj.append(Plane((0,1,0),0,wood_mat))
#obj.append(Plane((1,0,0),7,wall_mat))
cam=PerspectiveCamera((-13,5,9),
(0.8968228648554929,-0.2638730619654009,-0.3550772914081536),
(0.2453429990417326,0.9645575115980382,-0.09713815385474979),w,h,1500)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)