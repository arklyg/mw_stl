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


#include "mwsynchronizer.h"

MWEvent::MWEvent() {
}

MWEvent::~MWEvent() {
}

MWEvent::MWEvent(int id, void *const arg)
    : _id(id)
    , _arg(arg) {
}

MWSynchronizer::MWSynchronizer() {
    pthread_cond_init(&_cond, NULL);
}

MWSynchronizer::~MWSynchronizer() {
    pthread_cond_destroy(&_cond);
}

MWEvent *MWSynchronizer::Next(size_t nanoseconds) {
    _lock.Acquire();

    if (_list.empty()) {
        if (0 == nanoseconds) {
            pthread_cond_wait(&_cond, &_lock.GetMutex());
        } else {
            timespec max_time;
            max_time.tv_sec = 0;
            max_time.tv_nsec = nanoseconds;
            pthread_cond_timedwait(&_cond, &_lock.GetMutex(), &max_time);

            if (_list.empty()) {
                _lock.Release();
                return NULL;
            }
        }
    }

    MWEvent *ret = _list.front();
    _list.pop_front();

    _lock.Release();
    return ret;
}

void MWSynchronizer::Broadcast(MWEvent *const event) {
    _lock.Acquire();
    _list.push_back(event);
    pthread_cond_broadcast(&_cond);
    _lock.Release();
}
