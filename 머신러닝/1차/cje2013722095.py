# -*- coding: utf-8 -*-
"""
Created on Wed Mar 15 21:20:00 2017

@author: minsooyeo / minsooyeo119112@gmail.com
"""

import numpy as np


def ch_label(label, label_str=["'setosa'", "'versicolor'", "'virginica'"]):
    data_point = len(label)
    # you should get this parameter correctly // 
    label_num = np.zeros([data_point, 1])
    ## your code here
    for i in range(data_point):
        if (label[i] == label_str[0]):
            label_num[i] = 0
        elif (label[i] == label_str[1]):
            label_num[i] = 1
        else:
            label_num[i] = 2
    ## end
    return label_num


def feature_normalization(data):
    # parameter 
    feature_num = data.shape[1]
    data_point = data.shape[0]

    # you should get this parameter correctly
    nomal_feature = np.zeros([data_point, feature_num])
    mu = np.zeros([1, feature_num])
    std = np.zeros([1, feature_num])
    ## your code here
    for col in range(feature_num):
        var = []
        for low in range(data_point):
            var.append(data[low][col])
        mu[0][col] = np.mean(var)
        std[0][col] = np.std(var)

    for low in range(data_point):
        for col in range(feature_num):
            nomal_feature[low][col] = (data[low][col] - mu[0][col])/std[0][col]
    ## end
    return nomal_feature, mu, std


def spilt_data(data, label, spilt_factor):
    # you should get this parameter correctly
    feature_num = data.shape[1]
    data_point = data.shape[0]
    train_data = np.zeros([spilt_factor, feature_num])
    train_label = np.zeros([spilt_factor, 1])
    test_data = np.zeros([data_point - spilt_factor, feature_num])
    test_label = np.zeros([data_point - spilt_factor, 1])
    ## your code here
    train_num = [i for i in range(spilt_factor)]
    test_num = [i for i in range(spilt_factor, len(label))]
    train_data = data[train_num, :]
    test_data = data[test_num, :]
    train_label = label[train_num]
    test_label = label[test_num]
    ## end
    return train_data, test_data, train_label, test_label


def get_normal_parameter(train_data, train_label, lable_num):
    ## parameter
    feature_num = train_data.shape[1]
    ## you should get this parameter correctly
    mu = np.zeros([lable_num, feature_num])
    sigma = np.zeros([lable_num, feature_num])
    val = []
    ## your code here
    for low in range(lable_num):
        p = np.where(train_label == low)
        for col in range(feature_num):
            val = []
            for i in range(len(p[0])):
                val.append(train_data[p[0][i], [col]][0])
            mu[low, col] = np.mean(val)
            sigma[low, col] = np.std(val)


    ## end
    return mu, sigma


def prob(mu, sigma, data, label):
    ## parameter
    data_point = data.shape[0]
    lable_num = mu.shape[0]
    ## you should get this parameter correctly
    prob = np.zeros([data_point, lable_num])
    pi = np.zeros([lable_num, 1])
    ## your code here
    for i in range(lable_num):
        p = np.where(label == i)
        pi[i][0] = len(p[0]) / data_point

    for DC in range(data_point):
        for low in range(lable_num):
            val = 0
            for col in range(4):
                down = (2 * (sigma[low][col] ** 2) * 3.141592) ** .5
                up = np.exp(-((data[DC][col] - mu[low][col]) ** 2) / (2 * ((sigma[low][col]) ** 2)))
                val = val + np.log(up / down)
            prob[DC, low] = val + np.log(pi[low][0])
    ## end
    return prob, pi


def classifier(prob):
    ## parameter
    datacount = prob.shape[0]
    ## you should get this parameter correctly
    label = np.zeros([datacount])
    ## your code here
    mv = []
    for i in range(datacount):
        mv.append(np.max(prob[i,:]))
        lable = np.where(prob[i,:] == mv[i])
        label[i] = lable[0]
    ## end
    return label


def acc(est, gnd):
    ## parameter
    total_num = len(gnd)
    acc = 0
    for i in range(total_num):
        if (est[i] == gnd[i]):
            acc = acc + 1
        else:
            acc = acc;
    return (acc / total_num) * 100, acc