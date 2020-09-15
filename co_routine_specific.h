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

#pragma once
#include <pthread.h>
#include <stdlib.h>

/*
invoke only once in the whole program
CoRoutineSetSpecificCallBack(CoRoutineGetSpecificFunc_t pfnGet,CoRoutineSetSpecificFunc_t pfnSet)

struct MyData_t
{
	int iValue;
	char szValue[100];
};
CO_ROUTINE_SPECIFIC( MyData_t,__routine );

int main()
{
	CoRoutineSetSpecificCallBack( co_getspecific,co_setspecific );

	__routine->iValue = 10;
	strcpy( __routine->szValue,"hello world" );

	return 0;
}
*/
/*extern int 	co_setspecific( pthread_key_t key, const void *value );
extern void *	co_getspecific( pthread_key_t key );*/

#define CO_ROUTINE_SPECIFIC( name,y ) \
\
static pthread_once_t _routine_once_##name = PTHREAD_ONCE_INIT; \
static pthread_key_t _routine_key_##name;\
static int _routine_init_##name = 0;\
static void _routine_make_key_##name() \
{\
 	(void) pthread_key_create(&_routine_key_##name, NULL); \
}\
template <class T>\
class clsRoutineData_routine_##name\
{\
public:\
	inline T *operator->()\
	{\
		if( !_routine_init_##name ) \
		{\
			pthread_once( &_routine_once_##name,_routine_make_key_##name );\
			_routine_init_##name = 1;\
		}\
		T* p = (T*)co_getspecific( _routine_key_##name );\
		if( !p )\
		{\
			p = (T*)calloc(1,sizeof( T ));\
			int ret = co_setspecific( _routine_key_##name,p) ;\
            if ( ret )\
            {\
                if ( p )\
                {\
                    free(p);\
                    p = NULL;\
                }\
            }\
		}\
		return p;\
	}\
};\
\
static clsRoutineData_routine_##name<name> y;


#define CO_ROUTINE_SPECIFIC1(name0, name_y,y ) \
\
static pthread_once_t _routine_once_##name_y = PTHREAD_ONCE_INIT; \
static pthread_key_t _routine_key_##name_y;\
static int _routine_init_##name_y = 0;\
static void _routine_make_key_##name_y() \
{\
   (void) pthread_key_create(&_routine_key_##name_y, NULL); \
}\
name0 *clsRoutineData_routine_##name_y()\
{\
    if( !_routine_init_##name_y ) \
    {\
       pthread_once( &_routine_once_##name_y,_routine_make_key_##name_y );\
       _routine_init_##name_y = 1;\
    }\
    name0* p = (name0*)co_getspecific( _routine_key_##name_y );\
    if( !p )\
    {\
       p = (name0*)calloc(1,sizeof( name0 ));\
       int ret = co_setspecific( _routine_key_##name_y,(const void*)p) ;\
          if ( ret )\
          {\
              if ( p )\
              {\
                  free(p);\
                  p = NULL;\
              }\
          }\
    }\
    return p;\
}
#define ROUTINE_DEF(name, y) CO_ROUTINE_SPECIFIC1( name, name##y, y )
#define ROUTINE_VAR(name, y) (*clsRoutineData_routine_##name##y())
