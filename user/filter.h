#ifndef __FILTER_H
#define __FILTER_H

#include "main.h"


typedef struct {
    float F;        // ״̬ת��ϵ����ϵͳ����ѧ��
    float B;        // ��������ϵ��
    float Q;        // ��������Э����
    float R;        // ��������Э����
    float P;        // �������Э����
    float K;        // ����������
    float x;        // ״̬����ֵ��gyroz��
} KalmanFilter;


extern const float imu693kf_Q;
extern const float imu693kf_R;


extern KalmanFilter imu693_kf;


void Kalman_Init(KalmanFilter* kf, float F, float B, float Q, float R, float initial_x);
void Kalman_Predict(KalmanFilter* kf, float u);
float Kalman_Update(KalmanFilter* kf, float z);


#endif