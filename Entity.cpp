#include "Entity.h"

Entity::Entity(tmx::MapLoader& plvl):mlvl(plvl)
{}

void Entity::StandartMapCollisison(float DX, float DY, float& x, float& y, /*tmx::MapObject mEntityObj*/sf::FloatRect rct)
{
	for (auto layer = mlvl.GetLayers().begin(); layer != mlvl.GetLayers().end(); ++layer)
	{
		if (layer->name == "objects")
		{
			for (auto object = layer->objects.begin(); object != layer->objects.end(); ++object)
			{
				if (object->GetName() == "solid")
				{
					if (object->GetAABB().intersects(
						sf::FloatRect(x, y, /*mEntityObj.GetAABB()*/rct.width, /*mEntityObj.GetAABB()*/rct.height)))
					{
						if (DY<0)
						{
							y = object->GetAABB().top + object->GetAABB().height;
						}
						if (DY>0)
						{
							y = object->GetAABB().top - /*mEntityObj.GetAABB()*/rct.height;
						}
						if (DX<0)
						{
							x = object->GetAABB().left + object->GetAABB().width;
						}
						if (DX>0)
						{
							x = object->GetPosition().x - /*mEntityObj.GetAABB()*/rct.width;
						}
					}
				}
			}
		}
	}
}