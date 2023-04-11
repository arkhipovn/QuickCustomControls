#ifndef SORTEDLIST_H
#define SORTEDLIST_H
#include <QList>
#include <QDebug>

#include <QLinkedList>


template <typename T>
class CSortedList
{
public:
    CSortedList();
    CSortedList(const QList <T> &list);
    CSortedList(const QVector <T> &vector);

    template <typename Compare>
    CSortedList(const QList <T> &list, const Compare &comp) {
        data = list;
        std::sort(data.begin(), data.end(), comp);
    }

    template <typename Compare>
    CSortedList(const QVector <T> &vector, const Compare &comp) {
        data = vector.toList();
        std::sort(data.begin(), data.end(), comp);
    }

    inline const T &at(int i) const { return data.at(i); }
    inline T &operator[](int i) { return data[i]; }

    void append(const T &t);
    void append(const QList<T> &l);

    template <typename Compare>
    void append(const T &t, const Compare &comp) {
        auto it = lowerBound(t, comp);
        data.insert(it, t);
    }

    CSortedList<T> &operator=(const QList<T> &l);
    CSortedList<T> &operator=(const CSortedList<T> &l);

    inline T& first() { return data.first(); }
    inline const T& constFirst() const { return data.first(); }

    inline T& last() { return data.last(); }
    inline const T& constLast() const { return data.constLast(); }

    inline void removeAt(int i) { data.removeAt(i); }


    template <typename Type>
    void removeOne(const Type &t) { data.removeOne(t); }


    //    template <typename Type>
    //    int removeOne(const Type &t) {
    //        int i = 0;
    //        auto it = data.begin() + - 1;
    //        while(++it != data.end()) {
    //            *it == t;
    //            ++i;
    //        }
    //        return i;
    //    }


    int indexOf(const T &t, int from = 0) const;

    template <typename Type>
    int indexOf(const Type &t, int from = 0) const {
        if(from < 0) from = qMax(from + data.size(), 0);
        if(from < data.size()) {
            auto it = data.begin() + from - 1;
            while(++it != data.end()) {
                if(*it == t) return int(it - data.begin());
            }
        }
        return -1;
    }

    typename QList<T>::iterator erase(typename QList<T>::iterator first,
                                      typename QList<T>::iterator last) { return data.erase(first, last); }
    typename QList<T>::iterator erase(typename QList<T>::iterator pos) { return data.erase(pos); }



    typename QList<T>::iterator begin() { return data.begin(); }
    typename QList<T>::iterator end() { return data.end(); }

    typename QList<T>::const_iterator constBegin() const { return data.constBegin(); }
    typename QList<T>::const_iterator constEnd() const { return data.constEnd(); }


    inline bool contains(const T &t) const { return constFind(t) != data.constEnd(); }

    // Find
    template <typename Type>
    inline typename QList<T>::iterator find(const Type &value) {
        auto it = std::lower_bound(data.begin(), data.end(), value);
        return (it != data.end() && !(value < (*it))) ? it : data.end();
    }

    template <typename Type>
    inline typename QList<T>::const_iterator constFind(const Type &value) const {
        auto it = std::lower_bound(data.begin(), data.end(), value);
        return (it != data.end() && !(value < (*it))) ? it : data.end();
    }

    template <typename Type, typename Compare>
    typename QList<T>::iterator find(const Type &value, const Compare &comp) {
        auto it = std::lower_bound(data.begin(), data.end(), value, comp);
        return (it != data.end() && !(value < (*it))) ? it : data.end();
    }

    template <typename Type, typename Compare>
    typename QList<T>::const_iterator constFind(const Type &value, const Compare &comp) const {
        auto it = std::lower_bound(data.constBegin(), data.constEnd(), value, comp);
        return (it != data.end() && !(value < (*it))) ? it : data.constEnd();
    }


//    template <typename Type>
//    inline typename Records::iterator lowerBound(const Type &value) {
//        return std::lower_bound(records.begin(), records.end(), value);
//    }

    typename QList<T>::iterator lowerBound(const T &t);

    template <typename Type>
    typename QList<T>::iterator lowerBound(const Type &t) {
        return std::lower_bound(data.begin(), data.end(), t);
    }

    template <typename Type, typename Compare>
    typename QList<T>::iterator lowerBound(const Type &t, const Compare &comp) {
        return std::lower_bound(data.begin(), data.end(), t, comp);
    }

    void clear() { data.clear(); }
    inline int size() const { return data.size(); }
    inline bool isEmpty() const { return data.isEmpty(); }

    friend QDebug operator <<(QDebug stream, const CSortedList <T>&list) { return stream << list.data; }

    //    std::ostream& Point::operator<<(std::ostream& out)

    QList <T> toList() const;
    QVector <T> toVector() const;

private:
    QList <T> data;
};


template<typename T>
CSortedList<T>::CSortedList()
{

}

template<typename T>
CSortedList<T>::CSortedList(const QList<T> &list)
{
    data = list;
    std::sort(data.begin(), data.end());
}

template<typename T>
CSortedList<T>::CSortedList(const QVector<T> &vector)
{
    data = vector.toList();
    std::sort(data.begin(), data.end());
}

template<typename T>
void CSortedList<T>::append(const T &t)
{
    auto it = lowerBound(t);
    data.insert(it, t);
}


template<typename T>
void CSortedList<T>::append(const QList<T> &l)
{
    data.append(l);
    std::sort(data.begin(), data.end());
}


template<typename T>
CSortedList<T> &CSortedList<T>::operator=(const CSortedList<T> &l)
{
    if(this == &l) return *this;
    data = l.data;
    return *this;
}

template<typename T>
CSortedList<T> &CSortedList<T>::operator=(const QList<T> &l)
{
    data = l;
    std::sort(data.begin(), data.end());
    return *this;
}

template<typename T>
int CSortedList<T>::indexOf(const T &t, int from) const
{
    return data.indexOf(t, from);
}

template <typename T>
typename QList<T>::iterator CSortedList<T>::lowerBound(const T &t)
{
    return std::lower_bound(data.begin(), data.end(), t);
}

template<typename T>
QList<T> CSortedList<T>::toList() const
{
    return data;
}

template<typename T>
QVector<T> CSortedList<T>::toVector() const
{
    return data.toVector();
}


// Find
//template <typename T>
//typename QList<T>::iterator CSortedList<T>::find(const T &t)
//{
//    auto it = std::lower_bound(data.begin(), data.end(), t);
//    return (it != data.end() && !(t < (*it))) ? it : data.end();
//}

//template <typename T>
//typename QList<T>::const_iterator CSortedList<T>::constFind(const T &t) const
//{
//    auto it = std::lower_bound(data.constBegin(), data.constEnd(), t);
//    return (it != data.end() && !(t < (*it))) ? it : data.constEnd();
//}












/*
template <typename T>
class CSortedList
{
public:
    CSortedList();
    CSortedList(const QList <T> &list);


    inline const T &at(int i) const { return data.at(i); }
    inline T &operator[](int i) { return data[i]; }

    void append(const T &t);
    void append(const QList<T> &l);

    template <typename Compare>
    void append(const T &t, const Compare &comp) {
        auto it = lowerBound(t, comp);
        data.insert(it, t);
    }

    CSortedList<T> &operator=(const QList<T> &l);
    CSortedList<T> &operator=(const CSortedList<T> &l);

    inline T& first() { return data.first(); }
    inline const T& constFirst() const { return data.first(); }

    const T& last() const { return data.last(); }
    inline const T& constLast() const { return data.constLast(); }

    inline void removeAt(int i) { data.removeAt(i); }


    template <typename Type>
    void removeOne(const Type &t) { data.removeOne(t); }


//    template <typename Type>
//    int removeOne(const Type &t) {
//        int i = 0;
//        auto it = data.begin() + - 1;
//        while(++it != data.end()) {
//            *it == t;
//            ++i;
//        }
//        return i;
//    }


    int indexOf(const T &t, int from = 0) const;

    template <typename Type>
    int indexOf(const Type &t, int from = 0) const {
        if(from < 0) from = qMax(from + data.size(), 0);
        if(from < data.size()) {
            auto it = data.begin() + from - 1;
            while(++it != data.end()) {
                if(*it == t) return int(it - data.begin());
            }
        }
        return -1;
    }

    typename QList<T>::iterator erase(typename QList<T>::iterator first,
                                      typename QList<T>::iterator last) { return data.erase(first, last); }
    typename QList<T>::iterator erase(typename QList<T>::iterator pos) { return data.erase(pos); }



    typename QList<T>::iterator begin() { return data.begin(); }
    typename QList<T>::iterator end() { return data.end(); }

    typename QList<T>::const_iterator constBegin() const { return data.constBegin(); }
    typename QList<T>::const_iterator constEnd() const { return data.constEnd(); }


    bool contains(const T &t) const;

    // Find

    typename QList<T>::iterator find(const T &t);
    //    template <typename Compare>
    //    typename QList<T>::iterator find(const T &t, const Compare &comp) {
    //        auto it = std::lower_bound(data.begin(), data.end(), t, comp);
    //        return (it != data.end() && !(t < (*it))) ? it : data.end();
    //    }

    template <typename Type, typename Compare>
    typename QList<T>::iterator find(const Type &t, const Compare &comp) {
        auto it = std::lower_bound(data.begin(), data.end(), t, comp);
        return (it != data.end() && !(t < (*it))) ? it : data.end();
    }


    typename QList<T>::const_iterator constFind(const T &t) const;
    //    template <typename Compare>
    //    typename QList<T>::const_iterator constFind(const T &t, const Compare &comp) {
    //        auto it = std::lower_bound(data.constBegin(), data.constEnd(), t, comp);
    //        return (it != data.end() && !(t < (*it))) ? it : data.end();
    //    }
    template <typename Type, typename Compare>
    typename QList<T>::const_iterator constFind(const Type &t, const Compare &comp) {
        auto it = std::lower_bound(data.constBegin(), data.constEnd(), t, comp);
        return (it != data.end() && !(t < (*it))) ? it : data.constEnd();
    }


    typename QList<T>::iterator lowerBound(const T &t);

    template <typename Type>
    typename QList<T>::iterator lowerBound(const Type &t) {
        return std::lower_bound(data.begin(), data.end(), t);
    }

    template <typename Type, typename Compare>
    typename QList<T>::iterator lowerBound(const Type &t, const Compare &comp) {
        return std::lower_bound(data.begin(), data.end(), t, comp);
    }

    void clear() { data.clear(); }
    inline int size() const { return data.size(); }
    inline bool isEmpty() const { return data.isEmpty(); }

    friend QDebug operator <<(QDebug stream, const CSortedList <T>&list) { return stream << list.data; }

    //    std::ostream& Point::operator<<(std::ostream& out)

    QList <T> toList() const;
    QVector <T> toVector() const;

private:
    QList <T> data;

};


template<typename T>
CSortedList<T>::CSortedList()
{

}

template<typename T>
CSortedList<T>::CSortedList(const QList<T> &list)
{
    data = list;
    std::sort(data.begin(), data.end());
}

template<typename T>
void CSortedList<T>::append(const T &t)
{
    auto it = lowerBound(t);
    data.insert(it, t);
}


template<typename T>
void CSortedList<T>::append(const QList<T> &l)
{
    data.append(l);
    std::sort(data.begin(), data.end());
}


template<typename T>
CSortedList<T> &CSortedList<T>::operator=(const CSortedList<T> &l)
{
    if(this == &l) return *this;
    data = l.data;
    return *this;
}

template<typename T>
CSortedList<T> &CSortedList<T>::operator=(const QList<T> &l)
{
    data = l;
    std::sort(data.begin(), data.end());
    return *this;
}

template<typename T>
int CSortedList<T>::indexOf(const T &t, int from) const
{
    return data.indexOf(t, from);
}

template <typename T>
typename QList<T>::iterator CSortedList<T>::lowerBound(const T &t)
{
    return std::lower_bound(data.begin(), data.end(), t);
}

template<typename T>
QList<T> CSortedList<T>::toList() const
{
    return data;
}

template<typename T>
QVector<T> CSortedList<T>::toVector() const
{
    return data.toVector();
}


// Find
template <typename T>
typename QList<T>::iterator CSortedList<T>::find(const T &t)
{
    auto it = std::lower_bound(data.begin(), data.end(), t);
    return (it != data.end() && !(t < (*it))) ? it : data.end();
}

template <typename T>
typename QList<T>::const_iterator CSortedList<T>::constFind(const T &t) const
{
    auto it = std::lower_bound(data.constBegin(), data.constEnd(), t);
    return (it != data.end() && !(t < (*it))) ? it : data.constEnd();
}

template<typename T>
bool CSortedList<T>::contains(const T &t) const
{
    return constFind(t) != data.constEnd();
}
*/
#endif // SORTEDLIST_H
