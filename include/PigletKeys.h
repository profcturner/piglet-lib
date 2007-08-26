/* Keys */

/*
#define CUR_LEFT  (75 << 8)
#define CUR_RIGHT (77 << 8)
#define CUR_UP    (72 << 8)
#define CUR_DOWN  (80 << 8)
#define PAGE_DOWN (81 << 8)
#define PAGE_UP   (73 << 8)
#define HOME      (71 << 8)
#define END       (79 << 8)
#define INSERT    (82 << 8)
#define DELETE    (83 << 8)

#define ALT_H     (35 << 8)
#define FUNCT_1   (59 << 8)
#define ALT_Z     0x2C00
#define FUNCT_4  0x3e00
#define FUNCT_5  0x3f00
#define FUNCT_6  0x4000
#define TAB 9*/
#define SHIFT_TAB 0x0f00
/**************************************************************************/
/*  This header file contains defines for use with idle_getchar() in the  */
/*                            keypoll file.                               */
/**************************************************************************/

/* Function keys */

#define FUNCT_1         0x3B00
#define FUNCT_2         0x3C00        
#define FUNCT_3         0x3D00
#define FUNCT_4         0x3E00
#define FUNCT_5         0x3F00
#define FUNCT_6         0x4000
#define FUNCT_7         0x4100
#define FUNCT_8         0x4200
#define FUNCT_9         0x4300
#define FUNCT_10        0x4400
#define FUNCT_11        0x8500
#define FUNCT_12        0x8600

/* Cursor functions */

#define CUR_LEFT        0x4B00
#define CUR_RIGHT       0x4D00
#define CUR_UP          0x4800
#define CUR_DOWN        0x5000

/* Related functions */

#define PAGE_UP         0x4900
#define PAGE_DOWN       0x5100
#define HOME            0x4700
#define END             0x4F00

#define CTRL_PAGE_DOWN  0x7600
#define CTRL_PAGE_UP    0x8400


#define INSERT          0x5200
#define DELETE          0x5300
#define TAB             0x0009

/* Important Alt keys */

#define ALT_H           0x2300
#define ALT_Z           0x2C00
