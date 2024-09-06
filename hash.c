#define CACHE_SIZE 2000

// DJB2 Hash function to ensure uniform distribution & reduce collision.
// Hashing -> to compute the index for a given key

unsigned int hash(const char* key)
{
	unsigned int hash=0;
	while(*key)
	{
		hash=(hash<<5)+hash+*key++;
	}
	return (hash%CACHE_SIZE);
}
