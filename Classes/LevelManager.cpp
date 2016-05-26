#include "LevelManager.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/filestream.h"
#include "cocos2d.h"

USING_NS_CC;

LevelManager::LevelManager()
{
	totalLevels = 30; //for now 20 max levels!
}

void LevelManager::loadLevel(int level)
{
  m_lastLevel = level;

  auto fileUtils = FileUtils::getInstance();
  std::vector<LevelManager::BrickData> brickDataList;
	
  std::string fileName = "level_" + std::to_string(level) + ".json";
  std::string fullPath =  fileUtils->fullPathForFilename(fileName);
  //log(fullPath.c_str());

  std::string clearData = fileUtils->getStringFromFile(fullPath);
  if (clearData.length() <= 0)
  {
	  m_maxBalls = 0;
	  m_bricks = brickDataList;
	  return; //didnt find level, just return empty list and 0 max balls
  }

  size_t pos = clearData.rfind("}");
  clearData = clearData.substr(0, pos+1);

  rapidjson::Document document;
  document.Parse<0>(clearData.c_str());
  
  rapidjson::Value& balls = document["balls"];
  assert(balls.IsInt());
  this->m_maxBalls = balls.GetInt();

  rapidjson::Value& oneStar = document["onestar"];
  assert(oneStar.IsInt());
  this->m_oneStarScore = oneStar.GetInt();

  rapidjson::Value& twoStar = document["twostar"];
  assert(twoStar.IsInt());
  this->m_twoStarScore = twoStar.GetInt();

  rapidjson::Value& threeStar = document["threestar"];
  assert(threeStar.IsInt());
  this->m_threeStarScore = threeStar.GetInt();

  //log("Loading Level with Max Balls: %d", this->m_maxBalls);
  
  rapidjson::Value& bricks = document["bricks"]; // Using a reference for consecutive access is handy and faster.

  BrickData brickObj;

  assert(bricks.IsArray());
  for (rapidjson::SizeType i = 0; i < bricks.Size(); i++)
  {
	  rapidjson::Value& brick = bricks[i];
	  assert(brick["x"].IsDouble());
	  assert(brick["y"].IsDouble());
	  assert(brick["type"].IsInt());
	  
	  brickObj.x = brick["x"].GetDouble();
	  brickObj.y = brick["y"].GetDouble();
	  brickObj.type = brick["type"].GetInt();

	  brickDataList.push_back(brickObj);
  }

  m_bricks = brickDataList;
}
