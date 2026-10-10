#ifndef __BASE_TIMEBUF_HH__
#define __BASE_TIMEBUF_HH__

#include <cassert>
#include <cstring>
#include <vector>

template <class T>
class TimeBuffer
{
  protected:
    int past;
    int future;
    unsigned size;
    int _id;

    char *data;
    std::vector<char *> index;
    unsigned base;

    void valid(int idx) const
    {
        assert (idx >= -past && idx <= future);
    }

  public:
    friend class wire;
    class wire
    {
        friend class TimeBuffer;
      protected:
        TimeBuffer<T> *buffer;
        int index;

        void set(int idx)
        {
            buffer->valid(idx);
            index = idx;
        }

        wire(TimeBuffer<T> *buf, int i)
            : buffer(buf), index(i)
        { }

      public:
        wire()
        { }

        wire(const wire &i)
            : buffer(i.buffer), index(i.index)
        { }

        const wire &operator=(const wire &i)
        {
            buffer = i.buffer;
            set(i.index);
            return *this;
        }

        const wire &operator=(int idx)
        {
            set(idx);
            return *this;
        }

        const wire &operator+=(int offset)
        {
            set(index + offset);
            return *this;
        }

        const wire &operator-=(int offset)
        {
            set(index - offset);
            return *this;
        }

        wire &operator++()
        {
            set(index + 1);
            return *this;
        }

        wire &operator++(int)
        {
            int i = index;
            set(index + 1);
            return wire(this, i);
        }

        wire &operator--()
        {
            set(index - 1);
            return *this;
        }

        wire &operator--(int)
        {
            int i = index;
            set(index - 1);
            return wire(this, i);
        }
        T &operator*() const { return *buffer->access(index); }
        T *operator->() const { return buffer->access(index); }
    };


  public:
    TimeBuffer(int p, int f)
        : past(p), future(f), size(past + future + 1),
          data(new char[size * sizeof(T)]), index(size), base(0)
    {
        assert(past >= 0 && future >= 0);
        char *ptr = data;
        for (unsigned i = 0; i < size; i++) {
            index[i] = ptr;
            std::memset(ptr, 0, sizeof(T));
            new (ptr) T;
            ptr += sizeof(T);
        }

        _id = -1;
    }

    TimeBuffer()
        : data(NULL)
    {
    }

    ~TimeBuffer()
    {
        for (unsigned i = 0; i < size; ++i)
            (reinterpret_cast<T *>(index[i]))->~T();
        delete [] data;
    }

    void id(int id)
    {
        _id = id;
    }

    int id()
    {
        return _id;
    }

    void
    advance()
    {
        if (++base >= size)
            base = 0;

        int ptr = base + future;
        if (ptr >= (int)size)
            ptr -= size;
        (reinterpret_cast<T *>(index[ptr]))->~T();
        std::memset(index[ptr], 0, sizeof(T));
        new (index[ptr]) T;
    }

  protected:
    //Calculate the index into this->index for element at position idx
    //relative to now
    inline int calculateVectorIndex(int idx) const
    {
        //Need more complex math here to calculate index.
        valid(idx);

        int vector_index = idx + base;
        if (vector_index >= (int)size) {
            vector_index -= size;
        } else if (vector_index < 0) {
            vector_index += size;
        }

        return vector_index;
    }

  public:
    T *access(int idx)
    {
        int vector_index = calculateVectorIndex(idx);

        return reinterpret_cast<T *>(index[vector_index]);
    }

    T &operator[](int idx)
    {
        int vector_index = calculateVectorIndex(idx);

        return reinterpret_cast<T &>(*index[vector_index]);
    }

    const T &operator[] (int idx) const
    {
        int vector_index = calculateVectorIndex(idx);

        return reinterpret_cast<const T &>(*index[vector_index]);
    }

    wire getWire(int idx)
    {
        valid(idx);

        return wire(this, idx);
    }

    wire zero()
    {
        return wire(this, 0);
    }

    unsigned getSize()
    {
        return size;
    }

    int
    getPast() const
    {
        return past;
    }

    int
    getFuture() const
    {
        return future;
    }
};

#endif // __BASE_TIMEBUF_HH__
