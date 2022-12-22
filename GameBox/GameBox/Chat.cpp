#include "Chat.h"
#include "imgui/imgui.h"
#include <SFML/Graphics.hpp>
#include "Game.h"

static char* Strdup(const char* s) {
    IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len);
}

Chat::Chat(){
}

Chat::~Chat() {
    if (m_eventManager) {
        m_eventManager->unsubscribe<ChatMessageReceivedEvent>(*this);
    }

	for (auto& i : m_items) {
		delete[] i;
	}
}

bool Chat::Initialize(entityx::EventManager* pEventManager) {
    m_eventManager = pEventManager;
    m_eventManager->subscribe<ChatMessageReceivedEvent>(*this);

    return true;
}

void Chat::Draw(int xPos, int yPos, int width, int height) {
    ImGuiWindowFlags chatWindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

    ImGui::SetNextWindowSize(ImVec2(width, height));
    ImGui::SetNextWindowPos(ImVec2(xPos, yPos));

    if (!ImGui::Begin("Chat", (bool*)1, chatWindowFlags)) {
        ImGui::End();
        return;
    }

    // Options menu
    if (ImGui::BeginPopup("Options")) {
        ImGui::Checkbox("Auto-scroll", &m_autoScroll);
        ImGui::EndPopup();
    }

    // Options, Filter
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");
    ImGui::Separator();

    // Reserve enough left-over height for 1 separator + 1 input text
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false)) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

        for (int i = 0; i < m_items.size(); i++) {
            const char* item = m_items[i];

            ImVec4 color;
            bool has_color = false;
            if (strstr(item, "[error]")) {
                color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true;
            } else if (strncmp(item, "# ", 2) == 0) {
                color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true;
            }
            if (has_color)
                ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextWrapped(item);
            if (has_color)
                ImGui::PopStyleColor();
        }

        // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
        // Using a scrollbar or mouse-wheel will take away from the bottom edge.
        if (m_scrollToBottom || (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
            ImGui::SetScrollHereY(1.0f);
        m_scrollToBottom = false;

        ImGui::PopStyleVar();
    }
    ImGui::EndChild();
    ImGui::Separator();

    // Command-line
    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll;
    if (ImGui::InputText("Input", m_inputBuf, IM_ARRAYSIZE(m_inputBuf), input_text_flags)) {
        char* s = m_inputBuf;
        if (s[0]) {
            //AddMessage(s);
            ChatMessageSentEvent chatSent;
            chatSent.message = s;
            m_eventManager->emit(chatSent);
        }
        strcpy_s(s, IM_ARRAYSIZE(m_inputBuf), "");
        reclaim_focus = true;
    }

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

    ImGui::End();
}

void Chat::AddMessage(const char* msg){
    m_items.push_back(Strdup(msg));
    if (m_autoScroll) {
        m_scrollToBottom = true;
    }
}

void Chat::receive(const ChatMessageReceivedEvent& event) {
    AddMessage(event.message);
}
