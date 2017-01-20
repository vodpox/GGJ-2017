#ifndef PLAYER
#define PLAYER

class Player {
	private:
		int x = 5;
		int y = 5;
		int health = 100;
		int maxHealth = 100;
		int jammerCount = 0;
		int maxJammerCount = 3;
	
	public:
		int getX();
		int getY();
		int getHealth();
		void changeHealth(int change);
		void changeJammerCount(int change);
		void setXY(int x, int y);
		void move(int x, int y);
};

#endif