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


#include <iostream>
#include <typeinfo>

#include "classlockable.h"

pthread_mutex_t ClassLockable::_map_mutex = PTHREAD_MUTEX_INITIALIZER;
std::map<std::string, pthread_mutex_t*> ClassLockable::_mutex_map;

void ClassLockable::Acquire() {
    pthread_mutex_lock(&_map_mutex); // 对mutex_map的改动或取值保持互斥
    pthread_mutex_t *mutex = GetMutex();
    if (mutex == NULL) {
        mutex = AddMutex();
    }
    pthread_mutex_unlock(&_map_mutex); // 结束互斥

    pthread_mutex_lock(mutex);
}

void ClassLockable::Release() {
    pthread_mutex_lock(&_map_mutex); // 对mutex_map的改动或取值保持互斥
    pthread_mutex_t *mutex = GetMutex();
    pthread_mutex_unlock(&_map_mutex); // 结束互斥

    if (mutex != NULL) {
        pthread_mutex_unlock(mutex);
    }
}

pthread_mutex_t *ClassLockable::GetMutex() {
    std::string class_name = typeid(*this).name();

    auto finder = _mutex_map.find(class_name);
    return finder == _mutex_map.end() ? NULL : finder->second;
}

pthread_mutex_t *ClassLockable::AddMutex() {
    pthread_mutex_t *mutex = new pthread_mutex_t;
    pthread_mutex_init(mutex, NULL);
    _mutex_map.insert(std::pair<std::string, pthread_mutex_t*>(typeid(*this).name(), mutex));
    return mutex;
}
