#pragma once
struct ManaFlow
{
	float Amount;
	bool Impulse;
};

enum class Substance
{
	Water,
	Ice,
	Air,
	Fire
};

struct Vector3
{
	float X;
	float Y;
	float Z;

	Vector3(float xi, float yi, float zi)
	{
		X = xi;
		Y = yi;
		Z = zi;
	}
};


enum class Shape
{
	Sphere,
	Ring,
	Spear
};

struct Effect
{
	Substance Type;

	Vector3 size;

	float Intensity;
	float Temperature;
};

class Rune
{
public:
	Rune();
	Rune(Rune* i, Rune* n);
	Rune(Rune* i, Rune* i2, Rune* n);

	virtual float Compute(Effect* e) = 0;

	void SetNext(Rune* n);
	ManaFlow GetOutput();

protected:
	Rune* in;
	Rune* in2;
	Rune* next;

	ManaFlow flow;
};

class ImpulseFocus : public Rune
{
public:
	float Compute(Effect* e) override;
};

class Combustor : public Rune
{
public:
	float Compute(Effect* e) override;
};