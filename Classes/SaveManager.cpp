#include "cocos2d.h"
#include "SaveManager.h"
#include "LevelManager.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/filestream.h"

using namespace cocos2d;

bool SaveManager::isNewUser;
std::vector<SaveManager::LevelData *> SaveManager::m_levels;

void SaveManager::unlockNextLevel()
{
	int nextLevel = LevelManager::getInstance().m_lastLevel + 1;
	if (nextLevel > LevelManager::getInstance().totalLevels || SaveManager::levelExists(nextLevel)) 
	{
		return;
	}
	else {
		LevelData *newLevel = new LevelData();
		newLevel->id = nextLevel;
		newLevel->score = 0;
		newLevel->stars = 0;
		m_levels.push_back(newLevel);

		//save the new level!
		SaveManager::saveFile();
	}
}

void SaveManager::updateScore(int id, int score)
{
	if(SaveManager::levelExists(id))
	{
		LevelData* data = SaveManager::getLevel(id);
		if (score > data->score) //only save higher score
		{
			data->score = score;

			log("ONE STAR: %d TWO STAR: %d, THREE STAR %d", LevelManager::getInstance().m_oneStarScore, LevelManager::getInstance().m_twoStarScore, LevelManager::getInstance().m_threeStarScore);

			//save the number of stars they get!
			int numStars = 0;
			if (data->score >= LevelManager::getInstance().m_oneStarScore)
					numStars = 1;
			if (data->score >= LevelManager::getInstance().m_twoStarScore)
					numStars = 2;
			if (data->score >= LevelManager::getInstance().m_threeStarScore)
					numStars = 3;

			log("NUM STARS: %d", numStars);
		    data->stars = numStars;

			//update the save file
			SaveManager::saveFile();
		}
	}
}

SaveManager::LevelData* SaveManager::getLevel(int id)
{
	for(LevelData* level : m_levels)
	{
		if (level->id == id)
			return level;
	}

	return nullptr;
}

void SaveManager::init()
{ 
	auto fileUtils = FileUtils::getInstance();
	std::string path = fileUtils->getWritablePath() + "save.json";
	std::string clearData = fileUtils->getStringFromFile(path);

	if (clearData.length() <= 0)
	{
		SaveManager::isNewUser = true;
		//no save data, give the user the first level and save it!
		log("NEW USER, CANT FIND SAVE FILE, SAVING TO PATH: %s", path.c_str());
		LevelData* levelObj = new LevelData();
		levelObj->id = 1;
		levelObj->score = 0;
		levelObj->stars = 0;
		m_levels.push_back(levelObj);
		SaveManager::saveFile();
	}
	else
	{
		SaveManager::isNewUser = false;
		log("Loading Saved User!");
		//not a new user, read the file!
		SaveManager::readFile();
	}
}

void SaveManager::saveFile()
{
	std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + "save.json";
	log("Saving to Path: %s", path.c_str());
	
	FILE* pFile = fopen(path.c_str(), "wb");

	//build the json string to save
	std::string output = "{"; //start of json

	output.append("\"levels\" :[");
	for(LevelData* level : m_levels)
	{
		output.append("{");
		output.append(" \"id\":" + std::to_string(level->id)); 
		output.append(",");
		output.append(" \"score\":" + std::to_string(level->score));
		output.append(",");
		output.append(" \"stars\":" + std::to_string(level->stars));
		output.append(" },");
	}
	output.pop_back();//remove the last comma...
	output.append("]");

	output.append("}"); //end of json

	fwrite(output.c_str(), sizeof(char) * output.size(), 1, pFile);
	fclose (pFile);
}

bool SaveManager::levelExists(int id)
{
	for(LevelData* level : m_levels)
	{
		if (level->id == id)
			return true;
	}

	return false;
}

void SaveManager::readFile()
{
  std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + "save.json";
  auto fileUtils = FileUtils::getInstance();

  std::string clearData = fileUtils->getStringFromFile(path);
  if (clearData.length() <= 0)
  {
	  return; 
  }

  size_t pos = clearData.rfind("}");
  clearData = clearData.substr(0, pos+1);

  rapidjson::Document document;
  document.Parse<0>(clearData.c_str());
  
  rapidjson::Value& levels = document["levels"]; // Using a reference for consecutive access is handy and faster.

  //clear out current levels
  m_levels.clear();

  assert(levels.IsArray());
  for (rapidjson::SizeType i = 0; i < levels.Size(); i++)
  {
	  rapidjson::Value& level = levels[i];
	  assert(level["id"].IsInt());
	  assert(level["score"].IsInt());
	  assert(level["stars"].IsInt());

	  LevelData* levelObj = new LevelData();
	  levelObj->id = level["id"].GetInt();
	  levelObj->score = level["score"].GetInt();
	  levelObj->stars = level["stars"].GetInt();

	  m_levels.push_back(levelObj);
  }

}
