/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Debug Output Definition
 * Explorer/include/debug.h
 * version:Alpha
 * 12/6/2014 7:13 PM
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#define DEBUG_PART


#ifdef DEBUG_PART
    #define PART_START(msg) printk("-> "msg" ")
    #define PART_END() printk("<-\n")
    #define PART_TIP " |- "
    #define PART_WARRING " ^- "
    #define PART_ERROR " !- "
#else
    #define PART_START(msg) 
    #define PART_END() 
    #define PART_TIP 
    #define PART_WARRING
    #define PART_ERROR
#endif

#define ERROR_BAKEGROUND_COLOR	0xff40B0ff
#define ERROR_FOREGROUND_COLOR	0xffffffff

//断言
#define CONFIG_ASSERT
#ifdef CONFIG_ASSERT
void AssertionFailure(char *exp, char *file, char *baseFile, int line);
#define ASSERT(exp)  if (exp) ; \
        else debug("%s in file %s in base file %s in line %d", #exp, __FILE__, __BASE_FILE__, __LINE__)
        //else AssertionFailure(#exp, __FILE__, __BASE_FILE__, __LINE__)
#else
#define ASSERT(exp)
#endif

#define DEBUG debug("\n----------Debug Info----------\nFile:     %s\nFunction: %s\nLine:     %d\nCompiled in %s %s\n------------------------------\n", __FILE__, __func__, __LINE__, __DATE__, __TIME__);


void abort(void);
int fflush(void *p);

#endif