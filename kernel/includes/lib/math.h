/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Math Library Definition
 * Explorer/include/lib/math.h
 * version:Alpha
 * 11/1/2014 12:09 AM
 */

#ifndef MATH_H_
#define MATH_H_

/**取一个有符号数的绝对值函数*/
long abs(long number);

/*
 * max - 求最大值
 * @a: 第一个值
 * @b: 第二个值
 * 
 * 返回二者的大值
 */
int max(int a, int b);

/*
 * min - 求最大值
 * @a: 第一个值
 * @b: 第二个值
 * 
 * 返回二者的小值
 */
int min(int a, int b);

int pow(int x, int y);

#endif