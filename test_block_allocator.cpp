#include "block_allocator.h"
#include <gtest/gtest.h>
#include <thread>
#include <vector>

TEST(BlockAllocator, BasicAllocationAndDeallocation) // базового распределение и деаллокация.
{
    BlockAllocator allocator(64, 10);

    void *ptr = allocator.allocate();
    EXPECT_NE(ptr, nullptr);

    allocator.deallocate(ptr);
}

TEST(BlockAllocator, ExhaustingAllocator) // исчерпывающий аллокатор
{
    BlockAllocator allocator(64, 10);

    std::vector<void *> ptrs;
    for (size_t i = 0; i < 10; ++i)
    {
        void *ptr = allocator.allocate();
        EXPECT_NE(ptr, nullptr);
        ptrs.push_back(ptr);
    }

    //  пытаемся выделить дополнительный блок памяти после исчерпания и проверяем, что возвращаемый указатель равен nullptr.
    void *exhaustedPtr = allocator.allocate();
    EXPECT_EQ(exhaustedPtr, nullptr);


    for (void *ptr : ptrs)
    {
        allocator.deallocate(ptr);
    }
}

// функция, будет использоваться в качестве точки входа потока. Функция принимает указатель на экземпляр BlockAllocator и ссылку на вектор для хранения выделенных блоков памяти.
void threadFunc(BlockAllocator *allocator, std::vector<void *> &allocated)
{
    for (size_t i = 0; i < 5; ++i)
    {
        void *ptr = allocator->allocate();
        if (ptr != nullptr)
        {
            allocated.push_back(ptr);
        }
    }
}

TEST(BlockAllocator, MultithreadedAllocationAndDeallocation) // Многопоточное выделение и распределение
{
    BlockAllocator allocator(64, 10);
    std::vector<void *> allocated1, allocated2;
    std::thread t1(threadFunc, &allocator, std::ref(allocated1));
    std::thread t2(threadFunc, &allocator, std::ref(allocated2));

    t1.join();
    t2.join();

    EXPECT_EQ(allocated1.size() + allocated2.size(), 10); // проверяем, что общее количество выделенных блоков памяти по обоим потокам равно 10

    for (void *ptr : allocated1)
    {
        allocator.deallocate(ptr);
    }
    for (void *ptr : allocated2)
    {
        allocator.deallocate(ptr);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}