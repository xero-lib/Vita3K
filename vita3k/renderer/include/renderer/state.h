// Vita3K emulator project
// Copyright (C) 2021 Vita3K team
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#pragma once

#include <features/state.h>
#include <renderer/commands.h>
#include <renderer/types.h>
#include <threads/queue.h>

#include <condition_variable>
#include <mutex>

struct SDL_Cursor;
struct DisplayState;
struct GxmState;

namespace renderer {
struct State {
    Backend current_backend;
    FeatureState features;
    int res_multiplier;
    bool disable_surface_sync;

    GXPPtrMap gxp_ptr_map;
    Queue<CommandList> command_buffer_queue;
    std::condition_variable command_finish_one;
    std::mutex command_finish_one_mutex;

    std::condition_variable notification_ready;
    std::mutex notification_mutex;

    int last_scene_id = 0;

    uint32_t shaders_count_compiled;
    uint32_t programs_count_pre_compiled;

    std::atomic<bool> should_display;

    virtual bool init(const char *base_path, const bool hashless_texture_cache) = 0;
    virtual void render_frame(const SceFVector2 &viewport_pos, const SceFVector2 &viewport_size, const DisplayState &display,
        const GxmState &gxm, MemState &mem)
        = 0;
    virtual void set_fxaa(bool enable_fxaa) = 0;
    virtual int get_max_anisotropic_filtering() = 0;
    virtual void set_anisotropic_filtering(int anisotropic_filtering) = 0;

    virtual ~State() = default;
};
} // namespace renderer
