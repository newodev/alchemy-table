#include "Rune.h"

Rune::Rune()
{
	Init();
}

Rune::Rune(Rune* i, Rune* n)
{
	Init();

	in = i;
	next = n;
}

Rune::Rune(Rune* i, Rune* i2, Rune* n)
{
	Init();

	in = i;
	in2 = i2;
	next = n;
}

void Rune::SetNext(Rune* n)
{
	next = n;
}

ManaFlow Rune::GetOutput()
{
	return flow;
}

void Rune::Init()
{
	flow = { 0.0f, true };

	in = in2 = next = nullptr;
}

float ImpulseFocus::Compute(Effect* e)
{
	flow = { 1.0f, false };

	return 0.0f;
}

float Combustor::Compute(Effect* e)
{
	flow = in->GetOutput();

	float strength = flow.Amount * 0.1f;
	flow.Amount -= strength;

	if (e->Type == Substances::Air)
	{
		e->Type = Substances::Fire;
		e->size = Vector3(0.0f, 0.0f, 0.0f);

		// TODO: should still have low intensity at -deg
		float intensity = strength * e->Temperature / 25.0f;

		e->Intensity = strength * 10.0f;
	}

	return strength;
}

float SphereShaper::Compute(Effect* e)
{
	flow = in->GetOutput();
	
	float cost = e->Intensity * 0.2;

	if (!flow.Impulse)
		return -1.0f;

	if (flow.Amount < cost)
		return -1.0f;

	if (e->Type != type)
		return -1.0f;

	e->Shape = Shapes::Sphere;

	flow.Amount -= cost;

	return cost;
}

float Concentrator::Compute(Effect* e)
{
	flow = in->GetOutput();
	
	float volume = e->size.Product();

	float newVolume = (
		e->size.X * Magnitudes.X *
		e->size.Y * Magnitudes.Y *
		e->size.Z * Magnitudes.Z
	);

	if (newVolume < 0.0f)
		newVolume = -newVolume;

	float ratio = volume / newVolume;
	float cost = 0.2 * ratio;

	
	flow.Amount -= cost;

	return cost;
}
