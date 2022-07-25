#include "Rune.h"

#include <cmath>

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

	// Some shapes can be inverted by strong single-axis concentration, eg sphere -> torus
	bool invert = newVolume < 0.0f;

	if (newVolume < 0.0f)
		newVolume = -newVolume;

	float ratio = volume / newVolume;
	float cost = 0.2 * ratio;

	if (invert && e->Shape != Shapes::Sphere)
		return -1.0f;

	if (invert && e->Shape == Shapes::Sphere)
		e->Shape == Shapes::Ring;

	e->size.X = abs(e->size.X * Magnitudes.X);
	e->size.Y = abs(e->size.Y * Magnitudes.Y);
	e->size.Z = abs(e->size.Z * Magnitudes.Z);
	
	flow.Amount -= cost;

	return cost;
}

float Capacitor::Compute(Effect* e)
{
	ManaFlow input = in->GetOutput();

	float quant = input.Amount;

	// If no in-flow, capcitor fires
	if (quant == 0.0f && CurrentCharge > 0.0f && !Triggered)
	{
		flow = { CurrentCharge, true };
		ChargedUnits = 0;
		CurrentCharge = 0.0f;
		return 0.0f;
	}

	// Add input to capacitor charge
	while (ChargedUnits < Units && quant > 0.0f)
	{
		CurrentCharge += quant;

		if (CurrentCharge >= UnitCapacity)
		{
			quant -= UnitCapacity;
			CurrentCharge = 0;
			ChargedUnits++;
		}
	}

	// Non-triggered capacitors discharge if full
	// If not triggered, can only have a single unit
	if (ChargedUnits == 1 && !Triggered)
	{
		flow = { UnitCapacity, true };
		ChargedUnits = 0;
		CurrentCharge = 0.0f;
		return 0.0f;
	}

	// Triggered Capacitors discharge when the second input sends mana
	// Trigger must be an impulse
	if (Triggered && ChargedUnits > 0)
	{
		if (in2->GetOutput().Amount <= 0.0f || !in->GetOutput().Impulse)
		{
			flow = { 0.0f, true };
			return 0.0f;
		}

		flow = { UnitCapacity, true };
		ChargedUnits--;
		return 0.0f;
	}
	
	flow = { 0.0f, true };
	return 0.0f;
}
