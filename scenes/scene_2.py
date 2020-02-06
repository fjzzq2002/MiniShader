from scene_helper import *
import sys
import math
print('=== Warm Bedroom Scene ===',file=sys.stderr)
w=1920
h=1080
obj=[]
light=[]
default_mat=BRDFMaterial((1,1,1),(0.09,0.09,0.09),0.8)
#light_mat=BRDFMaterial(ec=(1,1,1))
blue_glass_mat=GlassMaterial(1.55,(1,1,1),(0.7,0.7,0.9))
#light_mat=Material(ec=(1,1,1))
light.append(PointLightDecay((0,0,-5),(5,5,5)))
light.append(PointLightDecay((20,10,-3),(7,7,7)))
light.append(PointLightDecay((20,10,-1),(7,7,7)))
ParseObj_Blender_Norm("cuarto-blanco-o2.obj",obj,lambda x:(x[0]*10,-x[2]*10,x[1]*10),default_mat,
{'images':None, #BRDFMaterial((0,0,0),(0.7,0.7,0.7),0.8),#,(2.55*17,2.55*17,2.44*17)),
'Material.014':BRDFMaterial((0.8,0.8,0.8),(0.03,0.03,0.03),0.5),
'Material.019':BRDFMaterial((1,1,1),(0.04,0.04,0.04),0.8),
'Material.007':BRDFMaterial(("wood.jpg",(0.1,0.1)),(0.04,0.04,0.04),0.9),
'Material.013':BRDFMaterial((0.1,0.1,0.1),(1,204.0/255.0,114.0/255.0),0.3), #to adjust
'Material.026':BRDFMaterial((0.202,0.136,0.111),(0.04,0.04,0.04),0.9),
'Material.015':BRDFMaterial(("wall_hor.jpg",(0.1,0.1)),(0.04,0.04,0.04),0.9),
'Material':BRDFMaterial((0.8,0.8,0.8),(0.8,0.8,0.8),0.04),
'Material.016':BRDFMaterial((0,98.0/255.0,0),(0.1,0.1,0.1),0.9),
'Material.009':BRDFMaterial((0.9,0.97,0.91),(0.015,0.015,0.015),0.9),
'Material.010':BRDFMaterial((0.68,0.8,0.64),(0.015,0.015,0.015),0.9),
'Material.012':BRDFMaterial((0.42,0.64,0.54),(0.015,0.015,0.015),0.9),
'Material.026':BRDFMaterial((0.20,0.13,0.11),(0.1,0.1,0.1),0.9),
'Material.011':BRDFMaterial((0.57,0.8,0.79),(0.8,0.8,0.8),0.05),
'Material.025':BRDFMaterial((0.59,1,0.52),(0.8,0.8,0.8),0.95),
'Material.024':BRDFMaterial((0.8,0.8,0.8),(0.015,0.015,0.015),0.9),
'Material.032':BRDFMaterial((0.8,0.8,0.8),(0.015,0.015,0.015),0.9),
'Material.033':BRDFMaterial((0.8,0.8,0.8),(0.015,0.015,0.015),0.9),
'Material.034':BRDFMaterial((0.8,0.8,0.8),(0.015,0.015,0.015),0.9),
'Material.027':BRDFMaterial(ImageTexture("pic13.jpg",(1,1),(0.1,0)),(0.015,0.015,0.015),0.9), #1
'Material.030':BRDFMaterial(ImageTexture("pic0.jpg",(1,1),(0,0)),(0.015,0.015,0.015),0.9), #2
'Material.031':BRDFMaterial(ImageTexture("pic9.jpg",(1,1),(0,0.6)),(0.015,0.015,0.015),0.9), #3
'Material.023':BRDFMaterial((0.384,0.154,0.07),(0.015,0.015,0.015),0.9)
})
blue_wall_mat=Material((0,0,0.85),(0,0,0.07),0.3)
wineglass_seq=[[0.0, -0.459543], [0.0, -0.459544], [0.0, -0.459545], [-0.351882, -0.426747], [-0.848656, -0.278898], [-1.112097, 0.084005], [-1.164785, 1.105511], [-0.991667, 2.328629], [-1.029301, 2.50336], [-1.0888, 2.3456], [-1.278, 1.1628], [-1.2148, 0.0552], [-0.9156, -0.3812], [-0.3804, -0.622], [-0.144, -0.9684], [-0.0968, -1.48], [-0.1284, -2.1124], [-0.3172, -2.2028], [-0.9944, -2.2628], [-1.2148, -2.3232], [-1.1992, -2.3984], [-1.0576, -2.4588], [-0.7112, -2.4588], [0.0, -2.4588], [0.0, -2.458801], [0.0, -2.458802]]
glass_seq=[[-0.8, -0.459543], [-0.8, -0.459544], [-0.8, -0.459545], [-0.871882, -0.426747], [-0.948656, -0.278898], [-1.112097, 0.084005], [-1.164785, 1.105511], [-0.991667, 2.328629], [-1.029301, 2.50336], [-1.0888, 2.3456], [-1.278, 1.1628], [-1.2148, 0.0552], [-0.9156, -0.3812], [-0.3804, -0.522], [-0.0, -0.6684]]
glass_seq=list(map(lambda x:(x[0]/2,x[1]/2),glass_seq))
glass_mat=GlassMaterial(1.55,(1,1.6,1),(0.85,0.85,0.85))
obj.append(RotateBSpline([-7,-5,11],glass_seq,glass_mat))
#obj.append(Sphere((-7,-4,11),1,BRDFMaterial((1,0,1),(0.09,0.09,0.09),0.8)))
u=(2,0.5,2)
l=-math.sqrt(u[0]*u[0]+u[1]*u[1]+u[2]*u[2])
u=(u[0]/l,u[1]/l,u[2]/l)
#{"camera_center":[2.0995082623504895,2.000449756210496,-0.37264228800156407],"camera_direction":[-0.806895291507471,-0.5903052223873025,-0.021440933973687425],"camera_dist":500,"camera_height":800,"camera_horizontal":[0.5712911204706731,-0.7891027355278495,0.2257061469580288],"camera_type":0,"camera_up":[0.15015461837337815,-0.16987221639448885,-0.9739594325816998],"camera_width":800,"client_speed":0.6,"client_width":800,"client_height":800,"client_sensitivity":50}
cam=PerspectiveCamera((20,5,20),u,(0,1,0),w,h,1000)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)
