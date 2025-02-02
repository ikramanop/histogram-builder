#include "arraysequence.hpp"

template <typename T>
ArraySequence<T>::ArraySequence() {
    ISequence<T>::length = 0;
    data = nullptr;
}

template <typename T>
ArraySequence<T>::ArraySequence(ISequence<T>* sequence) {
    ISequence<T>::length = sequence->getLength();
    data = new T[ISequence<T>::length];
    for (int i = 0; i < ISequence<T>::length; i++) {
        *(data + i) = sequence->get(i);
    }
}

template <typename T>
ArraySequence<T>::ArraySequence(int n, int leftLimit, int rightLimit) {
    if (rightLimit < leftLimit) {
        exception_incorrectSelection e;
        throw e;
    }
    data = new T[n];
    ISequence<T>::length = n;
    for (int i = 0; i < n; i++) {
        *(data + i) = rnd::randInt(leftLimit, rightLimit);
    }
}

template <typename T>
ArraySequence<T>& ArraySequence<T>::operator=(const ArraySequence<T>& sequence) {
    free(this->data);
    this->data = nullptr;
    ISequence<T>::length = 0;
    for (int i = 0; i < sequence.getLength(); i++) {
        this->append(sequence.get(i));
    }
    return *this;
}

template <typename T>
int ArraySequence<T>::getLength() const {
    return ISequence<T>::length;
}

template <typename T>
bool ArraySequence<T>::getIsEmpty() const {
    if (!ISequence<T>::length) {
        return true;
    }
    return false;
}

template <typename T>
T ArraySequence<T>::get(const int& index) const {
    if ((index >= ISequence<T>::length) || (index < 0)) {
        exception_outOfRange e;
        throw e;
    }
    return *(data + index);
}

template <typename T>
T ArraySequence<T>::getFirst() const {
    if (!ISequence<T>::length) {
        exception_sequenceIsEmpty e;
        throw e;
    }
    return *(data);
}

template <typename T>
T ArraySequence<T>::getLast() const {
    if (!ISequence<T>::length) {
        exception_sequenceIsEmpty e;
        throw e;
    }
    return *(data + ISequence<T>::length - 1);
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::getSubSequence(const int& startIndex, const int& endIndex) const {
    if (endIndex < startIndex) {
        exception_incorrectSelection e;
        throw e;
    }
    if (endIndex >= ISequence<T>::length) {
        exception_outOfRange e;
        throw e;
    }
    ArraySequence<T>* buff = new ArraySequence<T>;
    for (int i = startIndex; i <= endIndex; i++) {
        buff->append(this->get(i));
    }
    return buff;
}

template <typename T>
void ArraySequence<T>::append(const T& item) {
    ISequence<T>::length++;
    if (!(ISequence<T>::length - 1)) {
        data = new T[ISequence<T>::length];
    }
    else {
        T *buffer = new T[ISequence<T>::length];
        for (int i = 0; i < ISequence<T>::length - 1; i++) {
            *(buffer + i) = *(data + i);
        }
        delete [] data;
        data = buffer;
    }
    *(data + ISequence<T>::length - 1) = item;
}

template <typename T>
void ArraySequence<T>::prepend(const T& item) {
    ISequence<T>::length++;
    if (!(ISequence<T>::length - 1)) {
        data = new T[ISequence<T>::length];
    }
    else {
        T *buffer = new T[ISequence<T>::length];
        for (int i = 1; i < ISequence<T>::length; i++) {
            *(buffer + i) = *(data + i - 1);
        }
        delete [] data;
        data = buffer;
    }
    *(data) = item;
}

template <typename T>
void ArraySequence<T>::insertAt(const int& index, const T& item) {
    if (index > ISequence<T>::length) {
        exception_outOfRange e;
        throw e;
    }
    ISequence<T>::length++;
    if (!(ISequence<T>::length - 1)) {
        data = new T[ISequence<T>::length];
        *(data) = item;
    }
    else {
        T *buffer = new T[ISequence<T>::length];
        for (int i = 0; i < index; i++) {
            *(buffer + i) = *(data + i);
        }
        *(data + index) = item;
        for (int i = index + 1; i < ISequence<T>::length; i++) {
            *(buffer + i) = *(data + i - 1);
        }
        delete [] data;
        data = buffer;
    }
}

template <typename T>
void ArraySequence<T>::remove(const T& item) {
    int index = -1;
    for (int i = 0; i < ISequence<T>::length; i++) {
        if (*(data + i) == item) {
            index = i;
            break;
        }
    }
    ISequence<T>::length--;
    T *buffer = new T[ISequence<T>::length];
    for (int i = 0; i < index; i++) {
        *(buffer + i) = *(data + i);
    }
    for (int i = index; i < ISequence<T>::length; i++) {
        *(buffer + i) = *(data + i + 1);
    }
    delete [] data;
    data = buffer;
}

template <typename T>
void ArraySequence<T>::set(const int& index, const T& item) {
    if (index >= ISequence<T>::length) {
        exception_outOfRange e;
        throw e;
    }
    *(data + index) = item;
}

template <typename T>
template <typename ...F>
ArraySequence<T>* ArraySequence<T>::arraySequenceOf(const F&... items) {
    auto buffer = {items...};
    ArraySequence<T>* tmp = new ArraySequence<T>;
    for (auto i : buffer) {
        tmp->append(i);
    }
    return tmp;
}

template <typename T>
ArraySequence<T>::~ArraySequence() {
    delete [] data;
    data = nullptr;
    ISequence<T>::length = 0;
}

//ITERATOR METHODS//

template <typename T>
ArraySequence<T>::MyIterator::MyIterator(T* pos): pos(pos) {}

template <typename T>
ArraySequence<T>::MyIterator::MyIterator(const MyIterator& it): pos(it.pos) {}

template <typename T>
typename ArraySequence<T>::MyIterator& ArraySequence<T>::MyIterator::operator=(const MyIterator& it) {
    pos = it.pos;
    return *this;
}

template <typename T>
typename ArraySequence<T>::MyIterator::reference ArraySequence<T>::MyIterator::operator*() const {
    return *pos;
}

template <typename T>
typename ArraySequence<T>::MyIterator::pointer ArraySequence<T>::MyIterator::operator->() const {
    return pos;
}

template <typename T>
typename ArraySequence<T>::MyIterator::reference ArraySequence<T>::MyIterator::operator[](const typename MyIterator::difference_type& n) const {
    return *(pos)[n];
}

template <typename T>
typename ArraySequence<T>::MyIterator::difference_type ArraySequence<T>::MyIterator::operator-(const ArraySequence<T>::MyIterator& it) const {
    return pos - it.pos;
}

template <typename T>
typename ArraySequence<T>::MyIterator ArraySequence<T>::MyIterator::MyIterator::operator++(int) {
    return MyIterator(pos++);
}

template <typename T>
typename ArraySequence<T>::MyIterator& ArraySequence<T>::MyIterator::MyIterator::operator++() {
    ++pos;
    return *this;
}

template <typename T>
typename ArraySequence<T>::MyIterator ArraySequence<T>::MyIterator::MyIterator::operator--(int) {
    return MyIterator(pos--);
}

template <typename T>
typename ArraySequence<T>::MyIterator& ArraySequence<T>::MyIterator::MyIterator::operator--() {
    --pos;
    return *this;
}

template <typename T>
typename ArraySequence<T>::MyIterator ArraySequence<T>::MyIterator::operator+(const typename MyIterator::difference_type& n) const {
    return MyIterator(pos + n);
}

template <typename T>
typename ArraySequence<T>::MyIterator& ArraySequence<T>::MyIterator::operator+=(const typename MyIterator::difference_type& n) {
    pos += n;
    return *this;
}

template <typename T>
typename ArraySequence<T>::MyIterator ArraySequence<T>::MyIterator::operator-(const typename MyIterator::difference_type& n) const {
    return MyIterator(pos - n);
}

template <typename T>
typename ArraySequence<T>::MyIterator& ArraySequence<T>::MyIterator::operator-=(const typename MyIterator::difference_type& n) {
    pos -= n;
    return *this;
}

template <typename T>
bool ArraySequence<T>::MyIterator::operator!=(const MyIterator& it) const {
    return pos != it.pos;
}

template <typename T>
bool ArraySequence<T>::MyIterator::operator==(const MyIterator& it) const {
    return pos == it.pos;
}

template <typename T>
bool ArraySequence<T>::MyIterator::operator<(const MyIterator& it) const {
    return pos < it.pos;
}

template <typename T>
bool ArraySequence<T>::MyIterator::operator>(const MyIterator& it) const {
    return pos > it.pos;
}

template <typename T>
bool ArraySequence<T>::MyIterator::operator<=(const MyIterator& it) const {
    return pos <= it.pos;
}

template <typename T>
bool ArraySequence<T>::MyIterator::operator>=(const MyIterator& it) const {
    return pos >= it.pos;
}

template <typename T>
ArraySequence<T>::MyIterator::~MyIterator() {}

//ARRAY ITERATOR METHODS//

template <typename T>
typename ArraySequence<T>::iterator ArraySequence<T>::begin() {
    return MyIterator(data);
}

template <typename T>
typename ArraySequence<T>::iterator ArraySequence<T>::end() {
    return MyIterator(data + ISequence<T>::length);
}

template <typename T>
typename ArraySequence<T>::const_iterator ArraySequence<T>::begin() const {
    return MyIterator(data);
}

template <typename T>
typename ArraySequence<T>::const_iterator ArraySequence<T>::end() const {
    return MyIterator(data + ISequence<T>::length);
}
