#encoding: utf-8
"""
@project=Deep Neural Network
@file=FCN_main.py
@author="QHX"
@create_time=2018/10/29 14:14
"""

import scipy.io as sio
import numpy as np
import os
from PIL import Image
from sklearn.decomposition import PCA
from FCN import NN


def read_dir(path):
    X=[]
    Y=[]
    img_list=os.listdir(path)
    m=len(img_list)
    for i in range(m):
        img=Image.open(os.path.join(path,img_list[i]))
        img_arr=np.asarray(img)/255.0
        X.append(np.ndarray.flatten(img_arr))
        Y.append(int("label_1" in img_list[i]))
    X=np.array(X).T
    Y=np.array(Y)
    #print(X.shape,Y.shape)
    return X,Y


def load_data_from_dir():
    train_path="..\\data\\train"
    test_path="..\\data\\test"
    train_X,train_Y=read_dir(train_path)
    test_X,test_Y=read_dir(test_path)
    sio.savemat("..\\data\\data_1024.mat",{"train_X":train_X,"train_Y":train_Y,"test_X":test_X,"test_Y":test_Y})
    return train_X,train_Y,test_X,test_Y


def load_data_from_mat():
    data=sio.loadmat("..\\data\\data_1024.mat")
    return data["train_X"],data["train_Y"],data["test_X"],data["test_Y"]


if __name__=="__main__":

    #train_X,train_Y,test_X,test_Y=load_data_from_dir()
    train_X, train_Y, test_X, test_Y = load_data_from_mat()
    #print(train_X.shape,train_Y.shape,test_X.shape,test_Y.shape)

    dimension_reduction=False
    if dimension_reduction:
        transformer=PCA(n_components=800)
        transformer.fit(train_X.T)
        train_X=transformer.transform(train_X.T).T
        test_X=transformer.transform(test_X.T).T
        sio.savemat("..\\data\\data_800.mat", {"train_X": train_X, "train_Y": train_Y, "test_X": test_X, "test_Y": test_Y})
    else:
        pass

    print(train_X.shape, train_Y.shape, test_X.shape, test_Y.shape)
    nn=NN(X=train_X,Y=train_Y,test_X=test_X,test_Y=test_Y,input_dims=[1024,128,32,32,4,1],dropout=None,lr=0.05,epoch=600,batch_size=256,use_batch=True)
    nn.fit()

    train_predict=nn.predict(train_X)
    print("train_Accuracy:{}".format(np.mean((train_predict>0.5)==train_Y)))
    test_predict=nn.predict(test_X)
    print("test_Accuracy:{}".format(np.mean((test_predict>0.5)==test_Y)))


