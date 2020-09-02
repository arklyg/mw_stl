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


#ifndef _MW_SYNCHRONIZER_H_
#define _MW_SYNCHRONIZER_H_

#include <list>

#include "proclockable.h"


class MWEvent {
  public:
    size_t _id;
    void *_arg;

  public:
    MWEvent();
    ~MWEvent();

    MWEvent(int event_id, void *const arg);
};

class MWSynchronizer {
  private:
    ProcLockable _lock;
    pthread_cond_t _cond;
    std::list<MWEvent*> _list;

  public:
    MWSynchronizer();
    ~MWSynchronizer();

    MWEvent *Next(size_t nanoseconds = 0);
    void Broadcast(MWEvent *const event);
};

#endif

