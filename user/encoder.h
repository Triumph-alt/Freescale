#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "main.h"

#define  LEFT_DIR    P35
#define  RIGHT_DIR   P53

extern int g_EncoderLeft, g_EncoderRight;

void encoder_init(void);
int get_left_encoder(void);
int get_right_encoder(void);

#endif