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

#include "co_routine_specific.h"
#include "co_routine.h"
#include "co_routine_inner.h"
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;
struct stRoutineArgs_t
{
	stCoRoutine_t* co;
	int routine_id;
};
struct stRoutineSpecificData_t
{
	int idx;
};

#define __routine \
ROUTINE_VAR(stRoutineSpecificData_t, __routine)
ROUTINE_DEF(stRoutineSpecificData_t, __routine)

#define __routine1 \
ROUTINE_VAR(stRoutineSpecificData_t, __routine1)
ROUTINE_DEF(stRoutineSpecificData_t, __routine1)

void* RoutineFunc(void* args)
{
	co_enable_hook_sys();
    stCoRoutine_t *co = GetCurrThreadCo();
	stRoutineArgs_t* routine_args = (stRoutineArgs_t*)args;
	__routine.idx = routine_args->routine_id;
	__routine1.idx = __routine.idx+1;
    while (1)
    {
        __routine1.idx = __routine1.idx + 1;
        printf("%s:%d routine specific data idx %d  %d\n", __func__, __LINE__, __routine.idx, __routine1.idx);
		poll(NULL, 0, 1000);
	}
	return NULL;
}
int main()
{
	stRoutineArgs_t args[10];
	for (int i = 0; i < 10; i++)
	{
		args[i].routine_id = i;
		co_create(&args[i].co, NULL, RoutineFunc, (void*)&args[i]);
		co_resume(args[i].co);
	}
	co_eventloop(co_get_epoll_ct(), NULL, NULL);
	return 0;
}
