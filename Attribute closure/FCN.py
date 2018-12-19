#encoding: utf-8
"""
@project=Deep Neural Network
@file=FCN.py
@author="QHX"
@create_time=2018/9/30 15:03
"""

import numpy as np
import scipy.io as sio
import matplotlib
#matplotlib.use('Agg')
import matplotlib.pyplot as plt

class NN():
    def __init__(self,X=None,Y=None,test_X=None,test_Y=None,input_dims=None,dropout=None,lr=1,epoch=10,batch_size=24,use_batch=True):
        self.X=X
        self.Y=Y
        self.test_X=test_X
        self.test_Y=test_Y
        self.input_dims=input_dims
        self.dropout=dropout
        self.lr=lr
        self.epoch=epoch
        self.use_batch=use_batch
        self.batch_size=batch_size
        self.parameters={}
        self.masks={}
        self.m_samples=self.X.shape[1]





    def fit(self):
        for i in range(1,len(self.input_dims)):
            self.parameters['W'+str(i)]=np.sqrt(2/(self.input_dims[i]+self.input_dims[i-1]))*np.random.randn(self.input_dims[i],self.input_dims[i-1])
            self.parameters['b'+str(i)]=np.zeros((self.input_dims[i],1))

        cost_record=[]
        train_acc_record=[]
        test_acc_record=[]

        if (self.use_batch==True):
            for epoch in range(self.epoch):

                #### shuffle the datasets
                index=np.arange(self.m_samples)
                np.random.shuffle(index)
                self.X = self.X.T[index].T
                self.Y=self.Y.T[index].T

                for batch_num in range(self.m_samples//self.batch_size):
                    batch_X=self.X[:,batch_num*self.batch_size:(batch_num+1)*self.batch_size]
                    batch_Y=self.Y[:,batch_num*self.batch_size:(batch_num+1)*self.batch_size]
                    self.batch_samples=batch_X.shape[1]


                    AL,caches=self.forward_propagation(batch_X,dropout=self.dropout)

                    cost = -1 / self.batch_size * np.sum(
                        batch_Y * np.log(AL + pow(10.0, -9)) + (1 - batch_Y) * np.log(1 - AL + pow(10.0, -9)))

                    if(batch_num==30):
                        cost_record.append(cost)
                        train_acc=np.mean((AL>0.5)==batch_Y)
                        test_acc=np.mean((self.predict(self.test_X)>0.5)==self.test_Y)
                        train_acc_record.append(train_acc)
                        test_acc_record.append(test_acc)
                        print("epoch:{}  cost:{}  train_acc:{} test_acc:{}".format(epoch+1,cost,train_acc,test_acc))

                    ## sigmoid
                    dAL=-(batch_Y/(AL+pow(10.0,-9))-(1-batch_Y)/(1.0-AL+pow(10.0,-9)))
                    grads=self.backward_propagation(dAL,caches)

                    self.update_parameters(grads)

                if(epoch==int(self.epoch*0.5)):
                   self.lr*=0.2

            ####plot the process
            plt.plot(np.arange(len(cost_record))*10*self.batch_size,cost_record)
            plt.xlabel("iter")
            plt.ylabel("Cost")
            plt.title("Cost in training")
            plt.savefig("..//result//train_cost.png")
            plt.show()


            train,=plt.plot(np.arange(len(train_acc_record))*10*self.batch_size,train_acc_record)
            test,=plt.plot(np.arange(len(test_acc_record))*10*self.batch_size,test_acc_record)
            plt.xlabel("iter")
            plt.ylabel("accuracy")
            plt.title("train_test_accuracy")
            plt.legend([train,test],["train","test"],loc="best")
            plt.savefig("..//result//train_test_acc.png")
            plt.show()

        else:
            pass





    def forward_propagation(self,X=None,dropout=None):
        A=X
        caches=[]
        Y=None
        layer_num=len(self.parameters)//2

        ### for the layers from 1 to L-1, we use 'relu' activation func

        if (dropout==None):
            for i in range(1,layer_num):
                A_prev=A
                Z=np.dot(self.parameters['W'+str(i)],A_prev)+self.parameters['b'+str(i)]
                ### use relu
                A=(Z>0)*Z
                dA_dZ=Z>0
                ### store the info for the backward propagation
                caches.append((A_prev,dA_dZ))

            ### for the layer L, we use 'sigmoid' activation func
            Z=np.dot(self.parameters['W'+str(layer_num)],A)+self.parameters['b'+str(layer_num)]
            Y = 1./(1+np.exp(-Z))
            caches.append((A,Y*(1-Y)))

        else:
            for i in range(1,layer_num):
                A_prev=A
                if('d'+str(i) in dropout):
                    prob = dropout['d' + str(i)]
                    mask=np.random.binomial(1,prob,A_prev.shape)
                    A_prev = 1/prob*A_prev*mask
                    self.masks['mask_' + str(i)]=mask
                Z = np.dot(self.parameters['W' + str(i)], A_prev) + self.parameters['b' + str(i)]
                ### use relu
                A = (Z > 0) * Z
                dA_dZ = (Z > 0)
                ### store the info for the backward propagation
                caches.append((A_prev, dA_dZ))

             ### for the layer L, we use 'sigmoid' activation func
            Z = np.dot(self.parameters['W' + str(layer_num)], A) + self.parameters['b' + str(layer_num)]
            Y = 1. / (1 + np.exp(-Z))
            caches.append((A, Y * (1 - Y)))


        return Y,caches


    def backward_propagation(self,dZ,caches):
        dA_prev=dZ
        grads={}
        layer_num=len(self.parameters)//2
        if(self.dropout==None):
            for i in reversed(range(1,layer_num+1)):
                A_prev,dA_dZ=caches[i-1]
                dZ=dA_prev*dA_dZ
                grads["dW"+str(i)]=1/self.batch_samples*np.dot(dZ,A_prev.T)
                grads['db'+str(i)]=1/self.batch_samples*np.sum(dZ,axis=1,keepdims=True)
                dA_prev=np.dot(self.parameters['W'+str(i)].T,dZ)
        else:
            for i in reversed(range(1,layer_num+1)):
                A_prev,dA_dZ=caches[i-1]
                dZ=dA_prev*dA_dZ
                grads["dW"+str(i)]=1/self.batch_samples*np.dot(dZ,A_prev.T)
                grads['db'+str(i)]=1/self.batch_samples*np.sum(dZ,axis=1,keepdims=True)
                dA_prev=np.dot(self.parameters['W'+str(i)].T,dZ)
                if ('mask_' + str(i) in self.masks):
                    mask = self.masks['mask_' + str(i)]
                    dA_prev = dA_prev * mask
        return grads


    def update_parameters(self,grads):
        for i in range(1,len(self.parameters)//2+1):
            self.parameters['W'+str(i)]-=self.lr*grads['dW'+str(i)]
            self.parameters['b'+str(i)]-=self.lr*grads['db'+str(i)]
        return


    def predict(self,test_X):
        Y,caches=self.forward_propagation(test_X,dropout=None)
        del caches
        return Y

    def save_model(self):
        sio.savemat("..\\result\\weights.mat",self.parameters)
        return

