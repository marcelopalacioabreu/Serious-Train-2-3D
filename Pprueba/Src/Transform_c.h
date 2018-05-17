#ifndef _H_TRANSFORM_C_H_
#define _H_TRANSFORM_C_H_

#include "PhysicsManager.h"
#include "Component.h"

class Transform_c :
	public Component
{
public:

	Transform_c(const btVector3& position, const btQuaternion& rotation);
	virtual ~Transform_c();

	inline btTransform& get() { return t; };
	inline const btTransform& get() const { return t; };

	void setPosition(float x, float y, float z);
	void set(const btTransform& trans) { t = trans; };
	
	virtual void start();
	virtual void update();
private:
	btTransform t;
	btTransform old;

};
#endif //!_H_TRANSFORM_C_H_





