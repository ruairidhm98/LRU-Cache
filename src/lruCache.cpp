#include "lruCache.h"

#include <cmath>
#include <iostream>

template <unsigned CacheSize, unsigned CacheLineSize, unsigned verbose>
LRUCache<CacheSize,CacheLineSize,verbose>::LRUCache()
  : CL_MASK(CacheLineSize-1)
  , CL_SHIFT(log(CacheLineSize)/log(2))
  , m_lastUsedLoc(CacheSize/CacheLineSize - 1)
  , m_addressToCacheLoc(CacheSize/CacheLineSize)
  , m_cacheLocToAddress(CacheSize/CacheLineSize)
{
  initLocationStack();
}

// Initialises all the free locations in the stack to keep track of them
template <unsigned CacheSize, unsigned CacheLineSize, unsigned verbose>
inline void LRUCache<CacheSize,CacheLineSize,verbose>::initLocationStack()
{
  iterCache([this](uint32_t const& i) { m_locationStack.push(i); });
}

// If we wish to print an error message determined by template parameter verbose
template <unsigned CacheSize, unsigned CacheLineSize, unsigned verbose>
inline void LRUCache<CacheSize,CacheLineSize,verbose>::writeMessage(const char* message) const
{
  if constexpr(verbose)
  {
    std::cout << message << std::endl;
  }
}

// Common loop pattern across several functions encapsulated in own function
template <unsigned CacheSize, unsigned CacheLineSize, unsigned verbose>
template <typename Func>
inline void LRUCache<CacheSize,CacheLineSize,verbose>::iterCache(Func&& f)
{
  for (auto i = 0UL; i < CacheSize; i+=CacheLineSize)
  {
    f(i);
  }
}

// Flush the contents of the cache - evicts all data to RAM and resets state
template <unsigned CacheSize, unsigned CacheLineSize, unsigned verbose>
void LRUCache<CacheSize,CacheLineSize,verbose>::flush(int ram[])
{
  writeMessage("Flushing cache");
  iterCache([this,ram](uint32_t const& i) { writeToMemOnEvict(ram, i); });
  initLocationStack();
  m_addressToCacheLoc.clear();
  m_cacheLocToAddress.clear();
}

// Test if a main memory address is in a cache line stored in the cache
template <unsigned CacheSize, unsigned CacheLineSize, unsigned verbose>
inline bool LRUCache<CacheSize,CacheLineSize,verbose>::addressInCacheLine(uint32_t address) const
{
  return m_addressToCacheLoc.find(cacheLineAddress(address)) == m_addressToCacheLoc.end();
}

// Given a main memory address, return the corresponding cache line address
template <unsigned CacheSize, unsigned CacheLineSize, unsigned verbose>
inline uint32_t LRUCache<CacheSize,CacheLineSize,verbose>::cacheLineAddress(uint32_t address) const
{
  return address >> CL_SHIFT;
}

// Given a main memory address, return the corresponding index into the cache line
template <unsigned CacheSize, unsigned CacheLineSize, unsigned verbose>
inline uint32_t LRUCache<CacheSize,CacheLineSize,verbose>::cacheEntryPosition(uint32_t address) const
{
  return address & CL_MASK;
}

// Given a cache line address, return the corresponding main memory address
// This is the starting address of the cache line in main memory
template <unsigned CacheSize, unsigned CacheLineSize, unsigned verbose>
uint32_t LRUCache<CacheSize,CacheLineSize,verbose>::cacheLineStartMemAddress(uint32_t address) const
{
  return address << CL_SHIFT;
}

int main()
{

}