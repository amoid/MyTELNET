#pragma once

#ifndef _DEFINE_H

#define _DEFINE_H

#define	MAX_BUFFER_SIZE	10000
/*
#define	SE		240
#define	SB		250
#define	IAC		255
#define	WILL	251
#define	DO		253
#define	WONT	252
#define	DONT	254
*/
typedef struct _IACCommand
{
	unsigned char iac = 255;
	unsigned char command;
	unsigned char option;
} IACCommand;

enum Command
{
	DO = 253,
	WILL = 251,
	WONT = 252,
	DONT = 254,
	IAC = 255,
	SE = 240,
	SB = 250
};

enum Option
{
	ECHO = 1,				//����
	SUPRESS = 3,			//���Ƽ�������
	STATUS = 5,				//״̬
	CLOCK = 6,				//��ʱ���
	TERMINAL_TYPE = 24,		//�ն�����
	WINDOWS_SIZE = 31,		//���ڴ�С
	TERMINAL_SPEED = 32,	//�ն��ٶ�
	FLOW_CONTROL = 33,		//Զ����������
	LINE_TYPE = 34,			//�з�ʽ
	ENVIRONMENT = 36,		//�������� 
};

//#define _DEBUG_MODE

#endif // !_DEFINE_H
