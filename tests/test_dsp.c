/**
 * AI-generated Unit tests for dsp.h data structures
 * TODO:  Pending review
 */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>

#include "cdsp/app.h"
#include "cdsp/dsp.h"

/* Test: num format enum values */
static void test_num_format_enum(void **state) {
    (void)state;

    assert_int_equal(CDSP_FLOAT, 0);
    assert_int_equal(CDSP_DOUBLE, 1);
}

/* Test: side enum values */
static void test_side_enum(void **state) {
    (void)state;

    assert_int_equal(CDSP_LEFT, 0);
    assert_int_equal(CDSP_RIGHT, 1);
}

/* Test: audio buffer structure with float32 data */
static void test_audio_buffer_float32(void **state) {
    (void)state;

    cdsp_audio_buffer_t buffer;
    memset(&buffer, 0, sizeof(buffer));

    /* Allocate stereo float32 buffer with 256 frames */
    uint32_t channels = 2;
    uint32_t frames = 256;

    float** data32 = (float**)malloc(channels * sizeof(float*));
    for (uint32_t i = 0; i < channels; i++) {
        data32[i] = (float*)calloc(frames, sizeof(float));
    }

    buffer.data.data32 = data32;
    buffer.format = CDSP_FLOAT;
    buffer.channel_count = channels;
    buffer.latency = 0;
    buffer.constant_mask = 0;

    assert_int_equal(buffer.format, CDSP_FLOAT);
    assert_int_equal(buffer.channel_count, 2);
    assert_non_null(buffer.data.data32);
    assert_non_null(buffer.data.data32[0]);
    assert_non_null(buffer.data.data32[1]);

    /* Fill with test data */
    for (uint32_t f = 0; f < frames; f++) {
        buffer.data.data32[0][f] = (float)f / frames;  /* Left channel */
        buffer.data.data32[1][f] = -(float)f / frames; /* Right channel */
    }

    /* Verify data */
    assert_float_equal(buffer.data.data32[0][0], 0.0f, 0.001f);
    assert_float_equal(buffer.data.data32[0][128], 0.5f, 0.001f);
    assert_float_equal(buffer.data.data32[1][128], -0.5f, 0.001f);

    /* Cleanup */
    for (uint32_t i = 0; i < channels; i++) {
        free(data32[i]);
    }
    free(data32);
}

/* Test: audio buffer structure with float64 data */
static void test_audio_buffer_float64(void **state) {
    (void)state;

    cdsp_audio_buffer_t buffer;
    memset(&buffer, 0, sizeof(buffer));

    /* Allocate stereo float64 buffer with 512 frames */
    uint32_t channels = 2;
    uint32_t frames = 512;

    double** data64 = (double**)malloc(channels * sizeof(double*));
    for (uint32_t i = 0; i < channels; i++) {
        data64[i] = (double*)calloc(frames, sizeof(double));
    }

    buffer.data.data64 = data64;
    buffer.format = CDSP_DOUBLE;
    buffer.channel_count = channels;
    buffer.latency = 10;
    buffer.constant_mask = 0x01;

    assert_int_equal(buffer.format, CDSP_DOUBLE);
    assert_int_equal(buffer.channel_count, 2);
    assert_int_equal(buffer.latency, 10);
    assert_int_equal(buffer.constant_mask, 0x01);
    assert_non_null(buffer.data.data64);

    /* Fill with test data */
    for (uint32_t f = 0; f < frames; f++) {
        buffer.data.data64[0][f] = (double)f / frames;
        buffer.data.data64[1][f] = 1.0 - (double)f / frames;
    }

    /* Verify precision */
    assert_float_equal(buffer.data.data64[0][256], 0.5, 0.0001);
    assert_float_equal(buffer.data.data64[1][256], 0.5, 0.0001);

    /* Cleanup */
    for (uint32_t i = 0; i < channels; i++) {
        free(data64[i]);
    }
    free(data64);
}

/* Test: process data structure */
static void test_process_data(void **state) {
    (void)state;

    cdsp_process_data_t process_data;
    memset(&process_data, 0, sizeof(process_data));

    process_data.timestamp = 1234567890;
    process_data.frames_count = 128;

    /* Setup input buffer */
    process_data.input_buffer.format = CDSP_FLOAT;
    process_data.input_buffer.channel_count = 2;

    /* Setup output buffer */
    process_data.output_buffer.format = CDSP_FLOAT;
    process_data.output_buffer.channel_count = 2;

    assert_int_equal(process_data.timestamp, 1234567890);
    assert_int_equal(process_data.frames_count, 128);
    assert_int_equal(process_data.input_buffer.format, CDSP_FLOAT);
    assert_int_equal(process_data.output_buffer.format, CDSP_FLOAT);
}

/* Test: dsp structure */
static void test_dsp_struct(void **state) {
    (void)state;

    cdsp_dsp_t* dsp = (cdsp_dsp_t*)calloc(1, sizeof(cdsp_dsp_t));

    assert_non_null(dsp);
    assert_null(dsp->ctx);
    assert_null(dsp->init);
    assert_null(dsp->start_processing);
    assert_null(dsp->process);
    assert_null(dsp->stop_processing);
    assert_null(dsp->destroy);

    free(dsp);
}

/* Test: dsp context pointer */
static void test_dsp_context(void **state) {
    (void)state;

    cdsp_dsp_t* dsp = (cdsp_dsp_t*)calloc(1, sizeof(cdsp_dsp_t));

    int custom_ctx = 123;
    dsp->ctx = &custom_ctx;

    assert_ptr_equal(dsp->ctx, &custom_ctx);
    assert_int_equal(*(int*)dsp->ctx, 123);

    free(dsp);
}

/* Test: mono audio buffer */
static void test_audio_buffer_mono(void **state) {
    (void)state;

    cdsp_audio_buffer_t buffer;
    memset(&buffer, 0, sizeof(buffer));

    uint32_t channels = 1;
    uint32_t frames = 64;

    float** data32 = (float**)malloc(channels * sizeof(float*));
    data32[0] = (float*)calloc(frames, sizeof(float));

    buffer.data.data32 = data32;
    buffer.format = CDSP_FLOAT;
    buffer.channel_count = channels;

    assert_int_equal(buffer.channel_count, 1);

    free(data32[0]);
    free(data32);
}

/* Test: multi-channel audio buffer */
static void test_audio_buffer_multichannel(void **state) {
    (void)state;

    cdsp_audio_buffer_t buffer;
    memset(&buffer, 0, sizeof(buffer));

    uint32_t channels = 8; /* 7.1 surround */
    uint32_t frames = 256;

    float** data32 = (float**)malloc(channels * sizeof(float*));
    for (uint32_t i = 0; i < channels; i++) {
        data32[i] = (float*)calloc(frames, sizeof(float));
    }

    buffer.data.data32 = data32;
    buffer.format = CDSP_FLOAT;
    buffer.channel_count = channels;

    assert_int_equal(buffer.channel_count, 8);

    /* Verify all channels are accessible */
    for (uint32_t ch = 0; ch < channels; ch++) {
        assert_non_null(buffer.data.data32[ch]);
        buffer.data.data32[ch][0] = (float)ch;
    }

    for (uint32_t ch = 0; ch < channels; ch++) {
        assert_float_equal(buffer.data.data32[ch][0], (float)ch, 0.001f);
    }

    for (uint32_t i = 0; i < channels; i++) {
        free(data32[i]);
    }
    free(data32);
}

/* Test: buffer data union */
static void test_buffer_data_union(void **state) {
    (void)state;

    cdsp_buffer_data_t data;

    /* Test that the union can hold either type */
    float* float_ptr = (float*)malloc(sizeof(float));
    double* double_ptr = (double*)malloc(sizeof(double));

    data.data32 = &float_ptr;
    assert_ptr_equal(data.data32, &float_ptr);

    data.data64 = &double_ptr;
    assert_ptr_equal(data.data64, &double_ptr);

    free(float_ptr);
    free(double_ptr);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_num_format_enum),
        cmocka_unit_test(test_side_enum),
        cmocka_unit_test(test_audio_buffer_float32),
        cmocka_unit_test(test_audio_buffer_float64),
        cmocka_unit_test(test_process_data),
        cmocka_unit_test(test_dsp_struct),
        cmocka_unit_test(test_dsp_context),
        cmocka_unit_test(test_audio_buffer_mono),
        cmocka_unit_test(test_audio_buffer_multichannel),
        cmocka_unit_test(test_buffer_data_union),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
