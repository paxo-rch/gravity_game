from math import *
from kandinsky import *
from ion import *
from time import *
from random import *

objects=[]

def rotate(x,y,r):
  r*=-1
  cosR=cos(r)
  sinR=sin(r)
  a2=[0,0]
  a2[0]=x*cosR-y*sinR
  a2[1]=y*cosR+x*sinR
      
  return a2

def r(v1,v2):
  return randint(v1,v2)

def addObject(weight,x,y,canMove=0,c=color(200,10,150),ix=0,iy=0):
  global objects
  objects.append([weight,x,y,canMove,ix,iy,c])
  
def drawall():
  global objects
  
  for i in objects:
    fill_circle(int(i[1]),int(i[2]),int((((i[0]*3)/(pi*4))**(1/3))/2),i[6])
    
def generate(nb):
  for ii in range(int(nb)):
    addObject(
    randint(10000,100000),
    randint(0,320),
    randint(0,240),
    0)

def d2d(x,y):
  return sqrt(x**2+y**2)

def force(p1,p2):
  if(p1[1]-p2[1]==0 and p1[1]-p2[1]==0):
    return [0,0]
  f=((p1[0]/100000)*(p2[0]/100000))/(sqrt(((p1[1]-p2[1])**2)+((p1[2]-p2[2])**2))**2)
  dx=p1[1]-p2[1]
  d=sqrt((p1[1]-p2[1])**2+(p1[2]-p2[2])**2)
  if(dx<0):
    dx*=-1
  a=acos(dx/d)
  ff=[cos(a)*f,sin(a)*f]
  if(p1[1]>p2[1]):
    ff[0]*=-1
  if(p1[2]>p2[2]):
    ff[1]*=-1
  return ff
    

def update():
    global objects
    
    for i1 in objects:
      vx,vy=0,0
      
      if(i1[3]==1):
        for i2 in objects:
          ff=force(i1,i2)
          vx+=ff[0]
          vy+=ff[1]
        
        i1[4]+=vx*50
        i1[5]+=vy*50
    
    for i1 in objects:
      i1[1]+=i1[4]*100000/i1[0]
      i1[2]+=i1[5]*100000/i1[0]

while True:
  objects=[]
  print(objects)
  
  addObject(10000,300,200,0,color(0,0,0))
  
  generate(5)
  fill_rect(0,0,320,240,color(255,255,255))
  drawall()
  
  r=0

  while not keydown(KEY_EXE):
    if(keydown(KEY_RIGHT)):
      r+=1
      cc=rotate(50,0,radians(-r))
      fill_rect(0,0,320,240,color(255,255,255))
      draw_line(20,20,20+int(cc[0]),20+int(cc[1]),color(255,0,0))
      drawall()
    if(keydown(KEY_LEFT)):
      r-=1
      cc=rotate(50,0,radians(-r))
      fill_rect(0,0,320,240,color(255,255,255))
      draw_line(20,20,20+int(cc[0]),20+int(cc[1]),color(255,0,0))
      drawall()
    
  cc=rotate(0.01,0,radians(-r))
  addObject(1000,20,20,1,color(255,0,0),cc[0],cc[1])
  
  while True:
    update()
    if(keydown(KEY_OK)):
      break
    drawall()
    a=objects[0]
    b=objects[len(objects)-1]
    if(sqrt((a[1]-b[1])**2+(a[2]-b[2])**2) < 20):
      fill_rect(0,0,320,240,color(0,255,0))
      break
    if(a[1]<0 or a[1]>320 or a[2]<0 or a[2]>240):
      break