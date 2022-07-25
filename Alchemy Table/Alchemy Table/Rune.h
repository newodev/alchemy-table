#pragma once
struct ManaFlow
{
	float Amount;
	bool Impulse;
};

enum class Substances
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

	float Product()
	{
		return X * Y * Z;
	}
};


enum class Shapes
{
	Sphere,
	Ring,
	Spear
};

struct Effect
{
	Substances Type;
	Shapes Shape;
	Vector3 size;
	Vector3 Velocity;

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
	void Init();

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

class SphereShaper : public Rune
{
public:
	Substances type;
	float Compute(Effect* e) override;
};

class Concentrator : public Rune
{
public:
	Vector3 Magnitudes;
	Substances type;
	float Compute(Effect* e) override;
};