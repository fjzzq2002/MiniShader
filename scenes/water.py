from scene_helper import *
import sys
import numpy as np
from numba import jit
import random
import math
@jit
def work(n,m):
    u=np.zeros((n,m))
    for uu in range(0,n*m):
        u[random.randint(0,n-1)][random.randint(0,m-1)]+=random.uniform(0,1)
    pu=np.zeros((n,m))
    v=np.zeros((n,m))
    for w in range(0,50):
        for x in range(0,n):
            for y in range(0,m):
                v[x][y]=u[(x+1)%n][y]+u[(x+n-1)%n][y]+u[x][(y+1)%m]+u[x][(y+m-1)%m]
    #            v[x][y]=(v[x][y]/4-u[x][y])*0.99
        pu,u=u,0.02*pu+0.01*u+v*0.2425
    w=np.zeros((n-2,m-2))
    for x in range(0,n-2):
        for y in range(0,m-2):
            w[x][y]=(u[x][y]+u[x][y+1]+u[x][y+2]+u[x+1][y]+u[x+1][y+1]+u[x+2][y+2]+u[x+2][y]+u[x+2][y+1]+u[x+2][y+2])/9.0
    return w
print('=== Water Wave Scene ===',file=sys.stderr)
w=800
h=800
obj=[]
light=[]
wall_mat=Material((1,1,1),(0.7,0.7,0.7),0.3)
blue_wall_mat=Material((0,0,0.85),(0,0,0.7),0.3)
red_wall_mat=Material((0.85,0,0),(0.7,0,0),0.3)
light_mat=Material((0,0,0),(0,0,0),0.3,(3,3,3))
ball_mat=Material((0.85,0.85,0.85),(0.9,0.9,0.9),0.05)
water_mat=Material((0.35,0.55,0.9),(0.9,0.9,0.9),0.05)
water_glass_mat=SolidGlassMaterial(1.33,(0.8,0.8,0.8),(0.8,0.8,0.8))
cb_mat=Material((0,0,0),(0,0,0),0.3,("sand.jpg",(0.00005,0.00005)))
#obj.append(HeightField((work(1200,600)*2-80).tolist(),(200,-100),-400,200,water_glass_mat))
obj.append(HeightField((work(1200,600)*5-80).tolist(),(-200,-100),400,200,water_glass_mat))
obj.append(Plane((0,1,0),-5000,cb_mat))
light.append(PointLight((0,0,0),(0.9,0.9,0.9)))
cam=PerspectiveCamera((0,0,0),(0,-1,0),(0,0,-1),w,h,200)
setup={'obj':obj,'light':light,'cam':cam}
dump_setup(setup)