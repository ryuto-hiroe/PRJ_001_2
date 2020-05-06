#include "main.h"
#include "System/GameManager.h"
#include "BaseComponent.h"


void BaseComponent::ImGuiUpdate()
{
	ImGui::Checkbox("Is Enable", &m_enable);
}

