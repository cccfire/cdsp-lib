/**
 * AI-generated Unit tests for clap-extensions-adapters.c
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
#include "cdsp/clap-extensions-adapters.h"

/* Test helper: create a minimal app */
static cdsp_app_t* create_test_app(void) {
    cdsp_app_t* app = (cdsp_app_t*)calloc(1, sizeof(cdsp_app_t));
    app->name = "test_app";
    app->type = CDSP_PLUGIN_APP_TYPE;
    app->gui = (cdsp_gui_t*)calloc(1, sizeof(cdsp_gui_t));
    app->dsp = (cdsp_dsp_t*)calloc(1, sizeof(cdsp_dsp_t));
    return app;
}

/* Test: generate extensions from app */
static void test_generate_extensions(void **state) {
    (void)state;

    cdsp_app_t* app = create_test_app();
    size_t extensions_length = 0;

    cdsp_clap_extension_t* extensions = cdsp_clap_generate_extensions_from_app(app, &extensions_length);

    assert_non_null(extensions);
    assert_int_equal(extensions_length, 1);  /* Currently only GUI extension */

    /* Verify GUI extension is present */
    assert_string_equal(extensions[0].name, CLAP_EXT_GUI);
    assert_non_null(extensions[0].extension);

    /* Cleanup - note: extensions[0].extension contains allocated gui struct */
    free(extensions[0].extension);
    free(extensions);
    cdsp_destroy_app(app);
    free(app);
}

/* Test: generated GUI extension has proper vtable */
static void test_generated_gui_extension(void **state) {
    (void)state;

    cdsp_app_t* app = create_test_app();
    size_t extensions_length = 0;

    cdsp_clap_extension_t* extensions = cdsp_clap_generate_extensions_from_app(app, &extensions_length);

    /* Cast to clap_plugin_gui_t to verify structure */
    clap_plugin_gui_t* gui = (clap_plugin_gui_t*)extensions[0].extension;

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

    free(extensions[0].extension);
    free(extensions);
    cdsp_destroy_app(app);
    free(app);
}

/* Test: free extensions */
static void test_free_extensions(void **state) {
    (void)state;

    cdsp_clap_package_t package;
    memset(&package, 0, sizeof(package));

    /* Allocate extensions manually */
    package.extensions = (cdsp_clap_extension_t*)calloc(2, sizeof(cdsp_clap_extension_t));
    package.extensions_length = 2;

    assert_non_null(package.extensions);
    assert_int_equal(package.extensions_length, 2);

    cdsp_clap_free_extensions(&package);

    assert_null(package.extensions);
    assert_int_equal(package.extensions_length, 0);
}

/* Test: free extensions with NULL */
static void test_free_extensions_null(void **state) {
    (void)state;

    cdsp_clap_package_t package;
    memset(&package, 0, sizeof(package));

    package.extensions = NULL;
    package.extensions_length = 0;

    /* Should not crash */
    cdsp_clap_free_extensions(&package);

    assert_null(package.extensions);
    assert_int_equal(package.extensions_length, 0);
}

/* Test: extension structure */
static void test_extension_struct(void **state) {
    (void)state;

    cdsp_clap_extension_t extension;
    memset(&extension, 0, sizeof(extension));

    int mock_data = 42;
    extension.name = "test.extension";
    extension.extension = &mock_data;

    assert_string_equal(extension.name, "test.extension");
    assert_ptr_equal(extension.extension, &mock_data);
}

/* Test: multiple extensions can be stored */
static void test_multiple_extensions(void **state) {
    (void)state;

    cdsp_clap_extension_t* extensions = (cdsp_clap_extension_t*)calloc(3, sizeof(cdsp_clap_extension_t));

    int data1 = 1, data2 = 2, data3 = 3;

    extensions[0].name = "extension.one";
    extensions[0].extension = &data1;

    extensions[1].name = "extension.two";
    extensions[1].extension = &data2;

    extensions[2].name = "extension.three";
    extensions[2].extension = &data3;

    assert_string_equal(extensions[0].name, "extension.one");
    assert_string_equal(extensions[1].name, "extension.two");
    assert_string_equal(extensions[2].name, "extension.three");

    assert_int_equal(*(int*)extensions[0].extension, 1);
    assert_int_equal(*(int*)extensions[1].extension, 2);
    assert_int_equal(*(int*)extensions[2].extension, 3);

    free(extensions);
}

/* Test: output length is set correctly */
static void test_output_length(void **state) {
    (void)state;

    cdsp_app_t* app = create_test_app();
    size_t extensions_length = 999;  /* Set to non-zero to verify it's updated */

    cdsp_clap_extension_t* extensions = cdsp_clap_generate_extensions_from_app(app, &extensions_length);

    /* Verify the output length was set */
    assert_int_equal(extensions_length, 1);

    free(extensions[0].extension);
    free(extensions);
    cdsp_destroy_app(app);
    free(app);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_generate_extensions),
        cmocka_unit_test(test_generated_gui_extension),
        cmocka_unit_test(test_free_extensions),
        cmocka_unit_test(test_free_extensions_null),
        cmocka_unit_test(test_extension_struct),
        cmocka_unit_test(test_multiple_extensions),
        cmocka_unit_test(test_output_length),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
