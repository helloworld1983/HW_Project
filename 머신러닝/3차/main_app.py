# -*- coding: utf-8 -*-
"""
Created on Tue May 30 16:43:10 2017
☜☜☜☜☜☜★☆★☆★☆★☆ provided code ★☆★☆★☆★☆☞☞☞☞☞☞
@author: Minsooyeo
"""

import os
import matplotlib.image as mpimg
import matplotlib.pyplot as plt
from PIL import Image as im
import numpy as np
import tensorflow as tf
import utills as ut

sess = tf.InteractiveSession()
train_epoch = 1000
# define parameter
class_num = 5
data_length = []
dir_image = []
data = []
label = []
data_shape = [72, 40]
current_pwd = os.getcwd()
for i in range(class_num):
    dir_image.append(ut.search(current_pwd + '\\' + str(i+1)))
    data_length.append(len(dir_image[i]))
    data.append(np.zeros([data_length[i], data_shape[1], data_shape[0]]))
    label.append(np.zeros([data_length[i], class_num]))
    label[i][:,i] = 1
    
# load data
for q in range(class_num):
    for i in range(data_length[q]):
        data[q][i,:,:] = np.mean(im.open(current_pwd + '\\' + str(q+1) + '\\' + dir_image[q][i]), -1)

rawdata = np.concatenate((data[0], data[1], data[2], data[3], data[4]),axis=0)
del data
raw_label = np.concatenate((label[0], label[1], label[2], label[3], label[4]),axis=0)
del label

# under : my code (CJE)

total_data_poin = rawdata.shape[0]
permutation = np.random.permutation(total_data_poin)
rawdata = rawdata[permutation,:,:]
raw_label = raw_label[permutation,:]


# Train/Test용 data, label들을 분류함
Train_data = rawdata[:2000]
Train_label = raw_label[:2000]
Test_data = rawdata[2000:2490]
Test_label = raw_label[2000:2490]

CNNModel, x = ut._CNNModel(72*40)
FlatModel = ut._FlatModel(CNNModel)
DropOut, keep_prob = ut._DropOut(FlatModel)
SoftMaxModel = ut._SoftMax(DropOut)
TrainStep, Accuracy, y_, correct_prediction = ut._SetAccuracy(SoftMaxModel, 5)

sess.run(tf.global_variables_initializer())

for i in range(train_epoch):
  tmp_trainX, tmp_trainY = ut.Nextbatch(Train_data, Train_label, 50)

  #if (i%100 == 0):
  train_accuracy = Accuracy.eval(feed_dict={
        x: tmp_trainX,
        y_: tmp_trainY,
        keep_prob: 1.0})
  print("step %d, training accuracy %g"%(i, train_accuracy))
  TrainStep.run(feed_dict={x: tmp_trainX, y_: tmp_trainY, keep_prob: 0.67})

Test_x = np.asarray(Test_data).reshape(len(Test_data),2880)
Test_y = np.asarray(Test_label)
print("test accuracy %g"%Accuracy.eval(feed_dict={
    x: Test_x, y_: Test_y, keep_prob: 1.0}))
