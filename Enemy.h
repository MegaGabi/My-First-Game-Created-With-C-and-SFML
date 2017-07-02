#pragma once
#include <cmath>
#include "Entity.h"
#include "Player.h"
#include "World.h"

const int dir = 9;
const int dx[dir] = { -32, 0, 32, -32, 0, 32, -32, 0, 32 };
const int dy[dir] = { -32, -32, -32, 0, 0, 0, 32, 32, 32 };

struct Node
{
	sf::Vector2f node;

	sf::Vector2f CameFrom;

	int GValue;
	int FValue;
	int Hvalue;

	Node(sf::Vector2f pos)
	{
		node = pos;
		GValue = 0; FValue = 0; Hvalue = 0;
	}

	Node(sf::Vector2f pos, int g, int h, int f)
	{
		node = pos;
		GValue = g; FValue = f; Hvalue = h;
	}

	sf::Vector2f getSize()
	{
		return sf::Vector2f(32, 32);
	}

	int getFValue(sf::Vector2f position)
	{
		FValue = GValue + getHValue(position);
		return FValue;
	}


	int getGValue(const int& i, int prevG) // i: direction
	{
		GValue += (i % 2 == 0 ? 32 : 44) + prevG;
		return GValue;
	}

	const int & getHValue(sf::Vector2f position) const
	{
		static int rd, cd, d;
		rd = position.x - node.x;
		cd = position.y - node.y;

		d = abs(rd) + abs(cd);

		return(d);
	}

	friend bool operator<(const Node & a, const Node & b)
	{
		return a.FValue < b.FValue;
	}
};

class Enemy:public Entity
{
	enum Enemies
	{
		crab,
	};

	sf::RenderWindow&	W;
	
	std::vector<Enemy>&			Brothers;
	std::vector<sf::Vector2f>	pathMap;
	sf::Vector2f				toGoIfAvoid;
	sf::Sprite					body;
	sf::Time					LastHit;
	sf::Time					PathFindCoolDown;
	Player&						player;
	World&						mWorld;
	int							health;
	std::vector<int>			attack_frames;
	int							str;//strength
	float						spd;//speed
	bool						avoid;
	bool						pathFinded;
public:

public:
	bool						alive;

	Enemy(World& mWorld, sf::RenderWindow& W, Player& player, std::vector<Enemy>& Brothers)
		:player(player), W(W), Entity(mWorld.mlvl), spd(0.2), health(100), alive(true), Brothers(Brothers), avoid(true), LastHit(sf::seconds(0.3)),
		mWorld(mWorld), PathFindCoolDown(sf::seconds(0))
	{
		sf::Vector2f Position;
		sf::View V = W.getView();
		sf::FloatRect Vw = sf::FloatRect(V.getCenter().x - V.getSize().x / 2.0f,
			V.getCenter().y - V.getSize().y / 2.0f,
			V.getSize().x,
			V.getSize().y);

		do
		{
			Position = sf::Vector2f((rand() % 39 + 1) * 32, (rand() % 39 + 1) * 32);
			if (EnemyCollisison(Position.x, Position.y)
				&& 
				!(Vw.contains(Position)))
					body.setPosition(Position.x, Position.y);
		} while (!(EnemyCollisison(Position.x, Position.y)) || (Vw.contains(Position)));
	}

	int& GetHealth()
	{
		return health;
	}

	void operator=(Enemy& a)
	{
		W = a.W;
		Brothers = a.Brothers;
		pathMap = a.pathMap;
		toGoIfAvoid = a.toGoIfAvoid;
		body = a.body;
		LastHit = a.LastHit;
		PathFindCoolDown = a.PathFindCoolDown;
		player = a.player;
		mWorld = a.mWorld;
		health = a.health;
		attack_frames = a.attack_frames;
		str = a.str;//strength
		spd = a.spd;//speed
		avoid = a.avoid;
		pathFinded = a.pathFinded;
	}

	bool Enemy::EnemyCollisison(float &x, float &y)
	{
		for (auto layer = mlvl.GetLayers().begin(); layer != mlvl.GetLayers().end(); ++layer)
		{
			if (layer->name == "objects")
			{
				for (auto object = layer->objects.begin(); object != layer->objects.end(); ++object)
				{
					if (object->GetName() == "solid")
					{
						/*int distance_x = abs(object->GetPosition().x - x);
						int distance_y = abs(object->GetPosition().x - y);
						if (
							distance_x <= 33
							&&
							distance_x >= 0
							&&
							distance_y <= 33
							&&
							distance_y >= 0
							)
						{*/
							if (object->GetAABB().intersects(sf::FloatRect(sf::Vector2f(x, y),
								sf::Vector2f(body.getGlobalBounds().width, body.getGlobalBounds().height))))
							{
								return false;
							}
						//}
					}
				}
			}
		}
		
		for (auto i : Brothers)
		{
			int distance_x = abs(i.GetRect().left - x);
			int distance_y = abs(i.GetRect().top - y);
			if (
				distance_x <= 33
				&&
				distance_x >= 0
				&&
				distance_y <= 33
				&&
				distance_y >= 0
				)
			{
				if (i.GetRect().intersects(sf::FloatRect(x, y, body.getGlobalBounds().width, body.getGlobalBounds().width)) && i.body.getPosition() != body.getPosition() && i.GetHealth() > 0)
				{
					return false;
				}
			}
		}

		return true;
	}

	void FindPath(sf::Sprite& plr)
	{
		if (PathFindCoolDown > sf::seconds(1))
		{
			sf::View V = W.getView();
			sf::FloatRect Vw = sf::FloatRect(V.getCenter().x - V.getSize().x / 2.0f,
				V.getCenter().y - V.getSize().y / 2.0f,
				V.getSize().x,
				V.getSize().y);
			if ((body.getPosition().x < Vw.left && body.getPosition().y < Vw.top)
				||
				((body.getPosition().x > Vw.left + Vw.width)
				&&
				body.getPosition().y > Vw.top + Vw.height))
			{
				avoid = pathFinded = false;
				return;
			}

			sf::Clock clock;
			sf::Time elapsedTime = clock.restart();
			pathFinded = false;
			pathMap.erase(pathMap.begin(), pathMap.end());
			std::vector<Node> closedSet;

			std::vector<Node> openSet; openSet.push_back(Node(body.getPosition(), 0, 0, 0));
			openSet.begin()->getFValue(plr.getPosition());

			Node start(body.getPosition(), 0, 0, 0);
			while (!openSet.empty())
			{
				elapsedTime += clock.restart();
				if (elapsedTime >= sf::seconds(0.033))
				{
					avoid = pathFinded = false;
					break;
				}

				std::vector<Node>::iterator smallest_iter = (min_element(openSet.begin(), openSet.end()));
				Node smallest = *smallest_iter;

				if ((abs(smallest.node.x - player.GetPlayerPosititon().x) < 35
					&&
					abs(smallest.node.y - player.GetPlayerPosititon().y) < 35))
				{
					avoid = pathFinded = true;
					closedSet.push_back(smallest);
					closedSet.push_back(Node(sf::Vector2f(-32, -32)));
					closedSet[closedSet.size() - 1].CameFrom = smallest.node;
					break;
				}
				openSet.erase(smallest_iter);
				closedSet.push_back(smallest);

				start = Node(smallest.node, 0, 0, 0);
				for (int i = 0; i < dir; i++)
				{
					if (i == 4)
						continue;
					bool Add = true;
					start.node = sf::Vector2f(smallest.node.x + dx[i], smallest.node.y + dy[i]);
					sf::View V = W.getView();
					sf::FloatRect Vw = sf::FloatRect(V.getCenter().x - V.getSize().x / 2.0f,
						V.getCenter().y - V.getSize().y / 2.0f,
						V.getSize().x,
						V.getSize().y);
					if ((smallest.node.x < Vw.left && smallest.node.y < Vw.top)
						||
						((smallest.node.x > Vw.left + Vw.width)
						&&
						smallest.node.y > Vw.top + Vw.height))
					{
						continue;
					}

					Add = EnemyCollisison(start.node.x, start.node.y);
					/*for (int j = 0; j < walls.size(); j++)
					{

					int distance_x = abs(walls[j].left - smallest.node.x);
					int distance_y = abs(walls[j].top - smallest.node.y);
					if (
					distance_x <= 96
					&&
					distance_x >= 0
					&&
					distance_y <= 96
					&&
					distance_y >= 0
					)
					{
					if (walls[j].contains(start.node))
					{
					Add = false;
					break;
					}
					}
					}*/
					if (Add)
					{
						for (int j = 0; j < closedSet.size(); j++)
						{
							int distance_x = abs(closedSet[j].node.x - smallest.node.x);
							int distance_y = abs(closedSet[j].node.y - smallest.node.y);
							if (
								distance_x <= 96
								&&
								distance_x >= 0
								&&
								distance_y <= 96
								&&
								distance_y >= 0
								)
							{
								if (closedSet[j].node == start.node)
								{
									Add = false;
									break;
								}
							}
						}
					}

					if (Add)
					{
						for (int z = 0; z < openSet.size(); z++)
						{
							if (openSet[z].node == start.node)
							{
								Add = false;
							}
						}
					}
					if (Add)
					{
						openSet.push_back(start);
						openSet[openSet.size() - 1].getGValue(i, smallest.GValue);
						openSet[openSet.size() - 1].getFValue(plr.getPosition());
						openSet[openSet.size() - 1].CameFrom = smallest.node;
					}
				}
				
			}

			if (pathFinded)
			{
				for (int i = closedSet.size() - 1; i >= 2; i--)
				{
					if (body.getGlobalBounds().contains(closedSet[i].node))
					{
						pathMap.push_back(closedSet[i].CameFrom);
						break;
					}

					if (pathMap.size() > 0)
					{
						for (int j = closedSet.size() - 1; j >= 3; j--)
						{
							if (closedSet[j].node == pathMap[pathMap.size() - 1])
							{
								pathMap.push_back(closedSet[j].CameFrom);
								break;
							}
						}
					}
					else
					{
						pathMap.push_back(closedSet[i].CameFrom);
					}
				}
			}
			PathFindCoolDown = sf::seconds(0);
		}
		/*else
		{
			avoid = pathFinded = false;
		}*/
	}

	void avoiding(sf::Time time)
	{
		if (pathMap.size() > 0)
		{
			int index = pathMap.size() - 1;
			int rotation = (atan2(body.getPosition().y - pathMap[index].y, body.getPosition().x - pathMap[index].x)) * 180 / 3.14159265;
			body.setRotation(rotation - 90);
			float x = spd * time.asMilliseconds() * cos(((body.getRotation() - 90)* 3.14 / 180));
			float y = spd * time.asMilliseconds() * sin(((body.getRotation() - 90)* 3.14 / 180));
			x += body.getPosition().x;
			y += body.getPosition().y;
			body.setPosition(x, y);

			int distance_x = abs(pathMap[index].x - body.getPosition().x);
			int distance_y = abs(pathMap[index].y - body.getPosition().y);
			if (distance_x <= 2
				&&
				distance_y <= 2)
			{
				for (int i = 0; i < mWorld.cells.size(); i++)
				{
					if (mWorld.cells[i].getGlobalBounds().contains(pathMap[index]))
						mWorld.cells[i].setFillColor(sf::Color::Blue);
				}
				body.setPosition(pathMap[index]);
				pathMap.erase(pathMap.begin() + index);
			}
		}
		else
		{
			avoid = false;
			int rotation = (atan2(body.getPosition().y - player.GetBodySprite().getPosition().y, body.getPosition().x - player.GetBodySprite().getPosition().x)) * 180 / 3.14159265;
			body.setRotation(rotation - 90);
		}
	}

	void SetTextures(TextureHolder& pTxt, Textures::ID name)
	{
		body.setTexture(pTxt.get(name));
		body.setTextureRect(sf::IntRect(0, 0, 32, 32));
		body.setOrigin(body.getGlobalBounds().width / 2, body.getGlobalBounds().height / 2);
	}

	void ToPlayer(sf::Time time)
	{
		if (health <= 0 && alive)
		{
			alive = false;
		}

		if (alive)
		{
			PathFindCoolDown += time;
			float rotation
			= (atan2(body.getPosition().y - player.GetPlayerPosititon().y, body.getPosition().x - player.GetPlayerPosititon().x)) * 180 / 3.141592;
			body.setRotation(rotation - 90);

			if ((abs(body.getPosition().x - player.GetPlayerPosititon().x) > 25
				||
				abs(body.getPosition().y - player.GetPlayerPosititon().y) > 25))
			{
				if (!avoid)
				{
					body.setTextureRect(sf::IntRect(0, 0, 32, 32));
					float x = spd * time.asMilliseconds() * cos(((body.getRotation() - 90)* 3.14 / 180));
					float y = spd * time.asMilliseconds() * sin(((body.getRotation() - 90)* 3.14 / 180));
					x += body.getPosition().x;
					y += body.getPosition().y;

					if (EnemyCollisison(x, y))
					{
						body.setPosition(x, y);
					}
					else 
					{
						FindPath(player.GetBodySprite());
					}
				}
				else
				{
					avoiding(time);
				}

			}
			else
			{
				if (LastHit.asSeconds() > 0.3)
				{
					if (player.GetArmor() > 0)
					{
						player.GetArmor()-=5;
					}
					else if (player.GetHealth() > 0)
					{
						player.GetHealth()-=5;
					}
					body.setTextureRect(sf::IntRect(32 * (rand() % 2 + 1), 0, 32, 32));
					LastHit = sf::seconds(0);
				}

				if (LastHit.asSeconds() > 0.2)
					body.setTextureRect(sf::IntRect(0, 0, 32, 32));
			}
		}
		else
		{
			body.setTextureRect(sf::IntRect(96, 0, 32, 32));
		}


		LastHit += time;
	}

	sf::FloatRect GetRect()
	{
		return body.getGlobalBounds();
	}

	sf::Vector2f GetPosition()
	{
		return body.getPosition();
	}

	void Draw()
	{
		W.draw(body);
	}
};

