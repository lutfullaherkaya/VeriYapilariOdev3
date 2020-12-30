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

    Bucket *_table; // THE HASH TABLE

    // == DEFINE HELPER METHODS & VARIABLES BELOW ==


    Entry &bul(const K &key, bool ekliyoruz) const;

    /*
    * iostream yokmus
    void yazdir();
    */

    

public:
    // TODO: IMPLEMENT THESE FUNCTIONS.
    // CONSTRUCTORS, ASSIGNMENT OPERATOR, AND THE DESTRUCTOR
    HashTable();
    // COPY THE WHOLE CONTENT OF RHS INCLUDING THE KEYS THAT WERE SET AS DELETED
    HashTable(const HashTable<K, T> &rhs);
    HashTable<K, T> &operator=(const HashTable<K, T> &rhs);
    ~HashTable();

    // TODO: IMPLEMENT THIS FUNCTION.
    // INSERT THE ENTRY IN THE HASH TABLE WITH THE GIVEN KEY & VALUE
    // IF THE GIVEN KEY ALREADY EXISTS, THE NEW VALUE OVERWRITES
    // THE ALREADY EXISTING ONE. IF THE LOAD FACTOR OF THE TABLE IS GREATER THAN 0.6,
    // RESIZE THE TABLE WITH THE NEXT PRIME NUMBER.
    void Insert(const K &key, const T &value);

    // TODO: IMPLEMENT THIS FUNCTION.
    // DELETE THE ENTRY WITH THE GIVEN KEY FROM THE TABLE
    // IF THE GIVEN KEY DOES NOT EXIST IN THE TABLE, THROW ItemNotFoundException()
    void Delete(const K &key);

    // TODO: IMPLEMENT THIS FUNCTION.
    // IT SHOULD RETURN THE VALUE THAT CORRESPONDS TO THE GIVEN KEY.
    // IF THE KEY DOES NOT EXIST, THROW ItemNotFoundException()
    T &Get(const K &key) const;

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
    void getKeys(K *keys); // PUT THE ACTIVE KEYS TO THE GIVEN INPUT PARAMETER
};


template <class K, class T>
HashTable<K, T>::HashTable() : _capacity(NextCapacity(0)), _size(0) {
    _table = new Bucket[_capacity];
}

template <class K, class T>
HashTable<K, T>::HashTable(const HashTable<K, T> &rhs) : _table(NULL) {
    *this = rhs;
}

template <class K, class T>
HashTable<K, T> &HashTable<K, T>::operator=(const HashTable<K, T> &rhs) {
    if (_table) {
        delete[] _table;
    }
    _capacity = rhs._capacity;
    _table = new Bucket[_capacity];
    _size = rhs._size;
    for (int i = 0; i < _capacity; ++i) {
        for (int j = 0; j < BUCKET_SIZE; ++j) {
            _table[i].entries[j] = rhs._table[i].entries[j];
        }
    }
    return (*this);
}

template <class K, class T>
HashTable<K, T>::~HashTable() {
    if (_table) {
        delete[] _table;
    }
}

template <class K, class T>
void HashTable<K, T>::Insert(const K &key, const T &value) {
    /* 1. bul yap, bulursan degeri degistir don, catch yaparsan 2 ye git. 
     * not: bunun sebebi deletedlere yeni deger ekleyebilmek istememizdir.
     * 2. ara, aktifleri atlayarak ilerlerken inaktifle karþýlaþýrsan oraya ekle.
     * not: boyle yapinca deletede kadar 2 kere probe yapiyorsun ama zaten O(1) + O(1) = O(1) => 2 = 1 sorun yok.
     */
    try {
        bul(key, false).Value = value;
    }
    catch (ItemNotFoundException &hata) {
        Entry &entry = bul(key, true);
        entry.Key = key;
        entry.Value = value;
        entry.Active = true;
        entry.Deleted = false;
        _size++;
        if (_size / ((double)_capacity * BUCKET_SIZE) > 0.6) {
            Resize(NextCapacity(_capacity));
        }
    }

}

template <class K, class T>
void HashTable<K, T>::Delete(const K &key) {
    /* ara, aktifleri ve deletedleri atlayarak ilerlerken bulursan [sil], 
     * inaktif ve silinmemisle karsýlasýrsan throw
     */
    Entry &buluntu = bul(key, false);
    buluntu.Active = false;
    buluntu.Deleted = true;
    _size--;
}

template <class K, class T>
T &HashTable<K, T>::Get(const K &key) const {
    /* ara, aktifleri ve deletedleri atlayarak ilerlerken bulursan degerini [dondur]
     * inaktif ve silinmemisle karsilasirsan throw 
     */

    return bul(key, false).Value;
}


template <class K, class T>
void HashTable<K, T>::Resize(int newCapacity) {
    Bucket *eskiTable = _table;
    int eskiKapasite = _capacity;

    _capacity = newCapacity;
    _table = new Bucket[_capacity];
    _size = 0;
    
    for (int i = 0; i < eskiKapasite; ++i) {
        for (int j = 0; j < BUCKET_SIZE; ++j) {
            Entry &entry = eskiTable[i].entries[j];
            if (entry.Active) {
                Insert(entry.Key, entry.Value);
            }
        }
    }
    delete[] eskiTable;

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
void HashTable<K, T>::getKeys(K *keys) {
    int index = 0;

    for (int i = 0; i < _capacity; i++) {
        Bucket &bucket = _table[i];
        for (int j = 0; j < BUCKET_SIZE; j++) {
            if (bucket.entries[j].Active && !bucket.entries[j].Deleted) {
                keys[index++] = bucket.entries[j].Key;
            }
        }
    }
}


/*
3 durum
aktif	silinmiþ
1	    0
0	    0
0	    1
*/
template<class K, class T>
typename HashTable<K,T>::Entry & HashTable<K, T>::bul(const K & key, bool ekliyoruz) const {
    int merkez = Hash(key) % _capacity;
    for (int i = 0; i < _capacity; ++i) {
        for (int j = 0; j < BUCKET_SIZE; ++j) {
            Entry &entry = _table[(merkez+i*i)%_capacity].entries[j];
            if (ekliyoruz) {
                if (!entry.Active) {
                    return entry;
                }
            }
            else {
                if (entry.Active || entry.Deleted) {
                    if (entry.Active && entry.Key == key) {
                        return entry;
                    }
                }
                else {
                    throw ItemNotFoundException();
                }
            }
        }
    }
    if (!ekliyoruz) {
        throw ItemNotFoundException();
    }
}

/*
* iostream yokmus
template<class K, class T>
void HashTable<K, T>::yazdir() {
    std::cout << "[";
    for (int i = 0; i < _capacity; ++i) {
        std::cout << "[";
        for (int j = 0; j < BUCKET_SIZE; ++j) {
            Entry &entry = _table[i].entries[j];
            std::cout << "(" << entry.Key << "," << entry.Value<< ")";
        }
        std::cout << "]";
        if (i < _capacity - 1) {
            std::cout << ",";
        }
    }
    std::cout << "]";
}
*/


#endif