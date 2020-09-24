#pragma once

#include "Turtle/Core/Core.h"

namespace Turtle {

	struct FramebufferSpecification
	{
		uint32_t Width, Height; 
		// FramebufferFormat Format = 
		uint32_t Samples = 1;
		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		/*NOTE: binding should eventually be managed by the renderer */
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}