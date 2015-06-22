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

#endif // !_DEFINE_H
