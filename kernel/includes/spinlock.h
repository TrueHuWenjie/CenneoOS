/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Spin Lock Definition
 * Explorer/include/spinlock.h
 * version:Alpha
 * 12/5/2014 11:47 PM
 */

#ifndef SPIN_LOCK_H_

#include <types.h>

/* 原子变量结构体 */
typedef struct {
   int value;   // 变量的值
}Atomic_t;

#define AtomicGet(atomic)	((atomic)->value)
#define AtomicSet(atomic,val)	(((atomic)->value) = (val))
#define AtomicGet(atomic)	((atomic)->value)
#define AtomicSet(atomic,val)	(((atomic)->value) = (val))

#define ATOMIC_INIT(val)	{val}

/**
 * AtomicAdd - 原子加运算
 * @atomic: 原子结构
 * @value: 数值
 */
PRIVATE INLINE void AtomicAdd(Atomic_t *atomic, int value)
{
   __AtomicAdd(&atomic->value, value);
}

/**
 * AtomicSub - 原子减运算
 * @atomic: 原子结构
 * @value: 数值
 */
PRIVATE INLINE void AtomicSub(Atomic_t *atomic, int value)
{
   __AtomicSub(&atomic->value, value);
}


/**
 * AtomicInc - 原子增运算
 * @atomic: 原子结构
 */
PRIVATE INLINE void AtomicInc(Atomic_t *atomic)
{
   __AtomicInc(&atomic->value);
}

/**
 * AtomicDec - 原子减运算
 * @atomic: 原子结构
 */
PRIVATE INLINE void AtomicDec(Atomic_t *atomic)
{
   __AtomicDec(&atomic->value);
}

/**
 * AtomicSetMask - 设置位
 * @atomic: 原子结构
 * @mask: 位值
 */
PRIVATE INLINE void AtomicSetMask(Atomic_t *atomic, int mask)
{
   __AtomicOr(&atomic->value, mask);
}

/**
 * AtomicClearMask - 清除位
 * @atomic: 原子结构
 * @mask: 位值
 */
PRIVATE INLINE void AtomicClearMask(Atomic_t *atomic, int mask)
{
   __AtomicAnd(&atomic->value, ~mask);
}

#define ATOMIC_XCHG(v, new) (XCHG(&((v)->value), new))

	typedef unsigned long spinlock_t;

	#define SPIN_LOCK_LOCKED	0
	#define SPIN_LOCK_UNLOCKED	1
	
	void spin_lock_init(spinlock_t *lock);
	
	void spin_lock(spinlock_t *lock);
	void spin_lock_irqsave(spinlock_t *lock, unsigned long flags);
	void spin_lock_irq(spinlock_t *lock);
	void spin_lock_bh(spinlock_t *lock);

	void spin_unlock(spinlock_t *lock);
	void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags);
	void spin_unlock_irq(spinlock_t *lock);
	void spin_unlock_bh(spinlock_t *lock);

#define SPIN_LOCK_H_

//#define SPINLOCK_DEBUG

typedef struct Spinlock {
    Atomic_t lock; /* 锁变量是原子，为0表示空闲，大于等于0表示已经被使用 */
} Spinlock_t;

/* 初始化未上锁的自旋锁 */
#define SPIN_LOCK_INIT_UNLOCKED(lockname) \
        { .lock = ATOMIC_INIT(0) }

/* 初始化上锁的自旋锁 */
#define SPIN_LOCK_INIT_LOCKED(lockname) \
        { .lock = ATOMIC_INIT(1) }

/* 初始化自旋锁，默认是未上锁的自旋锁 */
#define SPIN_LOCK_INIT(lockname) \
        SPIN_LOCK_INIT_UNLOCKED(lockname) 


PUBLIC void SpinLockInit(struct Spinlock *lock);

PUBLIC void SpinLock(struct Spinlock *lock);
PUBLIC void SpinUnlock(struct Spinlock *lock);

PUBLIC enum InterruptStatus SpinLockSaveIntrrupt(struct Spinlock *lock);
PUBLIC void SpinUnlockRestoreInterrupt(struct Spinlock *lock, enum InterruptStatus oldStatus);

PUBLIC void SpinLockIntrrupt(struct Spinlock *lock);
PUBLIC void SpinUnlockInterrupt(struct Spinlock *lock);

PUBLIC int SpinTryLock(struct Spinlock *lock);
PUBLIC int SpinIsLocked(struct Spinlock *lock);


PUBLIC unsigned long SpinLockIrqSave(struct Spinlock *lock);
PUBLIC void SpinUnlockIrqSave(struct Spinlock *lock, unsigned long eflags);

#endif