#ifndef SYSTEM_ALL_H
#define SYSTEM_ALL_H

#include<vector>
#include<stdexcept>
#include"System_base.h"
#include"Component_all.h"

struct S_Movement : System
{
	S_Movement(Entity_manager* em) : System(em) { }

	void process(long previousFrameTime=0);
};

struct S_Damage : System
{
	S_Damage(Entity_manager* em) : System(em) { }

	void process(long previousFrameTime=0);
};

struct S_Trap : System
{
	S_Trap(Entity_manager* em) : System(em) { }

	void process(long previousFrameTime=0);
};

struct S_IA : System
{
	S_IA(Entity_manager* em) : System(em) { }

	void process(long previousFrameTime=0);
};

struct S_Player : System
{
	S_Player(Entity_manager* em) : System(em) { }

	void process(long previousFrameTime=0);

	void get_input(Controllable*);
	void display_pre_feedback(const std::vector<Position*>&);
	void display_post_feedback(const std::vector<Position*>&);
};

struct S_Generate : System
{
	S_Generate(Entity_manager* em) : System(em) { }

	void process(long previousFrameTime=0);
};

struct S_Display : System
{
	S_Display(Entity_manager* em) : System(em) { }

	void process(long previousFrameTime=0);
};

struct S_Feedback : System
{
	S_Feedback(Entity_manager* em) : System(em) { }

	void process(long previousFrameTime=0);
};

struct S_EndOfGame : System
{
	S_EndOfGame(Entity_manager* em) : System(em) { }

	void process(long previousFrameTime=0)
	{
		throw std::runtime_error("S_EndOfGame(): process() called instead of special_process()");
	}
	bool special_process(long previousFrameTime=0);
};

#endif
