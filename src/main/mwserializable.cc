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


#include "mwserializablesingleton.h"

#include "mwserializable.h"

MWSerializable::~MWSerializable() {
}

void MWSerializable::AddSequenceForDeserializing() {
    AddSequence();
}

std::vector<char> MWSerializable::Serialize() {
    SetSequenceForSerializing();

    size_t i, j;
    std::vector<char> ret, a_ret;
    for (i = 0; i < _serializing_sequence_void_pt_vector.size(); ++ i) {
        a_ret = (this->*(_serialize_function_vector[i]))(
                    _serializing_sequence_void_pt_vector[i]);
        for (j = 0; j < a_ret.size(); ++ j) {
            ret.push_back(a_ret[j]);
        }
    }

    return ret;
}

void MWSerializable::SetSequenceForSerializing() {
    _serializing_sequence_void_pt_vector.clear();
    _serialize_function_vector.clear();
    _deserialize_function_vector.clear();
    AddSequence();
}

void MWSerializable::SetSequenceForDeserializing() {
    _serializing_sequence_void_pt_vector.clear();
    _serialize_function_vector.clear();
    _deserialize_function_vector.clear();
    AddSequenceForDeserializing();
}

size_t MWSerializable::Deserialize(const std::vector<char> &char_vector, size_t off) {
    SetSequenceForDeserializing();

    size_t i;
    const size_t original_off = off;
    for (i = 0; i < _serializing_sequence_void_pt_vector.size(); ++ i) {
        off += (this->*(_deserialize_function_vector[i]))(
                _serializing_sequence_void_pt_vector[i], char_vector, off);
    }

    return off - original_off;
}

std::vector<char> SerializeSingletonPT(const MWSerializableSingleton**
                                       instance_pt_pt) {
    size_t len = sizeof(size_t), 
           class_id = (*instance_pt_pt)->GetClassID();
    char chs[len];

    memcpy(chs, &class_id, len);

    std::vector<char> ret(len);
    for (size_t i = 0; i < len; ++ i) {
        ret[i] = chs[i];
    }

    return ret;
}

size_t DeserializeSingletonPT(MWSerializableSingleton **instance_pt_pt,
                              const std::vector<char> &char_vector, 
                              size_t off) {
    size_t len = sizeof(size_t), class_id;
    char chs[len];
    for (size_t i = 0; i < len; ++ i) {
        chs[i] = char_vector[off + i];
    }

    memcpy(&class_id, chs, len);

    *instance_pt_pt = MWSerializableSingleton::GetSerializableSingletonInstance(class_id);

    return len;
}
