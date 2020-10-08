#pragma once

#include <stdint.h>

#define TURT_HASH_SEED 0x32F1A902

namespace Turtle {

	inline uint32_t rotl32 ( uint32_t x, int8_t r )
	{
	  return (x << r) | (x >> (32 - r));
	}


	inline uint32_t fmix32 ( uint32_t h )
	{
	  h ^= h >> 16;
	  h *= 0x85ebca6b;
	  h ^= h >> 13;
	  h *= 0xc2b2ae35;
	  h ^= h >> 16;

	  return h;
	}

	uint32_t MurmurHash( const void * key, int numBytes)
	{
	  const uint8_t * data = (const uint8_t*)key;
	  const int nblocks = numBytes / 4;

	  uint32_t hash = TURT_HASH_SEED;

	  const uint32_t c1 = 0xcc9e2d51;
	  const uint32_t c2 = 0x1b873593;



	  const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

	  for(int i = -nblocks; i; i++)
	  {
	    uint32_t k1 = blocks[i];

	    k1 *= c1;
	    k1 = rotl32(k1,15);
	    k1 *= c2;
	    
	    hash ^= k1;
	    hash = rotl32(hash,13); 
	    hash = hash*5+0xe6546b64;
	  }


	  const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

	  uint32_t k1 = 0;

	  switch(len & 3)
	  {
	  case 3: k1 ^= tail[2] << 16;
	  case 2: k1 ^= tail[1] << 8;
	  case 1: k1 ^= tail[0];
	          k1 *= c1; k1 = rotl32(k1,15); k1 *= c2; hash ^= k1;
	  };


	  hash ^= len;

	  hash = fmix32(hash);

	  return hash;
	} 
}