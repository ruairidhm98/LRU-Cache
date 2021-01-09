#include "lruCache.h"

#include <cmath>
#include <iostream>

template <unsigned CacheSize, unsigned CacheLineSize, unsigned verbose>
LRUCache::LRUCache()
  : CL_MASK(CacheLineSize-1)
  , CL_SHIFT(log(CACHELINE_SZ)/log(2))
  , m_lastUsedLoc(CacheSize/CacheLineSize - 1)
{
  initLocationStack();
}

// Initialises all the free locations in the stack to keep track of them
template <unsigned CacheSize, unsigned CacheLineSize, unsigned verbose>
inline void LRUCache::initLocationStack()
{
  iterCache([this](uint32_t const& i) { m_locationStack.push(i); });
}

// If we wish to print an error message determined by template parameter verbose
template <unsigned CacheSize, unsigned CacheLineSize, unsigned verbose>
inline void LRUCache::writeMessage(const char* message) const
{
  if (constexpr(verbose))
  {
    std::cout << message << std::endl;
  }
}

// Common loop pattern across several functions encapsulated in own function
template <unsigned cachesize, unsigned cachelinesize, unsigned verbose>
template <typename Func>
inline void LRUCache::iterCache(Func&& f)
{
  for (auto i = 0UL; i < CacheSize; i+=CacheLineSize)
  {
    f(i);
  }
}


// Flush the contents of the cache - evicts all data to RAM and resets state
template <unsigned cachesize, unsigned cachelinesize, unsigned verbose>
void LRUCache::flush(int ram[])
{
  writeMessage("Flushing cache");
  iterCache([this,ram](uint32_t const& i) { writeToMemOnEvict(ram, i); });
  initLocationStack();
  m_addressToCacheLoc.clear();
  m_cacheLocToAddress.clear();
}
