// Interface for a cache

class Status;

class Cache
{
public:
  // Reads a value from a given main memory address location
  virtual int read(int address, int ram[], Status status) = 0;
   // Write a value to a given main memory address location
  virtual void write(int address, int data, int ram[], Status status) = 0;
   // Flush means remove all data from the cache and write it back to memory
  virtual void flush(int ram[]) = 0;
};