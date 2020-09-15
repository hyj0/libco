/*
* Tencent is pleased to support the open source community by making Libco available.

* Copyright (C) 2014 THL A29 Limited, a Tencent company. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/


#ifndef __CO_ROUTINE_INNER_H__

#include "co_routine.h"
#include "coctx.h"
struct stCoRoutineEnv_t;
typedef struct stCoSpec_t
{
	void *value;
}stCoSpec_t;

typedef struct stStackMem_t
{
    struct stCoRoutine_t* occupy_co;
	int stack_size;
	char* stack_bp; //stack_buffer + stack_size
	char* stack_buffer;

}stStackMem_t;

typedef struct stShareStack_t
{
	unsigned int alloc_idx;
	int stack_size;
	int count;
	stStackMem_t** stack_array;
}stShareStack_t;


typedef struct stCoRoutine_t
{
    struct stCoRoutineEnv_t *env;
	pfn_co_routine_t pfn;
	void *arg;
    struct coctx_t ctx;

	char cStart;
	char cEnd;
	char cIsMain;
	char cEnableSysHook;
	char cIsShareStack;

	void *pvEnv;

	//char sRunStack[ 1024 * 128 ];
	stStackMem_t* stack_mem;


	//save satck buffer while confilct on same stack_buffer;
	char* stack_sp; 
	unsigned int save_size;
	char* save_buffer;

    struct stCoSpec_t aSpec[1024];

}stCoRoutine_t;


#ifdef __cplusplus
extern "C" {
#endif
//1.env
void 				co_init_curr_thread_env();
struct stCoRoutineEnv_t *	co_get_curr_thread_env();

//2.coroutine
void    co_free( stCoRoutine_t * co );
void    co_yield_env(struct  stCoRoutineEnv_t *env );

//3.func



//-----------------------------------------------------------------------------------------------

struct stTimeout_t;
struct stTimeoutItem_t ;

struct stTimeout_t *AllocTimeout( int iSize );
void 	FreeTimeout(struct stTimeout_t *apTimeout );
int  	AddTimeout(struct stTimeout_t *apTimeout, struct stTimeoutItem_t *apItem ,uint64_t allNow );

struct stCoEpoll_t;
struct stCoEpoll_t * AllocEpoll();
void 		FreeEpoll(struct stCoEpoll_t *ctx );

stCoRoutine_t *		GetCurrThreadCo();
void 				SetEpoll(struct stCoRoutineEnv_t *env,struct stCoEpoll_t *ev );

typedef void (*pfnCoRoutineFunc_t)();
#ifdef __cplusplus
}
#endif
#endif

#define __CO_ROUTINE_INNER_H__
