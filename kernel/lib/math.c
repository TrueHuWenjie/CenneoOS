/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Math Library
 * Explorer/lib/math.c
 * version:Alpha
 * 12/6/2014 11:10 AM
 */

#include <lib/math.h>

/**取一个有符号数的绝对值函数*/
long abs(long number)
{
	if (number < 0) return number * (-1);
	return number;
}

/*
 * max - 求最大值
 * @a: 第一个值
 * @b: 第二个值
 * 
 * 返回二者的大值
 */
int max(int a, int b)
{
	return a>b?a:b;
}

/*
 * min - 求最大值
 * @a: 第一个值
 * @b: 第二个值
 * 
 * 返回二者的小值
 */
int min(int a, int b)
{
	return a<b?a:b;
}

/*
 * pow - 求指数
 * @x: 底数
 * @y: 指数
 */
int pow(int x, int y)
{
	int r = 1;
	while(y > 1){
		if((y&1) == 1){
			r *= x;
		}
		x *= x;
		y >>= 1;
	}
	return r * x;
}
