/**
 * @file lv_conf.h
 */

#ifndef LV_CONF_H
#define LV_CONF_H

/* clang-format off */

#define LV_USE_SYSMON        		1
#define LV_USE_PERF_MONITOR  		1

//#define LV_USE_LOG  				1
//#define LV_LOG_LEVEL 				LV_LOG_LEVEL_WARN
//#define LV_LOG_PRINTF 				1

/* Show some widget. It might be required to increase `LV_MEM_SIZE` */
#define LV_USE_DEMO_WIDGETS        	0

/* Benchmark your system */
#define LV_USE_DEMO_BENCHMARK   	0

/* Stress test for LVGL */
#define LV_USE_DEMO_STRESS      	0

/* Music player demo */
#define LV_USE_DEMO_MUSIC       	1
#if LV_USE_DEMO_MUSIC
# define LV_DEMO_MUSIC_SQUARE       0
# define LV_DEMO_MUSIC_LANDSCAPE    1
# define LV_DEMO_MUSIC_ROUND        0
# define LV_DEMO_MUSIC_LARGE        0
# define LV_DEMO_MUSIC_AUTO_PLAY    1
#endif

/* Flex layout demo */
#define LV_USE_DEMO_FLEX_LAYOUT     0

/* Smart-phone like multi-language demo */
#define LV_USE_DEMO_MULTILANG       0

/* Widget transformation demo */
#define LV_USE_DEMO_TRANSFORM       0

/* Demonstrate scroll settings */
#define LV_USE_DEMO_SCROLL          0


/*
 * COLOR DEPTH
 */
#define DEMO_BUFFER_BYTE_PER_PIXEL 2
#if defined(DEMO_BUFFER_BYTE_PER_PIXEL)

#if DEMO_BUFFER_BYTE_PER_PIXEL == 2
#define LV_COLOR_DEPTH     16
#define LV_COLOR_16_SWAP 1
#elif DEMO_BUFFER_BYTE_PER_PIXEL == 3
#define LV_COLOR_DEPTH     24
#elif DEMO_BUFFER_BYTE_PER_PIXEL == 4
#define LV_COLOR_DEPTH     32
#elif DEMO_BUFFER_BYTE_PER_PIXEL == 1
#if (CONFIG_LV_COLOR_DEPTH == 1)
#define LV_COLOR_DEPTH     1 /* Can be 8 or 1. */
#else
#define LV_COLOR_DEPTH     8 /* Can be 8 or 1. */
#endif
#else
#error Unsupport color depth
#endif

#endif /* defined(DEMO_BUFFER_BYTE_PER_PIXEL) */

#define LV_DISP_DEF_REFR_PERIOD 20  // ~50 FPS target

//#define LV_MEM_SIZE       (56U * 1024U)
#define LV_MEM_SIZE (512U * 1024U)

#define LV_FONT_MONTSERRAT_12  	1
#define LV_FONT_MONTSERRAT_16  	1

/* clang-format on */
#endif /*LV_CONF_H*/
