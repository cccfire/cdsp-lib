/**
 * AI-generated Unit tests for clap-adapters.c
 * TODO:  Pending review
 */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>

#include <clap/clap.h>

#include "cdsp/app.h"
#include "cdsp/gui.h"
#include "cdsp/dsp.h"
#include "cdsp/clap-adapters.h"

/* Mock event list for clap_process_t */
static uint32_t mock_events_size(const struct clap_input_events *list) {
    return 0;
}

static const clap_event_header_t* mock_events_get(const struct clap_input_events *list, uint32_t index) {
    return NULL;
}

/* Test helper: create a mock CLAP package */
static cdsp_clap_package_t* create_test_package(void) {
    cdsp_clap_package_t* package = (cdsp_clap_package_t*)calloc(1, sizeof(cdsp_clap_package_t));

    cdsp_app_t* app = (cdsp_app_t*)calloc(1, sizeof(cdsp_app_t));
    app->name = "test_plugin";
    app->type = CDSP_PLUGIN_APP_TYPE;
    app->gui = (cdsp_gui_t*)calloc(1, sizeof(cdsp_gui_t));
    app->dsp = (cdsp_dsp_t*)calloc(1, sizeof(cdsp_dsp_t));

    package->app = app;
    package->host = NULL;
    package->extensions = NULL;
    package->extensions_length = 0;

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

/* Test: clap_package structure */
static void test_clap_package_struct(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();

    assert_non_null(package);
    assert_non_null(package->app);
    assert_null(package->host);
    assert_null(package->extensions);
    assert_int_equal(package->extensions_length, 0);

    cdsp_destroy_app(package->app);
    free(package->app);
    free(package);
}

/* Test: plugin activate */
static void test_plugin_activate(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    clap_plugin_t* plugin = create_test_plugin(package);

    bool result = cdsp_clap_plugin_activate(plugin, 48000.0, 32, 4096);

    assert_true(result);
    assert_float_equal(package->app->sample_rate, 48000.0, 0.001);
    assert_int_equal(package->app->min_frames_count, 32);
    assert_int_equal(package->app->max_frames_count, 4096);
    assert_true(package->app->active);

    cleanup_test_plugin(plugin);
}

/* Test: plugin deactivate */
static void test_plugin_deactivate(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    clap_plugin_t* plugin = create_test_plugin(package);

    /* Activate first */
    cdsp_clap_plugin_activate(plugin, 44100.0, 64, 2048);
    assert_true(package->app->active);

    /* Then deactivate */
    cdsp_clap_plugin_deactivate(plugin);
    assert_false(package->app->active);

    cleanup_test_plugin(plugin);
}

/* Test: plugin start/stop processing */
static void test_plugin_processing_lifecycle(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    clap_plugin_t* plugin = create_test_plugin(package);

    bool start_result = cdsp_clap_plugin_start_processing(plugin);
    assert_true(start_result);

    /* Stop processing should not crash */
    cdsp_clap_plugin_stop_processing(plugin);

    cleanup_test_plugin(plugin);
}

/* Test: plugin reset */
static void test_plugin_reset(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    clap_plugin_t* plugin = create_test_plugin(package);

    /* Reset should not crash */
    cdsp_clap_plugin_reset(plugin);

    cleanup_test_plugin(plugin);
}

/* Test: plugin process (passthrough) */
static void test_plugin_process(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    clap_plugin_t* plugin = create_test_plugin(package);

    /* Setup audio buffers */
    uint32_t frames = 128;
    float* input_left = (float*)calloc(frames, sizeof(float));
    float* input_right = (float*)calloc(frames, sizeof(float));
    float* output_left = (float*)calloc(frames, sizeof(float));
    float* output_right = (float*)calloc(frames, sizeof(float));

    /* Fill input with test data */
    for (uint32_t i = 0; i < frames; i++) {
        input_left[i] = (float)i / frames;
        input_right[i] = -(float)i / frames;
    }

    float* input_channels[2] = { input_left, input_right };
    float* output_channels[2] = { output_left, output_right };

    clap_audio_buffer_t input_buffer = {
        .data32 = input_channels,
        .data64 = NULL,
        .channel_count = 2,
        .latency = 0,
        .constant_mask = 0
    };

    clap_audio_buffer_t output_buffer = {
        .data32 = output_channels,
        .data64 = NULL,
        .channel_count = 2,
        .latency = 0,
        .constant_mask = 0
    };

    clap_input_events_t in_events = {
        .ctx = NULL,
        .size = mock_events_size,
        .get = mock_events_get
    };

    clap_output_events_t out_events = {
        .ctx = NULL,
        .try_push = NULL
    };

    clap_process_t process = {
        .steady_time = 0,
        .frames_count = frames,
        .transport = NULL,
        .audio_inputs = &input_buffer,
        .audio_outputs = &output_buffer,
        .audio_inputs_count = 1,
        .audio_outputs_count = 1,
        .in_events = &in_events,
        .out_events = &out_events
    };

    clap_process_status status = cdsp_clap_plugin_process(plugin, &process);

    assert_int_equal(status, CLAP_PROCESS_CONTINUE);

    /* Verify passthrough - output should match input */
    for (uint32_t i = 0; i < frames; i++) {
        assert_float_equal(output_left[i], input_left[i], 0.0001f);
        assert_float_equal(output_right[i], input_right[i], 0.0001f);
    }

    free(input_left);
    free(input_right);
    free(output_left);
    free(output_right);
    cleanup_test_plugin(plugin);
}

/* Test: plugin get extension with no extensions */
static void test_plugin_get_extension_none(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    clap_plugin_t* plugin = create_test_plugin(package);

    const void* ext = cdsp_clap_plugin_get_extension(plugin, "nonexistent.extension");
    assert_null(ext);

    cleanup_test_plugin(plugin);
}

/* Test: plugin get extension with extensions */
static void test_plugin_get_extension_found(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    clap_plugin_t* plugin = create_test_plugin(package);

    /* Add a mock extension */
    int mock_extension = 42;
    cdsp_clap_extension_t* extensions = (cdsp_clap_extension_t*)calloc(1, sizeof(cdsp_clap_extension_t));
    extensions[0].name = "test.extension";
    extensions[0].extension = &mock_extension;

    package->extensions = extensions;
    package->extensions_length = 1;

    const void* ext = cdsp_clap_plugin_get_extension(plugin, "test.extension");
    assert_non_null(ext);
    assert_ptr_equal(ext, &mock_extension);

    free(extensions);
    cleanup_test_plugin(plugin);
}

/* Test: factory get plugin count */
static void test_factory_get_plugin_count(void **state) {
    (void)state;

    uint32_t count = cdsp_clap_plugin_factory_get_plugin_count(NULL);
    assert_int_equal(count, 1);
}

/* Test: entry init/deinit */
static void test_entry_lifecycle(void **state) {
    (void)state;

    bool init_result = cdsp_clap_entry_init("/some/path");
    assert_true(init_result);

    /* Deinit should not crash */
    cdsp_clap_entry_deinit();
}

/* Test: on_main_thread callback */
static void test_plugin_on_main_thread(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    clap_plugin_t* plugin = create_test_plugin(package);

    /* Should not crash */
    cdsp_clap_plugin_on_main_thread(plugin);

    cleanup_test_plugin(plugin);
}

/* Test: process with more output channels than input */
static void test_plugin_process_channel_mismatch(void **state) {
    (void)state;

    cdsp_clap_package_t* package = create_test_package();
    clap_plugin_t* plugin = create_test_plugin(package);

    uint32_t frames = 64;
    float* input_mono = (float*)calloc(frames, sizeof(float));
    float* output_left = (float*)calloc(frames, sizeof(float));
    float* output_right = (float*)calloc(frames, sizeof(float));

    /* Fill input */
    for (uint32_t i = 0; i < frames; i++) {
        input_mono[i] = 0.5f;
    }

    float* input_channels[1] = { input_mono };
    float* output_channels[2] = { output_left, output_right };

    clap_audio_buffer_t input_buffer = {
        .data32 = input_channels,
        .channel_count = 1,  /* Mono input */
    };

    clap_audio_buffer_t output_buffer = {
        .data32 = output_channels,
        .channel_count = 2,  /* Stereo output */
    };

    clap_input_events_t in_events = {
        .size = mock_events_size,
        .get = mock_events_get
    };

    clap_process_t process = {
        .frames_count = frames,
        .audio_inputs = &input_buffer,
        .audio_outputs = &output_buffer,
        .audio_inputs_count = 1,
        .audio_outputs_count = 1,
        .in_events = &in_events,
    };

    clap_process_status status = cdsp_clap_plugin_process(plugin, &process);
    assert_int_equal(status, CLAP_PROCESS_CONTINUE);

    /* Left channel should have input data, right channel should be zeroed */
    assert_float_equal(output_left[0], 0.5f, 0.0001f);
    assert_float_equal(output_right[0], 0.0f, 0.0001f);

    free(input_mono);
    free(output_left);
    free(output_right);
    cleanup_test_plugin(plugin);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_clap_package_struct),
        cmocka_unit_test(test_plugin_activate),
        cmocka_unit_test(test_plugin_deactivate),
        cmocka_unit_test(test_plugin_processing_lifecycle),
        cmocka_unit_test(test_plugin_reset),
        cmocka_unit_test(test_plugin_process),
        cmocka_unit_test(test_plugin_get_extension_none),
        cmocka_unit_test(test_plugin_get_extension_found),
        cmocka_unit_test(test_factory_get_plugin_count),
        cmocka_unit_test(test_entry_lifecycle),
        cmocka_unit_test(test_plugin_on_main_thread),
        cmocka_unit_test(test_plugin_process_channel_mismatch),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
