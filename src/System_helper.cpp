#include<vector>
#include"System_helper.h"
#include"Entity_manager.h"
#include"Component_all.h"

bool collision_with_wall(Entity_manager* em, int x, int y)
{
	std::vector<unsigned int> walls;
	em->fill_entities_componentType(walls, C_Position | C_Wall);
	std::vector<Position*> v;
	em->fill_components_of_entities(v, walls, C_Position);

	for (size_t i=0; i<v.size(); ++i)
	{
        if ((v[i]->x == x) && (v[i]->y == y))
		{
			return true;
		}
	}
	return false;
}

