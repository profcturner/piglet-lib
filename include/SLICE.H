/* SLICE.H
 *
 * Turbo/Borland C prototypes for the functions provided by SLICE.ASM
 *
 */

#ifndef SLICE_H_
#define SLICE_H_


#ifdef __cplusplus
  extern "C" {
#endif



#ifdef Slice_TaskerNames

static char *tasker_names[] =
{
    "No MultiTasker",
    "DesqView",
    "MicroSoft Windows",
    "OS/2 2.0",
    "OS/2 2.1 or above"
};

#endif


/* Returns zero if no known multi-tasker found, or an ID if one is. */
int     detect_multitasker(void);

/* Give up a timeslice. detect_multitasker should be called first. */
void    timeslice(void);

/* Disable timeslicing, but allows timeslice to be called safely */
/* May be used as an alternative to detect_multitasker           */
void    disable_multitasker(void);

/* Possible return values from detect_multitasker.                  */
#define MT_NONE     0
#define MT_DESQVIEW 1
#define MT_WIN_3    2
#define MT_OS2_20   3
#define MT_OS2_21   4


#ifdef __cplusplus
  }
#endif




#endif
