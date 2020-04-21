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

    return result;
}

void egmde::WindowManagerPolicy::advise_new_app(miral::ApplicationInfo& application)
{
    ++apps;

    WindowManagementPolicy::advise_new_app(application);
}

void egmde::WindowManagerPolicy::advise_delete_app(miral::ApplicationInfo const& application)
{
    --apps;

    start_launcher();

    WindowManagementPolicy::advise_delete_app(application);
}

void egmde::WindowManagerPolicy::start_launcher() const
{
    // If we only have the wallpaper and launcher, time to show the launcher!
    if (apps == 2)
    {
        std::thread([this] { this->launcher->show(); }).detach();
    }
}

void egmde::WindowManagerPolicy::advise_delete_window(miral::WindowInfo const& window_info)
{
    start_launcher();
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
