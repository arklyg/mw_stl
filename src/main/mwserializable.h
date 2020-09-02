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


#ifndef _MW_STL_MW_SERIALIZABLE_H_
#define _MW_STL_MW_SERIALIZABLE_H_

#include <vector>
#include <string.h>

#include <mwglobal.h>


class MWSerializableSingleton;
class MWSerializable;

typedef std::vector<char> (MWSerializable::*
                           MWSerializableFunctionGetVectorCharWithVoidPt)(const void*) const;
typedef size_t (MWSerializable::*
                MWSerializableFunctionGetSizeTAssignInstanceWithCharPtSizeT)(void*,
                                                                             const std::vector<char> &, 
                                                                             size_t);

class MWSerializable {
  protected:
    std::vector<void*> _serializing_sequence_void_pt_vector;
    std::vector<MWSerializableFunctionGetVectorCharWithVoidPt>
    _serialize_function_vector;
    std::vector<MWSerializableFunctionGetSizeTAssignInstanceWithCharPtSizeT>
    _deserialize_function_vector;

  private:
    inline std::vector<char> SerializeSerializable(const void *instance_pt) const;
    inline std::vector<char> SerializeSingletonPT(const void *instance_pt) const;
    inline std::vector<char> SerializeDouble(const void *instance_pt) const;
    inline std::vector<char> SerializeSizeT(const void *instance_pt) const;
    inline std::vector<char> SerializeChar(const void *instance_pt) const;

    inline size_t DeserializeSerializable(void *instance_pt,
                                          const std::vector<char> &char_vector, 
                                          size_t off);
    inline size_t DeserializeSingletonPT(void *instance_pt,
                                         const std::vector<char> &char_vector, 
                                         size_t off);
    inline size_t DeserializeDouble(void *instance_pt,
                                    const std::vector<char> &char_vector, 
                                    size_t off);
    inline size_t DeserializeSizeT(void *instance_pt,
                                   const std::vector<char> &char_vector, 
                                   size_t off);
    inline size_t DeserializeChar(void *instance_pt, 
                                  const std::vector<char> &char_vector,
                                  size_t off);

  protected:
    virtual void AddSequence() = 0;
    virtual void AddSequenceForDeserializing();

    virtual void SetSequenceForSerializing();
    virtual void SetSequenceForDeserializing();

    inline void AddSequenceData(MWSerializable *pt);
    inline void AddSequenceData(MWSerializableSingleton **pt);
    inline void AddSequenceData(double *pt);
    inline void AddSequenceData(size_t *pt);
    inline void AddSequenceData(char *pt);

  public:
    virtual ~MWSerializable();

    virtual std::vector<char> Serialize();
    virtual size_t Deserialize(const std::vector<char> &char_vector, size_t off);
};

template <typename T>
std::vector<char> SerializeBasic(const T *instance_pt) {
    size_t len = sizeof(T);
    char chs[len];

    memcpy(chs, instance_pt, len);

    std::vector<char> ret(len);
    for (size_t i = 0; i < len; ++ i) {
        ret[i] = chs[i];
    }

    return ret;
}

template <typename T>
size_t DeserializeBasic(T *instance_pt, 
                        const std::vector<char> &char_vector,
                        size_t off) {
    size_t len = sizeof(T);
    char chs[len];
    for (size_t i = 0; i < len; ++ i) {
        chs[i] = char_vector[off + i];
    }

    memcpy(instance_pt, chs, len);

    return len;
}

inline void MWSerializable::AddSequenceData(MWSerializable *pt) {
    _serializing_sequence_void_pt_vector.push_back(static_cast<void*>(pt));
    _serialize_function_vector.push_back(
        static_cast<MWSerializableFunctionGetVectorCharWithVoidPt>
        (&MWSerializable::SerializeSerializable));
    _deserialize_function_vector.push_back(
        static_cast<MWSerializableFunctionGetSizeTAssignInstanceWithCharPtSizeT>
        (&MWSerializable::DeserializeSerializable));
}

inline void MWSerializable::AddSequenceData(MWSerializableSingleton **pt) {
    _serializing_sequence_void_pt_vector.push_back(static_cast<void*>(pt));
    _serialize_function_vector.push_back(
        static_cast<MWSerializableFunctionGetVectorCharWithVoidPt>
        (&MWSerializable::SerializeSingletonPT));
    _deserialize_function_vector.push_back(
        static_cast<MWSerializableFunctionGetSizeTAssignInstanceWithCharPtSizeT>
        (&MWSerializable::DeserializeSingletonPT));
}

inline void MWSerializable::AddSequenceData(double *pt) {
    _serializing_sequence_void_pt_vector.push_back(static_cast<void*>(pt));
    _serialize_function_vector.push_back(
        static_cast<MWSerializableFunctionGetVectorCharWithVoidPt>
        (&MWSerializable::SerializeDouble));
    _deserialize_function_vector.push_back(
        static_cast<MWSerializableFunctionGetSizeTAssignInstanceWithCharPtSizeT>
        (&MWSerializable::DeserializeDouble));
}

inline void MWSerializable::AddSequenceData(size_t *pt) {
    _serializing_sequence_void_pt_vector.push_back(static_cast<void*>(pt));
    _serialize_function_vector.push_back(
        static_cast<MWSerializableFunctionGetVectorCharWithVoidPt>
        (&MWSerializable::SerializeSizeT));
    _deserialize_function_vector.push_back(
        static_cast<MWSerializableFunctionGetSizeTAssignInstanceWithCharPtSizeT>
        (&MWSerializable::DeserializeSizeT));
}

inline void MWSerializable::AddSequenceData(char *pt) {
    _serializing_sequence_void_pt_vector.push_back(static_cast<void*>(pt));
    _serialize_function_vector.push_back(
        static_cast<MWSerializableFunctionGetVectorCharWithVoidPt>
        (&MWSerializable::SerializeChar));
    _deserialize_function_vector.push_back(
        static_cast<MWSerializableFunctionGetSizeTAssignInstanceWithCharPtSizeT>
        (&MWSerializable::DeserializeChar));
}

inline std::vector<char> MWSerializable::SerializeSerializable(const void *instance_pt) const {
    return ((MWSerializable*) instance_pt)->Serialize();
}

inline std::vector<char> MWSerializable::SerializeSingletonPT(const void *instance_pt) const {
    return SerializeSingletonPT((MWSerializableSingleton**) instance_pt);
}

inline std::vector<char> MWSerializable::SerializeDouble(const void *instance_pt) const {
    return SerializeBasic((double*) instance_pt);
}

inline std::vector<char> MWSerializable::SerializeSizeT(const void *instance_pt) const {
    return SerializeBasic((size_t*) instance_pt);
}

inline std::vector<char> MWSerializable::SerializeChar(const void *instance_pt) const {
    return SerializeBasic((char*) instance_pt);
}

inline size_t MWSerializable::DeserializeSerializable(void *instance_pt,
                                                      const std::vector<char> &char_vector, 
                                                      size_t off) {
    return ((MWSerializable*) instance_pt)->Deserialize(char_vector, off);
}

inline size_t MWSerializable::DeserializeSingletonPT(void *instance_pt,
                                                     const std::vector<char> &char_vector, 
                                                     size_t off) {
    return DeserializeSingletonPT((MWSerializableSingleton**) instance_pt,
                                         char_vector, 
                                         off);
}

inline size_t MWSerializable::DeserializeDouble(void *instance_pt,
                                                const std::vector<char> &char_vector, 
                                                size_t off) {
    return DeserializeBasic((double*) instance_pt, char_vector, off);
}

inline size_t MWSerializable::DeserializeSizeT(void *instance_pt,
                                               const std::vector<char> &char_vector, 
                                               size_t off) {
    return DeserializeBasic((size_t*) instance_pt, char_vector, off);
}

inline size_t MWSerializable::DeserializeChar(void *instance_pt,
                                              const std::vector<char> &char_vector, 
                                              size_t off) {
    return DeserializeBasic((char*) instance_pt, char_vector, off);
}

std::vector<char> SerializeSingletonPT(const MWSerializableSingleton **instance_pt_pt);

size_t DeserializeSingletonPT(MWSerializableSingleton **instance_pt_pt,
                              const std::vector<char> &char_vector, 
                              size_t off);

#endif
