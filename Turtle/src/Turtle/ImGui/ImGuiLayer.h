#pragma once

#include "Turtle/Core/Layer.h"

#include "Turtle/Events/ApplicationEvent.h"
#include "Turtle/Events/KeyEvent.h"
#include "Turtle/Events/MouseEvent.h"


namespace Turtle {

    class ImGuiLayer : public Layer
    {
    public:
		ImGuiLayer();
		~ImGuiLayer() {};

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& e) override;

        void Begin();
        void End();

        void SetBlockEvents(bool block) { m_BlockEvents = block; }
    private:
        float m_Time = 0.0f;
        bool m_BlockEvents = true;

    };
}