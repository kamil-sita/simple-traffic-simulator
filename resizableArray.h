#ifndef SIMULATOR_RESIZABLEARRAY_H
#define SIMULATOR_RESIZABLEARRAY_H

#include <stdexcept>

const unsigned int ARRAY_GROWTH = 2;
const unsigned int INITIAL_SIZE_DEFAULT = 4;

/**
 * Zastępstwo klasy Wektor, zoptymalizowana dla obiektów o małym rozmiarze (wskaźniki), ze względu na to że dodawane obiekty
 * są kopiowane.
 * @tparam T typ
 */
template <class T> class resizableArray {
private:
    T* array = nullptr;
    ///zarezerwowane miejsce w pamięci
    unsigned int allocatedSize = 0;
    ///zajęte miejsce w tablicy
    unsigned int filledSize = 0;
    ///rezerwuje więcej miejsca w pamięci
    void addSpace() {
        auto oldArray = this->array;
        auto oldSize = allocatedSize;
        allocatedSize *= ARRAY_GROWTH;
        this->array = new T[allocatedSize];
        for (int i = 0; i < oldSize; i++) {
            this->array[i] = oldArray[i];
        }
        delete [] oldArray;
    }
public:
    ///inicjalizacja tablicy
    resizableArray(unsigned int initialSize = INITIAL_SIZE_DEFAULT) {
        this->allocatedSize = initialSize;
        if (initialSize <= 0) {
            return;
        }
        this->array = new T[initialSize];
    }

    ///ilość zajętych miejsc tablicy (do iterowania)
    inline unsigned int size() {
        return filledSize;
    }

    ///dodanie elementu na ostatnią pozycję
    void add(T element) {
            if (filledSize >= allocatedSize) {
            addSpace();
        }
        array[filledSize] = element;
        filledSize++;
    }

    ///zwraca czy element znajduje się w tablicy
    bool contains(T element) {
        for (int i = 0; i < filledSize; i++) {
            if (array[i] == element) {
                return true;
            }
        }
        return false;
    }

    ///zwraca indeks elementu lub -1 gdy element nie znajduje się w tablicy
    int indexOf(T element) {
        for (int i = 0; i < filledSize; i++) {
            if (array[i] == element) {
                return i;
            }
        }
        return -1;
    }

    ///usuwanie elementu o danym indeksie z tablicy
    void remove(int index) {
        if (index < 0 || index > filledSize) {
            throw std::invalid_argument( "remove: received value of " + std::to_string(index) + ", while lowest is 0, and biggest is " + std::to_string(filledSize));
        }
        for (int i = index; i < filledSize - 1; i++) {
            array[i] = array[i + 1];
        }
        filledSize--;
    }

    ///usuwanie danego elementu z tablicy
    void removeElement(T element) {
        int position = this->indexOf(element);
        remove(position);
    }

    ///usunięcie wszystkich elementów z tablicy
    inline void removeAll() {
        filledSize = 0;
    }

    ///pobranie elementu na pozycji index
    T get(int index) {
        if (index > filledSize || index < 0) {
            throw std::invalid_argument( "get: received value of " + std::to_string(index) + ", while lowest is 0, and biggest is " + std::to_string(filledSize));
        }
        return array[index];
    }

    ///zmienia istniejący element na liście na inny
    void replace(int index, T element) {
        if (index < 0 || index > size()) {
            throw std::invalid_argument( "replace: received value of " + std::to_string(index) + ", while lowest is 0, and biggest is " + std::to_string(filledSize));
        }
        array[index] = element;
    }

    ///pobranie ostatniego elementu z tablicy lub nullptr jeżeli tablica jest pusta
    T getLast() {
        if (filledSize == 0) return nullptr;
        return array[filledSize - 1];
    }

    ~resizableArray() {
        delete[] array;
    }

};

#endif