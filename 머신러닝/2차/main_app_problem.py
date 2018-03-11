# -*- coding: utf-8 -*-
"""
Created on Fri May 26 00:14:24 2017

@author: Minsooyeo
"""

from tensorflow.examples.tutorials.mnist import input_data
import tensorflow as tf
import numpy as np
import utills as utills

train_epoch = 5000
## load data
sess = tf.InteractiveSession()       
mnist = input_data.read_data_sets('MNIST_data', one_hot=True)
TrainX = mnist.train.images
TrainY = mnist.train.labels
testX = mnist.test.images
testY = mnist.test.labels

CNNModel, x = utills._CNNModel(784)
FlatModel = utills._FlatModel(CNNModel)
DropOut, keep_prob = utills._DropOut(FlatModel)
SoftMaxModel = utills._SoftMax(DropOut)
TrainStep, Accuracy, y_, correct_prediction = utills._SetAccuracy(SoftMaxModel, 10)

sess.run(tf.global_variables_initializer())

for i in range(train_epoch):
  tmp_trainX, tmp_trainY = utills.Nextbatch(TrainX, TrainY, 50)
  if i%100 == 0:
    train_accuracy = Accuracy.eval(feed_dict={
        x:tmp_trainX, y_: tmp_trainY, keep_prob: 1.0})
    print("step %d, training accuracy %g"%(i, train_accuracy))
  TrainStep.run(feed_dict={x: tmp_trainX, y_: tmp_trainY, keep_prob: 0.5})

print("test accuracy %g"%Accuracy.eval(feed_dict={
    x:testX[1:1000,:], y_: testY[1:1000], keep_prob: 1.0}))