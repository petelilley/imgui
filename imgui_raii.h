#pragma once

#include <functional>
#include <utility>

#include "imgui.h"

namespace ImGui {

class Scoped final {
    bool m_result;
    bool m_should_end;
    std::function<void()> m_end_func;

   private:
    Scoped(bool result, bool should_end, std::function<void()> end_func)
        : m_result(result), m_should_end(should_end), m_end_func(end_func) {}

    explicit Scoped(std::function<void()> end_func)
        : m_result(true), m_should_end(true), m_end_func(std::move(end_func)) {}

   public:
    operator bool() const { return m_result; }

    ~Scoped() {
        if (!m_should_end) return;

        if (m_end_func) {
            m_end_func();
        }
    }

    // Begin(), End()

    [[nodiscard]] static Scoped Window(const char* name, bool* p_open = NULL,
                                       ImGuiWindowFlags flags = 0) {
        bool result = Begin(name, p_open, flags);
        return Scoped(result, true, End);
    }

    // BeginChild(), EndChild()

    [[nodiscard]] static Scoped ChildWindow(const char* str_id,
                                            const ImVec2& size = ImVec2(0, 0),
                                            ImGuiChildFlags child_flags = 0,
                                            ImGuiWindowFlags window_flags = 0) {
        bool result = BeginChild(str_id, size, child_flags, window_flags);
        return Scoped(result, true, EndChild);
    }

    [[nodiscard]] static Scoped ChildWindow(ImGuiID id,
                                            const ImVec2& size = ImVec2(0, 0),
                                            ImGuiChildFlags child_flags = 0,
                                            ImGuiWindowFlags window_flags = 0) {
        bool result = BeginChild(id, size, child_flags, window_flags);
        return Scoped(result, true, EndChild);
    }

    // PushFont(), PopFont()

    [[nodiscard]] static Scoped Font(ImFont* font, float font_size_base_unscaled) {
        PushFont(font, font_size_base_unscaled);
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
        bool result = BeginCombo(label, preview_value, flags);
        return Scoped(result, result, EndCombo);
    }

    // TreePush(), TreePop()

    [[nodiscard]] static Scoped TreeNode(const char* label) {
        bool result = ImGui::TreeNode(label);
        return Scoped(result, result, TreePop);
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
        bool result = ImGui::TreeNodeV(str_id, fmt, args);
        return Scoped(result, result, TreePop);
    }

    [[nodiscard]] static Scoped TreeNodeV(const void* ptr_id, const char* fmt,
                                          va_list args) IM_FMTLIST(2) {
        bool result = ImGui::TreeNodeV(ptr_id, fmt, args);
        return Scoped(result, result, TreePop);
    }

    [[nodiscard]] static Scoped TreeNodeEx(const char* label,
                                           ImGuiTreeNodeFlags flags = 0) {
        bool result = ImGui::TreeNodeEx(label, flags);
        return Scoped(result, result, TreePop);
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
        bool result = ImGui::TreeNodeExV(str_id, flags, fmt, args);
        return Scoped(result, result, TreePop);
    }

    [[nodiscard]] static Scoped TreeNodeExV(const void* ptr_id,
                                            ImGuiTreeNodeFlags flags,
                                            const char* fmt, va_list args)
        IM_FMTLIST(3) {
        bool result = ImGui::TreeNodeExV(ptr_id, flags, fmt, args);
        return Scoped(result, result, TreePop);
    }

    [[nodiscard]] static Scoped Tree(const char* str_id) {
        bool result = ImGui::TreeNode(str_id);
        return Scoped(result, result, TreePop);
    }

    // BeginMultiSelect(), EndMultiSelect()

    std::pair<Scoped, ImGuiMultiSelectIO*> MultiSelect(
        ImGuiMultiSelectFlags flags, int selection_size = -1,
        int items_count = -1) {
        ImGuiMultiSelectIO* multi_select_io =
            BeginMultiSelect(flags, selection_size, items_count);

        bool result = multi_select_io != nullptr;

        return std::make_pair(
            Scoped(result, result, [] { (void)EndMultiSelect(); }),
            multi_select_io);
    }

    // BeginListBox(), EndListBox()

    [[nodiscard]] static Scoped ListBox(const char* label,
                                        const ImVec2& size = ImVec2(0, 0)) {
        bool result = BeginListBox(label, size);
        return Scoped(result, result, EndListBox);
    }

    // BeginMenuBar(), EndMenuBar()

    [[nodiscard]] static Scoped MenuBar() {
        bool result = BeginMenuBar();
        return Scoped(result, result, EndMenuBar);
    }

    // BeginMainMenuBar(), EndMainMenuBar()

    [[nodiscard]] static Scoped MainMenuBar() {
        bool result = BeginMainMenuBar();
        return Scoped(result, result, EndMainMenuBar);
    }

    // BeginMenu(), EndMenu()

    [[nodiscard]] static Scoped Menu(const char* label, bool enabled = true) {
        bool result = BeginMenu(label, enabled);
        return Scoped(result, result, EndMenu);
    }

    // BeginTooltip(), EndTooltip()

    [[nodiscard]] static Scoped Tooltip() {
        bool result = BeginTooltip();
        return Scoped(result, result, EndTooltip);
    }

    // BeginItemTooltip()

    [[nodiscard]] static Scoped ItemTooltip() {
        bool result = BeginItemTooltip();
        return Scoped(result, result, EndTooltip);
    }

    // BeginPopup(), EndPopup()

    [[nodiscard]] static Scoped Popup(const char* str_id,
                                      ImGuiWindowFlags flags = 0) {
        bool result = BeginPopup(str_id, flags);
        return Scoped(result, result, EndPopup);
    }

    [[nodiscard]] static Scoped PopupModal(const char* name,
                                           bool* p_open = NULL,
                                           ImGuiWindowFlags flags = 0) {
        bool result = BeginPopupModal(name, p_open, flags);
        return Scoped(result, result, EndPopup);
    }

    [[nodiscard]] static Scoped PopupContextItem(
        const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1) {
        bool result = BeginPopupContextItem(str_id, popup_flags);
        return Scoped(result, result, EndPopup);
    }

    [[nodiscard]] static Scoped PopupContextWindow(
        const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1) {
        bool result = BeginPopupContextWindow(str_id, popup_flags);
        return Scoped(result, result, EndPopup);
    }

    [[nodiscard]] static Scoped PopupContextVoid(
        const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1) {
        bool result = BeginPopupContextVoid(str_id, popup_flags);
        return Scoped(result, result, EndPopup);
    }

    // BeginTable(), EndTable()

    [[nodiscard]] static Scoped Table(const char* str_id, int columns,
                                      ImGuiTableFlags flags = 0,
                                      const ImVec2& outer_size = ImVec2(0.0f,
                                                                        0.0f),
                                      float inner_width = 0.0f) {
        bool result =
            BeginTable(str_id, columns, flags, outer_size, inner_width);
        return Scoped(result, result, EndTable);
    }

    // BeginTabBar(), EndTabBar()

    [[nodiscard]] static Scoped TabBar(const char* str_id,
                                       ImGuiTabBarFlags flags = 0) {
        bool result = BeginTabBar(str_id, flags);
        return Scoped(result, result, EndTabBar);
    }

    // BeginTabItem(), EndTabItem()

    [[nodiscard]] static Scoped TabItem(const char* label, bool* p_open = NULL,
                                        ImGuiTabItemFlags flags = 0) {
        bool result = BeginTabItem(label, p_open, flags);
        return Scoped(result, result, EndTabItem);
    }

    // BeginDragDropSource(), EndDragDropSource()

    [[nodiscard]] static Scoped DragDropSource(ImGuiDragDropFlags flags = 0) {
        bool result = BeginDragDropSource(flags);
        return Scoped(result, result, EndDragDropSource);
    }

    // BeginDragDropTarget(), EndDragDropTarget()

    [[nodiscard]] static Scoped DragDropTarget() {
        bool result = BeginDragDropTarget();
        return Scoped(result, result, EndDragDropTarget);
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

    [[nodiscard]] static Scoped Indent(float indent_w = 0.0f) {
        ImGui::Indent(indent_w);
        return Scoped([=] { ImGui::Unindent(indent_w); });
    }
};

}  // namespace ImGui
