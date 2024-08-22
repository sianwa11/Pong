#ifndef VEC2_H
#define VEC2_H

class Vec2
{
public:
	float x = 0;
	float y = 0;

	Vec2(float xin, float yin);
	Vec2();

	bool operator == (const Vec2& rhs) const;
	bool operator != (const Vec2& rhs) const;

	Vec2 operator + (const Vec2& rhs) const;
	Vec2 operator - (const Vec2& rhs) const;
	Vec2 operator * (float val) const;
	Vec2 operator / (float val) const;

	void operator += (const Vec2& rhs);
	void operator -= (const Vec2& rhs);
	void operator *= (float val);
	void operator /= (float val);

	// dist
	// normalize
};

#endif
