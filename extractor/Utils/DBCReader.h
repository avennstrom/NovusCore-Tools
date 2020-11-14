/*
# MIT License

# Copyright(c) 2018-2019 NovusCore

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files(the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions :

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
*/
#pragma once

#include <NovusTypes.h>
#include <Utils/ByteBuffer.h>
#include <stdint.h>
#include <stdio.h>
#include <string>

#define NOVUSDBC_WDBC_TOKEN 1128416343

class DBCReader
{
public:
    DBCReader(){};
    ~DBCReader()
    {
        if (_data)
        {
            delete[] _data;
        }
    }

    int Load(std::shared_ptr<Bytebuffer> buffer);

    class DBCRow
    {
    public:
        template <typename T>
        T* Get() const
        {
            return reinterpret_cast<T*>(offset);
        }

        float GetFloat(u32 field) const
        {
            return *reinterpret_cast<float*>(offset + field * 4);
        }

        u32 GetUInt32(u32 field) const
        {
            return *reinterpret_cast<u32*>(offset + field * 4);
        }

        u64 GetUInt64(u32 field) const
        {
            return *reinterpret_cast<u64*>(offset + field * 4);
        }

        i32 GetInt32(u32 field) const
        {
            return *reinterpret_cast<i32*>(offset + field * 4);
        }

        i64 GetInt64(u32 field) const
        {
            return *reinterpret_cast<i64*>(offset + field * 4);
        }

        std::string GetString(u32 field) const
        {
            return reinterpret_cast<char*>(file._stringTable + GetUInt32(field));
        }

    private:
        DBCRow(DBCReader& file, u8* offset) : file(file), offset(offset) {}
        DBCReader& file;
        u8* offset;

        friend class DBCReader;
    };

    DBCRow GetRow(u32 id);
    u32 GetNumRows() { return _rowCount; }
    u32 GetRowSize() { return _rowSize; }
    u32 GetNumFields() { return _fieldCount; }
    u8* GetStringTable() { return _stringTable; }

private:
    u32 _rowSize = 0;
    u32 _rowCount = 0;
    u32 _fieldCount = 0;
    u32 _stringSize = 0;
    u8* _data = nullptr; // Use Bytebuffer
    u8* _stringTable = nullptr; // Use Bytebuffer
};