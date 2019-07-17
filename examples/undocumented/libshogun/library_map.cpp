#include <shogun/lib/Map.h>
#include <shogun/io/SGIO.h>
#include <shogun/lib/common.h>

using namespace shogun;

#define SIZE 6

int main(int argc, char** argv)
{
	const char* v[SIZE] = {"Russia", "England", "Germany", "USA", "France", "Spain"};

	CMap<int32_t, const char*>* map = new CMap<int32_t, const char*>(SIZE/2, SIZE/2);

	for (int i=0; i<SIZE; i++)
		map->add(i, v[i]);

	map->remove(0);

	//SG_SPRINT("Num of elements: %d\n", map->get_num_elements());
	for (int i=0; i<SIZE; i++)
	{
		if (map->contains(i))
			;
			//SG_SPRINT("key %d contains in map with index %d and data=%s\n",
			//	i, map->index_of(i), map->get_element(i));
	}

	SG_UNREF(map);
	return 0;
}
