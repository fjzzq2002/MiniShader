import json
import sys
class Material():
    def __init__(self,dc=(0,0,0),sc=(0,0,0),r=0,ec=(0,0,0)):
        self.dc=dc
        self.sc=sc
        self.r=r
        self.ec=ec
class Plane():
    def __init__(self,k,c,m):
        self.k=k
        self.c=c
        self.m=m
class Sphere():
    def __init__(self,o,r,m):
        self.o=o
        self.r=r
        self.m=m
class Triangle():
    def __init__(self,a,b,c,m):
        self.a=a
        self.b=b
        self.c=c
        self.m=m
class PerspectiveCamera():
    def __init__(self,o,d,u,w,h,f,b=False):
        self.o=o
        self.d=d
        self.u=u
        self.w=w
        self.h=h
        self.f=f
        self.b=b
class ThinLensCamera():
    def __init__(self,o,d,u,w,h,f,l,r):
        self.o=o
        self.d=d
        self.u=u
        self.w=w
        self.h=h
        self.f=f
        self.l=l
        self.r=r
class PointLight():
    def __init__(self,o,c):
        self.o=o
        self.c=c
def obj_2_json(obj):
    if isinstance(obj,Material):
        return {
            "type": "Material",
            "dc": obj.dc,
            "sc": obj.sc,
            "r": obj.r,
            "ec": obj.ec
        }
    if isinstance(obj,Plane):
        return {
            "type": "Plane",
            "k": obj.k,
            "c": obj.c,
            "m": obj.m
        }
    if isinstance(obj,PerspectiveCamera):
        return {
            "type": "PerspectiveCamera",
            "o": obj.o,
            "d": obj.d,
            "u": obj.u,
            "w": obj.w,
            "h": obj.h,
            "f": obj.f,
            "b": obj.b
        }
    if isinstance(obj,ThinLensCamera):
        return {
            "type": "ThinLensCamera",
            "o": obj.o,
            "d": obj.d,
            "u": obj.u,
            "w": obj.w,
            "h": obj.h,
            "f": obj.f,
            "l": obj.l,
            "r": obj.r
        }
    if isinstance(obj,Sphere):
        return {
            "type": "Sphere",
            "o": obj.o,
            "r": obj.r,
            "m": obj.m
        }
    if isinstance(obj,Triangle):
        return {
            "type": "Triangle",
            "a": obj.a,
            "b": obj.b,
            "c": obj.c,
            "m": obj.m
        }
    if isinstance(obj,PointLight):
        return {
            "type": "PointLight",
            "o": obj.o,
            "c": obj.c
        }
    raise Exception("unknown object")
def ParseObj(file_name,obj,f,m):
    t=open(file_name,'r')
    u=t.read()
    u.replace('\t','\n')
    u.replace('\n\n','\n')
    u.replace('\n\n','\n')
    os=u.split('\n')
    vec=[]
    for o in os:
        l=o.split(' ')
        if l[0]=='v':
            vec.append(f((float(l[1]),float(l[2]),float(l[3]))))
    for o in os:
        l=o.split(' ')
        if l[0]=='f':
            obj.append(Triangle(vec[int(l[1])-1],vec[int(l[2])-1],vec[int(l[3])-1],m))
def dump_setup(setup):
    print(json.dumps(setup,default=obj_2_json))