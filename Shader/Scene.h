#pragma once

class Scene {
public:
	virtual ~Scene() {}
	virtual void run() = 0;
};
