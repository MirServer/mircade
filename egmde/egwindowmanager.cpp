/*
 * Copyright © 2016-19 Octopull Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Alan Griffiths <alan@octopull.co.uk>
 */

#include "egwindowmanager.h"
#include "egwallpaper.h"
#include "eglauncher.h"

#include <miral/application_info.h>
#include <miral/window_info.h>
#include <miral/window_manager_tools.h>

#include <linux/input.h>
#include <future>

using namespace mir::geometry;

egmde::WindowManagerPolicy::WindowManagerPolicy(
    miral::WindowManagerTools const& tools, Wallpaper const& wallpaper, Launcher& launcher) :
    MinimalWindowManager{tools},
    wallpaper{&wallpaper},
    launcher{&launcher}
{
}

miral::WindowSpecification egmde::WindowManagerPolicy::place_new_window(
    miral::ApplicationInfo const& app_info, miral::WindowSpecification const& request_parameters)
{
    auto result = MinimalWindowManager::place_new_window(app_info, request_parameters);

    if (app_info.application() == wallpaper->session())
    {
        result.type() = mir_window_type_decoration;
    }
    else if ((result.type() == mir_window_type_normal || result.type() == mir_window_type_freestyle) &&
             (!result.parent().is_set() || !result.parent().value().lock()))
    {
        result.state() = mir_window_state_maximized;
        result.size() = mir::optional_value<Size>{}; // Ignore requested size (if any) when we maximize
        tools.place_and_size_for_state(result, WindowInfo{});

        if (!request_parameters.state().is_set() || request_parameters.state().value() != mir_window_state_restored)
            result.state() = request_parameters.state();
    }

    return result;
}

void egmde::WindowManagerPolicy::handle_modify_window(WindowInfo& window_info, WindowSpecification const& modifications)
{
    WindowSpecification specification = modifications;

    if ((window_info.type() == mir_window_type_normal || window_info.type() == mir_window_type_freestyle) &&
        !window_info.parent())
    {
        specification.state() = mir_window_state_maximized;
        specification.size() = mir::optional_value<Size>{}; // Ignore requested size (if any) when we maximize
        tools.place_and_size_for_state(specification, window_info);

        if (!modifications.state().is_set() || modifications.state().value() != mir_window_state_restored)
            specification.state() = modifications.state();
    }

    MinimalWindowManager::handle_modify_window(window_info, specification);
}

void egmde::WindowManagerPolicy::start_launcher() const
{
    // If we only have the wallpaper and launcher, time to show the launcher!
    std::thread([this] { this->launcher->show(); }).detach();
}

void egmde::WindowManagerPolicy::advise_delete_window(miral::WindowInfo const& window_info)
{
    auto const& application = window_info.window().application();

    if (application != launcher->session() &&
        application != wallpaper->session())
    {
        auto const& app_info = tools.info_for(application);

        if (app_info.windows().size() == 1)
        {
            start_launcher();
        }
    }

    WindowManagementPolicy::advise_delete_window(window_info);
}

void egmde::WindowManagerPolicy::handle_window_ready(miral::WindowInfo& window_info)
{
    MinimalWindowManager::handle_window_ready(window_info);

    if (window_info.window().application() == wallpaper->session())
    {
        start_launcher();
    }
}
