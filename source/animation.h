#ifndef ANIMATION_H
#define ANIMATION_H

class sprite;
class phys_obj;
class projectile;


class sprite_animations {
public:

	sprite_animations();
	~sprite_animations();

	void Init(phys_obj* sprID);
	//void Init(projectile* sprID);

	void LoadAnimations(int count);
	void Animate();
	void Stop();

	void Start(int ID);
	void PlayOnce(int ID, int nextID);


	void Reset();

	phys_obj*			ptrSprite;

	bool active;
	bool initialized;

	long ID;

	int curAnimID;
	int nextAnimID;

	int curAnimFrame;
	long curAnimStartTime;

	//int angles;
	//int numAnimations;

	
};



#endif