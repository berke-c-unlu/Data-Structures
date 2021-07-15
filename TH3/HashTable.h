#ifndef __HASHTABLE__
#define __HASHTABLE__

#include "HashUtils.h"
#include "ItemNotFoundException.h"
/* Do not add new libraries or files */

#define BUCKET_SIZE 2

// Do not modify the public interface of this class.
// Otherwise, your code will note compile!
template <class K, class T>
class HashTable {
    struct Entry {
        K Key;             // the key of the entry
        T Value;   // the value of the entry
        bool Deleted;        // flag indicating whether this entry is deleted
        bool Active;         // flag indicating whether this item is currently used

        Entry() : Key(), Value(), Deleted(false), Active(false) {}
    };

    struct Bucket {
        Entry entries[BUCKET_SIZE];
    };

    int _capacity; // INDICATES THE TOTAL CAPACITY OF THE TABLE
    int _size; // INDICATES THE NUMBER OF ITEMS IN THE TABLE

    Bucket* _table; // THE HASH TABLE

    // == DEFINE HELPER METHODS & VARIABLES BELOW ==

public:
    // TODO: IMPLEMENT THESE FUNCTIONS.
    // CONSTRUCTORS, ASSIGNMENT OPERATOR, AND THE DESTRUCTOR
    HashTable();
    // COPY THE WHOLE CONTENT OF RHS INCLUDING THE KEYS THAT WERE SET AS DELETED
    HashTable(const HashTable<K, T>& rhs);
    HashTable<K, T>& operator=(const HashTable<K, T>& rhs);
    ~HashTable();

    // TODO: IMPLEMENT THIS FUNCTION.
    // INSERT THE ENTRY IN THE HASH TABLE WITH THE GIVEN KEY & VALUE
    // IF THE GIVEN KEY ALREADY EXISTS, THE NEW VALUE OVERWRITES
    // THE ALREADY EXISTING ONE. IF THE LOAD FACTOR OF THE TABLE IS GREATER THAN 0.6,
    // RESIZE THE TABLE WITH THE NEXT PRIME NUMBER.
    void Insert(const K& key, const T& value);

    // TODO: IMPLEMENT THIS FUNCTION.
    // DELETE THE ENTRY WITH THE GIVEN KEY FROM THE TABLE
    // IF THE GIVEN KEY DOES NOT EXIST IN THE TABLE, THROW ItemNotFoundException()
    void Delete(const K& key);

    // TODO: IMPLEMENT THIS FUNCTION.
    // IT SHOULD RETURN THE VALUE THAT CORRESPONDS TO THE GIVEN KEY.
    // IF THE KEY DOES NOT EXIST, THROW ItemNotFoundException()
    T& Get(const K& key) const;

    // TODO: IMPLEMENT THIS FUNCTION.
    // AFTER THIS FUNCTION IS EXECUTED THE TABLE CAPACITY MUST BE
    // EQUAL TO newCapacity AND ALL THE EXISTING ITEMS MUST BE REHASHED
    // ACCORDING TO THIS NEW CAPACITY.
    // WHEN CHANGING THE SIZE, YOU MUST REHASH ALL OF THE ENTRIES
    void Resize(int newCapacity);

    // THE IMPLEMENTATION OF THESE FUNCTIONS ARE GIVEN TO YOU
    // DO NOT MODIFY!
    int Capacity() const; // RETURN THE TOTAL CAPACITY OF THE TABLE
    int Size() const; // RETURN THE NUMBER OF ACTIVE ITEMS
    void getKeys(K* keys); // PUT THE ACTIVE KEYS TO THE GIVEN INPUT PARAMETER
};


template <class K, class T>
HashTable<K, T>::HashTable() {
    _capacity = 7; // the smallest prime number in hashutils
    _size = 0; // there is no entry
    _table = new Bucket[_capacity];
}

template <class K, class T>
HashTable<K, T>::HashTable(const HashTable<K, T>& rhs) {

    this->_table = nullptr;
    *this = rhs;
}

template <class K, class T>
HashTable<K, T>& HashTable<K, T>::operator=(const HashTable<K, T>& rhs) {

    if (this != &rhs) {
        //we need to delete the lhs table
        delete[] this->_table;
        this->_table = nullptr;
        _table = new Bucket[rhs.Capacity()];

        for (int i = 0; i < rhs.Capacity(); i++) { // copy every entry to lhs
            for (int j = 0; j < BUCKET_SIZE; j++) {
                this->_table[i].entries[j] = rhs._table[i].entries[j];
            }
        }

        this->_size = rhs.Size();
        this->_capacity = rhs.Capacity();
    }
    return *this;
}

template <class K, class T>
HashTable<K, T>::~HashTable() {

    delete[] _table;
    _table = nullptr;
}

template <class K, class T>
void HashTable<K, T>::Insert(const K& key, const T& value) {

    double load_factor = (double)Size() / (2 * Capacity()); // load factor

    if (load_factor > 0.6) { // if load factor is too large rehash
        int nextCap = NextCapacity(_capacity);
        Resize(nextCap);
    }
    int code = Hash(key) % Capacity(); // hashcode

    Entry inserting_one = Entry(); // entry corresponding to parameters
    inserting_one.Key = key;
    inserting_one.Value = value;
    inserting_one.Active = true;
    inserting_one.Deleted = false;


    if (_table[code].entries[0].Key == key && (!_table[code].entries[0].Deleted && _table[code].entries[0].Active)) { // replace 0
        _table[code].entries[0] = inserting_one;
    }

    else if (_table[code].entries[1].Key == key && (!_table[code].entries[1].Deleted && _table[code].entries[1].Active)) { // replace 1
        _table[code].entries[1] = inserting_one;
    }

    else if (_table[code].entries[0].Deleted || !_table[code].entries[0].Active) { // one shot find 0
        _table[code].entries[0] = inserting_one;
        _size++;
    }

    else if (_table[code].entries[1].Deleted || !_table[code].entries[1].Active) { // one shot find 1
        _table[code].entries[1] = inserting_one;
        _size++;

    }

    else {
        //quadratic probing is needed
        for (int i = 1; i < Capacity(); i++) {
            int newCode = (code + (i * i)) % (_capacity); // quad code

            //replace 0
            if (_table[newCode].entries[0].Key == key && (!_table[newCode].entries[0].Deleted && _table[newCode].entries[0].Active)) {
                _table[newCode].entries[0] = inserting_one;
                break;

            }
            //replace 1 
            else if (_table[newCode].entries[1].Key == key && (!_table[newCode].entries[1].Deleted && _table[newCode].entries[1].Active)) {
                _table[newCode].entries[1] = inserting_one;
                break;

            }
            //insert 0
            else if (_table[newCode].entries[0].Deleted || !_table[newCode].entries[0].Active) {
                _table[newCode].entries[0] = inserting_one;
                _size++;
                break;
            }
            //insert 1
            else if (_table[newCode].entries[1].Deleted || !_table[newCode].entries[1].Active) {
                _table[newCode].entries[1] = inserting_one;
                _size++;
                break;
            }
        }
    }
}

template <class K, class T>
void HashTable<K, T>::Delete(const K& key) {

    bool flag = false; // decide found or not

    int code = Hash(key) % Capacity(); // hash code

    for (int i = 0; i < Capacity(); i++) {

        int newCode = (code + (i * i)) % Capacity(); // quad code but it searches for (code + 0) % Capacity() too.

        if (_table[newCode].entries[0].Key == key && !_table[newCode].entries[0].Deleted) { // 0 index

            _table[newCode].entries[0].Deleted = true;
            _table[newCode].entries[0].Active = false;
            _size--;
            flag = true;
            break;
        }
        else if (_table[newCode].entries[1].Key == key && !_table[newCode].entries[1].Deleted) { // 1 index

            _table[newCode].entries[1].Deleted = true;
            _table[newCode].entries[1].Active = false;
            _size--;
            flag = true;
            break;
        }
    }
    //if the entry does not exist throw exception
    if (!flag)
        throw ItemNotFoundException();

}

template <class K, class T>
T& HashTable<K, T>::Get(const K& key) const {

    bool flag = false; // decide found or not

    int code = Hash(key) % Capacity(); // hash code

    for (int i = 0; i < Capacity(); i++) { // search every entry

        int index = (code + (i * i)) % Capacity(); // quad code + (code+0)

        for (int j = 0; j < BUCKET_SIZE; j++) {

            if (_table[index].entries[j].Key == key && !_table[index].entries[j].Deleted && _table[index].entries[j].Active) {
                flag = true;
                return _table[index].entries[j].Value;
            }
        }
    }
    // if the entry does not exist, throw exception
    if (!flag)
        throw ItemNotFoundException();
}


template <class K, class T>
void HashTable<K, T>::Resize(int newCapacity) {

    Bucket* newTable = new Bucket[newCapacity]; // new array with newCapacity
    int oldCapacity = Capacity();

    //rehash

    for (int i = 0; i < oldCapacity; i++) { // iterate every element in old table

        if (!_table[i].entries[0].Deleted || _table[i].entries[0].Active) { // if it is not deleted or it is active

            int code1 = Hash(_table[i].entries[0].Key) % newCapacity; // first entry's code

            if (!newTable[code1].entries[0].Active) { // check for empty 1st
                newTable[code1].entries[0] = _table[i].entries[0];
            }
            else if (!newTable[code1].entries[1].Active) {// check for empty 2nd
                newTable[code1].entries[1] = _table[i].entries[0];
            }
            else {
                //quad probing
                for (int j = 1; j < newCapacity; j++) {
                    int newCode1 = (code1 + (j * j)) % newCapacity;
                    if (!newTable[newCode1].entries[0].Active) {
                        newTable[newCode1].entries[0] = _table[i].entries[0];
                        break;
                    }
                    else if (!newTable[newCode1].entries[1].Active) {
                        newTable[newCode1].entries[1] = _table[i].entries[0];
                        break;
                    }
                }
            }
        }
        if (!_table[i].entries[1].Deleted || _table[i].entries[1].Active) { // if it is not deleted or it is active

            int code2 = Hash(_table[i].entries[1].Key) % newCapacity; // second entry's code

            if (!newTable[code2].entries[0].Active) { // check for empty 1st
                newTable[code2].entries[0] = _table[i].entries[1];
            }
            else if (!newTable[code2].entries[1].Active) { // check for empty 2nd
                newTable[code2].entries[1] = _table[i].entries[1];
            }
            else {
                //quad probing
                for (int j = 1; j < newCapacity; j++) {
                    int newCode2 = (code2 + (j * j)) % newCapacity;
                    if (!newTable[newCode2].entries[0].Active) { // check for empty 1st
                        newTable[newCode2].entries[0] = _table[i].entries[1];
                        break;
                    }

                    else if (!newTable[newCode2].entries[1].Active) {
                        newTable[newCode2].entries[1] = _table[i].entries[1];
                        break;
                    }
                }
            }

        }
    }
    _capacity = newCapacity;
    delete[] _table;
    _table = newTable;
}


template <class K, class T>
int HashTable<K, T>::Capacity() const {
    return _capacity;
}

template <class K, class T>
int HashTable<K, T>::Size() const {
    return _size;
}


template <class K, class T>
void HashTable<K, T>::getKeys(K* keys) {
    int index = 0;

    for (int i = 0; i < _capacity; i++) {
        Bucket& bucket = _table[i];
        for (int j = 0; j < BUCKET_SIZE; j++) {
            if (bucket.entries[j].Active && !bucket.entries[j].Deleted) {
                keys[index++] = bucket.entries[j].Key;
            }
        }
    }
}
#endif