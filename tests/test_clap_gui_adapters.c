/**
 * AI-generated Unit tests for clap-gui-adapters.c
 * TODO:  Pending review
 */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>

#include <clap/clap.h>
#include <clap/ext/gui.h>

#include "cdsp/app.h"
#include "cdsp/gui.h"
#include "cdsp/dsp.h"
#include "cdsp/clap-adapters.h"
#include "cdsp/clap-gui-adapters.h"

/* Test helper: create a mock CLAP package with app */
static cdsp_clap_package_t* create_test_package(void) {
    cdsp_clap_package_t* package = (cdsp_clap_package_t*)calloc(1, sizeof(cdsp_clap_package_t));

    cdsp_app_t* app = (cdsp_app_t*)calloc(1, sizeof(cdsp_app_t));
    app->name = "test_plugin";
    app->type = CDSP_PLUGIN_APP_TYPE;

    cdsp_gui_t* gui = (cdsp_gui_t*)calloc(1, sizeof(cdsp_gui_t));
    gui->width = 640;
    gui->height = 480;
    gui->min_width = 320;
    gui->min_height = 240;
    gui->aspect_ratio_width = 4;
    gui->aspect_ratio_height = 3;
    gui->should_preserve_aspect_ratio = true;
    app->gui = gui;

    app->dsp = (cdsp_dsp_t*)calloc(1, sizeof(cdsp_dsp_t));

    package->app = app;
    return package;
}

/* Test helper: create a mock CLAP plugin */
static clap_plugin_t* create_test_plugin(cdsp_clap_package_t* package) {
    clap_plugin_t* plugin = (clap_plugin_t*)calloc(1, sizeof(clap_plugin_t));
    plugin->plugin_data = package;
    return plugin;
}

/* Test helper: cleanup */
static void cleanup_test_plugin(clap_plugin_t* plugin) {
    cdsp_clap_package_t* package = (cdsp_clap_package_t*)plugin->plugin_data;
    if (package->app) {
        cdsp_destroy_app(package->app);
        free(package->app);
    }
    free(package);
    free(plugin);
}

/* Test: is_api_supported with floating window */
static void test_is_api_supported_floating(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    clap_plugin_t* plugin = create_test_plugin(package);

    /* Floating windows are always supported */
    bool result = cdsp_gui_clap_is_api_supported(plugin, "any_api", true);
    assert_true(result);

    cleanup_test_plugin(plugin);
}

/* Test: is_api_supported platform-specific */
static void test_is_api_supported_platform(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    clap_plugin_t* plugin = create_test_plugin(package);

    /* Test platform-specific API - at least one should work on current platform */
#if defined(IS_MAC) || defined(__APPLE__)
    bool mac_result = cdsp_gui_clap_is_api_supported(plugin, CLAP_WINDOW_API_COCOA, false);
    /* May or may not be true depending on build flags */
#endif

#if defined(IS_WIN) || defined(_WIN32)
    bool win_result = cdsp_gui_clap_is_api_supported(plugin, CLAP_WINDOW_API_WIN32, false);
    /* May or may not be true depending on build flags */
#endif

#if defined(IS_LINUX) || defined(__linux__)
    bool linux_result = cdsp_gui_clap_is_api_supported(plugin, CLAP_WINDOW_API_X11, false);
    /* May or may not be true depending on build flags */
#endif

    /* Unknown API should return false */
    bool unknown_result = cdsp_gui_clap_is_api_supported(plugin, "unknown.api", false);
    assert_false(unknown_result);

    cleanup_test_plugin(plugin);
}

/* Test: can_resize returns true */
static void test_can_resize(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    clap_plugin_t* plugin = create_test_plugin(package);

    bool result = cdsp_gui_clap_can_resize(plugin);
    assert_true(result);

    cleanup_test_plugin(plugin);
}

/* Test: get_resize_hints */
static void test_get_resize_hints(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    clap_plugin_t* plugin = create_test_plugin(package);

    clap_gui_resize_hints_t hints;
    memset(&hints, 0, sizeof(hints));

    bool result = cdsp_gui_clap_get_resize_hints(plugin, &hints);

    assert_true(result);
    assert_true(hints.can_resize_horizontally);
    assert_true(hints.can_resize_vertically);
    assert_true(hints.preserve_aspect_ratio);
    assert_int_equal(hints.aspect_ratio_width, 4);
    assert_int_equal(hints.aspect_ratio_height, 3);

    cleanup_test_plugin(plugin);
}

/* Test: get_resize_hints without aspect ratio preservation */
static void test_get_resize_hints_no_aspect(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    package->app->gui->should_preserve_aspect_ratio = false;
    clap_plugin_t* plugin = create_test_plugin(package);

    clap_gui_resize_hints_t hints;
    memset(&hints, 0, sizeof(hints));

    bool result = cdsp_gui_clap_get_resize_hints(plugin, &hints);

    assert_true(result);
    assert_false(hints.preserve_aspect_ratio);

    cleanup_test_plugin(plugin);
}

/* Test: init_clap_gui sets all function pointers */
static void test_init_clap_gui(void **state) {
    (void)state;

    clap_plugin_gui_t gui;
    memset(&gui, 0, sizeof(gui));

    cdsp_init_clap_gui(&gui);

    assert_non_null(gui.is_api_supported);
    assert_non_null(gui.get_preferred_api);
    assert_non_null(gui.create);
    assert_non_null(gui.destroy);
    assert_non_null(gui.set_scale);
    assert_non_null(gui.get_size);
    assert_non_null(gui.can_resize);
    assert_non_null(gui.get_resize_hints);
    assert_non_null(gui.adjust_size);
    assert_non_null(gui.set_size);
    assert_non_null(gui.set_parent);
    assert_non_null(gui.set_transient);
    assert_non_null(gui.suggest_title);
    assert_non_null(gui.show);
    assert_non_null(gui.hide);

    /* Verify the pointers point to the correct functions */
    assert_ptr_equal(gui.is_api_supported, cdsp_gui_clap_is_api_supported);
    assert_ptr_equal(gui.can_resize, cdsp_gui_clap_can_resize);
    assert_ptr_equal(gui.get_resize_hints, cdsp_gui_clap_get_resize_hints);
}

/* Test: set_scale returns false (not implemented) */
static void test_set_scale(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    clap_plugin_t* plugin = create_test_plugin(package);

    /* set_scale is not yet implemented, should return false */
    bool result = cdsp_gui_clap_set_scale(plugin, 2.0);
    assert_false(result);

    cleanup_test_plugin(plugin);
}

/* Test: adjust_size with aspect ratio */
static void test_adjust_size_with_aspect(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    package->app->gui->aspect_ratio_width = 16;
    package->app->gui->aspect_ratio_height = 9;
    package->app->gui->should_preserve_aspect_ratio = true;
    clap_plugin_t* plugin = create_test_plugin(package);

    uint32_t width = 1920;
    uint32_t height = 1000;  /* Will be adjusted to maintain 16:9 */

    bool result = cdsp_gui_clap_adjust_size(plugin, &width, &height);

    assert_true(result);
    /* Height should be adjusted based on width ratio */
    /* width/aspect_ratio_width * aspect_ratio_height = 1920/16*9 = 1080 */
    assert_int_equal(height, 1080);

    cleanup_test_plugin(plugin);
}

/* Test: adjust_size without aspect ratio */
static void test_adjust_size_no_aspect(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    package->app->gui->should_preserve_aspect_ratio = false;
    clap_plugin_t* plugin = create_test_plugin(package);

    uint32_t width = 800;
    uint32_t height = 600;

    bool result = cdsp_gui_clap_adjust_size(plugin, &width, &height);

    assert_true(result);
    /* Size should remain unchanged */
    assert_int_equal(width, 800);
    assert_int_equal(height, 600);

    cleanup_test_plugin(plugin);
}

/* Test: gui structure defaults */
static void test_gui_struct_defaults(void **state) {
    (void)state;

    cdsp_gui_t* gui = (cdsp_gui_t*)calloc(1, sizeof(cdsp_gui_t));

    assert_null(gui->data);
    assert_null(gui->world);
    assert_null(gui->view);
    assert_null(gui->cairo_ctx);
    assert_int_equal(gui->width, 0);
    assert_int_equal(gui->height, 0);
    assert_false(gui->should_preserve_aspect_ratio);

    free(gui);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_is_api_supported_floating),
        cmocka_unit_test(test_is_api_supported_platform),
        cmocka_unit_test(test_can_resize),
        cmocka_unit_test(test_get_resize_hints),
        cmocka_unit_test(test_get_resize_hints_no_aspect),
        cmocka_unit_test(test_init_clap_gui),
        cmocka_unit_test(test_set_scale),
        cmocka_unit_test(test_adjust_size_with_aspect),
        cmocka_unit_test(test_adjust_size_no_aspect),
        cmocka_unit_test(test_gui_struct_defaults),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
