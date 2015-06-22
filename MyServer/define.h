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
	ECHO = 1,				//回显
	SUPRESS = 3,			//抑制继续进行
	STATUS = 5,				//状态
	CLOCK = 6,				//定时标记
	TERMINAL_TYPE = 24,		//终端类型
	WINDOWS_SIZE = 31,		//窗口大小
	TERMINAL_SPEED = 32,	//终端速度
	FLOW_CONTROL = 33,		//远程流量控制
	LINE_TYPE = 34,			//行方式
	ENVIRONMENT = 36,		//环境变量 
};

//#define _DEBUG_MODE

#endif // !_DEFINE_H
