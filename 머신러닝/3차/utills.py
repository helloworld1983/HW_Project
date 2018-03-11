# -*- coding: utf-8 -*-
"""
Created on Tue May 30 16:43:10 2017
☜☜☜☜☜☜★☆★☆★☆★☆ provided code ★☆★☆★☆★☆☞☞☞☞☞☞
@author: Minsooyeo
"""
import os
import tensorflow as tf
import numpy as np


def search(dirname):
    filenames = os.listdir(dirname)
    for filename in filenames:
        full_filename = os.path.join(dirname, filename)
        return filenames

def weight_variable(shape):
       initial = tf.truncated_normal(shape, stddev=0.1)
       return tf.Variable(initial)

def bias_variable(shape):
       initial = tf.constant(0.1, shape=shape)
       return tf.Variable(initial)

def conv2d(x, W):
    return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')

def max_pool_2x2(x):
    return tf.nn.max_pool(x, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')

def Nextbatch(data, label, batchsize):
    #index = np.arange(0, len(data))
    #np.random.shuffle(index)
    #index = index[0:batchsize]
    #Ran_data가 [0:50] / [0:2880] 형의 array로 나가야 됨 ㅇㅅㅇ

    #Ran_data = [data[a] for a in index]
    #Ran_label = [label[a] for a in index]

    #Xarray = np.asarray(Ran_data).reshape(50, 2880)
    #Yarray = np.asarray(Ran_label)

    x = np.random.permutation(len(data))[:batchsize]
    Xarray = np.asarray(data[x]).reshape(50,2880)
    Yarray = np.asarray(label[x])
    return   Xarray, Yarray # list를 array로 변환해서 반환

def _DropOut(model):
    keep_prob = tf.placeholder("float32")
    Drop = tf.nn.dropout(model, keep_prob)
    print(Drop)
    return (Drop, keep_prob)

def _CNNModel(value1):
    x = tf.placeholder("float32", [None, value1])
    x_img = tf.reshape(x, [-1, 72, 40, 1])

    W1 = weight_variable([3, 3, 1, 32])
    C1 = conv2d(x_img, W1)
    W1 = weight_variable([3, 3, 32, 32])
    Conv1 = conv2d(C1, W1)
    B1 = bias_variable([32])
    add_bias1 = tf.nn.bias_add(Conv1, B1)
    L1 = tf.nn.relu(add_bias1)
    P1 = max_pool_2x2(L1)


    W2 = weight_variable([3, 3, 32, 64])
    C2 = conv2d(P1, W2)
    W2 = weight_variable([3, 3, 64, 64])
    Conv2 = conv2d(C2, W2)
    B2 = bias_variable([64])
    add_bias2 = tf.nn.bias_add(Conv2, B2)
    L2 = tf.nn.relu(add_bias2)
    P2 = max_pool_2x2(L2)

    W3 = weight_variable([3, 3, 64, 64])
    C3 = conv2d(P2, W3)
    Conv3 = conv2d(C3, W3)
    B3 = bias_variable([64])
    add_bias3= tf.nn.bias_add(Conv3, B3)
    L3 = tf.nn.relu(add_bias3)
    P3 = max_pool_2x2(L3)

    W4 = weight_variable([3, 3, 64, 128])
    B4 = bias_variable([128])
    C4 = conv2d(P3, W4)
    W4 = weight_variable([3, 3, 128, 128])
    Conv4 = conv2d(C4, W4)
    add_bias4 = tf.nn.bias_add(Conv4, B4)
    L4 = tf.nn.relu(add_bias4)
    P4 = max_pool_2x2(L4)
    normal2 = tf.nn.lrn(P4, 4, bias=1.0, alpha=0.001 / 9.0, beta=0.75)

    print(normal2)

    return normal2, x

def _FlatModel(model):
    w = weight_variable([5 * 3 * 128, 128])
    b = bias_variable([128])
    flat = tf.reshape(model, [-1, 5*3*128])

    RL = tf.nn.relu(tf.matmul(flat, w) + b)
    print(RL)
    return RL

def _SoftMax(Model):
    w_sm = weight_variable([128, 5])
    b_sm = bias_variable([5])
    SM = tf.nn.softmax(tf.matmul(Model, w_sm) + b_sm)
    print(SM)
    return SM

def _SetAccuracy(SoftMaxModel, k):
    y_ = tf.placeholder("float32", [None, k])
    cross_entropy = -tf.reduce_sum(y_ * tf.log(SoftMaxModel))
    train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)
    correct_prediction = tf.equal(tf.argmax(SoftMaxModel, 1), tf.argmax(y_, 1))
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float32"))
    return train_step, accuracy, y_, cross_entropy
