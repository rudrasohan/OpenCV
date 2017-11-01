import numpy as np
import cv2
import cv

i=0
rx=[]
ry=[]
nrx=[]
nry=[]
rows=0
cols=0

def isvalid(i,j,img):
	rows=img.shape[0]
	cols=img.shape[1]
	if(i>=rows or i<0 or j<0 or j>=cols):
		return 0
	return 1

def erosion(img):
	global rows,cols
	img1=np.zeros((rows,cols,1),np.uint8)
	for i in range(0,rows):
		for j in range(0,cols):
			flag=1
			if(img[i,j]==0):
				continue
			for k in range(i-1,i+2):
				for l in range(j-1,j+2):
					if(isvalid(k,l,img)==0):
						continue
					if(img[k,l]==0):
						flag=0
			if(flag==0):
				img1[i,j]=0
			if(flag==1):
				img1[i,j]=255
	cv2.imshow('eroded',img)
	cv2.waitKey(0)
	print "printed"		
	return img1

def dilate(img):
	global rows,cols
	img1=np.zeros((rows,cols,1),np.uint8)
	for i in range(0,rows):
		for j in range(0,cols):
			flag=1
			if(img[i,j]==255):
				continue
			for k in range(i-1,i+2):
				for l in range(j-1,j+2):
					if(isvalid(k,l,img)==0):
						continue
					if(img[k,l]==255):
						flag=0
			if(flag==0):
				img1[i,j]=255
			if(flag==1):
				img1[i,j]=0
	cv2.imshow('dilated',img)
	cv2.waitKey(0)
	print "printed"		
	return img1


def hypo(theta,x,m):
	z=np.array([[1.0 for i in range(0,m)]])
	z=np.dot(theta.transpose(),x.transpose())
	#z.astype([(k,float) for k in z.dtype.names])
	#print z
	for i in range(0,m):
		z[0][i]=float((float(1.0)/float((1+float(np.exp(-1.0*z[0][i]))))))
		#print float((float(1.0)/float((1+float(np.exp(-1.0*z[0][i]))))))
	#print z
	return z[0]

def grad_des(x,y,theta,alp,m,num_it):
	conv=0.0
	for i in range(0,num_it):
		if(i%50==0):
			print i
		h=hypo(theta,x,m)
		temp=[]

		for j in range(0,20):
			temp2=0.0
			for k in range(0,m):
				temp2=temp2+(h[k]-y[k][0])*x[k][j]
				#print k
			temp.append(temp2)
			
		for j in range(0,20):
			theta[j]=theta[j]-(alp/m)*temp[j]
		if(i == num_it-1 or i==num_it-2 or i==num_it-3 or i==num_it-4):
			print theta
		if(i!=0):	
			if(conv-theta[0][0]>=-0.0000001 and conv-theta[0][0]<=0.0000001):
				print i
				print conv,theta[0][0]
				break
		conv=theta[0][0]
		#cv2.waitKey(10)
	#cv2.waitKey(0)		
	return theta

def segment(rp,nrp,img,img_seg,img_hsv):
	global rows,cols,rx,ry,nrx,nry
	print img_seg[210,480]
	
	rowf=rows*1.0
	colf=cols*1.0	
	# cv2.imshow('img',img_hsv)
	# cv2.waitKey(0)	
	x=np.array([[1.0 for i in range(0,20)] for j in range(0,rp+nrp)])
	tmp=[rp,nrp]
	y=np.array([[i*1.0] for i in range(1,-1,-1) for k in range(0,tmp[1-i])])
	for i in range(0,rp):
		x[i][1]=img_hsv[rx[i],ry[i],0]*1.0
		x[i][2]=img[rx[i],ry[i],0]*1.0
		x[i][3]=img[rx[i],ry[i],1]*1.0
		x[i][4]=img[rx[i],ry[i],2]*1.0
		x[i][5]=img_hsv[rx[i],ry[i],0]*img_hsv[rx[i],ry[i],0]*1.0
		x[i][6]=img[rx[i],ry[i],0]*img[rx[i],ry[i],0]*1.0
		x[i][7]=img[rx[i],ry[i],1]*img[rx[i],ry[i],1]*1.0
		x[i][8]=img[rx[i],ry[i],2]*img[rx[i],ry[i],2]*1.0
		x[i][9]=img[rx[i],ry[i],0]*img[rx[i],ry[i],1]*1.0
		x[i][10]=img[rx[i],ry[i],1]*img[rx[i],ry[i],2]*1.0
		x[i][11]=img[rx[i],ry[i],2]*img[rx[i],ry[i],0]*1.0
		x[i][12]=img_hsv[rx[i],ry[i],1]*1.0
		x[i][13]=img_hsv[rx[i],ry[i],1]*img_hsv[rx[i],ry[i],1]*1.0
		x[i][14]=img_hsv[rx[i],ry[i],0]*img_hsv[rx[i],ry[i],1]*1.0
		x[i][15]=rx[i]*1.0/10.0
		x[i][16]=ry[i]*1.0/10.0
		x[i][17]=rx[i]*rx[i]*1.0/1000.0
		x[i][18]=ry[i]*ry[i]*1.0/1000.0
		x[i][19]=rx[i]*ry[i]*1.0/1000.0


	for i in range(rp,rp+nrp):
		x[i][1]=img_hsv[nrx[i-rp],nry[i-rp],0]*1.0
		x[i][2]=img[nrx[i-rp],nry[i-rp],0]*1.0
		x[i][3]=img[nrx[i-rp],nry[i-rp],1]*1.0
		x[i][4]=img[nrx[i-rp],nry[i-rp],2]*1.0
		x[i][5]=img_hsv[nrx[i-rp],nry[i-rp],0]*img_hsv[nrx[i-rp],nry[i-rp],0]*1.0
		x[i][6]=img[nrx[i-rp],nry[i-rp],0]*img[nrx[i-rp],nry[i-rp],0]*1.0
		x[i][7]=img[nrx[i-rp],nry[i-rp],1]*img[nrx[i-rp],nry[i-rp],1]*1.0
		x[i][8]=img[nrx[i-rp],nry[i-rp],2]*img[nrx[i-rp],nry[i-rp],2]*1.0
		x[i][9]=img[nrx[i-rp],nry[i-rp],0]*img[nrx[i-rp],nry[i-rp],1]*1.0
		x[i][10]=img[nrx[i-rp],nry[i-rp],1]*img[nrx[i-rp],nry[i-rp],2]*1.0
		x[i][11]=img[nrx[i-rp],nry[i-rp],2]*img[nrx[i-rp],nry[i-rp],0]*1.0
		x[i][12]=img_hsv[nrx[i-rp],nry[i-rp],1]*1.0
		x[i][13]=img_hsv[nrx[i-rp],nry[i-rp],1]*img_hsv[nrx[i-rp],nry[i-rp],1]*1.0
		x[i][14]=img_hsv[nrx[i-rp],nry[i-rp],0]*img_hsv[nrx[i-rp],nry[i-rp],1]*1.0
		x[i][15]=nrx[i-rp]*1.0/10.0
		x[i][16]=nry[i-rp]*1.0/10.0
		x[i][17]=nrx[i-rp]*nrx[i-rp]*1.0/1000.0
		x[i][18]=nry[i-rp]*nry[i-rp]*1.0/1000.0
		x[i][19]=nrx[i-rp]*nry[i-rp]*1.0/1000.0

	print 'x is'
	print 123
#	cv2.waitKey(1000000)
	#cv2.waitKey(0)	
	#print x[rp-1]
	#print x[rp+1]
	#cv2.waitKey(0)
	# theta=np.dot(np.dot(np.linalg.pinv(np.dot(x.transpose(),x)),x.transpose()),y)
	theta=np.array([[0.0],[0.0],[0.0],[0.0],[0.0],[0.0],[0.0],[0.0],[0.0],[0.0],[0.0],[0.0],[0.0],[0.0],[0.0],[0.0],[0.0],[0.0],[0.0],[0.0]])
	theta=grad_des(x,y,theta,0.001,rp+nrp,2000)
	print 'theta is '
	#cv2.waitKey(0)
	print theta
	#cv2.waitKey(0)
	print rowf,colf
	#cv2.waitKey(0)
	frame=cv2.imread("test_o.png")
	frame_hsv=cv2.cvtColor(frame,cv2.COLOR_BGR2HSV);
	img2=np.zeros((frame.shape[0],frame.shape[1],1),np.uint8)
	for i in range(frame.shape[0]):
		for j in range(0,frame.shape[1]):
			#if(j%250==0):
				#print np.dot(np.array([1.0,img_hsv[i,j,0]*1.0/179.0,img[i,j,0]*1.0/255.0,img[i,j,1]*1.0/255.0,img[i,j,2]*1.0/255.0,img_hsv[i,j,0]*img_hsv[i,j,0]*1.0/179.0,img[i,j,0]*img[i,j,0]/(255.0*255.0),img[i,j,1]*img[i,j,1]/(255.0*255.0),img[i,j,2]*img[i,j,2]/(255.0*255.0),img[i,j,0]*img[i,j,1]/(255.0*255.0),img[i,j,1]*img[i,j,2]/(255.0*255.0),img[i,j,2]*img[i,j,0]/(255.0*255.0)]),theta)
			##print np.dot(np.array([1,img[i,j,0]/179.0,i/rowf,j/colf,img[i,j,0]*img[i,j,0]/(179.0*179.0),i*i/(rowf*rowf),j*j/(colf*colf),img[i,j,0]*i/(179.0*rowf),i*j/(rowf*colf),j*img[i,j,0]/(colf*179.0)]),theta)
			##print float(img[i,j,0]/179.0)
			if(np.dot(np.array([1.0,frame_hsv[i,j,0]*1.0,frame[i,j,0]*1.0,frame[i,j,1]*1.0,frame[i,j,2]*1.0,frame_hsv[i,j,0]*frame_hsv[i,j,0]*1.0,frame[i,j,0]*frame[i,j,0]*1.0,frame[i,j,1]*frame[i,j,1]*1.0,frame[i,j,2]*frame[i,j,2]*1.0,frame[i,j,0]*frame[i,j,1]*1.0,frame[i,j,1]*frame[i,j,2]*1.0,frame[i,j,2]*frame[i,j,0]*1.0,frame_hsv[i,j,1]*1.0,frame_hsv[i,j,1]*frame_hsv[i,j,1]*1.0,frame_hsv[i,j,0]*frame_hsv[i,j,1]*1.0,i*1.0/10.0,j*1.0/10.0,i*i*1.0/1000.0,j*j*1.0/1000.0,i*j*1.0/1000.0]),theta)>=-0.2):
				img2[i,j]=255
	cv2.imshow("seg",img2);
	cv2.waitKey(10);

	#print img2
	cv2.imwrite('final.png',img2)
	cv2.imshow('segment',img2)
	#cv2.waitKey(0)
	# while (1):
	# 	img2=erosion(img2)
	# 	img2=dilate(img2)
	#cv2.imwrite('final.png',img2)
	cv2.waitKey(0)

# def pts(event,x,y,flags,userdata):
# 	if event==cv2.EVENT_LBUTTONDOWN:
# 		global i
# 		if(i>=100):
# 			return
# 		print x,y
# 		if(i<50):
# 			print i
# 			for j in range(y-2,y+3):
# 				for k in range(x-2,x+3):
# 					rx.append(j)
# 					ry.append(k)
# 		if(i>=50):
# 			print i	
# 			for j in range(y-2,y+3):
# 				for k in range(x-2,x+3):
# 					nrx.append(j)
# 					nry.append(k)
# 		i=i+1
# 		if(i==100):
# 			#print rx,ry,nrx,nry
# 			segment()


#img=cv2.imread("/home/sanskar/opencv/Road_Seg/road-segmentation/images/test2.png")
img=cv2.imread("test_o.png")
img_seg=cv2.imread("test_b.png")
rows=img.shape[0]
cols=img.shape[1]
img_hsv=cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
# cv2.imshow('img',img_hsv)
# cv2.waitKey(0)
rp=0
nrp=0
for i in range(0,rows):
	for j in range(0,cols):
		if j%20!=0 or i%20!=0:
			continue
		if(img_seg[i,j,0]==255 and img_seg[i,j,1]==255 and img_seg[i,j,2]==255):
			rx.append(i)
			ry.append(j)
			rp+=1
		else:
			nrx.append(i)
			nry.append(j)
			nrp+=1
print rp,nrp
#print rows,cols
#cv2.waitKey(0)

#cv2.imshow('road',img_seg)
#print img_seg[300,600,0],img_seg[300,600,1],img_seg[300,600,2]

segment(rp,nrp,img,img_seg,img_hsv)

#cv2.waitKey(0)

#print "Click 10 examples of road points and 10 of non road points"
#cv2.setMouseCallback('road',pts,param=None)
cv2.waitKey(0)	
