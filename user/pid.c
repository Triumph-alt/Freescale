#include "pid.h"

PID_t LeftPID = {0.00035, 0.0042, 0.0, 0, 0};       //�����ٶȻ�PID
PID_t RightPID = {0.00030, 0.0050, 0.0, 0, 0};      //�����ٶȻ�PID

PID_t IMU693PID = {0.6, 0.0, 0.89, 800, 900};        //������PID


//λ��ʽPID
float pid_poisitional(PID_t* pid, float now, float point)
{
	static int integral = 0, lasterror = 0;
	int error = 0;
	float realise = 0;
	
	error = point - now;
	integral += error;
	
	//�����޷�
	if (integral > pid->i_limit)
		integral = pid->i_limit;
	else if (integral < -pid->i_limit)
		integral = -pid->i_limit;
	
	realise = pid->kp * error + pid->ki * integral + pid->kd * (error - lasterror);
	lasterror = error;
	
	//����޷�
	if (realise > pid->o_limit)
		realise = pid->o_limit;
	else if (realise < -pid->o_limit)
		realise = -pid->o_limit;
	
	return realise;
}


//����ʽPID
float pid_increment(PID_t* pid, int now, int point)
{
	static int lasterror = 0, preverror = 0;
	int error;
	float increase;
	
	error = point - now;
	
	increase = pid->kp * (error - lasterror) + pid->ki * error + pid->kd * (error - 2 * lasterror + preverror);
	
	preverror = lasterror;
	lasterror = error;
	
	return increase;
}