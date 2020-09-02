// Micro Wave Standard Template Library
// Copyright (c) 2015-2020, Ark Lee
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// You must obey the GNU General Public License in all respects for
// all of the code used.  If you modify file(s) with this exception, 
// you may extend this exception to your version of the file(s), but 
// you are not obligated to do so.  If you do not wish to do so, delete 
// this exception statement from your version. If you delete this exception 
// statement from all source files in the program, then also delete it here.
//
// Contact:  Ark Lee <arklee@houduan.online>
//           Beijing, China


#ifndef _CLASS_LOCKABLE_H_
#define _CLASS_LOCKABLE_H_

#include <map>
#include <string>
#include <pthread.h>

#include "lockable.h"


// 用于同一个类的所有对象的同步, 用于此对象外的被同步
// 内部同步建议使用ProcLockable
class ClassLockable : public Lockable {
  private:
    static pthread_mutex_t _map_mutex;
    static std::map<std::string, pthread_mutex_t*> _mutex_map;

  private:
    pthread_mutex_t *GetMutex();
    pthread_mutex_t *AddMutex();

  public:
    virtual void Acquire();
    virtual void Release();
};


#endif
