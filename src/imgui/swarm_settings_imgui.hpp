#pragma once

#include "custom_imgui.hpp"

struct ExampleAppLog {
    ImGuiTextBuffer Buf;
    ImGuiTextFilter Filter;
    ImVector<int> LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool AutoScroll; // Keep scrolling if already at the bottom.

    ExampleAppLog()
    {
        AutoScroll = true;
        Clear();
    }

    void Clear()
    {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }

    void Draw()
    {
        // if (!ImGui::Begin(title, p_open)) {
        //     ImGui::End();
        //     return;
        // }
        //
        // // Options menu
        // if (ImGui::BeginPopup("Options")) {
        //     ImGui::Checkbox("Auto-scroll", &AutoScroll);
        //     ImGui::EndPopup();
        // }

        // Main window
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool clear = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        Filter.Draw("Filter", -100.0f);

        ImGui::Separator();

        if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
            if (clear)
                Clear();
            if (copy)
                ImGui::LogToClipboard();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            const char* buf = Buf.begin();
            const char* buf_end = Buf.end();
            if (Filter.IsActive()) {
                // In this example we don't use the clipper when Filter is enabled.
                // This is because we don't have random access to the result of our filter.
                // A real application processing logs with ten of thousands of entries may want to store the result of
                // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
                for (int line_no = 0; line_no < LineOffsets.Size; line_no++) {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    if (Filter.PassFilter(line_start, line_end))
                        ImGui::TextUnformatted(line_start, line_end);
                }
            } else {
                // The simplest and easy way to display the entire buffer:
                //   ImGui::TextUnformatted(buf_begin, buf_end);
                // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
                // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
                // within the visible area.
                // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
                // on your side is recommended. Using ImGuiListClipper requires
                // - A) random access into your data
                // - B) items all being the  same height,
                // both of which we can handle since we have an array pointing to the beginning of each line of text.
                // When using the filter (in the block of code above) we don't have random access into the data to display
                // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
                // it possible (and would be recommended if you want to search through tens of thousands of entries).
                ImGuiListClipper clipper;
                clipper.Begin(LineOffsets.Size);
                while (clipper.Step()) {
                    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
                        const char* line_start = buf + LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                        ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                clipper.End();
            }
            ImGui::PopStyleVar();

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
            // Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();
        // ImGui::End();
    }
};

class SwarmSettingsImgui : public CustomImgui {
public:
    SwarmSettingsImgui(GLFWwindow* window, OpenGLContext* context);
    void Update() override;
    void Reset();

    void SetNbParticles(int nbParticles) { _nbParticles = (nbParticles > _maxNbParticles) ? _maxNbParticles : nbParticles; }
    int GetNbParticles() { return _nbParticles; }
    glm::vec3 GetParticlesColor() { return glm::vec3({ _colorParticles.x, _colorParticles.y, _colorParticles.z }); }
    glm::vec3 GetGoalColor() { return glm::vec3({ _colorGoal.x, _colorGoal.y, _colorGoal.z }); }

    bool IsPaused() { return _paused; }
    bool IsReset() { return _reset; }
    bool TimeToQuit() { return _quit; }
    void LogFPS(float fps);
    template <typename... Args>
    void LogToGui(const char* fmt, Args&&... args)
    {
        _log.AddLog(fmt, std::forward<Args>(args)...);
    }

private:
    bool _paused = true;
    bool _reset = false;
    bool _quit = false;
    float _fps[100];
    int _nbParticles;
    int _maxNbParticles = 10000;
    ImVec4 _colorParticles = ImVec4(.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 250.0f / 255.0f);
    ImVec4 _colorGoal = ImVec4(255.f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 250.0f / 255.0f);
    ExampleAppLog _log;
};
