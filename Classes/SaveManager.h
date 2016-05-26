#ifndef __SAVE_MANAGER__
#define __SAVE_MANAGER__

class SaveManager 
{
public:

	struct LevelData {
		int id;
		int score;
		int stars;
	};

	static bool isNewUser;

	static std::vector<LevelData *> m_levels;

	static void unlockNextLevel();
	static bool levelExists(int id);
	static void init();
	static void saveFile();
	static void readFile();
	static void updateScore(int id, int score);
	static LevelData* getLevel(int id);
};

#endif
