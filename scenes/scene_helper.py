import json
import sys
class Material(): #for backward compatibility
    def __init__(self,dc=(0,0,0),sc=(0,0,0),r=0,ec=(0,0,0)):
        sys.stderr.write('[scene helper] warning: Material is deprecated, please use BRDFMaterial instead\n')
        self.dc=dc
        self.sc=sc
        self.r=r
        self.ec=ec
class BRDFMaterial():
    def __init__(self,dc=(0,0,0),sc=(0,0,0),r=0,ec=(0,0,0)):
        self.dc=dc
        self.sc=sc
        self.r=r
        self.ec=ec
class MirrorMaterial():
    def __init__(self,reflectc=(1,1,1)):
        self.c=reflectc
class SolidGlassMaterial():
    def __init__(self,n=0.9,reflectc=(1,1,1),refractc=(1,1,1)):
        self.n=n
        self.reflectc=reflectc
        self.refractc=refractc
class GlassMaterial():
    def __init__(self,n=0.9,reflectc=(1,1,1),refractc=(1,1,1),t=0):
        self.n=n
        self.reflectc=reflectc
        self.refractc=refractc
        self.t=t
class Plane():
    def __init__(self,k,c,m):
        self.k=k
        self.c=c
        self.m=m
class HeightField():
    def __init__(self,y,p,h,w,m):
        self.y=y
        self.p=p
        self.h=h
        self.w=w
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
class NormTriangle():
    def __init__(self,a,b,c,p,q,r,m):
        self.a=a
        self.b=b
        self.c=c
        self.p=p
        self.q=q
        self.r=r
        self.m=m
class TextNormTriangle():
    def __init__(self,a,b,c,u,v,w,p,q,r,m):
        self.a=a
        self.b=b
        self.c=c
        self.u=u
        self.v=v
        self.w=w
        self.p=p
        self.q=q
        self.r=r
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
class PointLightDecay():
    def __init__(self,o,c):
        self.o=o
        self.c=c
class RotateBSpline():
    def __init__(self,o,c,m):
        self.o=o
        self.c=c
        self.m=m
class ImageTexture():
    def __init__(self,s,k,o=(0,0)):
        self.s=s
        self.k=k
        self.o=o
def obj_2_json(obj):
    if isinstance(obj,Material) or isinstance(obj,BRDFMaterial):
        return {
            "type": "BRDFMaterial",
            "dc": obj.dc,
            "sc": obj.sc,
            "r": obj.r,
            "ec": obj.ec
        }
    if isinstance(obj,SolidGlassMaterial):
        return {
            "type": "SolidGlassMaterial",
            "n": obj.n,
            "reflectc": obj.reflectc,
            "refractc": obj.refractc
        }
    if isinstance(obj,GlassMaterial):
        return {
            "type": "GlassMaterial",
            "n": obj.n,
            "reflectc": obj.reflectc,
            "refractc": obj.refractc,
            "t": obj.t
        }
    if isinstance(obj,ImageTexture):
        return {
            "type": "ImageTexture",
            "s": obj.s,
            "k": obj.k,
            "o": obj.o
        }
    if isinstance(obj,MirrorMaterial):
        return {
            "type": "MirrorMaterial",
            "c": obj.c
        }
    if isinstance(obj,RotateBSpline):
        return {
            "type": "RotateBSpline",
            "o": obj.o,
            "c": obj.c,
            "m": obj.m
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
    if isinstance(obj,NormTriangle):
        return {
            "type": "NormTriangle",
            "a": obj.a,
            "b": obj.b,
            "c": obj.c,
            "p": obj.p,
            "q": obj.q,
            "r": obj.r,
            "m": obj.m
        }
    if isinstance(obj,TextNormTriangle):
        return {
            "type": "TextNormTriangle",
            "a": obj.a,
            "b": obj.b,
            "c": obj.c,
            "u": obj.u,
            "v": obj.v,
            "w": obj.w,
            "p": obj.p,
            "q": obj.q,
            "r": obj.r,
            "m": obj.m
        }
    if isinstance(obj,HeightField):
        return {
            "type": "HeightField",
            "y": obj.y,
            "p": obj.p,
            "h": obj.h,
            "w": obj.w,
            "m": obj.m
        }
    if isinstance(obj,PointLight):
        return {
            "type": "PointLight",
            "o": obj.o,
            "c": obj.c
        }
    if isinstance(obj,PointLightDecay):
        return {
            "type": "PointLightDecay",
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
def ParseObj_Blender(file_name,obj,f,m,mm):
    t=open(file_name,'r')
    u=t.read()
    u.replace('\t','\n')
    u.replace('\n\n','\n')
    u.replace('\n\n','\n')
    os=u.split('\n')
    vec=[]
    mats=set()
    objs=set()
    cm=m
    for o in os:
        l=o.split(' ')
        if l[0]=='s' or l[0]=='l' or l[0]=='#' or l[0]=='mtllib' or l[0]=='':
            continue
        if l[0]=='usemtl':
            mats.add(l[1])
            if l[1] in mm:
                cm=mm[l[1]]
            else:
                cm=m
        elif l[0]=='o':
            objs.add(l[1])
        elif l[0]=='v':
            vec.append(f((float(l[1]),float(l[2]),float(l[3]))))
        elif l[0]=='f':
            obj.append(Triangle(vec[int(l[1])-1],vec[int(l[2])-1],vec[int(l[3])-1],cm))
#            for u in range(2,len(l)-1):
#                obj.append(Triangle(vec[int(l[1])-1],vec[int(l[u])-1],vec[int(l[u+1])-1],m))
        else:
            print(l,file=sys.stderr)
    print(mats,file=sys.stderr)
def ParseObj_Blender_Norm(file_name,obj,f,m,mm):
    t=open(file_name,'r')
    u=t.read()
    u.replace('\t','\n')
    u.replace('\n\n','\n')
    u.replace('\n\n','\n')
    os=u.split('\n')
    vec=[]
    vecn=[]
    vect=[]
    mats=set()
    objs=set()
    cm=m
    for o in os:
        l=o.split(' ')
        if l[0]=='s' or l[0]=='l' or l[0]=='#' or l[0]=='mtllib' or l[0]=='':
            continue
        if l[0]=='usemtl':
            mats.add(l[1])
            if l[1] in mm:
                cm=mm[l[1]]
            else:
                print('sad',l[1],file=sys.stderr)
                cm=m
        elif l[0]=='o':
            objs.add(l[1])
        elif l[0]=='v':
            vec.append(f((float(l[1]),float(l[2]),float(l[3]))))
        elif l[0]=='vn':
            vecn.append(f((float(l[1]),float(l[2]),float(l[3]))))
        elif l[0]=='vt':
            vect.append((float(l[1]),float(l[2])))
        elif l[0]=='f':
            if cm!=None:
                if l[1].find('//')!=-1:
                    obj.append(NormTriangle(vec[int(l[1].split('//')[0])-1],vec[int(l[2].split('//')[0])-1],vec[int(l[3].split('//')[0])-1],
                    vecn[int(l[1].split('//')[1])-1],vecn[int(l[2].split('//')[1])-1],vecn[int(l[3].split('//')[1])-1],cm))
                else:
                    obj.append(TextNormTriangle(vec[int(l[1].split('/')[0])-1],vec[int(l[2].split('/')[0])-1],vec[int(l[3].split('/')[0])-1],
                    vect[int(l[1].split('/')[1])-1],vect[int(l[2].split('/')[1])-1],vect[int(l[3].split('/')[1])-1],
                    vecn[int(l[1].split('/')[2])-1],vecn[int(l[2].split('/')[2])-1],vecn[int(l[3].split('/')[2])-1],cm))
    #            for u in range(2,len(l)-1):
    #                obj.append(Triangle(vec[int(l[1])-1],vec[int(l[u])-1],vec[int(l[u+1])-1],m))
        else:
            print(l,file=sys.stderr)
    print(mats,file=sys.stderr)
def dump_setup(setup):
    print(json.dumps(setup,default=obj_2_json))