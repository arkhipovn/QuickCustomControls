#ifndef CRINGBUFFER_H
#define CRINGBUFFER_H
#include <QtGlobal>
#include <QDebug>
#include <QVector>

template <typename T>
class CRingBuffer
{
    typedef QTypedArrayData<T> Data;
    Data *data;

public:
    CRingBuffer(int bufferSize = 10);
    ~CRingBuffer();

    void setBufferSize(int bufferSize) { //@N проверить!
        // Q_ASSERT_X(bufferSize > 0, "CRingBuffer::CRingBuffer", "Size must be greater than 0");
        if(bufferSize < 1) return;
        if(bufferSize == data->size) return;

        //@N тут бы нужно восстановить данные подругому.. Не через вектор..
        auto v = CRingBuffer::toVector();
        Data *newData = Data::allocate(bufferSize);
        Q_CHECK_PTR(newData);
        newData->size = bufferSize;

        first = 0;
        last = -1;
        currentSize = 0;

        if(!data->ref.deref()) {
            //        destruct(x->begin(), x->end());
            Data::deallocate(data);
        }

        data = newData;
        for(auto value : v) append(value);  //@N Сделать не через вектор!!
    }

    const T &at(int i) const;
    T &operator[](int i);

    void append(const T &t);
    T pop();

    void clear();

    int size() const { return currentSize; }
    int bufferSize() const { return data->size; }
    bool isEmpty() const { return currentSize == 0; }

    QVector <T> toVector() const;

    //    CRingBuffer<T> &operator=(const CRingBuffer<T> &v); //@N проверить!
    void swap(CRingBuffer<T> &other) noexcept { qSwap(data, other.data); }  //@N проверить!

    //    bool operator==(const QVector<T> &v) const;
    inline bool operator!=(const QVector<T> &v) const { return !(*this == v); }

    friend QDebug operator <<(QDebug stream, const CRingBuffer <T>&buffer) { return stream << buffer.toVector(); }

private:
    int currentSize;
    int first;
    int last;
};


template <typename T>
CRingBuffer<T>::CRingBuffer(int bufferSize)
{
    // Q_ASSERT_X(bufferSize > 0, "CRingBuffer::CRingBuffer", "Size must be greater than 0");
    if(bufferSize < 1) return;

    data = Data::allocate(bufferSize);
    Q_CHECK_PTR(data);
    data->size = bufferSize;

    first = 0;
    last = -1;
    currentSize = 0;
}

template<typename T>
CRingBuffer<T>::~CRingBuffer()
{
    if(!data->ref.deref()) {
        //        destruct(x->begin(), x->end());
        Data::deallocate(data);
    }
}


template <typename T>
inline const T &CRingBuffer<T>::at(int i) const
{
    // Q_ASSERT_X(i >= 0 && i < data->size, "CRingBuffer<T>::at", "index out of range");
    int ind = (first + i) % data->size;
    return data->begin()[ind];
}

template <typename T>
inline T &CRingBuffer<T>::operator[](int i)
{
    // Q_ASSERT_X(i >= 0 && i < data->size, "CRingBuffer<T>::operator[]", "index out of range");
    int ind = (first + i) % data->size;
    return data->begin()[ind];
}

template<typename T>
void CRingBuffer<T>::append(const T &t)
{
    //    // Q_ASSERT_X(last - first < data->size, "CRingBuffer<T>::append", "last - first > size");
    //    if(last - first >= data->size) return;

    last = (last + 1) % data->size;
    if(last == first && !isEmpty()) first = (last + 1) % data->size;

    data->begin()[last] = t;

    if(currentSize < data->size) ++currentSize;
    //    qDebug() << "append" << first << last << t << currentSize;
    //    qDebug() << "last" << last;
}

template<typename T>
T CRingBuffer<T>::pop()
{
    // Q_ASSERT_X(!isEmpty(), "CRingBuffer<T>::append", "Buffer is empty");
    if(isEmpty()) return *data->data();
    --currentSize;

    // сначала берем
    T d = data->begin()[first];

    // после увеличиваем индекс, тк предыдущий убрали
    //    if(isEmpty()) first = last;  // если все забрали, то оставляем такой же индекс
    //    else
    first = (first + 1) % data->size;

    //    qDebug() << "pop" << first << last << d << currentSize;
    return d;
}

template<typename T>
void CRingBuffer<T>::clear()
{
    if(!data->size) return;
    //    destruct(begin(), end());

    first = 0;
    last = -1;
    currentSize = 0;
}

template<typename T>
QVector<T> CRingBuffer<T>::toVector() const
{
    QVector <T> v(currentSize);
    if(isEmpty()) return v;

    if(first == last) {
        v[0] = data->begin()[first];
    }
    else if(last > first) {
        int j = 0;
        for(int i = first; i <= last; ++i)
            v[j++] = data->begin()[i];
    }
    else {
        int j = 0;
        for(int i = first; i < data->size; ++i)
            v[j++] = data->begin()[i];
        for(int i = 0; i <= last; ++i)
            v[j++] = data->begin()[i];
    }
    return v;
}

//template<typename T>
//CRingBuffer<T> &CRingBuffer::operator=(const CRingBuffer<T> &v)
//{
//    currentSize = v.currentSize;
//    first = v.first;
//    last = v.last;

//    if(v.data != data) {
//        CRingBuffer<T> tmp(v);
//        tmp.swap(*this);
//    }
//    return *this;
//}



#endif // RINGBUFFER_H
