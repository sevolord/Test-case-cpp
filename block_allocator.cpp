#include "block_allocator.h"
#include <cstdlib>  // provides the std::malloc and std::free  

BlockAllocator::BlockAllocator(size_t blockSize, size_t blockCount)
    : blockSize_(blockSize), 
      blockCount_(blockCount), 
      freeBlockCount_(blockCount) 
{
    memory_ = std::malloc(blockSize * blockCount);
    freeBlocks_ = new void*[blockCount];

    for (size_t i = 0; i < blockCount; ++i) 
    {
        freeBlocks_[i] = static_cast<char*>(memory_) + blockSize * i;
    }
}

BlockAllocator::~BlockAllocator() 
{
    delete[] freeBlocks_;
    std::free(memory_);
}

void* BlockAllocator::allocate() 
{
    std::unique_lock<std::mutex> lock(mutex_); // Эта строка блокирует Mutex_, чтобы обеспечить безопасность потока во время процесса распределения.

    if (freeBlockCount_ == 0) 
    {
        return nullptr;
    }

    void* block = freeBlocks_[--freeBlockCount_];
    return block;
}

void BlockAllocator::deallocate(void* ptr) 
{
    std::unique_lock<std::mutex> lock(mutex_); // блокирует Mutex_, чтобы обеспечить безопасность потока во время процесса.

    if (ptr != nullptr && freeBlockCount_ < blockCount_) 
    {
        freeBlocks_[freeBlockCount_++] = ptr;
    }
}
