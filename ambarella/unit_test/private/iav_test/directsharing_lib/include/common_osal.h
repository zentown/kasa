/**
 * common_osal.h
 *
 * History:
 *  2012/12/07 - [Zhi He] create file
 *
 * Copyright (C) 2014 - 2024, the Ambarella Inc.
 *
 * All rights reserved. No Part of this file may be reproduced, stored
 * in a retrieval system, or transmitted, in any form, or by any means,
 * electronic, mechanical, photocopying, recording, or otherwise,
 * without the prior consent of the Ambarella Inc.
 */

#ifndef __COMMON_OSAL_H__
#define __COMMON_OSAL_H__

DCONFIG_COMPILE_OPTION_HEADERFILE_BEGIN

DCODE_DELIMITER;

//-----------------------------------------------------------------------
//
// IMutex
//
//-----------------------------------------------------------------------
class IMutex
{
public:
    virtual void Delete() = 0;

public:
    virtual void Lock() = 0;
    virtual void Unlock() = 0;

public:
    virtual void* GetContext() const = 0;
};

//-----------------------------------------------------------------------
//
// ICondition
//
//-----------------------------------------------------------------------
class ICondition
{
public:
    virtual void Delete() = 0;

public:
    virtual void Wait(IMutex *pMutex) = 0;
    //virtual EECode TimeoutWait(IMutex *pMutex, TU32 seconds) = 0;
    virtual void Signal() = 0;
    virtual void SignalAll() = 0;
};

//-----------------------------------------------------------------------
//
// CIAutoLock
//
//-----------------------------------------------------------------------
class CIAutoLock
{
public:
    CIAutoLock(IMutex *pMutex);
    ~CIAutoLock();

private:
    IMutex *_pMutex;
};

#define AUTO_LOCK(pMutex)	CIAutoLock __lock__(pMutex)
#define __LOCK(pMutex)		pMutex->Lock()
#define __UNLOCK(pMutex)	pMutex->Unlock()

//-----------------------------------------------------------------------
//
// IEvent
//
//-----------------------------------------------------------------------
class IEvent
{
public:
    virtual void Delete() = 0;

public:
    virtual EECode Wait(TInt ms = -1) = 0;
    virtual void Signal() = 0;
    virtual void Clear() = 0;
};

//-----------------------------------------------------------------------
//
// IThread
//
//-----------------------------------------------------------------------
enum {
    ESchedulePolicy_Other = 0,
    ESchedulePolicy_FIFO,
    ESchedulePolicy_RunRobin,
};

typedef EECode (*TF_THREAD_FUNC) (void*);
class IThread
{
public:
    virtual const TChar* Name() const = 0;
    virtual void Delete() = 0;
};

extern TInt __atomic_inc(TAtomic* value);
extern TInt __atomic_dec(TAtomic* value);

extern EECode gfGetCurrentDateTime(SDateTime* datetime);
extern TTime gfGetNTPTime();
extern void gfGetDateString(TChar* pbuffer, TUint size);

extern IMutex* gfCreateMutex();
extern ICondition* gfCreateCondition();
extern IEvent* gfCreateEvent();
extern IThread* gfCreateThread(const TChar *pName, TF_THREAD_FUNC entry, void *pParam, TUint schedule_policy = ESchedulePolicy_Other, TUint priority = 0, TUint affinity = 0);

extern EECode gfOSWritePipe(TSocketHandler fd, const TChar byte);
extern EECode gfOSReadPipe(TSocketHandler fd, TChar& byte);
extern EECode gfOSCreatePipe(TSocketHandler fd[2]);
extern void gfOSClosePipeFd(TSocketHandler fd);

extern EECode gfOSGetHostName(TChar* name, TU32 name_buffer_length);

extern EECode gfOSPollFdReadable(THandler fd);

extern void gfOSmsleep(TU32 ms);
extern void gfOSusleep(TU32 us);
extern void gfOSsleep(TU32 s);

extern void gfGetHostName(TChar* name, TU32 name_buffer_size);
extern void gfGetSTRFTimeString(TChar* time_string, TU32 time_string_buffer_length);

extern EECode gfGetSystemSettings(SSystemSettings* p_settings);
extern EECode gfGetSystemSoundInputDevices(SSystemSoundInputDevices* p_devices);

extern EECode gfInitPlatform();
extern void gfDeInitPlatform();

DCONFIG_COMPILE_OPTION_HEADERFILE_END

#endif
