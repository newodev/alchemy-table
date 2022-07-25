#include "Rune.h"

Rune::Rune()
{
}

Rune::Rune(Rune* i, Rune* n)
{
	in = i;
	next = n;
}

Rune::Rune(Rune* i, Rune* i2, Rune* n)
{
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

	if (e->Type == Substance::Air)
	{
		e->Type = Substance::Fire;
		e->size = Vector3(0.0f, 0.0f, 0.0f);

		float intensity = strength * e->Temperature / 25.0f;

		e->Intensity = strength * 10.0f;
	}

	return 0.1f;
}
