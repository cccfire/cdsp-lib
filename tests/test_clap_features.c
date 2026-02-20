/**
 * AI-generated Unit tests for clap-features-adapters.c
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
#include "cdsp/clap-features-adapters.h"

/* Test helper: create a minimal app */
static cdsp_app_t* create_test_app(void) {
    cdsp_app_t* app = (cdsp_app_t*)calloc(1, sizeof(cdsp_app_t));
    app->name = "test_app";
    app->type = CDSP_PLUGIN_APP_TYPE;
    app->gui = (cdsp_gui_t*)calloc(1, sizeof(cdsp_gui_t));
    app->dsp = (cdsp_dsp_t*)calloc(1, sizeof(cdsp_dsp_t));
    return app;
}

/* Test: generate features from app */
static void test_generate_features(void **state) {
    (void)state;

    cdsp_app_t* app = create_test_app();
    size_t features_length = 0;

    cdsp_clap_feature_t* features = cdsp_clap_generate_features_from_app(app, &features_length);

    assert_non_null(features);
    assert_int_equal(features_length, 1);  /* Currently only GUI feature */

    /* Verify GUI feature is present */
    assert_string_equal(features[0].name, CLAP_EXT_GUI);
    assert_non_null(features[0].feature);

    /* Cleanup - note: features[0].feature contains allocated gui struct */
    free(features[0].feature);
    free(features);
    cdsp_destroy_app(app);
    free(app);
}

/* Test: generated GUI feature has proper vtable */
static void test_generated_gui_feature(void **state) {
    (void)state;

    cdsp_app_t* app = create_test_app();
    size_t features_length = 0;

    cdsp_clap_feature_t* features = cdsp_clap_generate_features_from_app(app, &features_length);

    /* Cast to clap_plugin_gui_t to verify structure */
    clap_plugin_gui_t* gui = (clap_plugin_gui_t*)features[0].feature;

    assert_non_null(gui->is_api_supported);
    assert_non_null(gui->get_preferred_api);
    assert_non_null(gui->create);
    assert_non_null(gui->destroy);
    assert_non_null(gui->set_scale);
    assert_non_null(gui->get_size);
    assert_non_null(gui->can_resize);
    assert_non_null(gui->get_resize_hints);
    assert_non_null(gui->adjust_size);
    assert_non_null(gui->set_size);
    assert_non_null(gui->set_parent);
    assert_non_null(gui->set_transient);
    assert_non_null(gui->suggest_title);
    assert_non_null(gui->show);
    assert_non_null(gui->hide);

    free(features[0].feature);
    free(features);
    cdsp_destroy_app(app);
    free(app);
}

/* Test: free features */
static void test_free_features(void **state) {
    (void)state;

    cdsp_clap_package_t package;
    memset(&package, 0, sizeof(package));

    /* Allocate features manually */
    package.features = (cdsp_clap_feature_t*)calloc(2, sizeof(cdsp_clap_feature_t));
    package.features_length = 2;

    assert_non_null(package.features);
    assert_int_equal(package.features_length, 2);

    cdsp_clap_free_features(&package);

    assert_null(package.features);
    assert_int_equal(package.features_length, 0);
}

/* Test: free features with NULL */
static void test_free_features_null(void **state) {
    (void)state;

    cdsp_clap_package_t package;
    memset(&package, 0, sizeof(package));

    package.features = NULL;
    package.features_length = 0;

    /* Should not crash */
    cdsp_clap_free_features(&package);

    assert_null(package.features);
    assert_int_equal(package.features_length, 0);
}

/* Test: feature structure */
static void test_feature_struct(void **state) {
    (void)state;

    cdsp_clap_feature_t feature;
    memset(&feature, 0, sizeof(feature));

    int mock_data = 42;
    feature.name = "test.feature";
    feature.feature = &mock_data;

    assert_string_equal(feature.name, "test.feature");
    assert_ptr_equal(feature.feature, &mock_data);
}

/* Test: multiple features can be stored */
static void test_multiple_features(void **state) {
    (void)state;

    cdsp_clap_feature_t* features = (cdsp_clap_feature_t*)calloc(3, sizeof(cdsp_clap_feature_t));

    int data1 = 1, data2 = 2, data3 = 3;

    features[0].name = "feature.one";
    features[0].feature = &data1;

    features[1].name = "feature.two";
    features[1].feature = &data2;

    features[2].name = "feature.three";
    features[2].feature = &data3;

    assert_string_equal(features[0].name, "feature.one");
    assert_string_equal(features[1].name, "feature.two");
    assert_string_equal(features[2].name, "feature.three");

    assert_int_equal(*(int*)features[0].feature, 1);
    assert_int_equal(*(int*)features[1].feature, 2);
    assert_int_equal(*(int*)features[2].feature, 3);

    free(features);
}

/* Test: output length is set correctly */
static void test_output_length(void **state) {
    (void)state;

    cdsp_app_t* app = create_test_app();
    size_t features_length = 999;  /* Set to non-zero to verify it's updated */

    cdsp_clap_feature_t* features = cdsp_clap_generate_features_from_app(app, &features_length);

    /* Verify the output length was set */
    assert_int_equal(features_length, 1);

    free(features[0].feature);
    free(features);
    cdsp_destroy_app(app);
    free(app);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_generate_features),
        cmocka_unit_test(test_generated_gui_feature),
        cmocka_unit_test(test_free_features),
        cmocka_unit_test(test_free_features_null),
        cmocka_unit_test(test_feature_struct),
        cmocka_unit_test(test_multiple_features),
        cmocka_unit_test(test_output_length),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
