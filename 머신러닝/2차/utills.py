# -*- coding: utf-8 -*-
"""
Created on Fri May 26 00:15:19 2017

@author: Minsooyeo
"""
import tensorflow as tf
import numpy as np


def weight_variable(shape):
    initial = tf.truncated_normal(shape, stddev=0.1)
    return tf.Variable(initial)


def bias_variable(shape):
    initial = tf.constant(0.1, shape=shape)
    return tf.Variable(initial)


def conv2d(x, W):
    return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='VALID')


def max_pool_2x2(x):
    return tf.nn.max_pool(x, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='VALID')


def Nextbatch(data, label, batchsize):
    index = np.arange(0, len(data))
    np.random.shuffle(index)
    index = index[0:batchsize]
    Ran_data = [data[a] for a in index]
    Ran_label = [label[a] for a in index]
    return np.asarray(Ran_data), np.asarray(Ran_label) # list를 array로 변환해서 반환


def _DropOut(model):
    keep_prob = tf.placeholder("float32")
    Drop = tf.nn.dropout(model, keep_prob)
    print(Drop)
    print(keep_prob)
    return ( Drop, keep_prob)


def _CNNModel(value):
    x = tf.placeholder("float32", [None, value])
    x_img = tf.reshape(x, [-1, 28, 28, 1])

    W1 = weight_variable([5, 5, 1, 32])
    B1 = bias_variable([32])
    L1 = tf.nn.relu(conv2d(x_img, W1) + B1)
    P1 = max_pool_2x2(L1)

    W2 = weight_variable([5, 5, 32, 32])
    B2 = bias_variable([32])
    L2 = tf.nn.relu(conv2d(P1, W2) + B2)
    P2 = max_pool_2x2(L2)
    print(P2)

    return P2, x

#info.pdf 20 page보면 flat layer로 들어가는게 4.4.32던데 그 값이 나오려면 conv, pooling 둘다 패딩 안해야 함 ;;;

def _FlatModel(model):

    w = weight_variable([4*4*32, 256])
    b = bias_variable([256])
    flat = tf.reshape(model, [-1, 4*4*32])

    RL = tf.nn.relu(tf.matmul(flat, w) + b)
    print(RL)
    return RL


def _SoftMax(Model):
    w_sm = weight_variable([256, 10])
    b_sm = bias_variable([10])
    SM = tf.nn.softmax(tf.matmul(Model, w_sm) + b_sm)
    print(SM)
    return SM


def _SetAccuracy(SoftMaxModel, k):
    y_ = tf.placeholder("float32",[None, k])
    cross_entropy = -tf.reduce_sum(y_ * tf.log(SoftMaxModel))
    train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)
    correct_prediction = tf.equal(tf.argmax(SoftMaxModel, 1), tf.argmax(y_, 1))
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float32"))
    return (train_step, accuracy, y_, cross_entropy)

