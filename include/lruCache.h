#pragma once

#include "cache.h"

#include <stack>
#include <unordered_map>

template <unsigned CacheSize, unsigned CacheLineSize, unsigned verbose=0>
class LRUCache : public Cache
{
private:
  // Used for computing addresses fast with bitwise operations
  const uint32_t CL_MASK;
  const uint32_t CL_SHIFT;
  // The last recently used location in the cache
  uint32_t m_lastUsedLoc;
  // The cache itself
  int32_t m_cache[CacheSize];
  // Translate between memory address and cache locations
  std::unordered_map<int,int> m_addressToCacheLoc;
  // Translate between cache locations and memory addresses
  std::unordered_map<int,int> m_cacheLocToAddress;
  // Keep track of the next free locations in a stack
  std::stack<uint32_t> m_nextFreeLocations;

  void initLocationStack();
  void writeMessage(const char* message) const;
  template <typename Func>
  void iterCache(Func&& f);
  void writeDataToCache(int ram[], uint32_t address, int data);
  int readDataFromCache(int ram[], uint32_t address);
  void readFromMemOnMiss(int ram[], uint32_t address);
  void updateCacheEntry(uint32_t address, int data);
  int32_t fetchCacheEntry(uint32_t address);
  uint32_t getNextFreeLocation();
  void writeToMemOnEvict(int ram[], uint32_t loc);
  bool addressInCacheLine(uint32_t address);
  uint32_t cacheLineAddress(uint32_t address);
  uint32_t cacheEntryPosition(uint32_t address);
  uint32_t cacheLineStartMemAddress(uint32_t address);
public:

  LRUCache();
  virtual void flush(int ram[]) override;
  virtual int read(int address, int[] ram) override;
  virtual void write(int address, int data, int ram[]) override;


};