#ifndef __LEVEL_MANAGER_H__
#define __LEVEL_MANAGER_H__

#include <vector>
		
class LevelManager
{
    public:
        static LevelManager& getInstance()
        {
            static LevelManager instance; // Guaranteed to be destroyed.
            // Instantiated on first use.
            return instance;
        }

		struct BrickData{
			float x;
			float y;
			int type;
		};

		
		std::vector<LevelManager::BrickData> m_bricks;
		int m_maxBalls;
		int m_lastLevel;
		int totalLevels;
		int m_oneStarScore;
		int m_twoStarScore;
		int m_threeStarScore;

		void loadLevel(int level);
		
    private:
		LevelManager();              
        LevelManager(LevelManager const&);    // Don't Implement
        void operator=(LevelManager const&); // Don't implement
};


#endif // __LEVEL_MANAGER_H__
