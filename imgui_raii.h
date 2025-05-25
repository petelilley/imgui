#pragma once

#include <utility>

#include "imgui.h"

namespace ImGui {

class Scoped final {
    bool m_init = true;
    void (*m_end_func)() = nullptr;

   private:
    Scoped(bool init, void (*end_func)())
        : m_init(init), m_end_func(end_func) {}

    explicit Scoped(void (*end_func)()) : m_end_func(end_func) {}

   public:
    operator bool() const { return m_init; }

    ~Scoped() {
        if (!m_init) return;

        if (m_end_func) {
            m_end_func();
        }
    }

    // Begin(), End()

    [[nodiscard]] static Scoped Window(const char* name, bool* p_open = NULL,
                                       ImGuiWindowFlags flags = 0) {
        return Scoped(Begin(name, p_open, flags), End);
    }

    // BeginChild(), EndChild()

    [[nodiscard]] static Scoped ChildWindow(const char* str_id,
                                            const ImVec2& size = ImVec2(0, 0),
                                            ImGuiChildFlags child_flags = 0,
                                            ImGuiWindowFlags window_flags = 0) {
        return Scoped(BeginChild(str_id, size, child_flags, window_flags),
                      EndChild);
    }

    [[nodiscard]] static Scoped ChildWindow(ImGuiID id,
                                            const ImVec2& size = ImVec2(0, 0),
                                            ImGuiChildFlags child_flags = 0,
                                            ImGuiWindowFlags window_flags = 0) {
        return Scoped(BeginChild(id, size, child_flags, window_flags),
                      EndChild);
    }

    // PushFont(), PopFont()

    [[nodiscard]] static Scoped Font(ImFont* font) {
        PushFont(font);
        return Scoped(PopFont);
    }

    // PushStyleColor(), PopStyleColor()

    [[nodiscard]] static Scoped StyleColor(ImGuiCol idx, ImU32 color) {
        PushStyleColor(idx, color);
        return Scoped([] { PopStyleColor(); });
    }

    [[nodiscard]] static Scoped StyleColor(ImGuiCol idx, const ImVec4& color) {
        PushStyleColor(idx, color);
        return Scoped([] { PopStyleColor(); });
    }

    // PushStyleVar(), PopStyleVar()

    [[nodiscard]] static Scoped StyleVar(ImGuiStyleVar idx, float value) {
        PushStyleVar(idx, value);
        return Scoped([] { PopStyleVar(); });
    }

    [[nodiscard]] static Scoped StyleVar(ImGuiStyleVar idx,
                                         const ImVec2& value) {
        PushStyleVar(idx, value);
        return Scoped([] { PopStyleVar(); });
    }

    [[nodiscard]] static Scoped StyleVarX(ImGuiStyleVar idx, float value) {
        PushStyleVarX(idx, value);
        return Scoped([] { PopStyleVar(); });
    }

    [[nodiscard]] static Scoped StyleVarY(ImGuiStyleVar idx, float value) {
        PushStyleVarY(idx, value);
        return Scoped([] { PopStyleVar(); });
    }

    // PushItemFlag(), PopItemFlag()

    [[nodiscard]] static Scoped ItemFlag(ImGuiItemFlags option, bool enabled) {
        PushItemFlag(option, enabled);
        return Scoped(PopItemFlag);
    }

    // PushItemWidth(), PopItemWidth()

    [[nodiscard]] static Scoped ItemWidth(float item_width) {
        PushItemWidth(item_width);
        return Scoped(PopItemWidth);
    }

    // PushTextWrapPos(), PopTextWrapPos()

    [[nodiscard]] static Scoped TextWrapPos(float wrap_local_pos_x = 0.0f) {
        PushTextWrapPos(wrap_local_pos_x);
        return Scoped(PopTextWrapPos);
    }

    // PushID(), PopID()

    [[nodiscard]] static Scoped ID(const char* str_id) {
        PushID(str_id);
        return Scoped(PopID);
    }

    [[nodiscard]] static Scoped ID(const char* str_id_begin,
                                   const char* str_id_end) {
        PushID(str_id_begin, str_id_end);
        return Scoped(PopID);
    }

    [[nodiscard]] static Scoped ID(const void* ptr_id) {
        PushID(ptr_id);
        return Scoped(PopID);
    }

    [[nodiscard]] static Scoped ID(int int_id) {
        PushID(int_id);
        return Scoped(PopID);
    }

    // BeginCombo(), EndCombo()

    [[nodiscard]] static Scoped Combo(const char* label,
                                      const char* preview_value,
                                      ImGuiComboFlags flags = 0) {
        return Scoped(BeginCombo(label, preview_value, flags), EndCombo);
    }

    // TreePush(), TreePop()

    [[nodiscard]] static Scoped TreeNode(const char* label) {
        return Scoped(ImGui::TreeNode(label), TreePop);
    }

    [[nodiscard]] static Scoped TreeNode(const char* str_id, const char* fmt,
                                         ...) IM_FMTARGS(2) {
        va_list args;
        va_start(args, fmt);
        Scoped result = Scoped::TreeNodeV(str_id, fmt, args);
        va_end(args);
        return result;
    }

    [[nodiscard]] static Scoped TreeNode(const void* ptr_id, const char* fmt,
                                         ...) IM_FMTARGS(2) {
        va_list args;
        va_start(args, fmt);
        Scoped result = Scoped::TreeNodeV(ptr_id, fmt, args);
        va_end(args);
        return result;
    }

    [[nodiscard]] static Scoped TreeNodeV(const char* str_id, const char* fmt,
                                          va_list args) IM_FMTLIST(2) {
        return Scoped(ImGui::TreeNodeV(str_id, fmt, args), TreePop);
    }

    [[nodiscard]] static Scoped TreeNodeV(const void* ptr_id, const char* fmt,
                                          va_list args) IM_FMTLIST(2) {
        return Scoped(ImGui::TreeNodeV(ptr_id, fmt, args), TreePop);
    }

    [[nodiscard]] static Scoped TreeNodeEx(const char* label,
                                           ImGuiTreeNodeFlags flags = 0) {
        return Scoped(ImGui::TreeNodeEx(label, flags), TreePop);
    }

    [[nodiscard]] static Scoped TreeNodeEx(const char* str_id,
                                           ImGuiTreeNodeFlags flags,
                                           const char* fmt, ...) IM_FMTARGS(3) {
        va_list args;
        va_start(args, fmt);
        Scoped result = Scoped::TreeNodeExV(str_id, flags, fmt, args);
        va_end(args);
        return result;
    }

    [[nodiscard]] static Scoped TreeNodeEx(const void* ptr_id,
                                           ImGuiTreeNodeFlags flags,
                                           const char* fmt, ...) IM_FMTARGS(3) {
        va_list args;
        va_start(args, fmt);
        Scoped result = Scoped::TreeNodeExV(ptr_id, flags, fmt, args);
        va_end(args);
        return result;
    }

    [[nodiscard]] static Scoped TreeNodeExV(const char* str_id,
                                            ImGuiTreeNodeFlags flags,
                                            const char* fmt, va_list args)
        IM_FMTLIST(3) {
        return Scoped(ImGui::TreeNodeExV(str_id, flags, fmt, args), TreePop);
    }

    [[nodiscard]] static Scoped TreeNodeExV(const void* ptr_id,
                                            ImGuiTreeNodeFlags flags,
                                            const char* fmt, va_list args)
        IM_FMTLIST(3) {
        return Scoped(ImGui::TreeNodeExV(ptr_id, flags, fmt, args), TreePop);
    }

    [[nodiscard]] static Scoped Tree(const char* str_id) {
        return Scoped(ImGui::TreeNode(str_id), TreePop);
    }

    // BeginMultiSelect(), EndMultiSelect()

    std::pair<Scoped, ImGuiMultiSelectIO*> MultiSelect(
        ImGuiMultiSelectFlags flags, int selection_size = -1,
        int items_count = -1) {
        ImGuiMultiSelectIO* multi_select_io =
            BeginMultiSelect(flags, selection_size, items_count);

        return std::make_pair(
            Scoped(multi_select_io != nullptr, [] { (void)EndMultiSelect(); }),
            multi_select_io);
    }

    // BeginListBox(), EndListBox()

    [[nodiscard]] static Scoped ListBox(const char* label,
                                        const ImVec2& size = ImVec2(0, 0)) {
        return Scoped(BeginListBox(label, size), EndListBox);
    }

    // BeginMenuBar(), EndMenuBar()

    [[nodiscard]] static Scoped MenuBar() {
        return Scoped(BeginMenuBar(), EndMenuBar);
    }

    // BeginMainMenuBar(), EndMainMenuBar()

    [[nodiscard]] static Scoped MainMenuBar() {
        return Scoped(BeginMainMenuBar(), EndMainMenuBar);
    }

    // BeginMenu(), EndMenu()

    [[nodiscard]] static Scoped Menu(const char* label, bool enabled = true) {
        return Scoped(BeginMenu(label, enabled), EndMenu);
    }

    // BeginTooltip(), EndTooltip()

    [[nodiscard]] static Scoped Tooltip(const char* text = NULL) {
        return Scoped(BeginTooltip(), EndTooltip);
    }

    // BeginItemTooltip()

    [[nodiscard]] static Scoped ItemTooltip(const char* text = NULL) {
        return Scoped(BeginItemTooltip(), EndTooltip);
    }

    // BeginPopup(), EndPopup()

    [[nodiscard]] static Scoped Popup(const char* str_id,
                                      ImGuiWindowFlags flags = 0) {
        return Scoped(BeginPopup(str_id, flags), EndPopup);
    }

    [[nodiscard]] static Scoped PopupModal(const char* name,
                                           bool* p_open = NULL,
                                           ImGuiWindowFlags flags = 0) {
        return Scoped(BeginPopupModal(name, p_open, flags), EndPopup);
    }

    [[nodiscard]] static Scoped PopupContextItem(
        const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1) {
        return Scoped(BeginPopupContextItem(str_id, popup_flags), EndPopup);
    }

    [[nodiscard]] static Scoped PopupContextWindow(
        const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1) {
        return Scoped(BeginPopupContextWindow(str_id, popup_flags), EndPopup);
    }

    [[nodiscard]] static Scoped PopupContextVoid(
        const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1) {
        return Scoped(BeginPopupContextVoid(str_id, popup_flags), EndPopup);
    }

    // BeginTable(), EndTable()

    [[nodiscard]] static Scoped Table(const char* str_id, int columns,
                                      ImGuiTableFlags flags = 0,
                                      const ImVec2& outer_size = ImVec2(0.0f,
                                                                        0.0f),
                                      float inner_width = 0.0f) {
        return Scoped(
            BeginTable(str_id, columns, flags, outer_size, inner_width),
            EndTable);
    }

    // BeginTabBar(), EndTabBar()

    [[nodiscard]] static Scoped TabBar(const char* str_id,
                                       ImGuiTabBarFlags flags = 0) {
        return Scoped(BeginTabBar(str_id, flags), EndTabBar);
    }

    // BeginTabItem(), EndTabItem()

    [[nodiscard]] static Scoped TabItem(const char* label, bool* p_open = NULL,
                                        ImGuiTabItemFlags flags = 0) {
        return Scoped(BeginTabItem(label, p_open, flags), EndTabItem);
    }

    // BeginDragDropSource(), EndDragDropSource()

    [[nodiscard]] static Scoped DragDropSource(ImGuiDragDropFlags flags = 0) {
        return Scoped(BeginDragDropSource(flags), EndDragDropSource);
    }

    // BeginDragDropTarget(), EndDragDropTarget()

    [[nodiscard]] static Scoped DragDropTarget() {
        return Scoped(BeginDragDropTarget(), EndDragDropTarget);
    }

    // BeginDisabled(), EndDisabled()

    [[nodiscard]] static Scoped Disabled(bool disabled = true) {
        BeginDisabled(disabled);
        return Scoped(EndDisabled);
    }

    // PushClipRect(), PopClipRect()

    [[nodiscard]] static Scoped ClipRect(
        const ImVec2& clip_rect_min, const ImVec2& clip_rect_max,
        bool intersect_with_current_clip_rect) {
        PushClipRect(clip_rect_min, clip_rect_max,
                     intersect_with_current_clip_rect);
        return Scoped(PopClipRect);
    }
};

}  // namespace ImGui
