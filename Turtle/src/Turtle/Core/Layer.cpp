#include "turtpch.h"
#include "Layer.h"

namespace Turtle{
	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName)
	{
	}

	Layer::~Layer()
	{
	}
}