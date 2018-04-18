#include "DataManager.h"
#include "Game.h"
#include "Components.h"
#include "ComponentsConstructors.h"
#include "rapidxml.hpp"
#include <fstream>
#include <vector>
#include "ObjectsFactory.h"



DataManager::DataManager()
{
	gameState = {};
}

void DataManager::saveGame(std::string path)
{
	//TODO: path<<gameState
}

void DataManager::loadGame(std::string path)
{
	//TODO: gameState<<path
}

SceneData & DataManager::loadScene(std::string path)
{
	//Init the sceneData (it's just a custom vector)
	SceneData sceneData = SceneData();

	//Create the xml structure
	using namespace rapidxml;
	xml_document<> doc;
	xml_node<>* root_node;

	//Load the xml file
	std::ifstream file(path);
	std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);

	//Find Root node
	root_node = doc.first_node("Scene");

	//Iterate for all entities of the scene
	for (xml_node<>* entity_node = root_node->first_node("Entity"); entity_node; entity_node = entity_node->next_sibling()) 
	{
		//Create an Entity Construction Data
		EntityConstructionData eData;
		eData.entityName = entity_node->first_attribute("name")->value();
		eData.prefab = getPrefabFromString(entity_node->first_attribute("prefab")->value());

		//Iterate for all components of the entity
		for (xml_node<>* component_node = entity_node->first_node(); component_node; component_node = component_node->next_sibling()) 
		{
			//Get component type
			ComponentType componentType = getComponentTypeFromString(component_node->first_attribute("type")->value());
			//Get the constructor for that component
			ComponentConstructors::ComponentConstructor* cc = getComponentConstructor(componentType, component_node->first_node());
			//Add them to entity data
			eData.data.emplace(componentType, cc);
		}
		//add the complete entity data into the scene data
		sceneData.push_back(eData);
	}
	//return the scene data completed
	return sceneData;
}
//Get the prefab enum from a identificantion string
ObjectsFactory::Prefab getPrefabFromString(std::string s)
{
	int prefab = 0;
	while (s != ObjectsFactory::stringIdOfPrefab[prefab] && prefab < ObjectsFactory::Prefab::size) { prefab++; }
	return (ObjectsFactory::Prefab)prefab;
};

//Get the component type enum from a identification string
ComponentType getComponentTypeFromString(std::string s) 
{
	int componentType = 0;
	while (s != getComponentStringId[componentType] && componentType < ComponentType::size) { componentType++; }
	return (ComponentType)componentType;
}

//Create a ComponentConstructor of type "type" with the info of "node"(if needed)
ComponentConstructors::ComponentConstructor* getComponentConstructor(ComponentType type, rapidxml::xml_node<>* node) 
{
	ComponentConstructors::ComponentConstructor* componentConstructor = nullptr;
	switch (type)
	{
	case ComponentType::RIGIDBODY:
		//componentConstructor = new ComponentConstructors::RigidBody(node);
		break;
	case ComponentType::TRANSFORM:
		//componentConstructor = new ComponentConstructors::Transform(node);
		break;
	case ComponentType::COLLIDER:
		//componentConstructor = new ComponentConstructors::Collider(node);
		break;
	case ComponentType::MESHRENDERER:
		componentConstructor = new ComponentConstructors::MeshRenderer(node);
		break;
	case ComponentType::PLAYERCONTROLLER:
		componentConstructor = new ComponentConstructors::PlayerController();
		break;
	case ComponentType::CAMERACONTROLLER:
		componentConstructor = new ComponentConstructors::CameraController();
		break;
	case ComponentType::CAMERA:
		componentConstructor = new ComponentConstructors::Camera();
		break;
	case ComponentType::PLANERENDERER:
		componentConstructor = new ComponentConstructors::PlaneRenderer(node);
		break;
	case ComponentType::WALKER:
		componentConstructor = new ComponentConstructors::Walker();
		break;
	default:
		break;
	}
	return componentConstructor;
}