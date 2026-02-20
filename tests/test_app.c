/**
 * AI-generated Unit tests for app.c
 * TODO:  Pending review
 */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>

#include "cdsp/app.h"
#include "cdsp/gui.h"
#include "cdsp/dsp.h"

/* Test helper: create a minimal app */
static cdsp_app_t* create_test_app(void) {
    cdsp_app_t* app = (cdsp_app_t*)calloc(1, sizeof(cdsp_app_t));
    app->name = "test_app";
    app->type = CDSP_STANDALONE_APP_TYPE;
    app->gui = (cdsp_gui_t*)calloc(1, sizeof(cdsp_gui_t));
    app->dsp = (cdsp_dsp_t*)calloc(1, sizeof(cdsp_dsp_t));
    return app;
}

/* Test: app type enum values */
static void test_app_type_enum(void **state) {
    (void)state;

    assert_int_equal(CDSP_STANDALONE_APP_TYPE, 0);
    assert_int_equal(CDSP_PLUGIN_APP_TYPE, 1);
}

/* Test: app structure initialization */
static void test_app_struct_initialization(void **state) {
    (void)state;

    cdsp_app_t* app = create_test_app();

    assert_non_null(app);
    assert_string_equal(app->name, "test_app");
    assert_int_equal(app->type, CDSP_STANDALONE_APP_TYPE);
    assert_non_null(app->gui);
    assert_non_null(app->dsp);
    assert_false(app->active);
    assert_float_equal(app->sample_rate, 0.0, 0.001);
    assert_int_equal(app->min_frames_count, 0);
    assert_int_equal(app->max_frames_count, 0);

    cdsp_destroy_app(app);
    free(app);
}

/* Test: cdsp_destroy_app frees gui and dsp */
static void test_destroy_app(void **state) {
    (void)state;

    cdsp_app_t* app = create_test_app();

    /* Store pointers to verify they were allocated */
    assert_non_null(app->gui);
    assert_non_null(app->dsp);

    /* Destroy should free gui and dsp */
    cdsp_destroy_app(app);

    /* After destroy, we just verify no crash - the memory is freed */
    free(app);
}

/* Test: app with plugin type */
static void test_app_plugin_type(void **state) {
    (void)state;

    cdsp_app_t* app = create_test_app();
    app->type = CDSP_PLUGIN_APP_TYPE;

    assert_int_equal(app->type, CDSP_PLUGIN_APP_TYPE);

    cdsp_destroy_app(app);
    free(app);
}

/* Test: app sample rate and frame counts */
static void test_app_audio_params(void **state) {
    (void)state;

    cdsp_app_t* app = create_test_app();

    app->sample_rate = 48000.0;
    app->min_frames_count = 32;
    app->max_frames_count = 4096;
    app->active = true;

    assert_float_equal(app->sample_rate, 48000.0, 0.001);
    assert_int_equal(app->min_frames_count, 32);
    assert_int_equal(app->max_frames_count, 4096);
    assert_true(app->active);

    cdsp_destroy_app(app);
    free(app);
}

/* Test: app context pointer */
static void test_app_context(void **state) {
    (void)state;

    cdsp_app_t* app = create_test_app();

    int custom_data = 42;
    app->ctx = &custom_data;

    assert_ptr_equal(app->ctx, &custom_data);
    assert_int_equal(*(int*)app->ctx, 42);

    cdsp_destroy_app(app);
    free(app);
}

/* Test: app with NULL gui and dsp (edge case) */
static void test_app_null_components(void **state) {
    (void)state;

    cdsp_app_t* app = (cdsp_app_t*)calloc(1, sizeof(cdsp_app_t));
    app->gui = NULL;
    app->dsp = NULL;

    /* cdsp_destroy_app should handle NULL gracefully (free(NULL) is valid) */
    cdsp_destroy_app(app);
    free(app);
}

/* Test: app function pointers */
static void test_app_function_pointers(void **state) {
    (void)state;

    cdsp_app_t* app = create_test_app();

    /* Initially NULL */
    assert_null(app->init);
    assert_null(app->destroy);

    cdsp_destroy_app(app);
    free(app);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_app_type_enum),
        cmocka_unit_test(test_app_struct_initialization),
        cmocka_unit_test(test_destroy_app),
        cmocka_unit_test(test_app_plugin_type),
        cmocka_unit_test(test_app_audio_params),
        cmocka_unit_test(test_app_context),
        cmocka_unit_test(test_app_null_components),
        cmocka_unit_test(test_app_function_pointers),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
