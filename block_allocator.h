#ifndef BLOCK_ALLOCATOR_H
#define BLOCK_ALLOCATOR_H

#include <cstddef>  // предоставляет тип size_t
#include <mutex>    // для работы с потоками

//  описываем для Doxygen
/**
 * @brief A thread-safe block memory allocator.
 */
class BlockAllocator {
public:
    /**
     * @brief Construct a new Block Allocator.
     * 
     * @param blockSize The size of one memory block.
     * @param blockCount The number of blocks reserved by the allocator.
     */
    BlockAllocator(size_t blockSize, size_t blockCount);

    /**
     * @brief Destroy the Block Allocator.
     */
    ~BlockAllocator();

    /**
     * @brief Allocate a memory block.
     * 
     * @return void* A pointer to the allocated memory block, or nullptr if no blocks are available.
     */
    void* allocate();

    /**
     * @brief Deallocate a memory block.
     * 
     * @param ptr A pointer to the memory block to deallocate.
     */
    void deallocate(void* ptr);

private:
    size_t blockSize_;
    size_t blockCount_;
    void* memory_;  //  raw memory buffer  
    void** freeBlocks_;
    size_t freeBlockCount_;
    std::mutex mutex_;  // для синхронизации потоков
};

#endif // BLOCK_ALLOCATOR_H
