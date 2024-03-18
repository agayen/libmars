#include <assert.h>
#include <bare.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <js.h>

#include "chunker.h"

#include <gst/gst.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
    std::string input;
    std::string output;
    std::string muxer;
    int32_t silence_th;
    uint64_t rate;
} gstreamer_t;

static js_value_t *gstreamer_init(js_env_t *env, js_callback_info_t *info)
{
    int err;

    size_t argc = 4;
    js_value_t *argv[4];

    err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
    assert(err == 0);

    assert(argc == 4);

    js_value_t *handle;

    gstreamer_t *self;
    err = js_create_arraybuffer(env, sizeof(gstreamer_t), (void **)&self, &handle);
    assert(err == 0);

    size_t str_len;
    std::string temp_arg;

    err = js_get_value_string_utf8(env, argv[0], nullptr, 0, &str_len);
    assert(err == 0);
    err = js_get_value_string_utf8(env, argv[0], reinterpret_cast<utf8_t *>(&temp_arg), str_len, nullptr);
    assert(err == 0);
    self->input = temp_arg;

    err = js_get_value_string_utf8(env, argv[1], nullptr, 0, &str_len);
    assert(err == 0);
    err = js_get_value_string_utf8(env, argv[1], reinterpret_cast<utf8_t *>(&temp_arg), str_len, nullptr);
    assert(err == 0);
    self->muxer = temp_arg;

    err = js_get_value_string_utf8(env, argv[2], nullptr, 0, &str_len);
    assert(err == 0);
    err = js_get_value_string_utf8(env, argv[2], reinterpret_cast<utf8_t *>(&temp_arg), str_len, nullptr);
    assert(err == 0);
    self->output = temp_arg;

    int32_t silence_th;
    err = js_get_value_int32(env, argv[3], &silence_th);
    assert(err == 0);
    self->silence_th = silence_th;

    return handle;
}

static js_value_t *gstreamer_start(js_env_t *env, js_callback_info_t *info)
{
    int err;

    size_t argc = 1;
    js_value_t *argv[1];

    gstreamer_t *self;
    err = js_get_arraybuffer_info(env, argv[0], (void **)&self, NULL);
    assert(err == 0);

    g_autoptr (MarsChunker) chunker = NULL;

    std::cout<<"hello"<<std::endl;

    chunker = MARS_CHUNKER(g_object_new(MARS_TYPE_CHUNKER,
                           "input", &self -> input,
                           "output", &self -> output,
                           "muxer", &self -> muxer,
                           "rate", 8000,
                           "maximum-chunk-time", 8 * GST_SECOND,
                           "silence-threshold", &self -> silence_th, //-45 the best
                           NULL));

    // mars_chunker_play(chunker);

    // if (g_strcmp0(input, MARS_CHUNKER_INPUT_MIC) == 0)
    // {
    //     printf("Listening from microphone; enter any key to quit: ");
    //     scanf("%c", &key);
    // }
    // else
    // {
    //     printf("Waiting for %s to be chunked…\n", input);
    //     fflush(stdout);
    //     /* Wait for 1s so GStreamer can update its states. */
    //     sleep(1);
    //     while (mars_chunker_is_playing(chunker))
    //     {
    //     }
    // }
    js_value_t *res;
    return res;
}

static js_value_t *
init(js_env_t *env, js_value_t *exports)
{
#define V(name, fn)                                        \
    {                                                      \
        js_value_t *val;                                   \
        js_create_function(env, name, -1, fn, NULL, &val); \
        js_set_named_property(env, exports, name, val);    \
    }

    V("init", gstreamer_init)
    V("start", gstreamer_start)
#undef V

    return exports;
}

BARE_MODULE(bare_timers, init)