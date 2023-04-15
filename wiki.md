#block_allocator.h
```cpp

#ifndef BLOCK_ALLOCATOR_H
#define BLOCK_ALLOCATOR_H
```
Эти линии включают защиту.Они предотвращают множество включений одного и того же файла заголовка, что может вызвать ошибки переопределения.

```cpp

#include <cstddef>
#include <mutex>
```
Эти строки включают необходимые стандартные заголовки библиотеки C ++.<cstddef> предоставляет тип size_t, а <mutex> предоставляет класс std :: mutex для синхронизации потоков.

```cpp

/**
 * @brief A thread-safe block memory allocator.
 */
```
Это комментарий doxugen, который описывает класс блокалокатора как абонентского распределителя блочной памяти.

```cpp

class BlockAllocator {
```
Эта линия объявляет класс 

```cpp

public:
```
Эта строка знаменует собой начало публичного раздела класса, в котором содержится публичный API, к которому могут получить доступ пользователи класса.

```cpp

    /**
     * @brief Construct a new Block Allocator.
     * 
     * @param blockSize The size of one memory block.
     * @param blockCount The number of blocks reserved by the allocator.
     */
    BlockAllocator(size_t blockSize, size_t blockCount);
```
Это Doxygen Комментарий и конструктор объявления для класса BlockAllocator.Конструктор принимает два параметра: размер одного блока памяти (Blocksize) и количество блоков, зарезервированных распределителем (BlockCount).

```cpp

    /**
     * @brief Destroy the Block Allocator.
     */
    ~BlockAllocator();
```
Это комментирование Doxygen и декларация деструктора для класса Blockallocator.Деструктор очистит любые ресурсы, выделенные классом, когда объект будет уничтожен.
```cpp

    /**
     * @brief Allocate a memory block.
     * 
     * @return void* A pointer to the allocated memory block, or nullptr if no blocks are available.
     */
    void* allocate();
```
Это комментирование Doxygen и объявление функции членов для функции выделения.Функция выделяет блок памяти и возвращает указатель на него или nullptr, если блока нет.

```cpp

    /**
     * @brief Deallocate a memory block.
     * 
     * @param ptr A pointer to the memory block to deallocate.
     */
    void deallocate(void* ptr);
```
Это комментирование Doxygen и объявление функции членов для функции Deallocate.Функция принимает указатель на блок памяти (PTR) и делится его.

```cpp

private:
```
Эта линия знаменует собой начало частного раздела класса, который содержит внутреннюю детали реализации, которые не должны быть доступны для пользователей класса.

```cpp

    size_t blockSize_;
    size_t blockCount_;
    void* memory_;
    void** freeBlocks_;
    size_t freeBlockCount_;
    std::mutex mutex_;
```
Эти строки объявляют частные переменные класса.Они хранят размер блока (blocksize_), количество блоков (blockcount_), буфер с необработанной памятью (memory_), массив указателей на свободные блоки (Freeblocks_), количество свободных блоков (FreeblockCount_) и мутекс для потока.Синхронизация (mutex_).

```cpp

};
```
Эта линия знаменует собой конец объявления класса.

```cpp

#endif // BLOCK_ALLOCATOR_H
```
Эта линия знаменует собой конец включения защиты.Он сигнализирует о препроцессоре прекратить обработку содержимого файла заголовка, если макрос Block_allocator_H уже был определен.


# block_allocator.cpp
```cpp

#include "block_allocator.h"
#include <cstdlib>
```
Эти строки включают необходимые файлы заголовка.«block_allocator.h» - это файл заголовка для класса Blockallocator, а <cstdlib> предоставляет std :: malloc и std :: бесплатные функции для управления памятью.

```cpp

BlockAllocator::BlockAllocator(size_t blockSize, size_t blockCount)
    : blockSize_(blockSize), blockCount_(blockCount), freeBlockCount_(blockCount) {
```
Эта линия определяет конструктор класса.Он инициализирует переменные элемента blocksize_, blockcount_ и freeblockcount_ с предоставленными аргументами.
```cpp

    memory_ = std::malloc(blockSize * blockCount);
```
Эта линия выделяет необработанный буфер памяти размером блокировки * BlockCount с использованием std :: malloc и сохраняет указатель в переменной Memory_ .
```cpp

    freeBlocks_ = new void*[blockCount];
```
Эта линия выделяет массив указателей на свободные блоки с размером, равным блоку, и хранит указатель в переменной элемента FreeBlocks_.
```cpp

    for (size_t i = 0; i < blockCount; ++i) {
        freeBlocks_[i] = static_cast<char*>(memory_) + blockSize * i;
    }
```
Этот цикл инициализирует указатели в массиве FreeBlocks_, разделяя буфер Memory_ на блоки одинаково размера, определяемые блоками.

```cpp

BlockAllocator::~BlockAllocator() {
```
Эта строка определяет деструктор класса BlockAllocator.

```cpp

    delete[] freeBlocks_;
```
Эта линия складывает массив FreeBlocks_, который был выделен новым [] в конструкторе.

```cpp

    std::free(memory_);
```
Эта строка складывает необработанную память о буфере памяти, которая была выделена с помощью Std :: Malloc в конструкторе.


```cpp

void* BlockAllocator::allocate() {
    std::unique_lock<std::mutex> lock(mutex_);
```
Эта строка блокирует Mutex_, чтобы обеспечить безопасность потока во время процесса распределения.

```cpp

    if (freeBlockCount_ == 0) {
        return nullptr;
    }
```
проверка на пустые строки.Если нет, он возвращает nullptr.
```cpp

    void* block = freeBlocks_[--freeBlockCount_];
```
Эта линия получает следующий свободный указатель блока из массива freeblocks_ и уменьшает freeblockcount_ на один.

```cpp

    return block;
```
Эта строка возвращает выделенный указатель блока.

```cpp

void BlockAllocator::deallocate(void* ptr) {
```
Эта строка определяет функцию члена Deallocate в классе Blockallocator.

```cpp

    std::unique_lock<std::mutex> lock(mutex_);
```
Эта строка блокирует Mutex_, чтобы обеспечить безопасность потока во время процесса сделки.

```cpp

    if (ptr != nullptr && freeBlockCount_ < blockCount_) {
```
проверяет, не является ли предоставленным PTR PTRTER NULLPTR, и есть ли место в массиве `freeblocks_ для хранения блока Deallocated.
```cpp

        freeBlocks_[freeBlockCount_++] = ptr;
```
This line stores the deallocated block pointer ptr back into the freeBlocks_ array and increases the freeBlockCount_ by one.

#test_block_allocator.cppblock_allocator.cpp

```cpp

#include "block_allocator.h"
#include <gtest/gtest.h>
#include <thread>
#include <vector>

TEST(BlockAllocator, BasicAllocationAndDeallocation) {
```
Эта строка определяет новый тест Google Test Cases с именем Blockallocator с тестированием базового распределение и деаллокация.

```cpp

    BlockAllocator allocator(64, 10);
```
Эта линия создает новый экземпляр блоBlockAllocator калокатора с размером блока 64 байта и 10 блоков.

```cpp

    void* ptr = allocator.allocate();
    EXPECT_NE(ptr, nullptr);

Эти строки выделяют блок памяти от распределения, а затем используйте макрос EXPECT_NE, чтобы проверить, что возвращенный указатель не равен nullptr.
```cpp

    allocator.deallocate(ptr);
```
Эта линия разбирается с ранее выделенным блоком памяти.

```cpp

TEST(BlockAllocator, ExhaustingAllocator) { 
```
Эта строка определяет новый тест Google Test Cases с именем Blockallocator с тестовым исчерпывающим аллокатором.
```cpp

    BlockAllocator allocator(64, 10);
```
Эта линия создает новый экземпляр блокалокатора с размером блока 64 байта и 10 блоков.
```cpp

    std::vector<void*> ptrs;
```
Эта линия объявляет вектор для хранения указателей на выделенные блоки памяти.
```cpp

    for (size_t i = 0; i < 10; ++i) {
        void* ptr = allocator.allocate();
        EXPECT_NE(ptr, nullptr);
        ptrs.push_back(ptr);
    }
```
Эти строки выделяют 10 блоков памяти от выделения, убеждается, что возвращенные указатели не равны Nullptr, и хранят указатели в векторе PTRS.
```cpp

    void* exhaustedPtr = allocator.allocate();
    EXPECT_EQ(exhaustedPtr, nullptr);
```
Эти линии пытаются выделить дополнительный блок памяти после того, как аллокатор был закончен, и убеждается, что возвращенный указатель равен Nullptr.
```cpp

    for (void* ptr : ptrs) {
        allocator.deallocate(ptr);
    }
```
Эти строки удаляют все ранее выделенные блоки памяти.


```cpp

void threadFunc(BlockAllocator* allocator, std::vector<void*>& allocated) {
```
Эта строка определяет функцию Threadfunc, которая будет использоваться в качестве точки входа потока.Функция принимает указатель на экземпляр BlockAllocator и ссылку на вектор для хранения выделенных блоков памяти.

```cpp

    for (size_t i = 0; i < 5; ++i) {
        void* ptr = allocator->allocate();
        if (ptr != nullptr) {
            allocated.push_back(ptr);
        }
    }
}
```
Эти линии выделяют 5 блоков памяти, используя заданный распределитель, и сохраняют указатели в выделенном векторе.Цикл заканчивает определение функции для Threadfunc.

```cpp

TEST(BlockAllocator, MultithreadedAllocationAndDeallocation) {
```
Эта строка определяет новый тест Google Test Cases с именем Blockallocator с тестированием Многопоточное выделение и распределение.
```cpp

    BlockAllocator allocator(64, 10);
```
Эта линия создает новый экземпляр блокалокатора с размером блока 64 байта и 10 блоков.
```cpp

    std::vector<void*> allocated1, allocated2;
```
Эти линии объявляют два вектора для хранения указателей на выделенные блоки памяти для каждого потока.
```cpp

    std::thread t1(threadFunc, &allocator, std::ref(allocated1));
    std::thread t2(threadFunc, &allocator, std::ref(allocated2));
```
Эти строки создают два потока T1 и T2, передавая функцию Threadfunc в качестве точки входа и обеспечивая ссылку на распределитель и соответствующий отведенный вектор для каждого потока.
```cpp

    t1.join();
    t2.join();
```
Эти строки соединяются с двумя потоками, ожидая, когда они завершит выполнение.

```cpp

    EXPECT_EQ(allocated1.size() + allocated2.size(), 10);
```
Эта строка проверяет, что общее количество выделенных блоков памяти по обоим потокам равно 10, что является общим количеством блоков в распределении.

```cpp

    for (void* ptr : allocated1) {
        allocator.deallocate(ptr);
    }
    for (void* ptr : allocated2) {
        allocator.deallocate(ptr);
    }
```
Эти строки разбирают все ранее выделенные блоки памяти из обоих потоков.

```cpp

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```
Эти строки определяют основную функцию, которая инициализирует структуру тестирования Google с аргументами командной строки и запускает все определенные тестовые случаи.


#makefile

```makefile

CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O2
```
определяют переменные для Makefile.CXX установлен на компилятор G ++, а CXXFLAGS устанавливаются на различные параметры компилятора:
    -Wall: Включить все предупреждения.
    -Wextra: Включить дополнительные предупреждения, не покрытые -Wall.
    -pedantic: Применение строгого соответствия стандартов ISO C ++.
    -std=c++11: указывает стандарт языка C ++ 11.
    -O2: устанавливает уровень оптимизации на 2 для хорошего баланса между производительностью и временем компиляции.

```makefile

LDFLAGS = -pthread
```
Эта строка определяет флаги линкера.-pthread используется для связи программы с библиотекой потоков POSIX.

```makefile

LIBS = -lgtest -lgtest_main
```
Эта строка определяет библиотеки для связи.-lgtest ссылки на библиотеку тестирования Google, и -lgtest_main ссылается на основную функцию Test Google.

```makefile

all: test_block_allocator
```
Эта строка определяет цель по умолчанию и указывает, что она зависит от цели test_block_allocator.Когда вы запускаете марку без аргументов, он по умолчанию строит все цели.
```makefile

test_block_allocator: test_block_allocator.o block_allocator.o
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)
```
Эта строка определяет цель test_block_allocator, которая зависит от объектных файлов test_block_allocator.o и block_allocator.o., которая следует за составлением этих файлов объекта в исполняемый именованный TEST_BLOCK_ALLOCATOR, используя указанные флаги и библиотеки линкера.

```makefile

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
```
Эта строка определяет правило шаблона, в котором рассказывается, как создавать объектные файлы из исходных файлов.Для каждого файла .cpp он компилирует исходный файл в файл объекта с указанными флагами компилятора.

```makefile

clean:
	rm -f *.o test_block_allocator
```
Эта линия определяет чистую цель, которая не имеет никаких зависимостей.Команда, которая следует за удалением всех файлов объекта, и исполняемый файл test_block_allocator.

```makefile

.PHONY: all clean
```
Эта линия говорит, что все и чистые - фальшивые цели, что означает, что они не представляют фактические файлы.Это предотвращает создание из -за смущения этих целей с файлами одного и того же имени и гарантирует, что целевые показатели всегда выполняются при указании.